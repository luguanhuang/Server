#include "pch.h"
#include "competedragonconfig.h"
#include "foreach.h"
#include "timeutil.h"
#include "scene/sceneconfig.h"
#include "table/globalconfig.h"
#include "util/gametime.h"
#include "common/ptcg2m_updateglobalvalue.h"
#include "define/globalconfigtodbdef.h"
#include "util/timespecificmgr.h"
#include "util/XCommon.h"

INSTANCE_SINGLETON(CompeteDragonConfig)

CompeteDragonConfig::CompeteDragonConfig()
{
}

CompeteDragonConfig::~CompeteDragonConfig()
{

}

bool CompeteDragonConfig::Init()
{
	return LoadFile();
}

void CompeteDragonConfig::Uninit()
{

}

bool CompeteDragonConfig::LoadFile()
{
	m_competeDragonTable.Clear();
	m_competeDragonRankTable.Clear();

	if (!m_competeDragonTable.LoadFile("table/CompeteDragon.txt"))
	{
		LogWarn("Load CompeteDragon.txt failed!");
		return false;
	}

	if (!m_competeDragonRankTable.LoadFile("table/CompeteDragonRankReward.txt"))
	{
		LogWarn("Load CompeteDragonRankReward.txt failed!");
		return false;
	}

	return true;
}

CompeteDragonRankRewardTable::RowData* CompeteDragonConfig::GetCompeteDragonRankData(UINT32 rank)
{
	for(auto iter = m_competeDragonRankTable.Table.begin(); iter != m_competeDragonRankTable.Table.end(); ++iter)
	{
		if (rank <= (*iter)->rank)
		{
			return *iter;
		}
	}

	return NULL;
}

