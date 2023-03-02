#include "pch.h"
#include "buyexchangemgr.h"
#include "pb/project.pb.h"
#include "unit/role.h"
#include "bagtransition.h"
#include "buyrecord.h"
#include "globalconfig.h"
#include "chatmgr.h"
#include "midas.h"

INSTANCE_SINGLETON(BuyExchangeMgr)

BuyExchangeMgr::BuyExchangeMgr()
{

}

BuyExchangeMgr::~BuyExchangeMgr()
{

}

bool BuyExchangeMgr::Init()
{
	if (!m_GoldTable.LoadFile("table/BuyGold.txt"))
	{
		LogWarn("load table/BuyGold.txt failed!");
		return false;
	}

	if (!m_FatigueTable.LoadFile("table/BuyFatigue.txt"))
	{
		LogWarn("load table/BuyFatigue.txt failed!");
		return false;
	}

	if (!m_DragonCoinTable.LoadFile("table/BuyDragonCoin.txt"))
	{
		LogWarn("load table/BuyDragonCoin.txt failed!");
		return false;
	}

	return true;
}

void BuyExchangeMgr::Uninit()
{

}

int BuyExchangeMgr::BuyGold(Role *pRole, int useitemid)
{
	int Level = pRole->GetLevel();
	if (Level - 1 >= (int)m_GoldTable.Table.size())
	{
		return KKSG::ERR_FAILED;
	}

	BuyRecord *pBuyRecord = pRole->Get<BuyRecord>();
	if (!pBuyRecord->CanBuyGold())
	{
		return KKSG::ERR_BUY_LIMIT;
	}

	BuyGoldTable::RowData *pRow = m_GoldTable.Table[Level - 1];
	BagTakeItemTransition take(pRole);
	take.SetReason(ItemFlow_Buy, ItemFlow_Buy_Coin);
	int count = pBuyRecord->GetBuyGoldCount();
	int size = 0;
	int needNum = 0;
	if(DIAMOND == useitemid)
	{
		size = (int)pRow->DiamondCost.size();
		if(count >= size)
		{
			count = size - 1;
		}
		if(pRow->DiamondCost.empty())
		{
			return KKSG::ERR_FAILED;
		}
		needNum = pRow->DiamondCost[count];
	}
	else if(DRAGON_COIN == useitemid)
	{
		size = (int)pRow->DragonCoinCost.size();
		if(count >= size)
		{
			count = size - 1;
		}
		if(pRow->DragonCoinCost.empty())
		{
			return KKSG::ERR_FAILED;
		}
		needNum = pRow->DragonCoinCost[count];
	}

	if (!take.TakeItem(useitemid, needNum))
	{
		take.RollBack();
		if(DIAMOND == useitemid)
		{
			return KKSG::ERR_ITEM_NEED_DIAMOND;
		}
		else
		{
			return KKSG::ERR_ITEM_NEED_DRAGONCOIN;
		}
	}
	take.SetAction(COST_BUY_GOLD);
	take.NotifyClient();

	pBuyRecord->IncBuyGoldCount();
	pBuyRecord->SendToClient();

	BagGiveItemTransition give(pRole);
	give.SetReason(ItemFlow_Buy, ItemFlow_Buy_Coin);
	give.GiveItem(GOLD, (UINT32)pRow->Gold);
	give.NotifyClient();

	pRole->OnBuy(GOLD);
	return KKSG::ERR_SUCCESS;
}

int BuyExchangeMgr::BuyFatigue(Role *pRole, int useitemid, int fatigueID)
{
	BuyFatigueTable::RowData *pRow = NULL;
	for (UINT32 i = 0; i < m_FatigueTable.Table.size(); ++i)
	{
		if (m_FatigueTable.Table[i]->FatigueID == fatigueID)
		{
			pRow = m_FatigueTable.Table[i];
			break;
		}
	}

	if (pRow == NULL)
	{
		return KKSG::ERR_FAILED;
	}

	BuyRecord *pBuyRecord = pRole->Get<BuyRecord>();
	if (!pBuyRecord->CanBuyFatigue(fatigueID))
	{
		return KKSG::ERR_BUY_LIMIT;
	}

	int count = pBuyRecord->GetBuyFatigueCount(fatigueID);
	int size = 0;
	int needNum = 0;
	if(DIAMOND == useitemid)
	{
		size = pRow->DiamondCost.size();
		if (count >= size)
		{
			count = size-1;
		}
		if(pRow->DiamondCost.empty())
		{
			return KKSG::ERR_FAILED;
		}
		needNum = pRow->DiamondCost[count];
	}
	else if(DRAGON_COIN == useitemid)
	{
		size = pRow->DragonCoinCost.size();
		if (count >= size)
		{
			count = size-1;
		}
		if(pRow->DragonCoinCost.empty())
		{
			return KKSG::ERR_FAILED;
		}
		needNum = pRow->DragonCoinCost[count];
	}

	BagTakeItemTransition take(pRole);
	take.SetReason(ItemFlow_Buy, ItemFlow_Buy_Fatigue);
	if (!take.TakeItem(useitemid, needNum))
	{
		take.RollBack();
		if(DIAMOND == useitemid)
		{
			return KKSG::ERR_ITEM_NEED_DIAMOND;
		}
		else
		{
			return KKSG::ERR_ITEM_NEED_DRAGONCOIN;
		}
	}
	take.SetAction(COST_BUY_FATIGUE);
	take.NotifyClient();

	pBuyRecord->IncBuyFagitueCount(fatigueID);
	pBuyRecord->SendToClient();

	BagGiveItemTransition give(pRole);
	give.SetReason(ItemFlow_Buy, ItemFlow_Buy_Fatigue);
	give.GiveItem(fatigueID, pRow->Value);
	give.NotifyClient();

	pRole->OnBuy(FATIGUE);
	return KKSG::ERR_SUCCESS;
}

int BuyExchangeMgr::BuyDragonCoinFunPre(Role *pRole, int useitemid, UINT32 oncecount)
{
	int level = pRole->GetLevel();
	if(level - 1 >= (int)m_DragonCoinTable.Table.size())
	{
		return KKSG::ERR_FAILED;
	}
	BuyRecord* pBuyRecord = pRole->Get<BuyRecord>();
	if(!pBuyRecord->CanBuyDragon(oncecount))
	{
		return KKSG::ERR_BUY_LIMIT;
	}
	BuyDragonCoin::RowData* pRow = m_DragonCoinTable.Table[level - 1];
	if(pRow->DiamondCost.empty())
	{
		return KKSG::ERR_FAILED;
	}
	int count = pBuyRecord->GetBuyDragonCoinCount();
	int size = (int)pRow->DiamondCost.size();
	if(count >= size)
	{
		count = size - 1;
	}
	//BagTakeItemTransition take(pRole);
	//take.SetReason(ItemFlow_Buy, ItemFlow_Buy_DragonCoin);
	//if(!take.TakeItem(useitemid, pRow->DiamondCost[count] * oncecount))
	//{
	//	take.RollBack();
	//	return KKSG::ERR_ITEM_NEED_DIAMOND;
	//}
	//take.SetAction((COST_BUY_DRAGONCOIN));
	//take.NotifyClient();

	UINT32 costCount = pRow->DiamondCost[count] * oncecount;
	KKSG::PayConsumeBase info;
	info.set_type(MidasConsumeType_ExchangeDragon);
	info.set_amt(costCount);
	info.set_reason(ItemFlow_Buy);
	info.set_subreason(ItemFlow_Buy_DragonCoin);
	info.set_index(useitemid);
	info.set_count(oncecount);

	std::vector<ItemDesc> itemVec;
	itemVec.push_back(ItemDesc(DRAGON_COIN, UINT32(pRow->DragonCoin * oncecount)));

	if(!pRole->ConsumeDiamond(info, itemVec))
	{
		return KKSG::ERR_ITEM_NEED_DIAMOND;
	}

	return KKSG::ERR_SUCCESS;
}

void BuyExchangeMgr::BuyDragonCoinFunAft(Role *pRole, int useitemid, UINT32 oncecount)
{
	if(NULL == pRole)
	{
		return;
	}

	BuyRecord* pBuyRecord = pRole->Get<BuyRecord>();
	pBuyRecord->IncBuyDragonCoinCount(oncecount);
	pBuyRecord->SendToClient();

	//BagGiveItemTransition give(pRole);
	//give.SetReason(ItemFlow_Buy, ItemFlow_Buy_DragonCoin);
	//give.GiveItem(DRAGON_COIN, pRow->DragonCoin * oncecount);
	//give.NotifyClient();

	pRole->OnBuy(DRAGON_COIN);
}

int BuyExchangeMgr::BuyBlueBird(Role* pRole, int useitemid)
{
	if(useitemid != DRAGON_COIN)
	{
		return KKSG::ERR_UNKNOWN;
	}

	BagTakeItemTransition take(pRole);
	take.SetReason(ItemFlow_Buy, ItemFlow_Buy_BlueBird);
	if(!take.TakeItem(useitemid,  GetGlobalConfig().blueBirdPrice))
	{
		take.RollBack();
		return KKSG::ERR_CHECKIN_LACKDRAGONCOIN;
	}
	take.SetAction((COST_BUY_BLUEBIRD));
	take.NotifyClient();

	BagGiveItemTransition give(pRole);
	give.SetReason(ItemFlow_Buy, ItemFlow_Buy_BlueBird);
	give.GiveItem(BLUE_BIRD, 1);
	give.NotifyClient();

	return KKSG::ERR_SUCCESS;
}

int BuyExchangeMgr::ExchangeDiamond(Role* pRole, UINT32 count)
{
	if (count == UINT32(-1))
	{
		return KKSG::ERR_UNKNOWN;
	}
	KKSG::PayConsumeBase info;
	info.set_amt(count);
	info.set_subreason(ItemFlow_Exchange_Dragoncoin);
	info.set_reason(ItemFlow_Buy);
	std::vector<ItemDesc> itemVec;
	itemVec.push_back(ItemDesc(DRAGON_COIN, count));

	if(!pRole->ConsumeDiamond(info, itemVec))
	{
		return KKSG::ERR_ITEM_NEED_DIAMOND;
	}
	return KKSG::ERR_SUCCESS;
}