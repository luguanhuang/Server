#include "pch.h"
#include "role/rolemanager.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "friend/friendop.h"
#include "friend/rpcc2m_addblacklistnew.h"
#include "loghelper/tlogr.h"
#include "define/tlogenum.h"

// generate by ProtoGen at date: 2016/7/28 23:49:14

RPC_SERVER_IMPLEMETION(RpcC2M_AddBlackListNew, AddBlackListArg, AddBlackListRes)

void RpcC2M_AddBlackListNew::OnCall(const AddBlackListArg &roArg, AddBlackListRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(ERR_UNKNOWN);
		return;
	}
	CRoleSummary* summary = NULL;
	if (roArg.has_otherroleid())
	{
		summary = CRoleSummaryMgr::Instance()->GetRoleSummary(roArg.otherroleid());
	}
	else if (roArg.has_name())
	{
		summary = CRoleSummaryMgr::Instance()->GetRoleSummary(roArg.name());
	}
	if (NULL == summary)
	{
		roRes.set_errorcode(KKSG::ERR_ROLE_NOTEXIST);
		return;
	}
	UINT64 otherid = summary->GetID();
	if (role->GetID() == otherid)
	{
		roRes.set_errorcode(ERR_BLACK_SELF);
		return;
	}
	FriendOp op(role->GetID());
	op.SetOther(otherid);
	UINT32 errorcode = op.AddBlackList();

	KKSG::Friend2Client* black = roRes.mutable_black();
	Friend::Friend2Client(role->GetID(), otherid, black);

	roRes.set_errorcode(KKSG::ErrorCode(errorcode));

	if (roRes.errorcode() == KKSG::ErrorCode(ERR_SUCCESS))
	{
		TBlackListFlow oLog;
		oLog.m_iGetMode = TX_BlackList_DoMode;
		oLog.m_strOpenId = role->GetAccount();
		oLog.m_strPlayerOpenId = summary->GetAccount();
		oLog.m_ServerID = role->GetServerId();
		oLog.Do();
	}
	return;
}

void RpcC2M_AddBlackListNew::OnDelayReplyRpc(const AddBlackListArg &roArg, AddBlackListRes &roRes, const CUserData &roUserData)
{
}
