#include "pch.h"
#include "spactivity/rpcc2m_getfloweractivityreward.h"
#include "spactivity/spactivitymgr.h"
#include "define/spactivityiddef.h"
#include "util/gametime.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/flowerranklist.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "table/globalconfig.h"
#include "social/social.h"
#include "social/socialmgr.h"
#include "define/tlogenum.h"
#include "network/gslink.h"
#include "common/rpcm2g_msgiveitem.h"

// generate by ProtoGen at date: 2017/6/7 15:14:13

RPC_SERVER_IMPLEMETION(RpcC2M_GetFlowerActivityReward, GetFlowerActivityRewardArg, GetFlowerActivityRewardRes)

void RpcC2M_GetFlowerActivityReward::OnCall(const GetFlowerActivityRewardArg &roArg, GetFlowerActivityRewardRes &roRes)
{
	roRes.set_errorcode(KKSG::ERR_FAILED);

	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (!role)
	{
		return;
	}

	SocialInfo* socialInfo = SocialMgr::Instance()->GetSocialInfo(role->GetID());
	if (!socialInfo)
	{
		return;
	}

	if (!socialInfo->flower.CheckGetFlowerActivityReward())
	{
		return;
	}

	socialInfo->flower.m_activityFlowerData.gotReward = true;
	SocialMgr::Instance()->SetChanged(role->GetID());

	roRes.set_errorcode(KKSG::ERR_SUCCESS);

	LogInfo("%llu, %s get flower activity reward", role->GetID(), role->GetName().c_str());

	RpcM2G_MsGiveItem* rpc = RpcM2G_MsGiveItem::CreateRpc();
	rpc->m_oArg.set_roleid(role->GetID());
	rpc->m_oArg.set_reason(ItemFlow_Flower);
	rpc->m_oArg.set_subreason(ItemFlow_FlowerActivityDayRankAward);

	auto& reward = GetGlobalConfig().FlowerActivityReward;
	for (auto iter = reward.begin(); iter != reward.end(); ++iter)
	{
		KKSG::ItemBrief* brief = rpc->m_oArg.add_items();
		brief->set_itemid(iter->seq[0]);
		brief->set_itemcount(iter->seq[1]);
	}
	GSLink::Instance()->SendToLine(MAIN_HALL_GS_LINE, *rpc);
}

void RpcC2M_GetFlowerActivityReward::OnDelayReplyRpc(const GetFlowerActivityRewardArg &roArg, GetFlowerActivityRewardRes &roRes, const CUserData &roUserData)
{
}
