#include "pch.h"
#include "findbackrecord.h"
#include "itemfindback.h"
#include "pb/project.pb.h"
#include "util/XCommon.h"
#include "unit/role.h"
#include "table/globalconfig.h"
#include "findback/ptcg2c_expfindbackntf.h"
#include <time.h>
#include "findbackconfig.h"
#include "foreach.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/towerrecord.h"
#include "gamelogic/towerconfig.h"
#include "unit/systeminfo.h"
#include "findback/ptcg2c_itemfindbackntf.h"
#include "table/OpenSystemMgr.h"
#include "gamelogic/superriskrecord.h"
#include "gamelogic/levelsealMgr.h"
#include "util/gametime.h"
#include "define/systemiddef.h"
#include "itemfindbackbase.h"


int GetSysID(int id)
{
	int sysID = 0;
	auto data = FindBackConfig::Instance()->GetItemBackTableRowData(id);
	if (!data)
	{
		return 0;
	}
	sysID = data->SystemID;

	return sysID;
}

int GetFindBackBeginTime(Role* role, int id)
{
	int sysID = 0;
	sysID = GetSysID(id);

	int findBackOpen = role->Get<CFindBackRecord>()->GetItemFindBackInfo().GetOpenTime();
	if (!findBackOpen)
	{
		return 0;
	}

	int sysopen = role->Get<CRoleSystem>()->GetSystemOpenTime(sysID);

	if (!sysopen)
	{
		const std::vector<int>& vec = OpenSystemMgr::Instance()->OpendSystemWhenCreateRole();
		for (auto iter = vec.begin(); iter != vec.end(); ++iter)
		{
			if (*iter == sysID)
			{
				return XCommon::GetDayBeginTime(role->GetCreateTime());
			}
		}
		return 0;
	}

	int time = sysopen > findBackOpen ? sysopen : findBackOpen;
	if (!time)
	{
		return 0;
	}
	return XCommon::GetDayBeginTime(time);
}

int GetFindBackDays(int id, Role* role)
{
	int days = 0;
	auto data = FindBackConfig::Instance()->GetItemBackTableRowData(id, role->GetLevel());

	if (data)
	{
		days = data->FindBackDays;
	}

	return days;
}


BaseFindBackInfo::BaseFindBackInfo(int type, CItemFindBackInfo* info)
{
	m_type = type;
	m_pItemBack = info;
	m_usedCountMap = CreateOneDayBackInfo(XCommon::GetDayBeginTime(time(0)));
}

BaseFindBackInfo::~BaseFindBackInfo()
{
	for (auto iter = m_usedCountMap_h.begin(); iter != m_usedCountMap_h.end(); ++iter)
	{
		if (iter->second)
		{
			delete iter->second;
		}
	}

	m_usedCountMap_h.clear();

	delete m_usedCountMap;
}

bool BaseFindBackInfo::Load(const KKSG::RoleFindBackRecord* record)
{
	for (int i = 0; i < record->itemfindbackinfoshis_size(); ++i)
	{
		const KKSG::ItemFindBackInfo& info = record->itemfindbackinfoshis(i);
		if (info.id() == m_type)
		{
			auto data = FindBackConfig::Instance()->GetItemBackTableRowData(m_type);
			int recordTime = 0;
			if (!data)
			{
				continue;
			}
			if (data->IsWeekBack)
			{
				recordTime = GameTime::GetWeekBeginTime(info.daytime()) + 6 * 86400;
			}
			else
			{
				recordTime = info.daytime();
			}
			OneDayTypeInfo * oneDayInfo = NULL;
			auto find = m_usedCountMap_h.find(recordTime);
			if (find == m_usedCountMap_h.end())
			{
				oneDayInfo = CreateOneDayBackInfo(recordTime);
				//m_usedCountMap_h[info.daytime()] = oneDayInfo;
				m_usedCountMap_h[oneDayInfo->m_day] = oneDayInfo;
			}
			else
				oneDayInfo = find->second;

			oneDayInfo->m_alreadyFindBackCount = info.findbackcount();
			oneDayInfo->m_level = info.level();

			for (auto j = 0; j < info.oncebackexp_size(); ++j)
			{
				oneDayInfo->m_onceBackExp[info.oncebackexp(j).key()] = info.oncebackexp(j).value();
			}

			//auto findSubtype = oneDayInfo->m_subTypeInfos.find(info.subtype());
			//if (findSubtype != oneDayInfo->m_subTypeInfos.end())
			{
				oneDayInfo->m_subTypeInfos->m_todayUseCount = info.usecount();
				oneDayInfo->m_subTypeInfos->m_towerLevel = info.towerlevel();
			}
		}
	}
	for (int i = 0; i < record->itemfindbackinfocur_size(); ++i)
	{
		const KKSG::ItemFindBackInfo& info = record->itemfindbackinfocur(i);
		if (info.id() == m_type)
		{
			//auto findSubtype = m_usedCountMap->m_subTypeInfos.find(info.subtype());
			//if (findSubtype != m_usedCountMap->m_subTypeInfos.end())
			{
				m_usedCountMap->m_subTypeInfos->m_todayUseCount = info.usecount();
				m_usedCountMap->m_subTypeInfos->m_towerLevel = info.towerlevel();
			}
		}
	}

	return true;
}

OneDayTypeInfo * BaseFindBackInfo::CreateOneDayBackInfo(int day)
{
	return new OneDayTypeInfo(this, m_type, day);
}

void BaseFindBackInfo::Save(KKSG::RoleFindBackRecord* record)
{
	//for (auto iter = m_usedCountMap->m_subTypeInfos.begin(); iter != m_usedCountMap->m_subTypeInfos.end(); ++iter)
	{
		auto info = record->add_itemfindbackinfocur();
		info->set_id((KKSG::ItemFindBackType)m_type);
		info->set_usecount(m_usedCountMap->m_subTypeInfos->m_todayUseCount);
		info->set_towerlevel(m_usedCountMap->m_subTypeInfos->m_towerLevel);
		info->set_daytime(m_usedCountMap->m_day);
		info->set_findbackcount(m_usedCountMap->m_alreadyFindBackCount);
	}

	for (auto i = m_usedCountMap_h.begin(); i != m_usedCountMap_h.end(); ++i)
	{
		//for (auto iter = i->second->m_subTypeInfos.begin(); iter != i->second->m_subTypeInfos.end(); ++iter)
		{
			auto info = record->add_itemfindbackinfoshis();
			info->set_id((KKSG::ItemFindBackType)m_type);
			info->set_usecount(i->second->m_subTypeInfos->m_todayUseCount);
			info->set_towerlevel(i->second->m_subTypeInfos->m_towerLevel);
			info->set_daytime(i->second->m_day);
			info->set_findbackcount(i->second->m_alreadyFindBackCount);
			info->set_level(i->second->m_level);
			for (auto j = i->second->m_onceBackExp.begin(); j != i->second->m_onceBackExp.end(); ++j)
			{
				auto onceexp = info->add_oncebackexp();
				onceexp->set_key(j->first);
				onceexp->set_value(j->second);
			}
		}
	}
}

void BaseFindBackInfo::Update(int nowBegin, int infoBegin)
{
	std::vector<ItemBackTable::RowData*> tempInfo;
	auto& data = FindBackConfig::Instance()->GetItemBackTableData();
	for (auto iter = data.Table.begin(); iter != data.Table.end(); ++iter)
	{
		if ((*iter)->ID == m_type)
		{
			tempInfo.push_back(*iter);
		}
	}
	if (tempInfo.empty())
	{
		return;
	}

	if(!(*tempInfo.begin())->IsWeekBack)
	{
		_UpdatePerDay(nowBegin, infoBegin);
	}
	else
	{
		_UpdatePerWeek(nowBegin, infoBegin);
	}

	LogInfo("role:%llu, %s, type:%u, back days :%u", GetRole()->GetID(), GetRole()->GetName().c_str(), m_type, m_usedCountMap_h.size());
}

void BaseFindBackInfo::UpdateExtraSaveInfo(int day)
{
	if (!day)
	{
		for (auto iter = m_usedCountMap_h.begin(); iter != m_usedCountMap_h.end(); ++iter)
		{
			if (iter->second->m_onceBackExp.empty())
			{
				iter->second->UpdateExtraSaveInf();
			}
		}
	}
	else
	{
		auto find = m_usedCountMap_h.find(XCommon::GetDayBeginTime(day));
		if (find != m_usedCountMap_h.end())
		{
			find->second->UpdateExtraSaveInf();
		}
	}
}

UINT32 BaseFindBackInfo::CheckFindBackRewardAndCost(int type, std::map<int, int>& costs, std::map<int, int>& rewards)
{
	INT64 ownCostDragonCoinNum = 0;
	INT64 ownCostGoldNum = 0;
	INT64 ownCostTicketNum = 0;
	int costDragon = 0;
	int costGold = 0;
	int costTicket = 0;
	int exchangeAddGold = 0;

	int temptype = type;
	if (temptype == FindBack_Ticket)
	{
		temptype = FindBack_Dragon;
	}
	int cost = costs[temptype];

	costs.clear();

	if (type == FindBack_Dragon)
	{
		ownCostDragonCoinNum = GetRole()->Get<Bag>()->CountVirtualItem(DRAGON_COIN);
	}
	else if (type == FindBack_Gold)
	{
		ownCostGoldNum = GetRole()->Get<Bag>()->CountVirtualItem(GOLD);
	}
	else if (type == FindBack_Ticket)
	{
		ownCostTicketNum = GetRole()->Get<Bag>()->CountItemInBag(FINDBACK_TICKET);
	}

	if (type == FindBack_Dragon)
	{
		if (cost > ownCostDragonCoinNum)
		{
			return KKSG::ERR_ITEM_NOT_ENOUGH;
		}
		costDragon = cost;
	}
	else if (type == FindBack_Gold)
	{
		if (cost > ownCostGoldNum)
		{
			int needBuy = cost - ownCostGoldNum;
			UINT32 exchangeParam = GetGlobalConfig().GragonCoinExchangeGold > 0 ? GetGlobalConfig().GragonCoinExchangeGold : 1000;
			int needDragonCoin = needBuy / exchangeParam + (needBuy % exchangeParam > 0 ? 1 : 0);
			INT64 ownDragon = GetRole()->Get<Bag>()->CountVirtualItem(DRAGON_COIN);
			if (needDragonCoin > ownDragon)
			{
				return KKSG::ERR_ITEM_NOT_ENOUGH;
			}
			costDragon = needDragonCoin;
			exchangeAddGold = needDragonCoin * exchangeParam - needBuy;
			costGold = ownCostGoldNum;
		}
		else
		{
			costGold = cost;
		}
	}
	else if (type == FindBack_Ticket)
	{
		int ticketExchange = GetGlobalConfig().FindBackTicketExchangeDragon;
		if (!ticketExchange)
		{
			return KKSG::ERR_ITEM_NOT_ENOUGH;
		}
		if (cost > ownCostTicketNum * ticketExchange)
		{
			return KKSG::ERR_ITEM_NOT_ENOUGH;
		}
		costTicket = cost / ticketExchange + (cost % ticketExchange ? 1 : 0);
	}

	if (costDragon)
	{
		costs[DRAGON_COIN] = costDragon;
	}
	if (costGold)
	{
		costs[GOLD] = costGold;
	}
	if (costTicket)
	{
		costs[FINDBACK_TICKET] = costTicket;
	}
	if (exchangeAddGold)
	{
		rewards[GOLD] += exchangeAddGold;
	}

	return KKSG::ERR_SUCCESS;
}

void BaseFindBackInfo::OnOpenInit()
{
	int now = time(0);
	std::vector<ItemBackTable::RowData*> tempInfo;
	auto& data = FindBackConfig::Instance()->GetItemBackTableData();
	for (auto iter = data.Table.begin(); iter != data.Table.end(); ++iter)
	{
		if ((*iter)->ID == m_type)
		{
			tempInfo.push_back(*iter);
		}
	}
	if (tempInfo.empty())
	{
		return;
	}

	InitData(XCommon::GetDayBeginTime(now));
	//AddUseCount(1, 0, XCommon::GetDayBeginTime(now));
}
void BaseFindBackInfo::InitData(int dayBegin)
{
	AddUseCount(0, dayBegin);
}

void BaseFindBackInfo::_UpdatePerDay(int nowBegin, int infoBegin)
{
	if (nowBegin > infoBegin)
	{
		m_usedCountMap_h[infoBegin] = CreateOneDayBackInfo(infoBegin);
		(*m_usedCountMap_h[infoBegin]).assign(*m_usedCountMap);
		m_usedCountMap_h[infoBegin]->m_level = GetRole()->GetLevel();

		for(int i = 1; i <= GetFindBackDays(m_type, GetRole()); ++i)
		{
			if (m_usedCountMap_h.find(nowBegin - i * 86400) != m_usedCountMap_h.end())
			{
				continue;
			}
			if (nowBegin - i * 86400 < infoBegin)
			{
				continue;
			}
			
			InitData(nowBegin - i * 86400);
			m_usedCountMap_h[nowBegin - i * 86400]->m_level = GetRole()->GetLevel();
		}

		m_usedCountMap->Clear();

		InitData(nowBegin);
		m_usedCountMap->m_level = GetRole()->GetLevel();
	}

	for (auto iter = m_usedCountMap_h.begin(); iter != m_usedCountMap_h.end();)
	{
		if (nowBegin - iter->first > 86400 * GetFindBackDays(m_type, GetRole())/*MAX_ITEMFINDBACK_SAVEDAYS*/)
		{
			delete iter->second;
			m_usedCountMap_h.erase(iter++);
			continue;
		}
		else if(iter->first >= nowBegin)
		{
			delete iter->second;
			m_usedCountMap_h.erase(iter++);
			continue;
		}

		++iter;
	}

}

void BaseFindBackInfo::_UpdatePerWeek(int nowBegin, int infoBegin)
{
	if (XCommon::IsWeekSame(infoBegin, nowBegin))
	{
		return;
	}
	int nowWeekBegin = GameTime::GetWeekBeginTime(nowBegin);
	if (nowBegin > infoBegin)
	{
		int weekEnd = GameTime::GetWeekBeginTime(infoBegin) + 6 * 86400;
		m_usedCountMap_h[weekEnd] = CreateOneDayBackInfo(weekEnd);
		(*m_usedCountMap_h[weekEnd]).assign(*m_usedCountMap);
		m_usedCountMap_h[weekEnd]->m_level = GetRole()->GetLevel();

		for(int i = 1; i <= GetFindBackDays(m_type, GetRole())/*MAX_ITEMFINDBACK_SAVEDAYS*/; ++i)
		{
			int tempEnd = nowWeekBegin - i * 86400 * 7 + 6 * 86400;
			if (m_usedCountMap_h.find(tempEnd) != m_usedCountMap_h.end())
			{
				continue;
			}
			if (tempEnd < weekEnd)
			{
				continue;
			}
			
			InitData(tempEnd);
			m_usedCountMap_h[tempEnd]->m_level = GetRole()->GetLevel();
		}

		m_usedCountMap->Clear();

		InitData(nowWeekBegin + 6 * 86400);
		m_usedCountMap->m_level = GetRole()->GetLevel();
	}

	for (auto iter = m_usedCountMap_h.begin(); iter != m_usedCountMap_h.end();)
	{
		if (nowWeekBegin + 6 * 86400 - iter->first > 86400 * GetFindBackDays(m_type, GetRole()) * 7)
		{
			delete iter->second;
			m_usedCountMap_h.erase(iter++);
			continue;
		}
		else if(iter->first >= nowWeekBegin)
		{
			delete iter->second;
			m_usedCountMap_h.erase(iter++);
			continue;
		}

		++iter;
	}

}

void BaseFindBackInfo::DebugReset()
{
	ClearHis();
}

void BaseFindBackInfo::ClearHis(int dayBegin)
{
	if (!dayBegin)
	{
		foreach(i in m_usedCountMap_h)
		{
			delete i->second;
		}
		m_usedCountMap_h.clear();
	}
	else
	{
		for (auto iter = m_usedCountMap_h.begin(); iter != m_usedCountMap_h.end();)
		{
			if (iter->first < dayBegin)
			{
				delete iter->second;
				m_usedCountMap_h.erase(iter++);
			}
			else
				++iter;
		}
	}
}

void BaseFindBackInfo::ClearCur()
{
	m_usedCountMap->Clear();
}

Role* BaseFindBackInfo::GetRole()
{
	//return m_pItemBack->m_pRecord->GetRole();
	return m_pItemBack->GetCFindBackRecord()->GetRole();
}

UINT32 BaseFindBackInfo::CalcFindBackRewardAndCost(int type, int count, std::map<int, int>& costs, std::map<int, int>& rewards)
{
	int costDragon = 0;
	int costGold = 0;
	int exchangeAddGold = 0;
	int cost = 0;
	int totoalCount = count;
	for (auto iter = m_usedCountMap_h.rbegin(); iter != m_usedCountMap_h.rend() && totoalCount > 0; ++iter)
	{
		int leftcount = iter->second->GetLeftFindBackCount(GetRole());
		if (!leftcount)
		{
			continue;
		}
		if (leftcount > totoalCount)
		{
			leftcount = totoalCount;
		}
		std::map<int, int> dragonItems;
		std::map<int, int> goldItems;
		int goldCost = 0;
		int dragonCost = 0;
		if (iter->second->GetOnceFindBackCost(GetRole(), dragonItems, goldItems, dragonCost, goldCost))
		{
			if (type == FindBack_Dragon)
			{
				foreach(i in dragonItems)
				{
					rewards[i->first] += i->second * leftcount;
				}
				cost += dragonCost * leftcount;
			}
			if (type == FindBack_Gold)
			{
				foreach(i in goldItems)
				{
					rewards[i->first] += i->second * leftcount;
				}
				cost += goldCost * leftcount;
			}
		}
		totoalCount -= leftcount;
	}

	costs[type] = cost;

	return KKSG::ERR_SUCCESS;
}

UINT32 BaseFindBackInfo::GiveFindBackReward(int type, int count)
{
	int ownCount = GetCanFindBackCount();
	if (count > ownCount)
	{
		return KKSG::ERR_FAILED;
	}

	std::map<int, int> costs;
	std::map<int, int> rewards;

	int temptype = type;
	if (temptype == FindBack_Ticket)
	{
		temptype = FindBack_Dragon;
	}
	UINT32 error = CalcFindBackRewardAndCost(temptype, count, costs, rewards);
	if (error != KKSG::ERR_SUCCESS)
	{
		return error;
	}

	error = CheckFindBackRewardAndCost(type, costs, rewards);
	if (error != KKSG::ERR_SUCCESS)
	{
		return error;
	}

	BagTakeItemTransition takeTransition(GetRole());
	takeTransition.SetReason(ItemFlow_ItemFindBack, ItemFlow_ItemFindBackCost);

	for (auto iter = costs.begin(); iter != costs.end(); ++iter)
	{
		if(!takeTransition.TakeItem(iter->first, iter->second))
		{
			takeTransition.RollBack();
			return KKSG::ERR_FAILED;
		}
	}
	
	takeTransition.NotifyClient();

	GiveRewards(rewards);

	//AddFindBackCount(useCountMap);
	AddAlreadyFindBackCount(count);

	LogInfo("role:%llu, get findback reward,%d, type:%u, count:%u", GetRole()->GetID(), m_type, type, count);

	return KKSG::ERR_SUCCESS;
}


UINT32 BaseFindBackInfo::GiveRewards(std::map<int, int>& rewards)
{
	BagGiveItemTransition transition(GetRole());
	transition.SetReason(ItemFlow_ItemFindBack, ItemFlow_ItemFindBackGet);
	if (m_usedCountMap_h.empty())
	{
		return KKSG::ERR_FAILED;
	}

	//DealWithAddExp dealExp(GetRole(), m_usedCountMap_h.begin()->second->m_level);
	transition.SetIsRealExp(true);
	//只给一天的情况，不需要区别m_level

	foreach(i in rewards)
	{
		if (i->first == DICE)
			GetRole()->Get<CSuperRiskRecord>()->AddDiceNum(i->second);
		else
			transition.GiveItem(i->first, i->second);
	}
	
	transition.NotifyClient();

	return KKSG::ERR_SUCCESS;
}

UINT32 BaseFindBackInfo::GetCanFindBackCount()
{
	int temp = 0;
	for (auto iter = m_usedCountMap_h.begin(); iter != m_usedCountMap_h.end(); ++iter)
	{
		temp += iter->second->GetLeftFindBackCount(GetRole());
	}

	return temp;
}

UINT32 BaseFindBackInfo::GetOneDayInfoParam()
{
	return 0;
}

void BaseFindBackInfo::AddUseCount(int count, int dayBegin)
{
	UINT32 param = GetOneDayInfoParam();
	int now = time(0); 
	int recordTime = 0;
	auto data = FindBackConfig::Instance()->GetItemBackTableRowData(m_type);
	if (!data)
	{
		return;
	}
	bool isThisInterval = false;
	if (data->IsWeekBack)
	{
		isThisInterval = XCommon::IsWeekSame(dayBegin, now);
		recordTime = GameTime::GetWeekBeginTime(dayBegin) + 6 * 86400;
	}
	else
	{
		isThisInterval = XCommon::IsDaySame(dayBegin, now);
		recordTime = dayBegin;
	}

	if (m_usedCountMap_h.find(recordTime) != m_usedCountMap_h.end())
	{
		m_usedCountMap_h[recordTime]->m_subTypeInfos->m_todayUseCount += count;
		m_usedCountMap_h[recordTime]->m_subTypeInfos->m_towerLevel = param;
		return;
	}
	if (!isThisInterval)
	{
		OneDayTypeInfo * info = CreateOneDayBackInfo(recordTime);
		info->m_subTypeInfos->m_todayUseCount = count;
		info->m_subTypeInfos->m_towerLevel = param;
		m_usedCountMap_h[recordTime] = info;
		return;
	}

	m_usedCountMap->m_subTypeInfos->m_todayUseCount += count;
	m_usedCountMap->m_subTypeInfos->m_towerLevel = param;
}

void BaseFindBackInfo::AddAlreadyFindBackCount(int count)
{
	for (auto iter = m_usedCountMap_h.rbegin(); iter != m_usedCountMap_h.rend() && count > 0; ++iter)
	{
		int temp = iter->second->GetLeftFindBackCount(GetRole());
		if (!temp)
		{
			continue;
		}
		if (temp >= count)
		{
			iter->second->m_alreadyFindBackCount += count;
			count = 0;
		}
		else
		{
			iter->second->m_alreadyFindBackCount += temp;
			count -= temp;
		}
	}
}

void BaseFindBackInfo::GenerateFindBackInfo(KKSG::ItemFindBackInfoRes& roRes)
{
	KKSG::ItemFindBackInfo2Client temp;
	for (auto iter = m_usedCountMap_h.begin(); iter != m_usedCountMap_h.end(); ++iter)
	{
		temp.Clear();
		if (iter->second->Convert2KKSGBackInfo(GetRole(), temp))
		{
			auto info = roRes.add_backinfo();
			info->CopyFrom(temp);
		}
	}
}

void OneDayTypeInfo::Clear()
{
	m_subTypeInfos->Reset();
}

UINT32 OneDayTypeInfo::GetLeftFindBackCount(Role* role)
{
	if (!IsDayValid())
	{
		return 0;
	}
	if (!m_subTypeInfos->IsDayValid(role, m_day))
	{
		return 0;
	}

	int max = GetOneDayMaxCount(role, m_type);
	if (max > m_subTypeInfos->m_todayUseCount + m_alreadyFindBackCount)
	{
		return max - m_subTypeInfos->m_todayUseCount - m_alreadyFindBackCount;
	}
	return 0;
}

Role* OneDayTypeInfo::GetRole()
{
	return m_pBaseFindBack->GetRole();
}

bool OneDayTypeInfo::IsFirstUnlockDay()
{
	if (/*m_pBaseFindBack->m_pItemBack->IsSealFirstUnlockDay(time(0)) && */m_pBaseFindBack->m_pItemBack->IsSealLockedDay(m_level, m_day))
	{
		return true;
	}

	return false;
}

void OneDayTypeInfo::CheckUpdateExpInfo(std::map<int, int>& dragonItems, std::map<int, int>& goldItems)
{
	if (m_onceBackExp.find(DRAGON_COIN) != m_onceBackExp.end() && dragonItems.find(EXP) != dragonItems.end())
	{
		dragonItems[EXP] = m_onceBackExp[DRAGON_COIN];
	}
	if (m_onceBackExp.find(GOLD) != m_onceBackExp.end() && goldItems.find(EXP) != goldItems.end())
	{
		goldItems[EXP] = m_onceBackExp[GOLD];
	}
}

void OneDayTypeInfo::UpdateExtraSaveInf()
{
	//m_level = GetRole()->GetLevel();

	if (m_type == KKSG::DICE_BACK/* || m_type == KKSG::FATIGUE_RECOVER*/)
	{
		return;
	}
	std::map<int, int> dragonItems;
	std::map<int, int> goldItems;
	int dragon = 0;
	int gold = 0;
	if(!GetOnceFindBackCost(GetRole(), dragonItems, goldItems, dragon, gold))
	{
		return;
	}
	bool isFirstUnlock = IsFirstUnlockDay();
	bool isRoleInLock = GetRole()->GetLevelSealStatus();
	DealWithAddExp dealWithAddExp(GetRole(), m_level);
	if (isFirstUnlock)
	{
		dealWithAddExp.SetUseLevelSealBuff(false);
	}
	dealWithAddExp.SetUseWorldLevelBuff(false);
	if (dragonItems[EXP])
	{
		UINT64 realexp = dealWithAddExp.GetRealExp(dragonItems[EXP]);
		m_onceBackExp[DRAGON_COIN] = isFirstUnlock&&!isRoleInLock ? realexp * GetGlobalConfig().LevelSealExpRate : realexp;
	}
	if (goldItems[EXP])
	{
		UINT64 realexp = dealWithAddExp.GetRealExp(goldItems[EXP]);
		m_onceBackExp[GOLD] = isFirstUnlock&&!isRoleInLock ? realexp * GetGlobalConfig().LevelSealExpRate : realexp;
	}

	LogInfo("%llu,type:%u,is first unlock : %s,role in lock :%s, day:%d, level:%d", GetRole()->GetID(), m_type, isFirstUnlock ? "true" : "false", isRoleInLock ? "true" : "false", m_day, m_level);
}

bool OneDayTypeInfo::IsDayValid()
{
	if (!(m_type == KKSG::FATIGUE_BUY || m_type == KKSG::FATIGUE_GET || m_type == KKSG::FATIGUE_RECOVER))
		return true;

	if (m_pBaseFindBack->m_pItemBack->IsUnlockedSeal(m_day))
	{
		return false;
	}

	return true;
}

bool OneDayTypeInfo::Convert2KKSGBackInfo(Role* role, KKSG::ItemFindBackInfo2Client& temp)
{
	int leftCount = GetLeftFindBackCount(role);
	int alreadyCount = GetAlreadyFindBackCount();
	if (!leftCount && !alreadyCount)
	{
		return false;
	}

	if (!ConvertSingleSubtype(role, temp, leftCount))
	{
		return false;
	}

	if(!leftCount && alreadyCount)
	{
		temp.set_id((KKSG::ItemFindBackType)m_type);
		temp.set_findbackcount(0);
		temp.set_daytime(m_day);
	}


	return true;
}

bool OneDayTypeInfo::ConvertSingleSubtype(Role* role, KKSG::ItemFindBackInfo2Client& temp, int leftCount)
{
	std::map<int, int> dragonItems;
	std::map<int, int> goldItems;
	int totalGold = 0;
	int totalDragon = 0;

	auto data = FindBackConfig::Instance()->GetItemBackTableRowData(m_type, m_level);
	if (!data)
	{
		return false;
	}

	int left = leftCount;

	totalGold += data->CostGold;
	totalDragon += data->CostDragonCoin;

	for (int i = 0; i < data->ItemDragonCoin.size(); ++i)
	{
		dragonItems[data->ItemDragonCoin[i].seq[0]] += data->ItemDragonCoin[i].seq[1];
	}
	
	for (int i = 0; i < data->ItemGold.size(); ++i)
	{
		goldItems[data->ItemGold[i].seq[0]] += data->ItemGold[i].seq[1];
	}

	temp.set_id((KKSG::ItemFindBackType)m_type);
	temp.set_findbackcount(left);
	temp.set_daytime(m_day);
	temp.set_rolelevel(m_level);

	temp.set_dragoncoincost(totalDragon);
	temp.set_goldcoincost(totalGold);

	CheckUpdateExpInfo(dragonItems, goldItems);

	for (auto iter = dragonItems.begin(); iter != dragonItems.end(); ++iter)
	{
		KKSG::ItemBrief* brief = temp.add_dragoncoinfindbackitems();
		brief->set_itemid(iter->first);
		brief->set_itemcount(iter->second);
	}
	for (auto iter = goldItems.begin(); iter != goldItems.end(); ++iter)
	{
		KKSG::ItemBrief* brief = temp.add_goldcoinfindbackitems();
		brief->set_itemid(iter->first);
		brief->set_itemcount(iter->second);
	}

	return true;
}

bool OneDayTypeInfo::GetOnceFindBackCost(Role* role, std::map<int, int>& dragonItems, std::map<int, int>& goldItems, int& dragonCost, int& goldCost)
{
	int ownCostDragonCoinNum = 0;
	int ownCostGoldNum = 0;
	int costDragon = 0;
	int costGold = 0;
	int exchangeAddGold = 0;
	if (!GetLeftFindBackCount(role))
	{
		return false;
	}

	KKSG::ItemFindBackInfo2Client temp;
	if(!Convert2KKSGBackInfo(role, temp))
	{
		return false;
	}
	
	dragonCost = temp.dragoncoincost();
	goldCost = temp.goldcoincost();
	for (int i = 0; i < temp.dragoncoinfindbackitems_size(); ++i)
	{
		dragonItems[temp.dragoncoinfindbackitems(i).itemid()] += temp.dragoncoinfindbackitems(i).itemcount();
	}
	for (int i = 0; i < temp.goldcoinfindbackitems_size(); ++i)
	{
		goldItems[temp.goldcoinfindbackitems(i).itemid()] += temp.goldcoinfindbackitems(i).itemcount();
	}
	return true;
}

int OneDayTypeInfo::GetOneDayMaxCount(Role* role, int type)
{
	auto data = FindBackConfig::Instance()->GetItemBackTableRowData(type);
	if (data)
	{
		int max = data->count;
		return max;
	}

	return 0;
}

OneDayTypeInfo::OneDayTypeInfo(BaseFindBackInfo* baseFindBack, int type, int day)
{
	m_pBaseFindBack = baseFindBack;
	m_type = type;
	m_alreadyFindBackCount = 0;

	auto data = FindBackConfig::Instance()->GetItemBackTableRowData(m_type);
	if (data && data->IsWeekBack)
	{
		m_day = GameTime::GetWeekBeginTime(day) + 6 * 86400;
	}
	else
	{
		m_day = day;
	}

	m_level = 0;

	OneDaySubTypeInfo* subinfo = new OneDaySubTypeInfo(m_type);
	m_subTypeInfos = subinfo;
}

OneDayTypeInfo::~OneDayTypeInfo()
{
	delete m_subTypeInfos;
	m_subTypeInfos = NULL;
}

OneDayTypeInfo& OneDayTypeInfo::assign(const OneDayTypeInfo& info)
{
	m_alreadyFindBackCount = info.m_alreadyFindBackCount;
	m_subTypeInfos->m_todayUseCount = info.m_subTypeInfos->m_todayUseCount;
	m_subTypeInfos->m_towerLevel = info.m_subTypeInfos->m_towerLevel;

	return *this;
}

void OneDaySubTypeInfo::Reset()
{
	m_todayUseCount = 0;
}

bool OneDaySubTypeInfo::IsDayValid(Role* role, int day)
{
	if (!role)
	{
		return false;
	}

	int begin = GetFindBackBeginTime(role, m_type);
	if (begin && begin <= day)
	{
		return true;
	}

	return false;
}

