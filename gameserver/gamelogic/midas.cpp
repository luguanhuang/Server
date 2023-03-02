#include "pch.h"
#include "midas.h"
#include "ibshoprecord.h"
#include "ibshopmgr.h"
#include "buyexchangemgr.h"
#include "crpc.h"
#include "reportdatarecord.h"
#include "gamelogic/bagtransition.h"
#include "table/backflowbuyconfig.h"
#include "gamelogic/buyrecord.h"
#include "gamelogic/rolemisc.h"
#include "guild/guildcampparty.h"
#include "spactivity/spactivityrecord.h"
#include "spactivity/spactivitydatasmall.h"

IBShopConsume IBShopConsume::Listener;
ExchangeDragonConsume ExchangeDragonConsume::Listener;
CustomBattleConsume CustomBattleConsume::Listener;
BackFlowConsume BackFlowConsume::Listener;
LotteryDrawConsume LotteryDrawConsume::Listener;
GuildPartySummonSpiritConsume GuildPartySummonSpiritConsume::Listener;
BackFlowShopConsume BackFlowShopConsume::Listener;

bool IBShopConsume::OnFinish(Role* role, INT32 index, INT32 iCount)
{
	role->OnBuyIBShopItem();
    IBShopMgr::Instance()->AddBuyCount(role,index,iCount,true);
	role->Get<CReportDataRecord>()->UpdateBuyIbShopCount();

	return true;
}

bool ExchangeDragonConsume::OnFinish(Role* role, INT32 index, INT32 iCount)
{
	if(NULL == role)
	{
		return false;
	}
	BuyExchangeMgr::Instance()->BuyDragonCoinFunAft(role, index, iCount);
	return true;
}

bool CustomBattleConsume::OnFinish(Role* role, INT32 index, INT32 iCount)
{
	if(NULL == role)
	{
		return false;
	}
	CUserData data;
	data.m_dwUserData = KKSG::ERR_SUCCESS;
	CRpc::ReplyDelayRpc(index, data);
	return true;
}

bool BackFlowConsume::OnFinish(Role* role, INT32 index, INT32 iCount)
{
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
	UINT32 day = iCount; 
	role->Get<BuyRecord>()->GetBackFlow().DoBuy(index, day);
	role->Get<BuyRecord>()->GetBackFlow().UnLock();
	SSInfo<<"role:"<<role->GetID()<<" do buy, index:"<<index<<" day:"<<day<<END;
	CUserData data;
	data.m_dwUserData = KKSG::ERR_SUCCESS;
	CRpc::ReplyDelayRpc(index, data);

	return true;
}

bool LotteryDrawConsume::OnFinish(Role* role, INT32 index, INT32 iCount)
{
	if(NULL == role)
	{
		return false;
	}
	role->Get<RoleMisc>()->UpdateIsPay(true);
	CUserData data;
	data.m_dwUserData = KKSG::ERR_SUCCESS;
	CRpc::ReplyDelayRpc(index, data);
	return true;
}

bool GuildPartySummonSpiritConsume::OnFinish(Role* role, INT32 index, INT32 iCount)
{
	if(NULL == role)
	{
		return false;
	}
	if (!CGuildCampPartyMgr::Instance()->ActiveIsFinish())
	{
		// Éú³É¾«Áé
		CGuildCampPartyMgr::Instance()->SummonSpirit(role);
		CGuildCampPartyMgr::Instance()->ClearSummonSpiritIng(role->getGuildId());
	}
	else
	{
		LogWarn("GuildPartySummonSpiritConsume timeout ,role name  : [%s], role id : [%llu]", role->GetName().c_str(), role->GetRoleID());
	}
	CUserData data;
	data.m_dwUserData = KKSG::ERR_SUCCESS;
	CRpc::ReplyDelayRpc(index, data);
	return true;
}

bool BackFlowShopConsume::OnFinish(Role* role, INT32 index, INT32 iCount)
{
	if (role == NULL)
	{
		return false;
	}
	SpActBackFlowData* backflow = static_cast<SpActBackFlowData*>(role->Get<SpActivityRecord>()->GetSpActData(SpActivity_BackFlow));
	if (backflow == NULL)
	{
		return false;
	}
	if (!backflow->SetGoodBuySucceed(iCount))
	{
		LogWarn("BackFlowShopConsume's SetGoodBuySucceed failed, maybe fresh the shop before it reply");
	}
	CUserData data;
	data.m_dwUserData = KKSG::ERR_SUCCESS;
	CRpc::ReplyDelayRpc(index, data);
	return true;
}