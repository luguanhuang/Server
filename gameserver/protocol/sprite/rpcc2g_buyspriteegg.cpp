#include "pch.h"
#include "sprite/rpcc2g_buyspriteegg.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "unit/systeminfo.h"
#include "define/systemiddef.h"
#include "gamelogic/lotteryrecord.h"
#include "gamelogic/bagtransition.h"

// generate by ProtoGen at date: 2017/6/26 20:59:19

RPC_SERVER_IMPLEMETION(RpcC2G_BuySpriteEgg, BuySpriteEggArg, BuySpriteEggRes)

void RpcC2G_BuySpriteEgg::OnCall(const BuySpriteEggArg &roArg, BuySpriteEggRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	if (!pRole->Get<CRoleSystem>()->IsSystemOpened(SYS_SPRITE_SHOP))
	{
		roRes.set_errorcode(KKSG::ERR_SYS_NOTOPEN);
		return;
	}
	LotteryRecord* record = pRole->Get<LotteryRecord>();

	Sequence<UINT32, 2> cost;
	std::vector<Sequence<UINT32, 2>> gain;
	std::vector<Sequence<UINT32, 2>> gift;
	switch (roArg.type())
	{
	case KKSG::Sprite_Draw_One_Free:
		{
			if (!record->DragonCoinBuyCheckFree())
			{
				roRes.set_errorcode(KKSG::ERR_FAILED);
				return;
			}
			gain = GetGlobalConfig().SpriteShopOneGain;
			gift = GetGlobalConfig().SpriteShopOneGift;
			break;
		}
	case KKSG::Sprite_Draw_One:
		{
			cost = GetGlobalConfig().SpriteShopOneCost;
			gain = GetGlobalConfig().SpriteShopOneGain;
			gift = GetGlobalConfig().SpriteShopOneGift;
			break;
		}
	case KKSG::Sprite_Draw_Ten:
		{
			cost = GetGlobalConfig().SpriteShopTenCost;
			gain = GetGlobalConfig().SpriteShopTenGain;
			gift = GetGlobalConfig().SpriteShopTenGift;
			break;
		}
	case KKSG::Sprite_GoldDraw_One_Free:
		{
			if (!record->GoldBuyCheckFree())
			{
				roRes.set_errorcode(KKSG::ERR_FAILED);
				return;
			}
			gain = GetGlobalConfig().SpriteShopGoldOneGain;
			gift = GetGlobalConfig().SpriteShopGoldOneGift;
			break;
		}
	case KKSG::Sprite_GoldDraw_One:
		{
			cost = GetGlobalConfig().SpriteShopGoldOneCost;
			gain = GetGlobalConfig().SpriteShopGoldOneGain;
			gift = GetGlobalConfig().SpriteShopGoldOneGift;
			break;
		}
	case KKSG::Sprite_GoldDraw_Ten:
		{
			cost = GetGlobalConfig().SpriteShopGoldTenCost;
			gain = GetGlobalConfig().SpriteShopGoldTenGain;
			gift = GetGlobalConfig().SpriteShopGoldTenGift;
			break;
		}
	default:
		{
			roRes.set_errorcode(KKSG::ERR_UNKNOWN);
			return;
		}
	}
	if (cost[0] != 0 && cost[1] != 0)
	{
		BagTakeItemTransition take(pRole);
		take.SetReason(ItemFlow_Sprite, ItemFlow_Sprite_BuySpriteEgg_Take);
		if (!take.TakeItem(cost[0], cost[1]))
		{
			take.RollBack();
			roRes.set_errorcode(KKSG::ERR_ITEM_NOT_ENOUGH);
			return;
		}
		take.NotifyClient();
	}
	BagGiveItemTransition give(pRole);
	give.SetReason(ItemFlow_Sprite, ItemFlow_Sprite_BuySpriteEgg_Give);
	for (auto i = gain.begin(); i != gain.end(); ++i)
	{
		give.GiveItem((*i)[0], (*i)[1]);
	}
	for (auto i = gift.begin(); i != gift.end(); ++i)
	{
		give.GiveItem((*i)[0], (*i)[1]);
		KKSG::ItemBrief* brief = roRes.add_item();
		brief->set_itemid((*i)[0]);
		brief->set_itemcount((*i)[1]);
		brief->set_isbind(true);
	}
	give.NotifyClient();

	roRes.set_errorcode(KKSG::ERR_SUCCESS);
	roRes.set_cooldown(record->GetDragonCoinFreeBuyCoolDown());
	roRes.set_goldfreebuycount(record->GetGoldFreeBuyCount());
	roRes.set_goldfreebuycooldown(record->GetGoldFreeBuyCoolDown());
}

void RpcC2G_BuySpriteEgg::OnDelayReplyRpc(const BuySpriteEggArg &roArg, BuySpriteEggRes &roRes, const CUserData &roUserData)
{
}
