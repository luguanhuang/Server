#include "pch.h"
#include "skyteaminserttask.h"

#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"
#include "utildb/sqlbuilder.h"

SkyTeamInsertTask::SkyTeamInsertTask()
	:CMysqlTask("SkyTeamInsertTask")
	,m_rpcId(0)
	,m_id(0)
{
}

SkyTeamInsertTask::~SkyTeamInsertTask()
{ 
}

void SkyTeamInsertTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	SqlJoin columns;
	columns.addPair("_id", m_id);
	columns.addPair("base", m_data.base());
	columns.addPair("record", m_data.record());

	std::string sql;
	SqlBuilder::BuildInsertSql(poMysqlConn, sql, "skyteam", columns);

	if(!poMysqlConn->Query(sql.c_str(), sql.size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void SkyTeamInsertTask::OnReply()
{
	KKSG::ErrorCode nErrCode = KKSG::ERR_SUCCESS;
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		nErrCode = KKSG::ERR_FAILED;
		LogError("Insert skyteam failed[%llu], errMsg: %s", m_id, GetErrStream().str().c_str());
	}

	CUserData oUser((UINT32)nErrCode, NULL);
	CRpc::ReplyDelayRpc(m_rpcId, oUser);
}
