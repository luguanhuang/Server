#include "pch.h"
#include "worldlevelmgr.h"
#include "servermgr/servercrossdatamgr.h"
#include "gamelogic/dragonconfig.h"
#include "common/ptcg2c_worldlevelntf2client.h"
#include "unit/rolemanager.h"

INSTANCE_SINGLETON(WorldLevelMgr);


static bool WorldLevelConfCmp(WorldLevelConf* a, WorldLevelConf* b)
{
	return a->Level[0] < b->Level[0];
}

WorldLevelMgr::WorldLevelMgr()
{
	m_worldLevelNow = 0;
}

WorldLevelMgr::~WorldLevelMgr()
{

}

bool WorldLevelMgr::Init()
{
	return LoadFile();
}

void WorldLevelMgr::Uninit()
{
	ClearFile();
}

bool WorldLevelMgr::CheckFile()
{
	WorldLevelExpBuff tempTable;
	if(!tempTable.LoadFile("table/WorldLevelExpBuff.txt"))
	{
		LogWarn("Load WorldLevelExpBuff.txt failed!");
		return false;
	}
	return true;
}

bool WorldLevelMgr::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	if (!m_oTable.LoadFile("table/WorldLevelExpBuff.txt"))
	{
		LogWarn("Load WorldLevelExpBuff.txt failed!");
		return false;
	}

	for (auto i = m_oTable.Table.begin(); i != m_oTable.Table.end(); ++i)
	{
		if ((*i)->Level[0] > (*i)->Level[1])
		{
			LogWarn("worldLevel:%u, level[0]:%u > level[1]:%u", (*i)->WorldLevel, (*i)->Level[0], (*i)->Level[1]);
			return false;
		}
		m_worldLevelMap[(*i)->WorldLevel].push_back(*i);
	}

	for (auto i = m_worldLevelMap.begin(); i != m_worldLevelMap.end(); ++i)
	{
		std::sort(i->second.begin(), i->second.end(), WorldLevelConfCmp);
		for (UINT32 j = 1; j < i->second.size(); ++j)
		{
			if (i->second[j]->Level[0] <= i->second[j-1]->Level[1])
			{
				LogWarn("worldLevel:%u, the interval is overlapping", i->second[j]->WorldLevel);
				return false;
			}
		}
	}

	return true;
}

void WorldLevelMgr::ClearFile()
{
	m_oTable.Clear();
	m_worldLevelMap.clear();
}

void WorldLevelMgr::OnGetMsWorldLevel(UINT32 level)
{
	if (m_worldLevelNow != level)
	{
		PtcG2C_WorldLevelNtf2Client oPtc;
		oPtc.m_Data.set_worldlevel(level);
		for (auto i = RoleManager::Instance()->Begin(); i != RoleManager::Instance()->End(); ++i)
		{
			(*i)->Send(oPtc);
		}
	}
	m_worldLevelNow = level;

	DragonConfig::Instance()->SyncWorldLevel(level);
	LogInfo("Now worldLevel is %u", level);
}

double WorldLevelMgr::GetExpBuff(UINT32 level, UINT64 roleid)
{
	const WorldLevelConf* conf = GetWorldLevelConf(level, roleid);
	if (conf == NULL)
	{
		return 1.0;
	}
	return conf->ExpBuff;
}

double WorldLevelMgr::GetBackExpBuff(UINT32 level, UINT64 roleid)
{
	const WorldLevelConf* conf = GetWorldLevelConf(level, roleid);
	if (conf == NULL)
	{
		return 1.0;
	}
	return conf->BackExpBuff;
}

const WorldLevelConf* WorldLevelMgr::GetWorldLevelConf(UINT32 level, UINT64 roleid)
{
	UINT32 worldlevel = 0;
	if (GSConfig::Instance()->IsCrossGS())
	{
		auto pData = ServerCrossDataMgr::Instance()->GetDataByRoleID(roleid);
		if (pData)
		{
			worldlevel = pData->worldlevel();
		}
	}
	else
	{
		worldlevel = m_worldLevelNow;
	}
	auto iter = m_worldLevelMap.find(worldlevel);
	if(iter == m_worldLevelMap.end())
	{
		return NULL;
	}
	const std::vector<WorldLevelConf*>& vec = iter->second;
	for(UINT32 i = 0; i < vec.size(); ++i)
	{
		if(level >= vec[i]->Level[0] && level <= vec[i]->Level[1])
		{
			return vec[i];
		}
	}
	return NULL;
}