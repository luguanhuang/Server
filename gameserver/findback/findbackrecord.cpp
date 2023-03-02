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

CFindBackRecord::CFindBackRecord(Role* role):m_itemFindBackInfo(this)
{
	m_pRole = role;
	m_openTime = 0;
	m_updateTime = 0;
	m_isFoundBack = false;
}

CFindBackRecord::~CFindBackRecord()
{
}

bool CFindBackRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (poRoleAllInfo->has_findbackrecord())
	{
		const KKSG::RoleFindBackRecord* record = &poRoleAllInfo->findbackrecord();
		m_openTime = record->opentime();
		m_updateTime = record->updatetime();
		m_isFoundBack = record->isfoundback();
		for (int i = 0; i < record->usedinfos_size(); ++i)
		{
			int id = record->usedinfos(i).type();
			int usedcount = record->usedinfos(i).usedcount();
			m_usedCountMap_h[id] = usedcount;
		}
		for (int i = 0; i < record->curusedinfos_size(); ++i)
		{
			int id = record->curusedinfos(i).type();
			int usedcount = record->curusedinfos(i).usedcount();
			m_usedCountMap[id] = usedcount;
		}

		m_itemFindBackInfo.Load(record);

		Update();
	}
	return true;
}

void CFindBackRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (m_switch.TestAndReset())
	{
		KKSG::RoleFindBackRecord* record = poRoleAllInfo->mutable_findbackrecord();
		record->Clear();
		record->set_opentime(m_openTime);
		record->set_updatetime(m_updateTime);
		record->set_isfoundback(m_isFoundBack);
		for (auto iter = m_usedCountMap_h.begin(); iter != m_usedCountMap_h.end(); ++iter)
		{
			KKSG::ExpFindBackInfo* info = record->add_usedinfos();
			info->set_type((KKSG::ExpBackType)(iter->first));
			info->set_usedcount(iter->second);
		}
		for (auto iter = m_usedCountMap.begin(); iter != m_usedCountMap.end(); ++iter)
		{
			KKSG::ExpFindBackInfo* info = record->add_curusedinfos();
			info->set_type((KKSG::ExpBackType)(iter->first));
			info->set_usedcount(iter->second);
		}

		m_itemFindBackInfo.Save(record);

		roChanged.insert(record);
	}
}

void CFindBackRecord::CheckOpen()
{
	if (IsExpFindBackOpen())
	{
		return;
	}
	int level = m_pRole->GetLevel();
	int needLevel =  GetGlobalConfig().ExpFindBackNeedLevel;
	if (level >= needLevel)
	{
		int now = (int)time(0);
		m_updateTime = m_openTime = now;
		Change();
	}
}

void CFindBackRecord::UpdateExpFindBack(int now)
{
	CheckOpen();

	if (!IsExpFindBackOpen())
	{
		return;
	}
	//time_t now = time(0);
	if (XCommon::IsDaySame(m_openTime, now))
	{
		return;
	}

	if (XCommon::IsDaySame(m_updateTime, now))
	{
		return;
	}

	time_t nowBegin = XCommon::GetDayBeginTime(now);
	time_t infoBegin = XCommon::GetDayBeginTime(m_updateTime);

	if (nowBegin > infoBegin && ((nowBegin - infoBegin) == 86400))
	{
		m_usedCountMap_h.clear();
		m_usedCountMap_h = m_usedCountMap;
		m_usedCountMap.clear();
	}
	else if (nowBegin > infoBegin)
	{
		m_usedCountMap_h.clear();
		m_usedCountMap.clear();
	}
	m_isFoundBack = false;
	m_updateTime = (int)now;

	Change();
}

void CFindBackRecord::UpdateItemFindBack(int now)
{
	//m_itemFindBackInfo.Update(now);
}

void CFindBackRecord::Update()
{
	int now = time(0);

	UpdateExpFindBack(now);
	
	//UpdateItemFindBack(now);
}

void CFindBackRecord::SlowUpdate()
{
	m_itemFindBackInfo.SlowUpdate();
}

void CFindBackRecord::Change()
{
	m_switch.Set();
}

void CFindBackRecord::OnFirstEnterScene()
{
	if (IsExpFindBackOpen() && !XCommon::IsDaySame(m_openTime, time(0)) && !m_isFoundBack)
	{
		PtcG2C_ExpFindBackNtf ntf;
		bool bSend = false;

		const ExpBackTable& table = FindBackConfig::Instance()->GetExpBackTable();
		foreach(i in table.Table)
		{
			if ((*i)->count <= m_usedCountMap_h[(*i)->type])
			{
				continue;
			}

			KKSG::ExpFindBackInfo* info = ntf.m_Data.add_expbackinfos();
			info->set_type((KKSG::ExpBackType)((*i)->type));
			info->set_usedcount(m_usedCountMap_h[(*i)->type]);
		}

		if (ntf.m_Data.expbackinfos_size())
		{
			m_pRole->Send(ntf);
		}
	}

	m_itemFindBackInfo.OnFirstEnterScene();
}

UINT32 CFindBackRecord::GetFindBackReward(bool isFree)
{
	if (m_isFoundBack)
	{
		return KKSG::ERR_EXPBACK_ALREADYGET;
	}
	const ExpBackTable& table = FindBackConfig::Instance()->GetExpBackTable();

	int exp = 0;
	int cost = 0;
	int moneyExtExp = 0;
	foreach(i in table.Table)
	{
		int leftcount = (*i)->count > m_usedCountMap_h[(*i)->type] ? ((*i)->count - m_usedCountMap_h[(*i)->type]) : 0;
		exp += isFree ? (int)((*i)->exp * leftcount * (*i)->freeExpParam) : (int)((*i)->exp * leftcount * (*i)->moneyCostParam);
		if (!isFree)
		{
			moneyExtExp += ((*i)->moneyCostParam > (*i)->freeExpParam) ? (int)((*i)->exp * ((*i)->moneyCostParam - (*i)->freeExpParam) * leftcount) : 0;
		}
	}
	exp /= 100;
	moneyExtExp /= 100;
	if(!isFree)
		cost = moneyExtExp /  GetGlobalConfig().ExpFindBackParam + (moneyExtExp%  GetGlobalConfig().ExpFindBackParam ? 1 : 0);

	BagTakeItemTransition transition(m_pRole);
	transition.SetReason(ItemFlow_FindBack, ItemFlow_FindBack_Exp);
	if (!transition.TakeItem(DRAGON_COIN, cost))
	{
		transition.RollBack();
		return KKSG::ERR_ITEM_NEED_DRAGONCOIN;
	}
	transition.NotifyClient();

	BagGiveItemTransition giveTransition(m_pRole);
	giveTransition.SetReason(ItemFlow_FindBack, ItemFlow_FindBack_Exp);
	giveTransition.GiveItem(EXP, exp);
	giveTransition.NotifyClient();

	m_isFoundBack = true;

	Change();

	return KKSG::ERR_SUCCESS;
}

void CFindBackRecord::AddUsedCount(int type, int count)
{
	m_usedCountMap[type] += count;
	Change();
}

void CFindBackRecord::AddItemUsedCount(int id, int subtype, int count, int lastRecoverTime, int thisReoverTime)
{
	if (id == KKSG::FATIGUE_RECOVER || id == KKSG::DICE_BACK)
	{
		if(XCommon::IsDaySame(lastRecoverTime, thisReoverTime))
		{
			m_itemFindBackInfo.AddUsedCount(id, subtype, count, XCommon::GetDayBeginTime(thisReoverTime));
			return;
		}

		int lastDayCanRecover  = 0;
		if (id == KKSG::FATIGUE_RECOVER &&  GetGlobalConfig().FatigueRegeneration)
		{
			lastDayCanRecover= (XCommon::GetDayBeginTime(lastRecoverTime) + XCommon::GetOneDayTime() - lastRecoverTime)/60/ GetGlobalConfig().FatigueRegeneration;
		}
		else if (id == KKSG::DICE_BACK &&  GetGlobalConfig().RiskRecoverDiceNum.seq[0])
		{
			lastDayCanRecover= (XCommon::GetDayBeginTime(lastRecoverTime) + XCommon::GetOneDayTime() - lastRecoverTime)/60/ GetGlobalConfig().RiskRecoverDiceNum.seq[0] *  GetGlobalConfig().RiskRecoverDiceNum.seq[1];
		}
		if (lastDayCanRecover >= count)
		{
			m_itemFindBackInfo.AddUsedCount(id, subtype, count, XCommon::GetDayBeginTime(lastRecoverTime));
		}
		else
		{
			m_itemFindBackInfo.AddUsedCount(id, subtype, lastDayCanRecover, XCommon::GetDayBeginTime(lastRecoverTime));
			m_itemFindBackInfo.AddUsedCount(id, subtype, count - lastDayCanRecover, XCommon::GetDayBeginTime(lastRecoverTime) + XCommon::GetOneDayTime());
		}

	}
	else
		m_itemFindBackInfo.AddUsedCount(id, subtype, count, XCommon::GetDayBeginTime(time(0)));
}

void CFindBackRecord::OnDayPass()
{
	Update();
	OnFirstEnterScene();
}

UINT32 CFindBackRecord::GetItemFindBackInfoRes(KKSG::ItemFindBackInfoRes& roRes)
{
	return m_itemFindBackInfo.GetItemFindBackInfoRes(roRes);
}

void CFindBackRecord::DebugReset(int day)
{
	m_isFoundBack = false;
	int now = (int)time(0);
	m_openTime = m_updateTime = now - 86400;

	m_itemFindBackInfo.DebugReset(day);

	Update();
	OnFirstEnterScene();
}

