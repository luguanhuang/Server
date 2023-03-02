#include "pch.h"
#include "roletaskupdatetask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"
#include "utildb/sqlbuilder.h"


RoleTaskUpdateTask::RoleTaskUpdateTask()
:CMysqlTask("RoleTaskUpdateTask")
,m_rpcId(0)
,m_roleId(0)
{
}

RoleTaskUpdateTask::~RoleTaskUpdateTask()
{ 
}

void RoleTaskUpdateTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	SqlJoin columns;
	columns.addPair("_id", m_roleId);
	std::string data;
	m_data.SerializeToString(&data);
	columns.addPair("data", data);
	std::string sql;
	SqlBuilder::BuildReplaceIntoSql(poMysqlConn, sql, "roletask", columns);

	if(!poMysqlConn->Query(sql.c_str(), sql.size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void RoleTaskUpdateTask::OnReply()
{
	KKSG::ErrorCode nErrCode = KKSG::ERR_SUCCESS;
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		nErrCode = KKSG::ERR_FAILED;
		LogError("update roletask failed[%llu], errMsg: %s", m_roleId, GetErrStream().str().c_str());
	}

	CUserData oUser((UINT32)nErrCode, NULL);
	CRpc::ReplyDelayRpc(m_rpcId, oUser);
}
