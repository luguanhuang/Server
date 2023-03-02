#include "pch.h"
#include "partnerreplaceidtask.h"

#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"
#include "utildb/sqlbuilder.h"

PartnerReplaceIdTask::PartnerReplaceIdTask()
:CMysqlTask("PartnerReplaceIdTask")
,m_rpcId(0)
,m_partnerId(0)
,m_oldId(0)
{
}

PartnerReplaceIdTask::~PartnerReplaceIdTask()
{
}

void PartnerReplaceIdTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::ostringstream oss;
	oss << "update partner set _id=" << m_partnerId << " where _id=" << m_oldId;
	std::string sql = oss.str();

	if(!poMysqlConn->Query(sql.c_str(), sql.size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void PartnerReplaceIdTask::OnReply()
{
	KKSG::ErrorCode nErrCode = KKSG::ERR_SUCCESS;
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		nErrCode = KKSG::ERR_FAILED;
		LogError("Update partner id failed[%llu], errMsg: %s, oldId[%llu]", m_partnerId, GetErrStream().str().c_str(), m_oldId);
	}

	CUserData oUser((UINT32)nErrCode, NULL);
	CRpc::ReplyDelayRpc(m_rpcId, oUser);
}
