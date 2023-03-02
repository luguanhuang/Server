#include "pch.h"
#include "lottery/rpcc2g_drawlottery.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/LotteryMgr.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/lotteryrecord.h"
#include "gamelogic/AchivementDef.h"
#include "gamelogic/AchivementMgr.h"
#include "gamelogic/item.h"
#include "gamelogic/spriterecord.h"
#include "common/ptcg2c_gserrornotify.h"
#include "unit/systeminfo.h"
#include "define/systemiddef.h"

// generate by ProtoGen at date: 2015/5/13 11:42:37

RPC_SERVER_IMPLEMETION(RpcC2G_DrawLottery, DrawLotteryArg, DrawLotteryRes)

void RpcC2G_DrawLottery::OnCall(const DrawLotteryArg &roArg, DrawLotteryRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	if (pRole->Get<CSpriteRecord>()->GetSpriteNum() > GetGlobalConfig().SpriteMaxNum)
	{
		roRes.set_errorcode(KKSG::ERR_SPRITE_INFIGHT_SAMETYPE);
		return;
	}

	if (!pRole->Get<CRoleSystem>()->IsSystemOpened(SYS_SPRITE_LOTTERY))
	{
		roRes.set_errorcode(KKSG::ERR_SYS_NOTOPEN);
		return;
	}

	LotteryRecord *pLR = pRole->Get<LotteryRecord>();

	if (LotteryMgr::Instance()->TakeCost(roArg.type(), pRole) == false)
	{
		roRes.set_errorcode(KKSG::ERR_ITEM_NOT_ENOUGH);
		roRes.set_nextgoodcount(pLR->GetDragonCoinMinimumRewardCount()+1);
		return;
	}

	std::vector<UINT32> spriteppt;
	std::vector<ItemDesc> items = LotteryMgr::Instance()->DoDrawARaffle(roArg.type(), pRole, spriteppt);
	for (UINT32 i = 0; i < items.size(); ++i)
	{
		KKSG::ItemBrief *pItemBrief = roRes.add_items();
		pItemBrief->set_itemid(items[i].itemID);
		pItemBrief->set_itemcount(items[i].itemCount);
		pItemBrief->set_isbind(items[i].isbind);
	}
	for (UINT32 i = 0; i < spriteppt.size(); ++i)
	{
		roRes.add_spriteppt(spriteppt[i]);
	}
	roRes.set_errorcode(KKSG::ERR_SUCCESS);
	roRes.set_nextgoodcount(pLR->GetDragonCoinMinimumRewardCount()+1);
}

void RpcC2G_DrawLottery::OnDelayReplyRpc(const DrawLotteryArg &roArg, DrawLotteryRes &roRes, const CUserData &roUserData)
{
}
