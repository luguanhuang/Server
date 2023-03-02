#include "pch.h"
#include "platfriend/rpcc2m_sendgift2platfriend.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "platforminfo/platformfriendmgr.h"
#include "platfriend/rpcm2s_m2ssendgift2platfriend.h"
#include "network/cslink.h"
#include "table/globalconfig.h"

// generate by ProtoGen at date: 2016/10/26 16:45:31

RPC_SERVER_IMPLEMETION(RpcC2M_SendGift2PlatFriend, SendGift2PlatFriendArg, SendGift2PlatFriendRes)

void RpcC2M_SendGift2PlatFriend::OnCall(const SendGift2PlatFriendArg &roArg, SendGift2PlatFriendRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (!role)
	{
		return;
	}

	UINT32 error = CPlatformFriendMgr::Instance()->GiveGift2PlatFriend(role, roArg.openid());
	roRes.set_error((KKSG::ErrorCode)error);
	if (error != KKSG::ERR_SUCCESS)
	{
		return;
	}

	PlatFriendList* list = CPlatformFriendMgr::Instance()->GetPlatFriendList(role);
	RpcM2S_M2SSendGift2PlatFriend* rpc = RpcM2S_M2SSendGift2PlatFriend::CreateRpc();
	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_sourceopenid(role->GetAccount());
 	list->GetPlatFriendUserAllInfo(roArg.openid(), *rpc->m_oArg.mutable_destuser());
	KKSG::ItemBrief* brief = rpc->m_oArg.add_item();
	brief->set_itemid(GetGlobalConfig().GivePlatFriendGift.seq[0]);
	brief->set_itemcount(GetGlobalConfig().GivePlatFriendGift.seq[1]);
	CSLink::Instance()->SendTo(*rpc);

	LogDebug("%llu, %s, send gift to %s", role->GetID(), role->GetAccount().c_str(), roArg.openid().c_str());
}

void RpcC2M_SendGift2PlatFriend::OnDelayReplyRpc(const SendGift2PlatFriendArg &roArg, SendGift2PlatFriendRes &roRes, const CUserData &roUserData)
{
}
