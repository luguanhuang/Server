#include "pch.h"
#include "partnerinserttask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"
#include "utildb/sqlbuilder.h"


PartnerInsertTask::PartnerInsertTask()
:CMysqlTask("PartnerInsertTask")
,m_rpcId(0)
,m_partnerId(0)
{
}

PartnerInsertTask::~PartnerInsertTask()
{ 
}

void PartnerInsertTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	SqlJoin columns;
	columns.addPair("_id", m_partnerId);
	columns.addPair("base", m_data.base());
	columns.addPair("shop", m_data.shop());
	columns.addPair("liveness", m_data.liveness());

	std::string sql;
	SqlBuilder::BuildInsertSql(poMysqlConn, sql, "partner", columns);

	if(!poMysqlConn->Query(sql.c_str(), sql.size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void PartnerInsertTask::OnReply()
{
	KKSG::ErrorCode nErrCode = KKSG::ERR_SUCCESS;
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		nErrCode = KKSG::ERR_FAILED;
		LogError("Insert partner failed[%llu], errMsg: %s", m_partnerId, GetErrStream().str().c_str());
	}

	CUserData oUser((UINT32)nErrCode, NULL);
	CRpc::ReplyDelayRpc(m_rpcId, oUser);
}
