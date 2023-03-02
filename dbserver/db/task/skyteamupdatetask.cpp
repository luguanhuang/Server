#include "pch.h"
#include "skyteamupdatetask.h"

#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"
#include "utildb/sqlbuilder.h"

SkyTeamUpdateTask::SkyTeamUpdateTask()
:CMysqlTask("SkyTeamUpdateTask")
,m_rpcId(0)
,m_id(0)
{
}

SkyTeamUpdateTask::~SkyTeamUpdateTask()
{
}

void SkyTeamUpdateTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	SqlJoin columns;
	if (m_data.has_base()) columns.addPair("base", m_data.base());
	if (m_data.has_record()) columns.addPair("record", m_data.record());

	SqlJoin cond;
	cond.addPair("_id", m_id);

	std::string sql;
	SqlBuilder::BuildUpdateSql(poMysqlConn, sql, "skyteam", columns, cond);


	if(!poMysqlConn->Query(sql.c_str(), sql.size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void SkyTeamUpdateTask::OnReply()
{
	KKSG::ErrorCode nErrCode = KKSG::ERR_SUCCESS;
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		nErrCode = KKSG::ERR_FAILED;
		LogError("Update skyteam failed[%llu], errMsg: %s", m_id, GetErrStream().str().c_str());
	}

	CUserData oUser((UINT32)nErrCode, NULL);
	CRpc::ReplyDelayRpc(m_rpcId, oUser);
}
