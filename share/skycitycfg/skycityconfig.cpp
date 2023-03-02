#include "pch.h"
#include "skycityconfig.h"
#include "table/globalconfig.h"

INSTANCE_SINGLETON(SkyCityConfig);


SkyCityConfig::SkyCityConfig(){}

SkyCityConfig::~SkyCityConfig(){}

bool SkyCityConfig::Init()
{
	return LoadFile();
}

void SkyCityConfig::Uninit()
{
	ClearFile();
}

bool SkyCityConfig::CheckFile()
{
	SkyArenaReward tempSkyArenaReward;
	if (!tempSkyArenaReward.LoadFile("table/SkyArenaReward.txt"))
	{
		LogWarn("Load SkyArenaReward.txt failed!");
		return false;
	}
	return true;
}

bool SkyCityConfig::LoadFile()
{
	bool bRet = CheckFile();
	if (bRet==false)
	{
		return false;
	}
	ClearFile();
	if (!m_rewardTable.LoadFile("table/SkyArenaReward.txt"))
	{
		LogWarn("Load SkyArenaReward.txt failed!");
		return false;
	}
	return true;
}

void SkyCityConfig::ClearFile()
{
	m_rewardTable.Clear();
}

UINT32 SkyCityConfig::GetLevelSeal(UINT32 nLevelSeal)
{
	UINT32 nMaxSeal = 0;
	for(auto iter = m_rewardTable.Table.begin(); iter != m_rewardTable.Table.end(); iter++)
	{
		SkyArenaReward::RowData *pRow = *iter;
		if (pRow->LevelSegment ==nLevelSeal)
		{
			return nLevelSeal;
		}
		if (pRow->LevelSegment>nMaxSeal)
		{
			nMaxSeal = pRow->LevelSegment;
		}
	}
	return nMaxSeal;
}


const SkyArenaReward::RowData* SkyCityConfig::GetSkyArenaReward(int nLv, int nFloor)
{
	SkyArenaReward::RowData *pRet = NULL;
	for(auto iter = m_rewardTable.Table.begin(); iter != m_rewardTable.Table.end(); iter++)
	{
		SkyArenaReward::RowData *pRow = *iter;
		if (pRow->LevelSegment ==nLv &&pRow->Floor == nFloor)
		{
			pRet = pRow;
			break;
		}
	}
	return pRet;
}
