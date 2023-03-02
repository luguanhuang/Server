#include "pch.h"
#include "roletaskdeletetask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"

RoleTaskDelTask::RoleTaskDelTask()
:CMysqlTask("RoleTaskDelTask")
,m_rpcId(0)
,m_roleId(0)
{
}

RoleTaskDelTask::~RoleTaskDelTask()
{
}

void RoleTaskDelTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream ss;
	ss << "delete from roletask where _id=" << m_roleId;

	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void RoleTaskDelTask::OnReply()
{
	KKSG::ErrorCode nErrCode = KKSG::ERR_SUCCESS;
	if (m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		nErrCode = KKSG::ERR_FAILED;
		LogError("delete roletask failed[%llu], errMsg: %s", m_roleId, GetErrStream().str().c_str());
	}

	CUserData oUser((UINT32)nErrCode, NULL);
	CRpc::ReplyDelayRpc(m_rpcId, oUser);
}
