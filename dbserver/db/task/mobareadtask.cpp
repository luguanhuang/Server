#include "pch.h"
#include "mobareadtask.h"
#include "crpc.h"


MobaReadTask::MobaReadTask()
:CMysqlTask("MobaReadTask")
{
	m_qwRoleID = 0;
}

MobaReadTask::~MobaReadTask()
{

}

void MobaReadTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream ss;
	ss << "select * from mobarecord where _id=" << m_qwRoleID;

	IMysqlRecordSet* poRes = NULL;
	if(!poMysqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		m_oRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}
	if (poRes->GetRowNum() == 0)
	{
		m_oRes.set_errorcode(KKSG::ERR_SUCCESS);
	}
	else
	{
		poRes->FetchRow();
		char* pVal   = NULL;
		UINT32 dwLen = 0;
		if (!poRes->GetFieldValue(1, &pVal, dwLen))
		{
			GetErrStream() << "Read column 'record' failed";
			m_oRes.set_errorcode(KKSG::ERR_UNKNOWN);
			poRes->Release();
			return;
		}
		if (!m_oRes.mutable_record()->ParseFromArray(pVal, dwLen))
		{
			GetErrStream() << "Parse 'record' data failed";
			m_oRes.set_errorcode(KKSG::ERR_UNKNOWN);
			poRes->Release();
			return;
		}
		m_oRes.set_errorcode(KKSG::ERR_SUCCESS);
	}
	poRes->Release();
	SetMysqlError(MYSQL_ERR_SUCESS);
}

void MobaReadTask::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		LogError("MobaReadTask failed, roleid: %llu, errMsg: %s", m_qwRoleID, GetErrStream().str().c_str());
	}
	CUserData oUser;
	oUser.m_pUserPtr = (void*)&m_oRes;
	CRpc::ReplyDelayRpc(m_dwRpcDelayed, oUser);
}