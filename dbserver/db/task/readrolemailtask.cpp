#include "pch.h"
#include "readrolemailtask.h"
#include "crpc.h"


#define ROLE_MAX_MAIL_COUNT (500)


CReadRoleMailTask::CReadRoleMailTask()
:CMysqlTask("ReadRoleMailTask")
,m_qwRoleID(0)
,m_dwRpcDelayed(0)
,m_dwTimeNow(0)
{
}

CReadRoleMailTask::~CReadRoleMailTask()
{

}

void CReadRoleMailTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream ss;
	ss << "select mail from mail where roleid=" << m_qwRoleID;

	IMysqlRecordSet* poRes = NULL;
	if(!poMysqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}


	while(poRes->FetchRow())
	{
		char* pVal   = NULL;
		UINT32 dwLen = 0;

		std::string strData;
		if(poRes->GetFieldValue(0, &pVal, dwLen))
		{
			strData.assign(pVal, dwLen);
		}
		else
		{
			GetErrStream() << "Read 'mail' column failed";
			poRes->Release();
			return;
		}

		KKSG::SMail oMail;
		if(!oMail.ParseFromString(strData))
		{
			GetErrStream() << "Parse SMail data failed, roleID " << m_qwRoleID;
			poRes->Release();
			return;
		}

		if(oMail.timeexpire() <= m_dwTimeNow)
		{
			continue;
		}

		if(m_oMailMap.size() >= ROLE_MAX_MAIL_COUNT)
		{
			m_oMailMap.erase(m_oMailMap.begin());
		}

		m_oMailMap.insert(std::make_pair(oMail.uid(), oMail));
	}

	poRes->Release();
	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CReadRoleMailTask::OnReply()
{
	KKSG::ErrorCode nErrCode = KKSG::ERR_SUCCESS;
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		nErrCode = KKSG::ERR_FAILED;
		LogError("ReadRoleMailTask failed, roleid: %llu, errMsg: %s", m_qwRoleID, GetErrStream().str().c_str());
	}

	CUserData oUser((UINT32)nErrCode, &m_oMailMap);
	CRpc::ReplyDelayRpc(m_dwRpcDelayed, oUser);
}