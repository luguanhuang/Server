#include "pch.h"
#include "rolesummaryquerytask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"

CRolesummaryQueryTask::CRolesummaryQueryTask()
	:CMysqlTask("RolesummaryQueryTask")
	,m_dwRpcDelayed(0)
	,m_qwRoleID(0)
{
}

CRolesummaryQueryTask::~CRolesummaryQueryTask()
{ 
}

void CRolesummaryQueryTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream ss;
	ss << "select data from rolesummary where _id=" << m_qwRoleID;

	IMysqlRecordSet* poRes = NULL;
	if(!poMysqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	poRes->FetchRow();
	char* pVal   = NULL;
	UINT32 dwLen = 0;
	if(poRes->GetFieldValue(0, &pVal, dwLen))
	{
		m_strData.assign(pVal, dwLen);
	}
	else
	{
		GetErrStream() << "Read Role Summary Failed";
		poRes->Release();
		return;
	}

	poRes->Release();
	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CRolesummaryQueryTask::OnReply()
{
	KKSG::ErrorCode nErrCode = KKSG::ERR_SUCCESS;
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		nErrCode = KKSG::ERR_FAILED;
		LogError("Select role summary data task failed, roleid: %llu, errMsg: %s", m_qwRoleID, GetErrStream().str().c_str());
	}

	KKSG::RoleSummaryStored data;
	data.ParseFromString(m_strData);
	CUserData oUser((UINT32)nErrCode, &data);
	CRpc::ReplyDelayRpc(m_dwRpcDelayed, oUser);
}