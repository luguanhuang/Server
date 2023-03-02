#include "pch.h"
#include <time.h>
#include "LotteryMgr.h"
#include "pb/project.pb.h"
#include "unit/role.h"
#include "bag.h"
#include "bagtransition.h"
#include "globalconfig.h"
#include "lotteryrecord.h"
#include "noticemgr.h"
#include "utility/tlogger.h"
#include "payv2Record.h"
#include "foreach.h"
#include "event/eventmgr.h"
#include "spriterecord.h"
#include "util/XRandom.h"
#include "gamelogic/popwindows.h"
#include "lottery/ptcg2c_sharerandomgiftntf.h"


struct DrawCost
{
	UINT32 itemID;
	UINT32 itemCount;
};


typedef bool (LotteryRecord::*CheckFreeFun)();
//typedef DropResultInfo (LotteryRecord::*GetDropCallFun)();
typedef const ItemDesc* (LotteryRecord::*GetItemCallFun)();

class CommonDrawPrizeHandler : public IDrawHandler
{
public:

	CommonDrawPrizeHandler(int type, int count, CheckFreeFun freefun, GetItemCallFun itemFun, const DrawCost& cost)
	{
		m_DrawType = type;
		m_DropCount = count;
		assert(m_DropCount > 0);
		m_freeFun = freefun;
		m_itemFun = itemFun;
		m_Cost = cost;
		m_iCostCount = 0;
	}

	virtual int GetDrawType()
	{
		return m_DrawType;
	}

	bool TakeAnyCost(Role *pRole)
	{
		UINT32 allNum = pRole->Get<Bag>()->CountItemInBag(m_Cost.itemID);
		UINT32 count = allNum / m_Cost.itemCount;
		if (count > m_DropCount) count = m_DropCount;
		if (count == 0)
		{
			return false;
		}
		BagTakeItemTransition take(pRole);
		take.SetReason(ItemFlow_Lottory, ItemFlow_Lottory_Draw);
		if (!take.TakeItem(m_Cost.itemID, m_Cost.itemCount * count))
		{
			take.RollBack();
			return false;
		}
		take.NotifyClient();

		m_iCostCount = m_Cost.itemCount * count;
		m_canDrawCount = count;

		return true;
	}

	virtual bool TakeCost(Role *pRole)
	{
		LotteryRecord *pLR = pRole->Get<LotteryRecord>();
		if (m_freeFun)
		{
			if((pLR->*m_freeFun)())
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return TakeAnyCost(pRole);
		}
	}

	virtual std::vector<ItemDesc> DrawARaffle(Role *pRole)
	{
		LotteryRecord *pLR = pRole->Get<LotteryRecord>();
		std::vector<ItemDesc> result;
		for (int i = 0; i < m_canDrawCount; ++i)
		{
			const ItemDesc* desc = (pLR->*m_itemFun)();
			if(NULL != desc)
			{
				result.push_back(*desc);
			}
		}

		TLotteryFlowLog log(pRole);
		log.m_nLotteryType = m_DrawType;
		log.m_nVipLevel = pRole->Get<CPayV2Record>()->GetVipLevel();
		log.m_nMoney = m_iCostCount;
		log.Do();

		for (auto iter = result.begin(); iter != result.end(); ++iter)
		{
			ItemConf* pConf = ItemConfig::Instance()->GetItemConf(iter->itemID);
			if (pConf)
			{
				EventMgr::Instance()->AddGuildBonusEvent(pRole->GetID(), BONUS_EVENT_OFFSET + GUILDBONUS_COM_TYPE_DRAWLOTTERY_TEN, pConf->ItemQuality);
				PopWindows::Instance()->CheckComment(pRole, KKSG::COMMENT_SPRITE,pConf->ItemQuality,pConf->ItemID);
			}
		}
		return result;
	}

private:
	int m_DrawType;
	int m_DropCount;
	CheckFreeFun m_freeFun;
	//GetDropCallfun m_dropFun;
	GetItemCallFun m_itemFun;
	DrawCost m_Cost;
	int m_iCostCount;
	UINT32 m_canDrawCount;
};


INSTANCE_SINGLETON(LotteryMgr)

LotteryMgr::LotteryMgr()
{

}

LotteryMgr::~LotteryMgr()
{

}

bool LotteryMgr::Init()
{
	static DrawCost DrawOneCost = {GetGlobalConfig().SpriteDrawCost[0], GetGlobalConfig().SpriteDrawCost[1]};
	//static DrawCost DrawTenCost[] = {{GetGlobalConfig().SpriteTenDrawCost[0], GetGlobalConfig().SpriteTenDrawCost[1]}};
	static DrawCost GoldDrawOneCost = {GetGlobalConfig().SpriteGoldDrawCost[0], GetGlobalConfig().SpriteGoldDrawCost[1]};
	//static DrawCost GoldDrawTenCost[] = {{GetGlobalConfig().SpriteGoldTenDrawCost[0], GetGlobalConfig().SpriteGoldTenDrawCost[1]}};

	/*RegistDrawHandler(new CommonDrawPrizeHandler(
		(int)KKSG::Sprite_Draw_One_Free, 1, 
		&LotteryRecord::DragonCoinBuyCheckFree,
		&LotteryRecord::GetDragonCoinOneItemID, 
		DrawOneCost, ArraySize(DrawOneCost)));*/

	RegistDrawHandler(new CommonDrawPrizeHandler(
		(int)KKSG::Sprite_Draw_One, 1,
		NULL,
		&LotteryRecord::GetDragonCoinOneItemID,
		DrawOneCost));

	RegistDrawHandler(new CommonDrawPrizeHandler(
		(int)KKSG::Sprite_Draw_Ten, 10, 
		NULL, 
		&LotteryRecord::GetDragonCoinTenItemID,
		DrawOneCost));

	/*RegistDrawHandler(new CommonDrawPrizeHandler(
		(int)KKSG::Sprite_GoldDraw_One_Free, 1,
		&LotteryRecord::GoldBuyCheckFree,
		&LotteryRecord::GetGoldOneDrawItemID,
		GoldDrawOneCost, ArraySize(GoldDrawOneCost)));*/

	RegistDrawHandler(new CommonDrawPrizeHandler(
		(int)KKSG::Sprite_GoldDraw_One, 1,
		NULL,
		&LotteryRecord::GetGoldOneDrawItemID,
		GoldDrawOneCost));

	RegistDrawHandler(new CommonDrawPrizeHandler(
		(int)KKSG::Sprite_GoldDraw_Ten, 10,
		NULL,
		&LotteryRecord::GetGoldTenDrawItemID,
		GoldDrawOneCost));

	return LoadFile();
}

void LotteryMgr::Uninit()
{
	foreach (i in m_Handlers)
	{
		delete (*i);
	}

	m_Handlers.clear();
}

bool LotteryMgr::CheckFile()
{
	ShareRandomGift tmpShareGift;
	if (!tmpShareGift.LoadFile("table/ShareRandomGift.txt"))
	{
		LogWarn("Load ShareRandomGift.txt error");
		return false;
	}
	return true;
}

bool LotteryMgr::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}
	
	ClearFile();

	m_oShareGift.LoadFile("table/ShareRandomGift.txt");
	for (auto i = m_oShareGift.Table.begin(); i != m_oShareGift.Table.end(); i++)
	{
		m_shareGiftMap[(*i)->SceneType].push_back(*i);
	}

	return true;
}

void LotteryMgr::ClearFile()
{
	m_oShareGift.Clear();
	m_shareGiftMap.clear();
}

void LotteryMgr::GiveShareRandomGift(Role* pRole, UINT32 sceneType)
{
	if (pRole == NULL)
	{
		return;
	}
	
	// 系统是否已开
	if(!pRole->Get<CRoleSystem>()->IsSystemOpened(SYS_PLATFORMSHARE_BOX))
	{
		return;
	}

	auto iter = m_shareGiftMap.find(sceneType);
	if (iter == m_shareGiftMap.end())
	{
		return;
	}

	if (!pRole->Get<LotteryRecord>()->IsHaveShareGiftCount())
	{
		LogDebug("role [%llu] share gif count is over", pRole->GetID());
		return;
	}

	UINT32 prob = XRandom::randInt(1, 100 + 1);
	if (prob > GetGlobalConfig().ShareGiftProb)
	{
		LogDebug("role [%llu] share gif XRandom is %d", pRole->GetID(),prob);
		return;
	}

	const std::vector<ShareRandomGiftConf*>& vec = iter->second;
	UINT32 allProb = 0;
	for (auto i = vec.begin(); i != vec.end(); i++)
	{
		allProb += (*i)->Prob;
	}
	if (allProb == 0)
	{
		return;
	}
	prob = XRandom::randInt(1, allProb + 1);
	UINT32 count = 0;
	for (auto i = vec.begin(); i != vec.end(); i++)
	{
		count += (*i)->Prob;
		if (count >= prob)
		{	
			PtcG2C_ShareRandomGiftNtf oPtc;
			oPtc.m_Data.set_id((*i)->ID);			
			pRole->Send(oPtc);

			LogInfo("role [%llu] get share random gift, id [%u]", pRole->GetID(), (*i)->ID);

			break;
		}
	}	
}

bool LotteryMgr::TakeCost(int type, Role *pRole)
{
	IDrawHandler *pHandler = FindHandler(type);
	if (pHandler == NULL)
	{
		return false;
	}

	return pHandler->TakeCost(pRole);
}

std::vector<ItemDesc> LotteryMgr::DoDrawARaffle(int type, Role *pRole, std::vector<UINT32>& ppt)
{
	IDrawHandler *pHandler = FindHandler(type);
	if (pHandler == NULL)
	{
		return std::vector<ItemDesc>();
	}
	LotteryRecord* pRecord = pRole->Get<LotteryRecord>();
	pRecord->ClearLastLotteryItem();

	std::vector<ItemDesc> result = pHandler->DrawARaffle(pRole);
	BagGiveItemTransition transition(pRole);
	transition.SetReason(ItemFlow_Sprite, ItemFlow_SpriteLottery);
	for (unsigned i = 0; i < result.size(); ++i)
	{
		transition.GiveItem(result[i]);
		result[i].isbind = ItemConfig::Instance()->GetBind(result[i].itemID, result[i].isbind);

		ItemConf* conf = ItemConfig::Instance()->GetItemConf(result[i].itemID);
		if(NULL != conf && SPRITE == conf->ItemType)
		{
			if(conf->ItemQuality >= QualityIII)
			{
				pRecord->AddLotteryItem(result[i].itemID);
			}
			ppt.push_back(pRole->Get<CSpriteRecord>()->GetLastAddNewSpritePPT());
		}
	}
	transition.NotifyClient();

	return result;
}

void LotteryMgr::RegistDrawHandler(IDrawHandler *handler)
{
	m_Handlers.push_back(handler);
}

IDrawHandler * LotteryMgr::FindHandler(int type)
{
	foreach(i in m_Handlers)
	{
		if ((*i)->GetDrawType() == type)
		{
			return *i;
		}
	}

	return NULL;
}

