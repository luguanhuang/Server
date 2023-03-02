#include "pch.h"
#include "partnerdeltask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"

PartnerDelTask::PartnerDelTask()
:CMysqlTask("PartnerDelTask")
,m_rpcId(0)
,m_partnerId(0)
{
}

PartnerDelTask::~PartnerDelTask()
{
}

void PartnerDelTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	// delete partner
	std::stringstream ss;
	ss << "delete from partner where _id=" << m_partnerId;

	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void PartnerDelTask::OnReply()
{
	KKSG::ErrorCode nErrCode = KKSG::ERR_SUCCESS;
	if (m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		nErrCode = KKSG::ERR_FAILED;
		LogError("Update partner failed[%llu], errMsg: %s", m_partnerId, GetErrStream().str().c_str());
	}

	CUserData oUser((UINT32)nErrCode, NULL);
	CRpc::ReplyDelayRpc(m_rpcId, oUser);
}
