#include "pch.h"
#include "role/rolemanager.h"
#include "friend/friendop.h"
#include "friend/rpcc2m_removefriendnew.h"

// generate by ProtoGen at date: 2016/7/28 23:01:40

RPC_SERVER_IMPLEMETION(RpcC2M_RemoveFriendNew, RemoveFriendArg, RemoveFriendRes)

void RpcC2M_RemoveFriendNew::OnCall(const RemoveFriendArg &roArg, RemoveFriendRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}
	FriendOp op(role->GetID());
	op.SetOther(roArg.friendroleid());
	UINT32 errorcode = op.RemoveFriend();
	roRes.set_errorcode(KKSG::ErrorCode(errorcode));
	return;
}

void RpcC2M_RemoveFriendNew::OnDelayReplyRpc(const RemoveFriendArg &roArg, RemoveFriendRes &roRes, const CUserData &roUserData)
{
}
