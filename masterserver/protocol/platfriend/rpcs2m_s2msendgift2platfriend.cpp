#include "pch.h"
#include "platfriend/rpcs2m_s2msendgift2platfriend.h"
#include "platforminfo/platformfriendmgr.h"

// generate by ProtoGen at date: 2016/10/26 15:59:08

RPC_SERVER_IMPLEMETION(RpcS2M_S2MSendGift2PlatFriend, S2MSendGift2PlatFriendArg, S2MSendGift2PlatFriendRes)

void RpcS2M_S2MSendGift2PlatFriend::OnCall(const S2MSendGift2PlatFriendArg &roArg, S2MSendGift2PlatFriendRes &roRes)
{
	CPlatformFriendMgr::Instance()->ReceivePlatFriendGift(roArg);
}

void RpcS2M_S2MSendGift2PlatFriend::OnDelayReplyRpc(const S2MSendGift2PlatFriendArg &roArg, S2MSendGift2PlatFriendRes &roRes, const CUserData &roUserData)
{
}
