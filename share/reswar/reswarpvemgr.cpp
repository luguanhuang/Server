#include "pch.h"
#include "reswarpvemgr.h"
#include "util/XRandom.h"
#include "util/gametime.h"



INSTANCE_SINGLETON(ResWarAwardMgr);

ResWarAwardMgr::ResWarAwardMgr()
{

}

ResWarAwardMgr::~ResWarAwardMgr()
{

}

bool ResWarAwardMgr::Init()
{
	return LoadFile();
}

void ResWarAwardMgr::Uninit()
{
	ClearFile();
}


bool ResWarAwardMgr::CheckFile()
{
	GuildMineralBattle tempGuildMineralBattle;
	if (!tempGuildMineralBattle.LoadFile("table/GuildMineralBattle.txt"))
	{
		SSWarn<<"load table/GuildMineralBattleTable.txt failed"<<END;
		return false;
	}
	GuildArenaBattlePvpReward tempGuildArenaBattlePvpReward;
	if (!tempGuildArenaBattlePvpReward.LoadFile("table/GuildArenaBattlePvpReward.txt"))
	{
		SSWarn<<"load table/GuildArenaBattlePvpReward.txt failed"<<END;
		return false;
	}
	GuildBuffTable tempGuildBuffTable;
	if (!tempGuildBuffTable.LoadFile("table/GuildBuff.txt"))
	{
		LogWarn("Load GuildBuff.txt failed!");
		return false;
	}
	GuildMineralBufflist tempGuildMineralBufflist;
	if (!tempGuildMineralBufflist.LoadFile("table/GuildMineralBufflist.txt"))
	{
		LogWarn("Load GuildMineralBufflist.txt failed!");
		return false;
	}

	GuildMineralBattleReward  tmpGuildMineralBattleReward;
	if (!tmpGuildMineralBattleReward.LoadFile("table/GuildMineralBattleReward.txt"))
	{
		LogWarn("Load GuildMineralBufflist.txt failed!");
		return false;
	}


	Rite tmpRiteTable;
	if (!tmpRiteTable.LoadFile("table/Rite.txt"))
	{
		LogWarn("Load Rite.txt failed!");
		return false;
	}

	return true;
}

bool ResWarAwardMgr::LoadFile()
{
	bool bRet = CheckFile();
	if (bRet == false)
	{
		return false;
	}
	ClearFile();
	if (!m_guildMineralBattleTable.LoadFile("table/GuildMineralBattle.txt"))
	{
		SSWarn<<"load table/GuildMineralBattleTable.txt failed"<<END;
		return false;
	}
	if (!m_guildArenaBattlePvpReward.LoadFile("table/GuildArenaBattlePvpReward.txt"))
	{
		SSWarn<<"load table/GuildArenaBattlePvpReward.txt failed"<<END;
		return false;
	}

	if (!m_guildBuffTable.LoadFile("table/GuildBuff.txt"))
	{
		LogWarn("Load GuildBuff.txt failed!");
		return false;
	}

	if (!m_guildMineralBufflist.LoadFile("table/GuildMineralBufflist.txt"))
	{
		LogWarn("Load GuildMineralBufflist.txt failed!");
		return false;
	}

	if (!m_mineralBattleReward.LoadFile("table/GuildMineralBattleReward.txt"))
	{
		LogWarn("Load GuildMineralBattleReward.txt failed!");
		return false;
	}


	if (!m_riteTable.LoadFile("table/Rite.txt"))
	{
		LogWarn("Load Rite.txt failed!");
		return false;
	}

	for (auto iter = m_guildMineralBufflist.Table.begin(); iter!=m_guildMineralBufflist.Table.end();iter++)
	{
		GuildMineralBufflist::RowData *pRowData = *iter;
		m_mapGuildMineralBuff[pRowData->type].push_back(pRowData);
	}

	for (UINT32 nType =1; nType <= 3; nType++)
	{
		if(m_mapGuildMineralBuff[nType].size()<=0)
		{
			LogWarn("Load GuildMineralBufflist.txt    m_mapGuildMineralBuff   failed!");
			return false;
		}
	}
	return true;
}

void ResWarAwardMgr::ClearFile()
{
	m_mapGuildMineralBuff.clear();
	m_guildMineralBattleTable.Clear();
	m_guildArenaBattlePvpReward.Clear();
	m_guildBuffTable.Clear();
	m_guildMineralBufflist.Clear();
	m_mineralBattleReward.Clear();
	m_riteTable.Clear();
}

GuildArenaBattlePvpReward::RowData* ResWarAwardMgr::GetPVPAward(UINT32 nLevel)
{
	for (auto iter = m_guildArenaBattlePvpReward.Table.begin(); iter!=m_guildArenaBattlePvpReward.Table.end();iter++)
	{
		GuildArenaBattlePvpReward::RowData *pRowData = *iter;
		if (pRowData->Level == nLevel)
		{
			return pRowData;
		}
	}
	return NULL;
}

UINT32 ResWarAwardMgr::GetLevelSeal(UINT32 nSealType)
{
	UINT32 nMax = 0;
	for (auto iter = m_mineralBattleReward.Table.begin(); iter!=m_mineralBattleReward.Table.end();iter++)
	{
		GuildMineralBattleReward::RowData *pRowData = *iter;
		if (nSealType==pRowData->LevelSeal)
		{
			return nSealType;
		}
		if (pRowData->LevelSeal>nMax)
		{
			nMax = pRowData->LevelSeal;
		}
	}
	return nMax;

}

UINT32 ResWarAwardMgr::GetWorstRank()
{
	UINT32 nMax = 0;
	for (auto iter = m_mineralBattleReward.Table.begin(); iter!=m_mineralBattleReward.Table.end();iter++)
	{
		GuildMineralBattleReward::RowData *pRowData = *iter;
		if (nMax<pRowData->Rank)
		{
			nMax = pRowData->Rank;
		}
	}
	return nMax;
}


GuildMineralBattleReward::RowData* ResWarAwardMgr::GetGuildMineralBattleReward(UINT32 nRank,UINT32 nSealType)
{
	for (auto iter = m_mineralBattleReward.Table.begin(); iter!=m_mineralBattleReward.Table.end();iter++)
	{
		GuildMineralBattleReward::RowData *pRowData = *iter;
		if (nRank==pRowData->Rank&&nSealType==pRowData->LevelSeal)
		{
			return pRowData;
		}
	}
	return NULL;
}


bool ResWarAwardMgr::IsItemBuff(UINT32 nItemID)
{
	for (auto iter = m_guildBuffTable.Table.begin(); iter != m_guildBuffTable.Table.end(); iter++)
	{
		GuildBuffTable::RowData *pRowData = *iter;
		if (pRowData->itemid == nItemID)
		{
			return true;
		}
	}
	return false;
}


GuildMineralBufflist::RowData* ResWarAwardMgr::GetRandBuffList()
{
	UINT32 nRand = XRandom::randInt(1,3);
	UINT32 nPos  = XRandom::randInt(0,m_mapGuildMineralBuff[nRand].size()-1);
	return m_mapGuildMineralBuff[nRand][nPos];
}

GuildMineralBufflist::RowData* ResWarAwardMgr::GetGuildMineralBufflist(UINT32 i)
{
	return m_guildMineralBufflist.GetByBuffID(i);
}

GuildMineralBufflist::RowData* ResWarAwardMgr::GetGuildMineralBufflistMine(UINT32 i)
{
	GuildMineralBufflist::RowData*pRowData = m_guildMineralBufflist.GetByBuffID(i);
	if (pRowData&&pRowData->type!=1)
	{
		return pRowData;
	}
	return NULL;
}



GuildMineralBattle::RowData* ResWarAwardMgr::GetPVEAward(UINT32 nID)
{
	return m_guildMineralBattleTable.GetByID(nID);
}


UINT32 ResWarAwardMgr::GetMineralLevelSeal(UINT32 nLevelSeal,std::map<UINT32,UINT32> &mapMineral)
{
	UINT32 nTotal = 0;
	GuildMineralBattle::RowData *pRowData  = NULL;
	for (auto iter = m_guildMineralBattleTable.Table.begin(); iter != m_guildMineralBattleTable.Table.end(); iter++)
	{
		pRowData = *iter;
		if (pRowData->Level == nLevelSeal)
		{
			nTotal += pRowData->Percent;
			mapMineral[pRowData->ID] = nTotal;
		}
	}
	return nTotal;
}



UINT32 ResWarAwardMgr::GetRiteTime(UINT32 nDurationTime)
{
	time_t nDiffTime = GameTime::GetTime() - nDurationTime;
	for (auto iter = m_riteTable.Table.begin(); iter!=m_riteTable.Table.end(); iter++)
	{
		Rite::RowData *pRowData = *iter;
		if (pRowData)
		{
			if(nDiffTime>=pRowData->starttime && nDiffTime<pRowData->endtime)
				return pRowData->cd;
		}
	}
	return 0;
}



GuildBuffTable::RowData * ResWarAwardMgr::GetGuildBuffID(UINT32 nID)
{
	for (UINT32 i = 0; i< m_guildBuffTable.Table.size(); i++)
	{
		GuildBuffTable::RowData* pRowData =  m_guildBuffTable.Table[i];
		if (pRowData->id==nID)
		{
			return pRowData;
		}
	}
	return NULL;
}


GuildBuffTable::RowData * ResWarAwardMgr::GetGuildBuffItemID(UINT32 nItemID)
{
	for (UINT32 i = 0; i< m_guildBuffTable.Table.size(); i++)
	{
		GuildBuffTable::RowData* pRowData =  m_guildBuffTable.Table[i];
		if (pRowData->itemid==nItemID)
		{
			return pRowData;
		}
	}
	return NULL;
}