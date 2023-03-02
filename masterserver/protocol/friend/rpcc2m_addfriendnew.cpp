#include "pch.h"
#include "util.h"
#include "role/rolemanager.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "util/XCommon.h"
#include "friend/friendop.h"
#include "friend/rpcc2m_addfriendnew.h"

// generate by ProtoGen at date: 2016/7/28 22:59:57

RPC_SERVER_IMPLEMETION(RpcC2M_AddFriendNew, AddFriendArg, AddFriendRes)

void RpcC2M_AddFriendNew::OnCall(const AddFriendArg &roArg, AddFriendRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}
	CRoleSummary* rolesummary = NULL;
	if (roArg.has_friendroleid())
	{
		rolesummary = CRoleSummaryMgr::Instance()->GetRoleSummary(roArg.friendroleid());
	}
	else if (roArg.has_name())
	{
		rolesummary = CRoleSummaryMgr::Instance()->GetRoleSummary(roArg.name());
	}
	if (NULL == rolesummary)
	{
		roRes.set_errorcode(ERR_ROLE_NOTEXIST);
		return;
	}

	if (role->GetID() == rolesummary->GetID())
	{
		roRes.set_errorcode(ERR_FRIEND_SELF);
		return;
	}
	FriendOp op(role->GetID());
	op.SetOther(rolesummary->GetID());
	UINT32 errorcode = op.AddApply();

	roRes.set_errorcode(KKSG::ErrorCode(errorcode));

	///> 双方同时申请，直接加为好友
	if (errorcode == KKSG::ERR_SUCCESS)
	{
		if (op.IsInApply())
		{
			if (KKSG::ERR_SUCCESS == op.AddFriend())
			{
				op.DeleteApplyBoth();
			}
		}
	}

	return;
}

void RpcC2M_AddFriendNew::OnDelayReplyRpc(const AddFriendArg &roArg, AddFriendRes &roRes, const CUserData &roUserData)
{
}
