#include "pch.h"
#include "partnerupdatetask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"
#include "utildb/sqlbuilder.h"

PartnerUpdateTask::PartnerUpdateTask()
:CMysqlTask("PartnerUpdateTask")
,m_rpcId(0)
,m_partnerId(0)
{
}

PartnerUpdateTask::~PartnerUpdateTask()
{
}

void PartnerUpdateTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	SqlJoin columns;
	if (m_data.has_base()) columns.addPair("base", m_data.base());
	if (m_data.has_liveness()) columns.addPair("liveness", m_data.liveness());
	if (m_data.has_shop()) columns.addPair("shop", m_data.shop());

	SqlJoin cond;
	cond.addPair("_id", m_partnerId);

	std::string sql;
	SqlBuilder::BuildUpdateSql(poMysqlConn, sql, "partner", columns, cond);


	if(!poMysqlConn->Query(sql.c_str(), sql.size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void PartnerUpdateTask::OnReply()
{
	KKSG::ErrorCode nErrCode = KKSG::ERR_SUCCESS;
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		nErrCode = KKSG::ERR_FAILED;
		LogError("Update partner failed[%llu], errMsg: %s", m_partnerId, GetErrStream().str().c_str());
	}

	CUserData oUser((UINT32)nErrCode, NULL);
	CRpc::ReplyDelayRpc(m_rpcId, oUser);
}
