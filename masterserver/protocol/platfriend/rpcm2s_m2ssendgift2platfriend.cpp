#include "pch.h"
#include "platfriend/rpcm2s_m2ssendgift2platfriend.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "platforminfo/platformfriendmgr.h"

// generate by ProtoGen at date: 2016/10/26 15:56:11

RPC_CLIENT_IMPLEMETION(RpcM2S_M2SSendGift2PlatFriend, M2SSendGift2PlatFriendArg, M2SSendGift2PlatFriendRes)

void RpcM2S_M2SSendGift2PlatFriend::OnReply(const M2SSendGift2PlatFriendArg &roArg, const M2SSendGift2PlatFriendRes &roRes, const CUserData &roUserData)
{
	/*CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (!role)
	{
		return;
	}*/

	LogDebug("send platfriend gift %s,source:%s , to:%s", roRes.error() == KKSG::ERR_SUCCESS ? "ok" : "failed" , roArg.sourceopenid().c_str(), roArg.destuser().openid().c_str());
}

void RpcM2S_M2SSendGift2PlatFriend::OnTimeout(const M2SSendGift2PlatFriendArg &roArg, const CUserData &roUserData)
{
}
