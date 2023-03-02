#include "pch.h"
#include <time.h>
#include "lotteryrecord.h"
#include "pb/project.pb.h"
#include "globalconfig.h"
#include "util/XCommon.h"
#include "unit/role.h"
#include "itemconfig.h"
#include "rolefashion.h"
#include "define/systemiddef.h"
#include "config/itemdropconfig.h"
#include "util/timespecificmgr.h"
#include "gamelogic/payv2Mgr.h"
#include "table/ProfessionMgr.h"
#include "util/gametime.h"
#include "table/globalconfig.h"


PandoraDropTimes::PandoraDropTimes()
{
	betterDropTimes = 0;
	nextBetterDropTimes = 0;
}

PandoraDropTimes::PandoraDropTimes(const KKSG::PandoraDrop& pandoraDrop, const PandoraDropConf& conf)
{
	betterDropTimes = pandoraDrop.betterdroptimes();
	nextBetterDropTimes = pandoraDrop.nextbetterdroptimes();
	if (pandoraDrop.bestdroptimes_size() == 0 || pandoraDrop.nextbestdroptimes_size() == 0)
	{
		bestDropTimes.push_back(pandoraDrop.nousedbestdroptimes());
		nextBestDropTimes.push_back(pandoraDrop.nousednextbestdroptimes());
	}
	else
	{
		for (int i = 0; i < pandoraDrop.bestdroptimes_size(); ++i)
		{
			if (i >= pandoraDrop.nextbestdroptimes_size())
			{
				break;
			}
			bestDropTimes.push_back(pandoraDrop.bestdroptimes(i));
			nextBestDropTimes.push_back(pandoraDrop.nextbestdroptimes(i));
		}
	}
	CheckInit(conf);
}

void PandoraDropTimes::Init(const PandoraDropConf& conf)
{
	betterDropTimes = 0;
	nextBetterDropTimes = XPandoraRandom::randInt(conf.EnterBetterDropTimes[0], conf.EnterBetterDropTimes[1] + 1);

	bestDropTimes.clear();
	nextBestDropTimes.clear();
	for (auto i = conf.EnterBestDropTimes.begin(); i != conf.EnterBestDropTimes.end(); ++i)
	{
		bestDropTimes.push_back(0);
		nextBestDropTimes.push_back(XPandoraRandom::randInt((*i)[0], (*i)[1] + 1));
	}
}

void PandoraDropTimes::CheckInit(const PandoraDropConf& conf)
{
	if (bestDropTimes.size() != conf.BestDropID.size())
	{
		std::vector<UINT32> newBestDropTimes;
		for (UINT32 i = 0; i < conf.BestDropID.size(); ++i)
		{
			if (i < bestDropTimes.size())
			{
				newBestDropTimes.push_back(bestDropTimes[i]);
			}
			else
			{
				newBestDropTimes.push_back(0);
			}
		}
		newBestDropTimes.swap(bestDropTimes);
	}
	if (nextBestDropTimes.size() != conf.EnterBestDropTimes.size())
	{
		std::vector<UINT32> newNextBestDropTimes;
		for (UINT32 i = 0; i < conf.EnterBestDropTimes.size(); ++i)
		{
			if (i < nextBestDropTimes.size() 
				&& nextBestDropTimes[i] >= conf.EnterBestDropTimes[i][0]
			&& nextBestDropTimes[i] <= conf.EnterBestDropTimes[i][1])
			{
				newNextBestDropTimes.push_back(nextBestDropTimes[i]);
			}
			else
			{
				UINT32 dropTimes = XPandoraRandom::randInt(conf.EnterBestDropTimes[i][0], conf.EnterBestDropTimes[i][1] + 1);
				newNextBestDropTimes.push_back(dropTimes);
			}
		}
		newNextBestDropTimes.swap(nextBestDropTimes);
	}
}


LotteryRecord::LotteryRecord(Role* pRole):m_pRole(pRole)
{
	m_DragonCoin_1_FreeLastTime = 0;
	m_DragonCoin_1_DrawCount = 0;
	m_DragonCoin_10_MinimumRewardCount = 0;
	m_Gold_1_DrawCount = 0;
	m_Gold_1_FreeLastTime = 0;
	m_Gold_1_FreeCount = 0;
	m_Gold_1_FreeDay = 0;
	m_Gold_10_MinimumRewardCount = 0;	

	m_nextFreeBuyTime = (UINT32)(-1);
	m_nextFreeGoldBuyTime = (UINT32)(-1);
	
	m_GoldClickDay = 0;
	m_GoldClickFreeTime = 0;
	m_GoldClickFreeCount = 0;
	m_GoldClickCostCount = 0;

	m_lastGiftUpdateTime = 0;
	m_shareGiftCount = 0;

	m_spriteNextMinGuarantee = 0;
	m_spriteNextMinGuarantee = XRandom::randInt(GetGlobalConfig().SpriteMinGuaranteeCount[0], GetGlobalConfig().SpriteMinGuaranteeCount[1] + 1);

	m_isModify.Set();
}

LotteryRecord::~LotteryRecord()
{
}

void LotteryRecord::FirstInit(UINT32 roleNum)
{
	m_DragonCoin_1_FreeLastTime = 0;
	m_DragonCoin_1_DrawCount = 0;
	m_DragonCoin_10_MinimumRewardCount = XRandom::randInt(GetGlobalConfig().SpriteDrawGoodRange[0], GetGlobalConfig().SpriteDrawGoodRange[1] + 1);
	m_Gold_10_MinimumRewardCount = XRandom::randInt(GetGlobalConfig().SpriteGoldDrawGoodRange[0], GetGlobalConfig().SpriteGoldDrawGoodRange[1] + 1);
	m_isModify.Set();
}

bool LotteryRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	const KKSG::RoleLotteryInfo &roLottery = poRoleAllInfo->lottery();
	m_DragonCoin_1_FreeLastTime = roLottery.lastdrawtime();
	m_DragonCoin_1_DrawCount = roLottery.onedrawcount();
	m_DragonCoin_10_MinimumRewardCount = roLottery.minimumrewardcount();

	m_Gold_1_FreeLastTime = roLottery.goldfreedrawtime();
	m_Gold_1_FreeCount = roLottery.goldfreedrawcount();
	m_Gold_1_FreeDay = roLottery.goldfreedrawday();

	m_Gold_1_DrawCount = roLottery.goldonedrawcount();
	m_Gold_10_MinimumRewardCount = roLottery.goldminimumrewardcount();

	if(m_DragonCoin_1_FreeLastTime != 0 && m_Gold_1_FreeLastTime != 0) //开启系统才会设置这个时间
	{
		UpdateNextFreeBuyTime();
	}

	m_GoldClickDay = roLottery.clickday();
	m_GoldClickFreeTime = roLottery.clickfreetime();
	m_GoldClickFreeCount = roLottery.clickfreecount();
	m_GoldClickCostCount = roLottery.clickcostcount();

	UINT32 prof = ProfessionMgr::Instance()->GetBasicProfession(m_pRole->GetProfession());
	for(int i = 0; i < roLottery.pandora_size(); i++)
	{
		const KKSG::PandoraDrop& pandora = roLottery.pandora(i);
		const PandoraDropData* data = PandoraConfig::Instance()->GetPandoraDropData(pandora.pandoradropid(), prof);
		if (data != NULL)
		{
			m_mapPandora.insert(std::make_pair(pandora.pandoradropid(), PandoraDropTimes(pandora, *data->conf)));
		}
	}

	m_lastGiftUpdateTime = roLottery.lastgiftupdatetime();
	m_shareGiftCount = roLottery.sharegiftcount();
	m_spriteMinGuarantee = roLottery.spriteminguarantee();
	if (roLottery.has_spritenextminguarantee()) m_spriteNextMinGuarantee = roLottery.spritenextminguarantee();
	else m_spriteNextMinGuarantee = XRandom::randInt(GetGlobalConfig().SpriteMinGuaranteeCount[0], GetGlobalConfig().SpriteMinGuaranteeCount[1] + 1);

	m_isModify.Set();

	return true;
}

void LotteryRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (!m_isModify.TestAndReset())
	{
		return;
	}

	KKSG::RoleLotteryInfo *pLottery = poRoleAllInfo->mutable_lottery();
	pLottery->Clear();

	pLottery->set_lastdrawtime(m_DragonCoin_1_FreeLastTime);
	pLottery->set_onedrawcount(m_DragonCoin_1_DrawCount);
	pLottery->set_minimumrewardcount(m_DragonCoin_10_MinimumRewardCount);

	pLottery->set_goldfreedrawtime(m_Gold_1_FreeLastTime);
	pLottery->set_goldfreedrawcount(m_Gold_1_FreeCount);
	pLottery->set_goldfreedrawday(m_Gold_1_FreeDay);

	pLottery->set_goldonedrawcount(m_Gold_1_DrawCount);
	pLottery->set_goldminimumrewardcount(m_Gold_10_MinimumRewardCount);

	pLottery->set_clickday(m_GoldClickDay);
	pLottery->set_clickfreetime(m_GoldClickFreeTime);
	pLottery->set_clickfreecount(m_GoldClickFreeCount);
	pLottery->set_clickcostcount(m_GoldClickCostCount);

	for (auto i = m_mapPandora.begin(); i != m_mapPandora.end(); i++)
	{
		KKSG::PandoraDrop* pandora = pLottery->add_pandora();
		pandora->set_pandoradropid(i->first);
		pandora->set_betterdroptimes(i->second.betterDropTimes);
		pandora->set_nextbetterdroptimes(i->second.nextBetterDropTimes);
		for (auto j = i->second.bestDropTimes.begin(); j != i->second.bestDropTimes.end(); ++j)
		{
			pandora->add_bestdroptimes(*j);
		}	
		for (auto j = i->second.nextBestDropTimes.begin(); j != i->second.nextBestDropTimes.end(); ++j)
		{
			pandora->add_nextbestdroptimes(*j);
		}
	}

	pLottery->set_lastgiftupdatetime(m_lastGiftUpdateTime);
	pLottery->set_sharegiftcount(m_shareGiftCount);

	pLottery->set_spriteminguarantee(m_spriteMinGuarantee);
	pLottery->set_spritenextminguarantee(m_spriteNextMinGuarantee);

	roChanged.insert(pLottery);
}

void LotteryRecord::UpdatePerSecond()
{
	time_t now = GameTime::GetTime();
	///> 摇钱树
	UpdateGoldClick(now);

	if((UINT32)now >= m_nextFreeBuyTime && (UINT32)now <= m_nextFreeBuyTime + 2)
	{
		HintNotify();
		return;
	}
	if((UINT32)now >= m_nextFreeGoldBuyTime && (UINT32)now <= m_nextFreeGoldBuyTime + 2)
	{
		HintNotify();
		return;
	}
}

//抽奖系统开启后要有cd
void LotteryRecord::OnSystemOpened(UINT32 type)
{
	///> 精灵商店购买
	if (1 == type)
	{
		m_DragonCoin_1_FreeLastTime = GameTime::GetTime() + GetGlobalConfig().GetInt("SpriteDrawFreeInitialCD", 6000) - GetGlobalConfig().GetInt("SpriteDrawFreeCD", 6000);
		m_Gold_1_FreeLastTime = GameTime::GetTime() + GetGlobalConfig().GetInt("SpriteGoldDrawFreeInitialCD", 6000) - GetGlobalConfig().GetInt("SpriteGoldDrawFreeCD", 6000);;

		m_isModify.Set();
		UpdateNextFreeBuyTime();
	}
	///> 摇钱树
	else if (2 == type)
	{
		if (0 == m_GoldClickFreeTime)
		{
			SetNextFreeTime();
		}
	}
}

void LotteryRecord::UpdateNextFreeBuyTime()
{
	UINT32 nowTime = GameTime::GetTime();
	UINT32 cool = GetDragonCoinFreeBuyCoolDown();
	if(cool > 0)
	{
		m_nextFreeBuyTime = nowTime + cool;
		m_isModify.Set();
	}
	
	cool = GetGoldFreeBuyCoolDown();
	if((UINT32)(-1) == cool)
	{
		UINT32 nextDayBegin = XCommon::GetDayBeginTime(nowTime) + 3600*24;
		UINT32 nextTime = GetGlobalConfig().GetInt("SpriteGoldDrawFreeCD", 6000) + m_Gold_1_FreeLastTime;
		m_nextFreeGoldBuyTime = nextTime > nextDayBegin ? nextTime : nextDayBegin;
		m_isModify.Set();
	}
	else if(cool > 0)
	{
		m_nextFreeGoldBuyTime = nowTime + cool;
		m_isModify.Set();
	}
}

UINT32 LotteryRecord::GetDragonCoinFreeBuyCoolDown()
{
	UINT32 nowTime = GameTime::GetTime();
	int COOLDOWN = GetGlobalConfig().GetInt("SpriteDrawFreeCD", 6000);
	int leftCoolDown = COOLDOWN - (int)(nowTime - m_DragonCoin_1_FreeLastTime);
	if (leftCoolDown < 0)
	{
		leftCoolDown = 0;
	}
	return leftCoolDown;
}

UINT32 LotteryRecord::GetGoldFreeBuyCoolDown()
{
	UINT32 nowTime = GameTime::GetTime();
	int COOLDOWN = GetGlobalConfig().GetInt("SpriteGoldDrawFreeCD", 6000);
	int leftCoolDown = COOLDOWN - (int)(nowTime - m_Gold_1_FreeLastTime);
	int day = XCommon::GetTodayUnique();
	if ((int)m_Gold_1_FreeDay == day)
	{
		if ((int)m_Gold_1_FreeCount == GetGlobalConfig().GetInt("SpriteGoldDrawFreeDayCount", 5))
		{
			return (UINT32)(-1);
		}
	}
	else
	{
		m_Gold_1_FreeDay = day;
		m_Gold_1_FreeCount = 0;

		m_isModify.Set();
	}	

	if (leftCoolDown > 0)
	{
		return leftCoolDown;
	}

	return 0;
}

UINT32 LotteryRecord::GetGoldFreeBuyCount()
{
	return m_Gold_1_FreeCount;
}

void LotteryRecord::ClearCoolDown()
{
	const int coincd = GetGlobalConfig().GetInt("SpriteDrawFreeCD", 6000);
	m_DragonCoin_1_FreeLastTime = (UINT32)(GameTime::GetTime() - coincd + 10);

	const int dragoncd = GetGlobalConfig().GetInt("SpriteGoldDrawFreeCD", 6000);
	m_Gold_1_FreeLastTime = (UINT32)(GameTime::GetTime() - dragoncd + 10);

	m_isModify.Set();
}

void LotteryRecord::HintNotify()
{
	if (0 == m_DragonCoin_1_FreeLastTime || 0 == m_Gold_1_FreeLastTime)
	{
		return; //系统还未开启
	}
	UINT32 cool = GetDragonCoinFreeBuyCoolDown();
	UINT32 goldCool = GetGoldFreeBuyCoolDown();
	if (0 == cool || 0 == goldCool)
	{
		m_pRole->HintNotify(SYS_SPRITE_SHOP, false);
	}
}

const ItemDesc* LotteryRecord::GetDragonCoinOneItemID()
{
	UINT32 dropid;

	if (m_DragonCoin_1_DrawCount == 0) //第一次抽奖特殊
	{
		++m_DragonCoin_1_DrawCount;
		dropid = GetGlobalConfig().GetInt("FirstSpriteDrawItem", 0);
		--m_DragonCoin_10_MinimumRewardCount;
	}
	else
	{
		return GetDragonCoinTenItemID();
	}

	const ItemDesc* pDrop = ItemDropConfig::Instance()->GetRandomDrop(dropid, 0, SpriteRandom);

	m_isModify.Set();

	return pDrop;
}

const ItemDesc* LotteryRecord::GetDragonCoinTenItemID()
{
	++m_spriteMinGuarantee;
	UINT32 dropid;

	if ((int)m_DragonCoin_10_MinimumRewardCount <= 0)
	{
		dropid = GetGlobalConfig().GetInt("SpriteDrawGoodDropID", 0);
		m_DragonCoin_10_MinimumRewardCount = XRandom::randInt(GetGlobalConfig().SpriteDrawGoodRange[0], GetGlobalConfig().SpriteDrawGoodRange[1] + 1);
	}
	else
	{
		if (m_spriteMinGuarantee >= m_spriteNextMinGuarantee)
		{
			dropid = GetGlobalConfig().GetInt("SpriteMinGuaranteeDropID", 0);
			LogInfo("Role [%llu] sprite lottery get mininum guarantee, lottery count [%u]", m_pRole->GetID(), m_spriteMinGuarantee);
			m_spriteMinGuarantee = 0;
			m_spriteNextMinGuarantee = XRandom::randInt(GetGlobalConfig().SpriteMinGuaranteeCount[0], GetGlobalConfig().SpriteMinGuaranteeCount[1] + 1);
		}
		else
		{
			dropid = GetGlobalConfig().GetInt("SpriteDrawBadDropID", 0);
		}
		--m_DragonCoin_10_MinimumRewardCount;
	}

	const ItemDesc* pDrop = ItemDropConfig::Instance()->GetRandomDrop(dropid, 0, SpriteRandom);

	if (pDrop != NULL)
	{
		UINT32 quality = XItem::GetQuality(pDrop->itemID);
		if (quality != (UINT32)-1 && quality >= QualityIV)
		{
			m_spriteMinGuarantee = 0;
			m_spriteNextMinGuarantee = XRandom::randInt(GetGlobalConfig().SpriteMinGuaranteeCount[0], GetGlobalConfig().SpriteMinGuaranteeCount[1] + 1);
		}
	}

	m_isModify.Set();

	return pDrop;
}

const ItemDesc* LotteryRecord::GetGoldOneDrawItemID()
{
	UINT32 dropid;

	if (m_Gold_1_DrawCount == 0)
	{
		++m_Gold_1_DrawCount;
		dropid = GetGlobalConfig().GetInt("FirstSpriteGoldDrawItem", 0);
		--m_Gold_10_MinimumRewardCount;
	}
	else if (m_Gold_1_DrawCount == 1)
	{
		++m_Gold_1_DrawCount;
		dropid = GetGlobalConfig().GetInt("SecondSpriteGoldDrawItem", 0);
		--m_Gold_10_MinimumRewardCount;
	}
	else
	{
		return GetGoldTenDrawItemID();
	}

	const ItemDesc* pDrop = ItemDropConfig::Instance()->GetRandomDrop(dropid);

	m_isModify.Set();

	return pDrop;
}

const ItemDesc* LotteryRecord::GetGoldTenDrawItemID()
{
	UINT32 dropid;
	if((int)m_Gold_10_MinimumRewardCount <= 0)
	{	
		dropid =  GetGlobalConfig().GetInt("SpriteGoldDrawGoodDropID", 0);
		m_Gold_10_MinimumRewardCount = XRandom::randInt( GetGlobalConfig().SpriteGoldDrawGoodRange[0],
			 GetGlobalConfig().SpriteGoldDrawGoodRange[1] + 1);
	}
	else
	{	
		dropid =  GetGlobalConfig().GetInt("SpriteGoldDrawBadDropID", 0);
		--m_Gold_10_MinimumRewardCount;
	}

	const ItemDesc *pDrop = ItemDropConfig::Instance()->GetRandomDrop(dropid);

	m_isModify.Set();

	return pDrop;
}

bool LotteryRecord::DragonCoinBuyCheckFree()
{
	if (GetDragonCoinFreeBuyCoolDown() == 0)
	{
		///> 要抽奖的时候才会check free
		m_DragonCoin_1_FreeLastTime = GameTime::GetTime();

		UpdateNextFreeBuyTime();

		m_isModify.Set();
		return true;
	}

	return false;
}

bool LotteryRecord::GoldBuyCheckFree()
{
	if (GetGoldFreeBuyCoolDown() == 0)
	{
		///> 要抽奖的时候才会check free
		++m_Gold_1_FreeCount;
		m_Gold_1_FreeLastTime = GameTime::GetTime();

		UpdateNextFreeBuyTime();

		m_isModify.Set();
		return true;
	}

	return false;
}

void LotteryRecord::ClearLastLotteryItem()
{
	m_lastLotteryGoodItem.clear();
}

void LotteryRecord::AddLotteryItem(UINT32 itemid)
{
	m_lastLotteryGoodItem.insert(itemid);
}

bool LotteryRecord::IsHaveAndDelete(UINT32 itemid)
{
	auto i = m_lastLotteryGoodItem.find(itemid);
	if (i != m_lastLotteryGoodItem.end())
	{
		m_lastLotteryGoodItem.erase(i);
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////
UINT32 LotteryRecord::GetAllFreeCount()
{
	return GetGlobalConfig().GoldClickFreeLimit;
}

UINT32 LotteryRecord::GetAllCount()
{
	return PayV2Mgr::Instance()->GetGoldClickCount(m_pRole->Get<CPayV2Record>()->GetVipLevel());
}

UINT32 LotteryRecord::GetFreeTimeLeft()
{
	if (0 == m_GoldClickFreeTime || CountLimit(true))
	{
		return (UINT32)(-1);
	}
	UINT32 nowtime = GameTime::GetTime();
	return nowtime >= m_GoldClickFreeTime ? 0 : m_GoldClickFreeTime - nowtime;
}

bool LotteryRecord::CountLimit(bool isfree)
{
	// count limit
	UINT32 count = GetAllCount();
	if (m_GoldClickCostCount + m_GoldClickFreeCount >= count)
	{
		return true;
	}
	// free count limit
	if (isfree)
	{
		if (m_GoldClickFreeCount >= GetAllFreeCount())
		{
			return true;
		}
	}
	return false;
}

void LotteryRecord::UpdateGoldClick(time_t nowtime)
{
	///> 未开启
	if (0 == m_GoldClickFreeTime)
	{
		return;
	}
	UpdateGoldClickDay();

	if ((UINT32)nowtime >= m_nextFreeBuyTime && (UINT32)nowtime <= m_nextFreeBuyTime + 1)
	{
		HintNotifyGoldClick();
	}
}

void LotteryRecord::UpdateGoldClickDay()
{
	UINT32 day = XCommon::GetTodayUnique();
	if (m_GoldClickDay != day)
	{
		m_GoldClickFreeCount = 0;
		m_GoldClickCostCount = 0;
		
		///> 跨天立即可以免费
		SetNextFreeTime();

		m_GoldClickDay = day;

		m_isModify.Set();
	}
}

void LotteryRecord::HintNotifyGoldClick()
{
	if (CountLimit(true))
	{
		return;
	}
	if (CanFree())
	{
		m_pRole->HintNotify(SysGoldClick, false);
	}
}

bool LotteryRecord::CanFree()
{
	return (0 != m_GoldClickFreeTime && GameTime::GetTime() >= m_GoldClickFreeTime);
}

void LotteryRecord::UpdateGoldClickNextFreeTime()
{
	m_GoldClickFreeTime = GameTime::GetTime() + GetGlobalConfig().GoldClickFreeTime;
	m_isModify.Set();
}

void LotteryRecord::SetNextFreeTime()
{
	m_GoldClickFreeTime = GameTime::GetTime();
	HintNotifyGoldClick();

	m_isModify.Set();
}

void LotteryRecord::AddCount(bool isfree)
{
	if (isfree)
	{
		++m_GoldClickFreeCount;
	}
	else
	{
		++m_GoldClickCostCount;
	}

	m_isModify.Set();
}

const ItemDesc* LotteryRecord::GetPandoraDropItem(UINT32 pandoraDropID)
{
	std::vector<const ItemDesc*> vecItems;
	GetPandoraDropItem(pandoraDropID, 1, vecItems);
	if (vecItems.empty())
	{
		return NULL;
	}
	return *vecItems.begin();
}

void LotteryRecord::GetPandoraDropItem(UINT32 pandoraDropID, UINT32 openTimes, std::vector<const ItemDesc*>& vecItems)
{
	UINT32 profID = ProfessionMgr::Instance()->GetBasicProfession(m_pRole->GetProfession());

	const PandoraDropData* dropData = PandoraConfig::Instance()->GetPandoraDropData(pandoraDropID, profID);
	if (dropData == NULL)
	{
		return;
	}
	const PandoraDropConf* conf = dropData->conf;
	if (m_mapPandora.find(pandoraDropID) == m_mapPandora.end())
	{
		PandoraDropTimes pandoraDrop;
		pandoraDrop.Init(*conf);
		m_mapPandora.insert(std::make_pair(pandoraDropID, pandoraDrop));
	}
	PandoraDropTimes& dropTimes = m_mapPandora[pandoraDropID];
	///> 防止热加载表格后数据非法
	dropTimes.CheckInit(*conf);

	for (UINT32 i = 0; i < openTimes; ++i)
	{
		bool isBestDrop = false;
		for (UINT32 j = 0; j < dropTimes.bestDropTimes.size(); ++j)
		{
			if (dropTimes.bestDropTimes[j] >= dropTimes.nextBestDropTimes[j])
			{
				const ItemDesc* item = ItemDropConfig::Instance()->GetRandomDrop(conf->BestDropID[j], m_pRole->GetLevel(), PandoraRandom);
				if (item == NULL)
				{
					///> 没有随机到物品
					LogWarn("dropid:%u may get NULL in random, it's unfair to player", conf->BestDropID[j]);
					isBestDrop = true;
					break;
				}
				vecItems.push_back(item);
				
				_CheckPandoraDropTime(conf, 3, 3 + j, dropTimes);

				isBestDrop = true;
				break;
			}
		}
		if (isBestDrop)
		{
			continue;
		}
		if (dropTimes.betterDropTimes >= dropTimes.nextBetterDropTimes)
		{
			const ItemDesc* item = ItemDropConfig::Instance()->GetRandomDrop(conf->BetterDropID, m_pRole->GetLevel(), PandoraRandom);
			if (item == NULL)
			{
				///> 没有随机到物品
				LogWarn("dropid:%u may get NULL in random, it's unfair to player", conf->BetterDropID);
				continue;
			}
			vecItems.push_back(item);

			UINT32 itemQuality = PandoraConfig::Instance()->GetItemQuality(pandoraDropID, profID, *item);
			_CheckPandoraDropTime(conf, 2, itemQuality, dropTimes);

			continue;
		}
		const ItemDesc* item = ItemDropConfig::Instance()->GetRandomDrop(conf->CommonDropID, m_pRole->GetLevel(), PandoraRandom);
		if (item == NULL)
		{
			///> 没有随机到物品，默认是common drop
			_CheckPandoraDropTime(conf, 1, 1, dropTimes);
			continue;
		}
		vecItems.push_back(item);

		UINT32 itemQuality = PandoraConfig::Instance()->GetItemQuality(pandoraDropID, profID, *item);
		_CheckPandoraDropTime(conf, 1, itemQuality, dropTimes);
	}

	m_isModify.Set();
}

void LotteryRecord::_CheckPandoraDropTime(const PandoraDropConf* conf, UINT32 dropQuality, UINT32 itemQuality, PandoraDropTimes& dropTimes)
{
	std::string s1, s2;
	if (dropQuality == 3) s1 = "BestDrop";
	else if (dropQuality == 2) s1 = "BetterDrop";
	else s1 = "CommonDrop";

	CHECK_COND_NORETURN(dropTimes.bestDropTimes.size() == dropTimes.nextBestDropTimes.size() && dropTimes.bestDropTimes.size() == conf->BestDropID.size());
	if (itemQuality >= 3)
	{
		dropTimes.betterDropTimes = 0;
		dropTimes.nextBetterDropTimes = XPandoraRandom::randInt(conf->EnterBetterDropTimes[0], conf->EnterBetterDropTimes[1] + 1);
		if (dropTimes.bestDropTimes.size() <= itemQuality - 3)
		{
			LogError("PandoraDrop:%u, bestDropTimes size:%u <= itemQuality:%u - 3, code error!!", conf->PandoraDropID, dropTimes.bestDropTimes.size(), itemQuality);
			return;
		}
		dropTimes.bestDropTimes[itemQuality - 3] = 0;
		dropTimes.nextBestDropTimes[itemQuality - 3] = XPandoraRandom::randInt(conf->EnterBestDropTimes[itemQuality - 3][0], conf->EnterBestDropTimes[itemQuality - 3][1] + 1);
		s2 = "BestDrop " + ToString(itemQuality - 3);
	}
	else if (itemQuality == 2)
	{
		dropTimes.betterDropTimes = 0;
		dropTimes.nextBetterDropTimes = XPandoraRandom::randInt(conf->EnterBetterDropTimes[0], conf->EnterBetterDropTimes[1] + 1);
		for (UINT32 i = 0; i < dropTimes.bestDropTimes.size(); ++i)
		{
			++dropTimes.bestDropTimes[i];
		}

		s2 = "BetterDrop";
	}
	else
	{
		++dropTimes.betterDropTimes;
		for (UINT32 i = 0; i < dropTimes.bestDropTimes.size(); ++i)
		{
			++dropTimes.bestDropTimes[i];
		}
		s2 = "CommonDrop";
	}

	std::string s3;
	for (UINT32 i = 0; i < dropTimes.bestDropTimes.size(); ++i)
	{
		if (i != 0) s3 += ",";
		s3 += ToString(dropTimes.bestDropTimes[i]);
	}
	std::string s4;
	for (UINT32 i = 0; i < dropTimes.nextBestDropTimes.size(); ++i)
	{
		if (i != 0) s4 += ",";
		s4 += ToString(dropTimes.nextBestDropTimes[i]);
	}

	LogDebug("It's %s, get %s item, betterDropTimes:%u, bestDropTimes:%s, nextBetterTimes:%u, nextBestTimes:%s", 
		s1.c_str(), s2.c_str(), dropTimes.betterDropTimes, s3.c_str(), dropTimes.nextBetterDropTimes, s4.c_str());

	m_isModify.Set();
}

bool LotteryRecord::IsHaveShareGiftCount()
{
	UpdateShareGiftCount();
	return m_shareGiftCount < GetGlobalConfig().ShareGiftCountDay;
}

void LotteryRecord::AddShareGiftCount()
{
	UpdateShareGiftCount();
	m_shareGiftCount++;

	m_isModify.Set();
}

void LotteryRecord::UpdateShareGiftCount()
{
	time_t nowTime = GameTime::GetTime();
	if (!XCommon::IsDaySame(nowTime, m_lastGiftUpdateTime))
	{
		m_lastGiftUpdateTime = (UINT32)nowTime;
		m_shareGiftCount = 0;

		m_isModify.Set();
	}
}

void LotteryRecord::GMClearShareGiftCount()
{
	UpdateShareGiftCount();
	m_shareGiftCount = 0;

	m_isModify.Set();
}
