#include "pch.h"
#include "role/rolemanager.h"
#include "friend/friendop.h"
#include "friend/rpcc2m_removeblacklistnew.h"
#include "role/rolesummarymgr.h"
#include "role/rolesummary.h"
#include "loghelper/tlogr.h"
#include "define/tlogenum.h"

// generate by ProtoGen at date: 2016/7/28 23:49:44

RPC_SERVER_IMPLEMETION(RpcC2M_RemoveBlackListNew, RemoveBlackListArg, RemoveBlackListRes)

void RpcC2M_RemoveBlackListNew::OnCall(const RemoveBlackListArg &roArg, RemoveBlackListRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(ERR_UNKNOWN);
		return;
	}
	FriendOp op(role->GetID());
	UINT32 errorcode = op.RemoveBlackList(roArg.otherroleid());
	roRes.set_errorcode(KKSG::ErrorCode(errorcode));

	CRoleSummary* pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(roArg.otherroleid());
	if (pSum && roRes.errorcode() == KKSG::ErrorCode(ERR_SUCCESS))
	{
		TBlackListFlow oLog;
		oLog.m_iGetMode = TX_BlackList_UnDoMode;
		oLog.m_strOpenId = role->GetAccount();
		oLog.m_strPlayerOpenId = pSum->GetAccount();
		oLog.m_ServerID = role->GetServerId();
		oLog.Do();
	}
	return;
}

void RpcC2M_RemoveBlackListNew::OnDelayReplyRpc(const RemoveBlackListArg &roArg, RemoveBlackListRes &roRes, const CUserData &roUserData)
{
}
