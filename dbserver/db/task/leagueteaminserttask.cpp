#include "pch.h"
#include "leagueteaminserttask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"
#include "utildb/sqlbuilder.h"


LeagueTeamInsertTask::LeagueTeamInsertTask()
:CMysqlTask("LeagueTeamInsertTask")
,m_rpcId(0)
,m_id(0)
{
}

LeagueTeamInsertTask::~LeagueTeamInsertTask()
{ 
}

void LeagueTeamInsertTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	SqlJoin columns;
	columns.addPair("_id", m_id);
	columns.addPair("base", m_data.base());
	columns.addPair("battlerecord", m_data.battlerecord());

	std::string sql;
	SqlBuilder::BuildInsertSql(poMysqlConn, sql, "leagueteam", columns);

	if(!poMysqlConn->Query(sql.c_str(), sql.size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void LeagueTeamInsertTask::OnReply()
{
	KKSG::ErrorCode nErrCode = KKSG::ERR_SUCCESS;
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		nErrCode = KKSG::ERR_FAILED;
		LogError("Insert leagueteam failed[%llu], errMsg: %s", m_id, GetErrStream().str().c_str());
	}

	CUserData oUser((UINT32)nErrCode, NULL);
	CRpc::ReplyDelayRpc(m_rpcId, oUser);
}
