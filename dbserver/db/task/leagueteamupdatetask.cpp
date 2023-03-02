#include "pch.h"
#include "leagueteamupdatetask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"
#include "utildb/sqlbuilder.h"

LeagueTeamUpdateTask::LeagueTeamUpdateTask()
:CMysqlTask("LeagueTeamUpdateTask")
,m_rpcId(0)
,m_id(0)
{
}

LeagueTeamUpdateTask::~LeagueTeamUpdateTask()
{
}

void LeagueTeamUpdateTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	SqlJoin columns;
	if (m_data.has_base()) columns.addPair("base", m_data.base());
	if (m_data.has_battlerecord()) columns.addPair("battlerecord", m_data.battlerecord());

	SqlJoin cond;
	cond.addPair("_id", m_id);

	std::string sql;
	SqlBuilder::BuildUpdateSql(poMysqlConn, sql, "leagueteam", columns, cond);


	if(!poMysqlConn->Query(sql.c_str(), sql.size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void LeagueTeamUpdateTask::OnReply()
{
	KKSG::ErrorCode nErrCode = KKSG::ERR_SUCCESS;
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		nErrCode = KKSG::ERR_FAILED;
		LogError("Update leagueteam failed[%llu], errMsg: %s", m_id, GetErrStream().str().c_str());
	}

	CUserData oUser((UINT32)nErrCode, NULL);
	CRpc::ReplyDelayRpc(m_rpcId, oUser);
}
