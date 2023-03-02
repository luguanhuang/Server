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
#include "towerfindback.h"
#include "miscfindback.h"
#include "totalsinglefindback.h"
#include "itemfindbackbase.h"
#include "nestfindback.h"


CItemFindBackInfo::CItemFindBackInfo(CFindBackRecord* record)
{
	m_pRecord = record;
	m_openTime = 0;
	m_updateTime = 0;
	m_unlockSealUpdateTime = 0;
	m_notifyBackTime = 0;
	m_tempInfo2ClientRes = new KKSG::ItemFindBackInfoRes();

	InitBackMap();
}

CItemFindBackInfo::~CItemFindBackInfo()
{
	delete m_tempInfo2ClientRes;
	for (auto iter = m_backMap.begin(); iter != m_backMap.end(); ++iter)
	{
		delete iter->second;
	}

	m_backMap.clear();
}

bool CItemFindBackInfo::Load(const KKSG::RoleFindBackRecord* record)
{
	int now = time(0);
	if (record->has_findbackopentime())
	{
		m_openTime = record->findbackopentime();
	}
	if (record->has_itembackupdatetime())
	{
		m_updateTime = record->itembackupdatetime();
	}
	if (record->has_unlocksealtime())
	{
		m_unlockSealUpdateTime = record->unlocksealtime();
	}
	m_notifyBackTime = record->notifybacktime();
	for (int i = 0; i < record->unlocksealdata_size(); ++i)
	{
		auto& info = record->unlocksealdata(i);
		UnLockSealData temp;
		temp.state = info.state();
		temp.type = info.type();
		temp.time = info.time();
		m_unlockSealData.insert(std::make_pair(temp.type, temp));
	}
	
	for (auto iter = m_backMap.begin(); iter != m_backMap.end(); ++iter)
	{
		iter->second->Load(record);
	}

	return true;
}

void CItemFindBackInfo::Save(KKSG::RoleFindBackRecord* record)
{
	record->set_findbackopentime(m_openTime);
	record->set_itembackupdatetime(m_updateTime);
	record->set_unlocksealtime(m_unlockSealUpdateTime);
	record->set_notifybacktime(m_notifyBackTime);
	for (auto iter = m_unlockSealData.begin(); iter != m_unlockSealData.end(); ++iter)
	{
		auto temp = record->add_unlocksealdata();
		temp->set_state(iter->second.state);
		temp->set_type(iter->first);
		temp->set_time(iter->second.time);
	}
	
	for (auto iter = m_backMap.begin(); iter != m_backMap.end(); ++iter)
	{
		iter->second->Save(record);
	}
}

void CItemFindBackInfo::CheckOpen()
{
	if (IsOpen())
	{
		return;
	}
	int now = (int)time(0);
	Update(now);

	m_openTime = now;
	m_pRecord->Change();

	for (auto iter = m_backMap.begin(); iter != m_backMap.end(); ++iter)
	{
		iter->second->OnOpenInit();
	}

}

void CItemFindBackInfo::Update(int now)
{
	if (!IsOpen())
	{
		if (!XCommon::IsDaySame(m_updateTime, now))
		{
			for (auto iter = m_backMap.begin(); iter != m_backMap.end(); ++iter)
			{
				iter->second->ClearCur();
			}
			m_updateTime = now;
			m_pRecord->Change();
		}

		return;
	}

	if (XCommon::IsDaySame(m_updateTime, now))
		return;

	time_t nowBegin = XCommon::GetDayBeginTime(now);
	time_t infoBegin = XCommon::GetDayBeginTime(m_updateTime);


	for (auto iter = m_backMap.begin(); iter != m_backMap.end(); ++iter)
	{
		iter->second->Update(nowBegin, infoBegin);
	}

	m_updateTime = (int)now;

	m_pRecord->Change();

	ChangeRegenerate();
}

void CItemFindBackInfo::UpdateSealData(int now)
{
	m_unlockSealUpdateTime = now;

	m_pRecord->Change();

	ChangeRegenerate();

	std::map<UINT32, UINT32> unlockTime;
	CLevelSealMgr::Instance()->GetUnLockInfo(unlockTime);
	for (auto iter = unlockTime.begin(); iter != unlockTime.end(); ++iter)
	{
		if (iter->second <= 0)
		{
			continue;
		}
		auto find = m_unlockSealData.find(iter->first);
		if (find != m_unlockSealData.end())
		{
			continue;
		}
		UnLockSealData temp;
		temp.type = iter->first;
		temp.time = iter->second;
		temp.state = false;
		int level = m_pRecord->GetRole()->GetLevel();
		int minLevel = 0;
		auto data = CLevelSealMgr::Instance()->GetSealTypeTableData(temp.type);
		if (data && data->ClearFindBackMinLevel)
		{
			minLevel = data->ClearFindBackMinLevel;
			if (minLevel > data->Level)
			{
				minLevel = data->Level;
			}
			if (level >= minLevel)
			{
				temp.state = true;
			}
		}

		m_unlockSealData.insert(std::make_pair(temp.type, temp));
		LogInfo("%llu, %s, add sealinfo:%u, %u, %u", m_pRecord->GetRole()->GetID(), m_pRecord->GetRole()->GetName().c_str(), temp.type, temp.time, temp.state ? 1 : 0);
	}

	for (auto i = m_backMap.begin(); i != m_backMap.end(); ++i)
	{
		i->second->UpdateExtraSaveInfo();
	}
}

void CItemFindBackInfo::SlowUpdate()
{
	if (IsDataValid())
	{
		return;
	}

	int now = time(0);
	UINT32 syncTime = CLevelSealMgr::Instance()->GetUpdateTime();

	if (!XCommon::IsDaySame(m_updateTime, now))
	{
		Update(now);
	}

	if (XCommon::IsDaySame(syncTime, now) && !XCommon::IsDaySame(m_unlockSealUpdateTime, now))
	{
		UpdateSealData(now);
	}

}

bool CItemFindBackInfo::IsOpen()
{
	return m_openTime > 0;
}

UINT32 CItemFindBackInfo::GetOpenTime()
{
	return m_openTime;
}

bool CItemFindBackInfo::IsDataValid()
{
	int now = time(0);
	UINT32 syncTime = CLevelSealMgr::Instance()->GetUpdateTime();
	if (XCommon::IsDaySame(syncTime, now) && XCommon::IsDaySame(m_updateTime, now) && XCommon::IsDaySame(m_unlockSealUpdateTime, now))
	{
		return true;
	}

	return false;
}

bool CItemFindBackInfo::IsSealFirstUnlockDay(int time)
{
	for (auto iter = m_unlockSealData.begin(); iter != m_unlockSealData.end(); ++iter)
	{
		if (XCommon::GetDayBeginTime(time) == XCommon::GetDayBeginTime(iter->second.time))
		{
			return true;
		}
	}

	return false;
}

bool CItemFindBackInfo::IsSealLockedDay(int level, int day)
{
	for (auto iter = m_unlockSealData.begin(); iter != m_unlockSealData.end(); ++iter)
	{
		auto data = CLevelSealMgr::Instance()->GetSealTypeTableData(iter->second.type);
		if (data && XCommon::GetDayBeginTime(day) < XCommon::GetDayBeginTime(iter->second.time) && level >= data->Level)
		{
			return true;
		}
	}

	return false;
}

bool IsValidItemFindBackType(int id)
{
	if(id >= KKSG::FINDBACK_MAX || id < KKSG::TOWER)
		return false;

	return true;
}


void CItemFindBackInfo::DebugReset(int day)
{
	int now = (int)time(0);
	m_openTime = m_updateTime = now - 86400 * day;
	m_unlockSealUpdateTime = 0;
	m_unlockSealData.clear();
	//m_usedCountMap_h.clear();
	for (auto iter = m_backMap.begin(); iter != m_backMap.end(); ++iter)
	{
		iter->second->DebugReset();
		m_pRecord->Change();
	}

	Update(now);
}

UINT32 CItemFindBackInfo::GenerateItemFindBackInfo2Client(KKSG::ItemFindBackInfoRes& roRes)
{
	roRes.Clear();
	for (auto iter = m_backMap.begin(); iter != m_backMap.end(); ++iter)
	{
		iter->second->GenerateFindBackInfo(roRes);
	}

	return KKSG::ERR_SUCCESS;
}

void CItemFindBackInfo::AddUsedCount(int id, int subtype, int count, int dayBegin)
{
	auto find = m_backMap.find(id);
	if (find == m_backMap.end())
	{
		return;
	}

	int now = time(0);
	Update(now);

	find->second->AddUseCount(count, dayBegin);
	if (!XCommon::IsDaySame(dayBegin, now))
	{
		ChangeRegenerate();

		if (IsDataValid())
		{
			find->second->UpdateExtraSaveInfo(dayBegin);
		}
	}

	m_pRecord->Change();
}


void CItemFindBackInfo::OnFirstEnterScene()
{
	//AddUsedCount(KKSG::TOWER , 1, 0);

	GenerateItemFindBackInfo2Client(*m_tempInfo2ClientRes);

	bool has = false;
	for (int i = 0; i < m_tempInfo2ClientRes->backinfo_size(); ++i)
	{
		if (m_tempInfo2ClientRes->backinfo(i).findbackcount())
		{
			has = true;
			break;
		}
	}
	if (has)
	{
		bool isFirst = false;
		int now = time(0);
		if (!XCommon::IsDaySame(m_notifyBackTime, now))
		{
			isFirst = true;
			m_notifyBackTime = now;
			m_pRecord->Change();
		}
		if (!m_pRecord->GetRole()->Get<CRoleSystem>()->IsSystemOpened(SysItemFindBack))
		{
			return;
		}
		PtcG2C_ItemFindBackNtf ntf;
		ntf.m_Data.set_isdayfirstnofity(isFirst);
		m_pRecord->GetRole()->Send(ntf);
	}
}

void CItemFindBackInfo::InitBackMap()
{
	std::map<int, std::set<int>> tempTypeMapSubtype;
	auto& table = FindBackConfig::Instance()->GetItemBackTableData();
	for (auto iter = table.Table.begin(); iter != table.Table.end(); ++iter)
	{
		tempTypeMapSubtype[(*iter)->ID].insert(1);
	}

	for (auto iter = tempTypeMapSubtype.begin(); iter != tempTypeMapSubtype.end(); ++iter)
	{
		switch(iter->first)
		{
		case KKSG::TOWER:
			m_backMap[iter->first] = new TowerFindBackInfo(this);
			break;
		case KKSG::FATIGUE_BUY:
			m_backMap[iter->first] = new FatigueBuyFindBackInfo(this);
			break;
		case KKSG::NVSHENSHILIAN:
		case KKSG::FATIGUE_GET:
		case KKSG::WUJINSHENYUAN_BACK:
		case KKSG::DRAGONEXP_BACK:
		case KKSG::QAMULTI_BACK:
		case KKSG::COMMERCETASK_BACK:
		case KKSG::DayActiveBack:
			m_backMap[iter->first] = new BaseFindBackInfo(iter->first, this);
			break;
		case KKSG::GUILDACTIVITY:
		case KKSG::GUILDCHECKIN_BACK:
		case KKSG::GUILD_VOICE:
			m_backMap[iter->first] = new GuildFindBackInfo(iter->first, this);
			break;
		case KKSG::FATIGUE_RECOVER:
			m_backMap[iter->first] = new FatigueRecoverFindBackInfo(iter->first, this);
			break;
		case KKSG::DICE_BACK:
			m_backMap[iter->first] = new DiceFindBackInfo(iter->first, this);
			break;
		case KKSG::NestBack:
			m_backMap[iter->first] = new NestFindBackInfo(this);
			break;
		default:
			break;
		}
	}

}

void CItemFindBackInfo::ChangeRegenerate()
{
	m_needReGenerate.Set();
}

bool CItemFindBackInfo::IsValidFindBackCostType(int type)
{
	if (type != FindBack_Dragon && type != FindBack_Gold && type != FindBack_Ticket)
	{
		return false;
	}

	return true;
}

bool CItemFindBackInfo::IsUnlockedSeal(int day)
{
	for (auto iter = m_unlockSealData.begin(); iter != m_unlockSealData.end(); ++iter)
	{
		if (XCommon::GetDayBeginTime(day) < XCommon::GetDayBeginTime(iter->second.time) && iter->second.state)
		{
			return true;
		}
	}

	return false;
}

UINT32 CItemFindBackInfo::GetItemFindBackInfoRes(KKSG::ItemFindBackInfoRes& roRes)
{
	if (!m_pRecord->GetRole()->Get<CRoleSystem>()->IsSystemOpened(SysItemFindBack))
	{
		return KKSG::ERR_SYS_NOTOPEN;
	}
	if (!IsDataValid())
	{
		return KKSG::ERR_FAILED;
	}
	if (m_needReGenerate.TestAndReset())
	{
		GenerateItemFindBackInfo2Client(*m_tempInfo2ClientRes);
	}

	roRes.CopyFrom(*m_tempInfo2ClientRes);
	//roRes.set_error(KKSG::ERR_SUCCESS);
	roRes.set_time(time(0));
	return KKSG::ERR_SUCCESS;
}

UINT32 CItemFindBackInfo::GetItemFindBackReward(int id, int count, int type)
{
	if (count <= 0)
	{
		LogError("Receive invalid count [%d], id [%d], type [%d], Role: %llu, Account: %s", count, id, type, m_pRecord->GetRole()->GetID(), m_pRecord->GetRole()->GetAccount().c_str());
		return KKSG::ERR_FAILED;
	}
	if (!m_pRecord->GetRole()->Get<CRoleSystem>()->IsSystemOpened(SysItemFindBack))
	{
		return KKSG::ERR_SYS_NOTOPEN;
	}
	if (!IsDataValid())
	{
		return KKSG::ERR_FAILED;
	}
	auto find = m_backMap.find(id);
	if (find == m_backMap.end())
	{
		return KKSG::ERR_FAILED;
	}

	if (!IsValidFindBackCostType(type))
	{
		return KKSG::ERR_FAILED;
	}


	UINT32 error = find->second->GiveFindBackReward(type, count);
	if (error == KKSG::ERR_SUCCESS)
	{
		m_pRecord->Change();
		ChangeRegenerate();
	}

	return error;
}


