#include "pch.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "pay/rpcc2g_backflowbuyop.h"
#include "gamelogic/buyrecord.h"
#include "table/backflowbuyconfig.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/midas.h"
#include "scene/scene.h"

// generate by ProtoGen at date: 2017/8/4 15:47:17

RPC_SERVER_IMPLEMETION(RpcC2G_BackFlowBuyOp, BackFlowBuyOpArg, BackFlowBuyOpRes)

void RpcC2G_BackFlowBuyOp::OnCall(const BackFlowBuyOpArg &roArg, BackFlowBuyOpRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	if (NULL == role->GetCurrScene() || KKSG::SCENE_HALL != role->GetCurrScene()->GetSceneType())
	{
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}
	if (!role->IsBackFlowRole())
	{
		SSWarn<<"role:"<<role->GetID()<<" not back flow"<<END;
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}

	if (!role->Get<CRoleSystem>()->IsSystemOpened(SYS_BACKFLOW_BUY))
	{
		SSWarn<<"role:"<<role->GetID()<<" system not open"<<END;
		roRes.set_errorcode(KKSG::ERR_SYS_NOTOPEN);
		return;
	}

	BuyRecord* record = role->Get<BuyRecord>();
	BackFlowBuyInfo& buyinfo = record->GetBackFlow();
	UINT32 day = XCommon::GetTodayUnique();
	UINT32 index = buyinfo.GetTodayIndex(day);
	if (BackFlowBuyConfig::Instance()->IsBuyMax(index))
	{
		SSWarn<<"role:"<<role->GetID()<<" buy max, index:"<<index<<END;
		roRes.set_errorcode(KKSG::ERR_BACKFLOW_BUYEND);
		return;
	}
	if (buyinfo.IsLock())
	{
		SSWarn<<"role:"<<role->GetID()<<" is lock"<<END;
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}

	if (1 == roArg.op())
	{
		// query
		std::vector<ItemDesc> items = BackFlowBuyConfig::Instance()->GetItems(index);
		for (auto i = items.begin(); i != items.end(); ++i)
		{
			KKSG::ItemBrief* itembrief = roRes.add_items();
			i->ToKKSG(itembrief);
		}
		UINT32 countlimit = BackFlowBuyConfig::Instance()->GetCanBuyCount(index);
		UINT32 countleft = countlimit > buyinfo.buycount ? countlimit - buyinfo.buycount : 0;
		ItemDesc cost = BackFlowBuyConfig::Instance()->GetCost(index);
		cost.ToKKSG(roRes.mutable_cost());
		roRes.set_countmax(countlimit);
		roRes.set_countleft(countleft);
		roRes.set_errorcode(KKSG::ERR_SUCCESS);
	}
	else if (2 == roArg.op())
	{
		UINT32 countlimit = BackFlowBuyConfig::Instance()->GetCanBuyCount(index);
		// buy
		if (countlimit <= buyinfo.buycount)
		{
			SSWarn<<"role:"<<role->GetID()<<" index:"<<index<<" buycount:"<<buyinfo.buycount<<END;
			roRes.set_errorcode(KKSG::ERR_FAILED);
			return;
		}

		ItemDesc cost = BackFlowBuyConfig::Instance()->GetCost(index);
		if (cost.itemID != DIAMOND)
		{
			// cost
			BagTakeItemTransition take(role);
			take.SetReason(ItemFlow_BackFlowBuy, ItemFlow_BackFlowBuy_Take);
			if (!take.TakeItem(cost.itemID, cost.itemCount))
			{
				take.RollBack();
				roRes.set_errorcode(KKSG::ERR_ITEM_NOT_ENOUGH);
				return;
			}
			take.NotifyClient();

			buyinfo.Lock();

			// give
			BagGiveItemTransition give(role);
			give.SetReason(ItemFlow_BackFlowBuy, ItemFlow_BackFlowBuy_Give);
			std::vector<ItemDesc> items = BackFlowBuyConfig::Instance()->GetItems(index);
			for (auto i = items.begin(); i != items.end(); ++i)
			{
				give.GiveItem(i->itemID, i->itemCount);
			}
			give.NotifyClient();

			// do buy
			buyinfo.DoBuy(index, day);
			buyinfo.UnLock();

			SSInfo<<"role:"<<role->GetID()<<" do buy index:"<<index<<" day:"<<day<<END;

			roRes.set_errorcode(KKSG::ERR_SUCCESS);
		}
		else
		{
			KKSG::PayConsumeBase info;
			info.set_type((int)MidasConsumeType_BackFlow);
			info.set_amt(cost.itemCount);
			info.set_subreason(ItemFlow_BackFlowBuy_Take);
			info.set_reason(ItemFlow_BackFlowBuy);
			info.set_index(index);
			info.set_count(day);

			std::vector<ItemDesc> vecItem;
			if(!role->ConsumeDiamond(info, vecItem))
			{
				roRes.set_errorcode(KKSG::ERR_ITEM_NOT_ENOUGH);
				return;
			}

			buyinfo.Lock();
			SSInfo<<"role:"<<role->GetID()<<" begin buy index:"<<index<<" day:"<<day<<END;

			roRes.set_errorcode(KKSG::ERR_SUCCESS);
		}
	}
	else
	{
		roRes.set_errorcode(KKSG::ERR_FAILED);
	}
}

void RpcC2G_BackFlowBuyOp::OnDelayReplyRpc(const BackFlowBuyOpArg &roArg, BackFlowBuyOpRes &roRes, const CUserData &roUserData)
{
}
