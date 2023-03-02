#include "pch.h"
#include "dragonconfig.h"
#include "foreach.h"
#include "timeutil.h"
#include "scene/sceneconfig.h"
#include "network/mslink.h"
#include "table/globalconfig.h"
#include "util/gametime.h"
#include "common/ptcg2m_updateglobalvalue.h"
#include "define/globalconfigtodbdef.h"
#include "expeditionconfigmgr.h"
#include "gamelogic/dragonrecord.h"
#include "servermgr/servercrossdatamgr.h"

INSTANCE_SINGLETON(DragonConfig)

DragonConfig::DragonConfig()
{
}

DragonConfig::~DragonConfig()
{

}

bool DragonConfig::Init()
{
	return LoadFile();
}

void DragonConfig::Uninit()
{

}

bool DragonConfig::LoadFile()
{
	m_dragonTable.Clear();
	m_dragonTypeTable.Clear();

	if (!m_dragonTable.LoadFile("table/DragonNestList.txt"))
	{
		LogWarn("Load DragonNestList.txt failed!");
		return false;
	}

	if (!m_dragonTypeTable.LoadFile("table/DragonNestType.txt"))
	{
		LogWarn("Load DragonNestType.txt failed!");
		return false;
	}

	return true;

}

const DragonNestTable::RowData* DragonConfig::GetDragonTableData(int expID)
{
	foreach(i in m_dragonTable.Table)
	{
		if ((*i)->DragonNestID == expID)
		{
			return (*i);
		}
	}

	return NULL;
}

const DragonNestTable::RowData* DragonConfig::GetDragonTableData(int type, int hard, int floor)
{
	foreach(i in m_dragonTable.Table)
	{
		if ((*i)->DragonNestType == type && (*i)->DragonNestDifficulty == hard && (*i)->DragonNestWave == floor)
		{
			return (*i);
		}
	}

	return NULL;

}

int DragonConfig::GetTopestFloor(int type, int hard)
{
	int max = 0;
	foreach(i in m_dragonTable.Table)
	{
		if ((*i)->DragonNestDifficulty == hard && (*i)->DragonNestType == type && (*i)->DragonNestWave > max)
		{
			max = (*i)->DragonNestWave;
		}
	}

	return max;
}

const std::string* DragonConfig::GetDragonTypeName(int expID)
{
	int type = 0;
	auto data = GetDragonTableData(expID);
	if (!data)
	{
		return NULL;
	}

	type = data->DragonNestType;

	foreach(i in m_dragonTypeTable.Table)
	{
		if ((*i)->DragonNestType == type)
		{
			return &(*i)->TypeName;
		}
	}

	return NULL;
}

int DragonConfig::GetHardest(int type)
{
	int max = 0;
	foreach(i in m_dragonTable.Table)
	{
		if ((*i)->DragonNestType == type && (*i)->DragonNestDifficulty > max)
		{
			max = (*i)->DragonNestDifficulty;
		}
	}

	return max;
}

void DragonConfig::_FillDragonGlobalInfo(const KKSG::GlobalDragonInfo& info, std::map<UINT32, DragonSceneInfo>& id2time)
{
	id2time.clear();
	if (info.sceneid_size() != info.timestamp_size())
	{
		SSWarn<<"sceneid size:"<<info.sceneid_size()<<" time size:"<<info.timestamp_size()<<END;
		return;
	}
	for (int i = 0; i != info.sceneid_size(); ++i)
	{
		id2time[info.sceneid(i)].passTime = info.timestamp(i);
		SSInfo<<"id:"<<info.sceneid(i)<<" time:"<<info.timestamp(i)<<END;
	}
	for (int i = 0; i < info.weakinfos_size(); ++i)
	{
		id2time[info.weakinfos(i).sceneid()].openTime = info.weakinfos(i).opentime();
		SSInfo<<"id:"<<info.weakinfos(i).sceneid()<<" opentime:"<<info.weakinfos(i).opentime()<<END;
	}
}

void DragonConfig::FillGlobalInfo(const std::string& value)
{
	KKSG::GlobalDragonInfo info;
	info.ParseFromString(value);
	DragonConfig::_FillDragonGlobalInfo(info, m_id2time);
}

void DragonConfig::UpdateGlobalInfo(UINT32 sceneid)
{
	UINT32 now = GameTime::GetTime();

	bool change = false;
	auto i = m_id2time.find(sceneid);
	if (i == m_id2time.end())
	{
		m_id2time[sceneid].passTime = now;
		m_id2time[sceneid].openTime = now;

		change = true;

		SSInfo<<"update dragon global, sceneid:"<<sceneid<<" time:"<<now<<END;
	}
	else
	{
		if (i->second.passTime)
		{
			SSInfo<<"sceneid:"<<sceneid<<" is exist, time:"<<i->second.passTime<<END;
		}
		else
		{
			change = true;
			i->second.passTime = now;
			if (!i->second.openTime)
			{
				i->second.openTime = i->second.passTime;
			}
			SSInfo<<"update dragon global, sceneid:"<<sceneid<<" time:"<<now<<" opentime:"<<i->second.openTime<<END;
		}
	}
	if (change)
	{
		PtcG2M_UpdateGlobalValue ptc;
		ptc.m_Data.set_id(eDragonGlobal);

		KKSG::GlobalDragonInfo* info = ptc.m_Data.mutable_dragoninfo();
		info->add_sceneid(sceneid);
		info->add_timestamp(now);
		KKSG::GlobalDragonWeakInfo* weakInfo = info->add_weakinfos();
		weakInfo->set_sceneid(sceneid);
		weakInfo->set_opentime(m_id2time[sceneid].openTime);

		MSLink::Instance()->SendTo(ptc);

	}
}

bool DragonConfig::_CanRevive(UINT32 sceneid, const std::map<UINT32, DragonSceneInfo>& id2time)
{
	std::set<UINT32> tempsets;
	for (auto i = id2time.begin(); i != id2time.end(); ++i)
	{
		///> 0 表示 没通关
		if (0 == i->second.passTime)
		{
			continue;
		}
		if (GameTime::GetTime() >= (i->second.passTime + DragonConfig::Instance()->GetCanReviveCD(i->first)*3600))
		{
			tempsets.insert(i->first);
		}
		SceneInfo* info = SceneConfig::Instance()->FindSceneInfo(i->first);
		int count = 0;
		while (NULL != info && !info->m_pConf->PreScene.empty())
		{
			if (0 != count)
			{
				tempsets.insert(info->m_pConf->PreScene[0]);
			}
			++count;
			info = SceneConfig::Instance()->FindSceneInfo(info->m_pConf->PreScene[0]); 
		}
	}
	auto j = tempsets.find(sceneid);
	if (j == tempsets.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool DragonConfig::CanLocalRevive(UINT32 sceneid)
{
	return DragonConfig::_CanRevive(sceneid, m_id2time);
}

bool DragonConfig::CanCrossRevive(UINT32 sceneid, std::set<UINT32>& serverids)
{
	bool canrevive = false;
	std::map<UINT32, DragonSceneInfo> tempid2time;
	for (auto i = serverids.begin(); i != serverids.end(); ++i)
	{
		SSInfo<<"sceneid:"<<sceneid<<" parser serverid:"<<*i<<END;

		tempid2time.clear();
		KKSG::ServerCrossData* data = ServerCrossDataMgr::Instance()->GetData(*i);
		if (NULL != data)
		{
			DragonConfig::_FillDragonGlobalInfo(data->teamdata().globaldragoninfo(), tempid2time);
			canrevive = DragonConfig::_CanRevive(sceneid, tempid2time);
			if (canrevive)
			{
				break;
			}
		}
	}
	return canrevive;
}

UINT32 DragonConfig::GetCanReviveCD(UINT32 sceneid)
{
	for (auto i = GetGlobalConfig().DragonSceneCanReviveCD.begin(); i != GetGlobalConfig().DragonSceneCanReviveCD.end(); ++i)
	{
		if (i->seq[0] == sceneid)
		{
			return i->seq[1];
		}
	}
	return 0;
}

UINT32 DragonConfig::GetGlobalPassTimeByExpID(UINT32 expID)
{
	UINT32 sceneID = ExpeditionConfigMgr::Instance()->GetExpRandomScene(expID);
	auto find = m_id2time.find(sceneID);
	if (find == m_id2time.end())
	{
		return 0;
	}

	return find->second.passTime;
}

UINT32 DragonConfig::GetGlobalOpenTimeByExpID(UINT32 expID)
{
	UINT32 sceneID = ExpeditionConfigMgr::Instance()->GetExpRandomScene(expID);
	auto find = m_id2time.find(sceneID);
	if (find == m_id2time.end())
	{
		return 0;
	}

	return find->second.openTime;
}

void DragonConfig::CalcDragonWeakBuffInfo(UINT32 expID, UINT32& buffID, UINT32& buffLevel, KKSG::DragonWeakType& type, UINT32& refreshTimes)
{
	int passTime = GetGlobalPassTimeByExpID(expID);
	int openTime = GetGlobalOpenTimeByExpID(expID);

	CalcDragonWeakBuffInfo(expID, openTime, passTime, buffID, buffLevel, type, refreshTimes);
}

void DragonConfig::CalcDragonWeakBuffInfo(UINT32 expID, UINT32 openTime, UINT32 passTime, UINT32& buffID, UINT32& buffLevel, KKSG::DragonWeakType& type, UINT32& refreshTimes)
{
	auto tableData = DragonConfig::Instance()->GetDragonTableData(expID);
	if (!tableData)
	{
		return;
	}
	if (tableData->DragonNestDifficulty != 1)
	{
		return;
	}

	if (!openTime)
	{
		return;
	}
	int openRefresh = CDragonRecord::CalcDragonRefreshTimes(openTime);
	int passRefresh = CDragonRecord::CalcDragonRefreshTimes(passTime);
	int beforePassRefresh = 0;
	if (!passTime)
	{
		beforePassRefresh = openRefresh;
	}
	else
	{
		beforePassRefresh = openRefresh > passRefresh ? openRefresh - passRefresh : 0;
	}

	if (beforePassRefresh)
	{
		bool isNotPassWeak = false;
		for (auto iter = tableData->WeakInfoEx.begin(); iter != tableData->WeakInfoEx.end(); ++iter)
		{
			if (iter->seq[0] && beforePassRefresh >= iter->seq[0])
			{
				isNotPassWeak = true;
				break;
			}
		}

		if (isNotPassWeak)
		{
			for (auto iter = tableData->WeakInfoEx.begin(); iter != tableData->WeakInfoEx.end(); ++iter)
			{
				if (openRefresh >= iter->seq[0])
				{
					buffID = iter->seq[1];
					buffLevel = iter->seq[2];
				}
			}

			type = KKSG::DragonWeakType_NotPass;
			refreshTimes = openRefresh;
			return;
		}
	}

	if (passTime)
	{
		int tempBuffID = 0;
		int tempBuffLevel = 0;
		for (auto iter = tableData->WeakInfo.begin(); iter != tableData->WeakInfo.end(); ++iter)
		{
			if (passRefresh >= iter->seq[0])
			{
				tempBuffID = iter->seq[1];
				tempBuffLevel = iter->seq[2];
			}
		}

		buffID = tempBuffID;
		buffLevel = tempBuffLevel;

		type = KKSG::DragonWeakType_Pass;
		refreshTimes = passRefresh;
	}

}

void DragonConfig::SyncWorldLevel(UINT32 level)
{
	if (!GSConfig::Instance()->IsHallGS())
	{
		return;
	}

	for (auto i = SceneConfig::Instance()->GetAllSceneConf().begin(); i != SceneConfig::Instance()->GetAllSceneConf().end(); ++i)
	{
		UINT32 sceneID = i->first;
		auto pSceneInfo = &i->second;
		if (pSceneInfo->m_pConf && pSceneInfo->m_pConf->type == KKSG::SCENE_DRAGON)
		{
			auto find = DragonConfig::Instance()->m_id2time.find(sceneID);
			if (find != m_id2time.end())
			{
				continue;
			}

			if (pSceneInfo->m_pConf->RequiredLevel <= level)
			{
				m_id2time[sceneID].openTime = GameTime::GetTime();
				PtcG2M_UpdateGlobalValue ptc;
				ptc.m_Data.set_id(eDragonGlobal);

				KKSG::GlobalDragonInfo* info = ptc.m_Data.mutable_dragoninfo();
				KKSG::GlobalDragonWeakInfo* weakInfo = info->add_weakinfos();
				weakInfo->set_sceneid(sceneID);
				weakInfo->set_opentime(m_id2time[sceneID].openTime);

				MSLink::Instance()->SendTo(ptc);
				SSInfo<<"update dragon global, sceneid:"<<sceneID<<" opentime:"<<m_id2time[sceneID].openTime<<END;
			}
		}
	}
}

bool DragonConfig::IsAllServerNotPass(UINT32 expID)
{
    auto itor = m_id2time.find(expID);
    if(m_id2time.end() == itor)
    {
        return true;
    }
    if(0 == itor->second.passTime)
    {
        return true;
    }
    return false;
}

bool DragonConfig::IsReviveExcept(UINT32 sceneid)
{
	auto i = GetGlobalConfig().DragonSceneReviveExcept.find(sceneid);
	if (i == GetGlobalConfig().DragonSceneReviveExcept.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}
