#include "pch.h"
#include "table/expeditionconfigmgr.h"
#include "util/XRandom.h"
#include "define/teamdef.h"
#include "define/itemdef.h"
#include "util/XCommon.h"
#include "globalconfig.h"

INSTANCE_SINGLETON(ExpeditionConfigMgr)

ExpeditionConfigMgr::ExpeditionConfigMgr()
{

}

ExpeditionConfigMgr::~ExpeditionConfigMgr()
{

}

bool ExpeditionConfigMgr::Init()
{
	return LoadFile();
}

void ExpeditionConfigMgr::Uninit()
{
	ClearFile();
}

bool ExpeditionConfigMgr::CheckFile()
{
	ExpeditionTable tmpExpeditionTable;	
	if(!tmpExpeditionTable.LoadFile("table/DNExpedition.txt"))
	{
		LogWarn("load table/DNExpedition.txt failed!");
		return false;
	}

	RandomSceneTable tmpRandomSceneTable;
	if (!tmpRandomSceneTable.LoadFile("table/RandomSceneList.txt"))
	{
		LogWarn("load table/RandomSceneList.txt failed!");
		return false;
	}

	TeamCostReward tmpTeamCostReward;
	if (!tmpTeamCostReward.LoadFile("table/TeamCostReward.txt"))
	{
		LogWarn("load table/TeamCostReward.txt failed!");
		return false;
	}

	NestWeekList nwTable;
	if(!nwTable.LoadFile("table/NestWeekList.txt"))
	{
		SSWarn<<"load table/NestWeekList.txt failed!"<<END;
		return false;
	}

	AbyssPartyListTable abyssTable;
	if(!abyssTable.LoadFile("table/AbyssPartyList.txt"))
	{
		SSWarn<<"load table/AbyssPartyList.txt failed!"<<END;
		return false;
	}

	DragonNestTable dnTable;
	if(!dnTable.LoadFile("table/DragonNestList.txt"))
	{
		SSWarn<<"load table/DragonNestList.txt failed!"<<END;
		return false;
	}

	return true;
}

bool ExpeditionConfigMgr::LoadFile()
{
	if(!CheckFile())
	{
		return false;
	}

	ExpeditionTable tmpExpeditionTable;	
	tmpExpeditionTable.LoadFile("table/DNExpedition.txt");
	m_oExpeditionTable.CopyFrom(&tmpExpeditionTable);

	m_oRandomSceneTable.Clear();
	m_oRandomSceneTable.LoadFile("table/RandomSceneList.txt");

	m_costrewardtable.Clear();	
	m_costrewardtable.LoadFile("table/TeamCostReward.txt");

	m_nestWeekTable.Clear();
	m_nestWeekTable.LoadFile("table/NestWeekList.txt");

	m_abyssParty.Clear();
	m_abyssParty.LoadFile("table/AbyssPartyList.txt");

	m_dnListTable.Clear();
	m_dnListTable.LoadFile("table/DragonNestList.txt");

	m_RandomSceneGroup.clear();
	m_fastMatch.clear();
	m_downMatch.clear();

	m_TeamType2Exp.clear();
	m_expid2reward.clear();
	m_star2exp.clear();

	BuildRandomSceneGroup();
	BuildFastMatch();
	BuildDownMatch();
	BuildTeamCost();
    BuildTeamType2Conf();
	BuildStarExp();
	return true;
}

void ExpeditionConfigMgr::ClearFile()
{
	m_oExpeditionTable.Clear();
	m_oRandomSceneTable.Clear();
	m_costrewardtable.Clear();	
	m_nestWeekTable.Clear();
	m_abyssParty.Clear();
	m_dnListTable.Clear();

	m_RandomSceneGroup.clear();
	m_fastMatch.clear();
	m_downMatch.clear();

	m_TeamType2Exp.clear();
	m_expid2reward.clear();
	m_star2exp.clear();
}

UINT32 ExpeditionConfigMgr::GetRandomScene(UINT32 randomID)
{
	auto i = m_RandomSceneGroup.find(randomID);
	if (i == m_RandomSceneGroup.end())
	{
		return 0;
	}
	return i->second.GetRandomScene();
}

UINT32 ExpeditionConfigMgr::GetExpRandomScene(int expID)
{
	auto pConf = GetExpeditionConf(expID);
	if(NULL == pConf)
	{
		return 0;
	}
	if(pConf->RandomSceneIDs.empty())
	{
		return 0;
	}
	return GetRandomScene(pConf->RandomSceneIDs[0]); 
}

void ExpeditionConfigMgr::BuildRandomSceneGroup()
{
	for (UINT32 i = 0; i < m_oRandomSceneTable.Table.size(); ++i)
	{
		RandomSceneTable::RowData *pRow = m_oRandomSceneTable.Table[i];
		RandomSceneGroup& group = m_RandomSceneGroup[pRow->RandomID];
		group.totalProb += pRow->Prob;
		group.prob2SceneID.insert(std::make_pair(group.totalProb, pRow->SceneID));
	}
}

void ExpeditionConfigMgr::BuildFastMatch()
{
	m_fastMatch.clear();
	for(size_t i = 0; i < m_oExpeditionTable.Table.size(); ++i)
	{
		ExpeditionTable::RowData *pRow = m_oExpeditionTable.Table[i];
		if(pRow->fastmatch)
		{
			m_fastMatch.push_back(pRow->DNExpeditionID);
		}
	}
}

void ExpeditionConfigMgr::BuildDownMatch()
{
	m_downMatch.clear();
	for(size_t i = 0; i < m_oExpeditionTable.Table.size(); ++i)
	{
		ExpeditionTable::RowData *pRow = m_oExpeditionTable.Table[i];
		if(CanDownMatch(pRow))
		{
			m_downMatch.push_back(pRow->DNExpeditionID);
		}
	}
}

ExpeditionTable::RowData * ExpeditionConfigMgr::GetExpeditionConf(int expID)
{
	for (UINT32 i = 0; i < m_oExpeditionTable.Table.size(); ++i)
	{
		ExpeditionTable::RowData *pRow = m_oExpeditionTable.Table[i];
		if (pRow->DNExpeditionID == expID)
		{
			return pRow;
		}
	}

	return NULL;
}

DragonNestTable::RowData * ExpeditionConfigMgr::GetDnListConf(int expID)
{
	for (UINT32 i = 0; i < m_dnListTable.Table.size(); ++i)
	{
		DragonNestTable::RowData *pRow = m_dnListTable.Table[i];
		if (pRow->DragonNestID == expID)
		{
			return pRow;
		}
	}
	return NULL;
}

//ÉîÔ¨ÊÇsceneIDÎ¨Ò»
UINT32 ExpeditionConfigMgr::GetAbyssSceneByExpID( int expID )
{
	ExpeditionTable::RowData* data = GetExpeditionConf(expID); 
	if(data && !data->RandomSceneIDs.empty())
	{
		return GetRandomScene(data->RandomSceneIDs[0]);
	}
	return 0;
}

int ExpeditionConfigMgr::GetAbyssExpIDByScene( UINT32 sceneTempid )
{
	ExpeditionTable::RowData* data = GetAbyssConfByScene(sceneTempid);
	if(data)
	{
		return data->DNExpeditionID;
	}
	return 0;
}

ExpeditionTable::RowData* ExpeditionConfigMgr::GetAbyssConfByScene( UINT32 sceneTempid )
{
	for(size_t i = 0; i < m_oExpeditionTable.Table.size(); ++i)
	{
		ExpeditionTable::RowData* data = m_oExpeditionTable.Table[i];
		if(ABYSS_TEAM_SCENE != data->Type)
		{
			continue;
		}
		if(!data->RandomSceneIDs.empty() && GetRandomScene(data->RandomSceneIDs[0]) == sceneTempid)
		{
			return data;
		}
	}
	return NULL;
}

UINT32 ExpeditionConfigMgr::GetPkTwoScene()
{
	for(size_t i = 0; i < m_oExpeditionTable.Table.size(); ++i)
	{
		ExpeditionTable::RowData* data = m_oExpeditionTable.Table[i];
		if(TEAM_TYPE_PKTWO == data->Type && !data->RandomSceneIDs.empty())
		{
			return GetRandomScene(data->RandomSceneIDs[0]);
		}
	}
	return 0;
}

UINT32 ExpeditionConfigMgr::GetPvpScene()
{
	for(size_t i = 0; i < m_oExpeditionTable.Table.size(); ++i)
	{
		ExpeditionTable::RowData* data = m_oExpeditionTable.Table[i];
		if(PVP_TEAM_SCENE == data->Type && !data->RandomSceneIDs.empty())
		{
			return GetRandomScene(data->RandomSceneIDs[0]);
		}
	}
	return 0;
}

UINT32 ExpeditionConfigMgr::GetHeroBattleScene()
{
	for(size_t i = 0; i < m_oExpeditionTable.Table.size(); ++i)
	{
		ExpeditionTable::RowData* data = m_oExpeditionTable.Table[i];
		if(HEROBATTLE_TEAM_SCENE == data->Type)
		{
			if (!data->RandomSceneIDs.empty())
			{
				return GetRandomScene(data->RandomSceneIDs[0]);
			}
		}
	}
	return 0;
}

UINT32 ExpeditionConfigMgr::GetMobaBattleScene()
{
	for(size_t i = 0; i < m_oExpeditionTable.Table.size(); ++i)
	{
		ExpeditionTable::RowData* data = m_oExpeditionTable.Table[i];
		if(MOBABATTLE_TEAM_SCENE == data->Type)
		{
			if (!data->RandomSceneIDs.empty())
			{
				return GetRandomScene(data->RandomSceneIDs[0]);
			}
		}
	}
	return 0;
}

bool ExpeditionConfigMgr::CanDownMatch(ExpeditionTable::RowData* conf)
{
	if(conf && TEAM_TOWER == conf->Type)
	{
		return true;
	}
	return false;
}

UINT32 ExpeditionConfigMgr::RandomSceneGroup::GetRandomScene()
{
	UINT32 x = XRandom::randInt(0, totalProb);
	auto i = prob2SceneID.upper_bound(x);
	if (i == prob2SceneID.end())
	{
		return 0;
	}
	return i->second;
}

const Sequence<UINT32, 3>* ExpeditionConfigMgr::GetTeamCost(UINT32 expid, UINT32 index)
{
	ExpeditionTable::RowData* data = ExpeditionConfigMgr::Instance()->GetExpeditionConf(expid);
	if (NULL == data)
	{
		return NULL;
	}
	if (index >= data->CostType.size())
	{
		return NULL;
	}
	return &data->CostType[index];
}

ItemDesc ExpeditionConfigMgr::GetTeamCostReward(UINT32 count, UINT32 expid)
{
	ItemDesc item(DRAGON_COIN, 0);
	auto i = m_expid2reward.find(expid);
	if (i != m_expid2reward.end())
	{
		for (auto j = i->second.begin(); j != i->second.end(); ++j)
		{
			if (count <= j->first)
			{
				UINT32 allrandom = 0;
				for (auto k = j->second.begin(); k != j->second.end(); ++k)
				{
					allrandom += k->seq[1];	
				}
				UINT32 random = XRandom::randInt(0, allrandom + 1);
				UINT32 temp = 0;
				for (auto k = j->second.begin(); k != j->second.end(); ++k)
				{
					temp += k->seq[1];
					if (random <= temp)
					{
						item.itemID = DRAGON_COIN;
						item.itemCount = k->seq[0];
						break;
					}
				}
				break;
			}
		}
	}
	return item;
}

void ExpeditionConfigMgr::BuildTeamCost()
{
	for (auto i = m_costrewardtable.Table.begin(); i != m_costrewardtable.Table.end(); ++i)
	{
		TeamCostReward::RowData* data = *i;
		for (auto j = data->Reward.begin(); j != data->Reward.end(); ++j)
		{
			m_expid2reward[data->DNExpeditionID][data->Count].push_back(*j);
		}
	}
}

UINT32 ExpeditionConfigMgr::GetLevelSealTeamCost(UINT32 expid)
{
	ExpeditionTable::RowData* data = GetExpeditionConf(expid);
	if (NULL == data)
	{
		return 0;
	}
	return data->LevelSealTeamCost;
}

int ExpeditionConfigMgr::GetNestWeekCount(int sealType)
{
	auto pConf = m_nestWeekTable.GetBySealType(sealType);
	if(pConf)
	{
		return pConf->WeekCount;
	}
	return 0;
}




void ExpeditionConfigMgr::GetNestWeekExp(int sealType, std::vector<int>& expIDs)
{
	auto pConf = m_nestWeekTable.GetBySealType(sealType);
	if(pConf)
	{
		expIDs = pConf->ExpID;
	}
}

void ExpeditionConfigMgr::BuildTeamType2Conf()
{
    for(size_t i = 0; i < m_oExpeditionTable.Table.size(); ++i)
    {
        auto pConf = m_oExpeditionTable.Table[i];
        if(NULL == pConf)
        {
            continue;
        }
        int teamtype = pConf->Type;
        int expid = pConf->DNExpeditionID;
        auto it = m_TeamType2Exp.find(teamtype);
        if(it == m_TeamType2Exp.end())
        {
            m_TeamType2Exp[teamtype] = expid;
        }
    }    
}

ExpeditionTable::RowData* ExpeditionConfigMgr::GetTeamType2ExpConf(int teamtype)
{
    auto it = m_TeamType2Exp.find(teamtype);
    if(it != m_TeamType2Exp.end())
    {
        return GetExpeditionConf((it->second));
    }
    return NULL;
}

const AbyssPartyListTable::RowData* ExpeditionConfigMgr::GetAbssParty(const UINT32 nID)
{
	return m_abyssParty.GetByID(nID);
}


void ExpeditionConfigMgr::BuildStarExp()
{
	for(size_t i = 0; i < m_oExpeditionTable.Table.size(); ++i)
	{
		if(!IsStarExp(m_oExpeditionTable.Table[i]))
		{
			continue;
		}
		const Sequence<UINT32,2>& seq = m_oExpeditionTable.Table[i]->Stars; 
		UINT32 type = seq[1];
		UINT32 star = seq[0];
		int expid = m_oExpeditionTable.Table[i]->DNExpeditionID;
		m_star2exp[type][star] = expid;
	}
}

void ExpeditionConfigMgr::GetAllStarExpByType(UINT32 type, std::map<UINT32, int>& expids)
{
	auto it = m_star2exp.find(type);
	if(it == m_star2exp.end())
	{
		return;
	}
	expids = it->second;
}

bool ExpeditionConfigMgr::IsStarExp(ExpeditionTable::RowData* pRow)
{
	if(NULL == pRow)
	{
		return false;
	}
	if(0 == pRow->Stars[0] || 0 == pRow->Stars[1])
	{
		return false;
	}
	return true;
}

UINT32 ExpeditionConfigMgr::GetStarExpStar(ExpeditionTable::RowData* pRow)
{
	if(NULL == pRow)
	{
		return 0;
	}
	return pRow->Stars[0];
}

UINT32 ExpeditionConfigMgr::GetStarExpType(ExpeditionTable::RowData* pRow)
{
	if(NULL == pRow)
	{
		return 0;
	}
	return pRow->Stars[1];
}

bool ExpeditionConfigMgr::IsGrayExp(int expID)
{
	auto pConf = GetExpeditionConf(expID);
	if(pConf)
	{
		return (DRAGON_TEAM_SCENE == pConf->Type);
	}
	return false;
}

bool ExpeditionConfigMgr::NeedInTs(int expID, UINT32 serverID)
{
	if(!NeedInTs(expID))
	{
		return false;
	}
	else
	{
		if(!GetGlobalConfig().TeamGrayOpen || !IsGrayExp(expID))
		{
			return true;
		}
		else
		{
			const auto& sids = GetGlobalConfig().TeamGrayServerIDs;
			if(sids.find(serverID) != sids.end())
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}

UINT32 ExpeditionConfigMgr::GetBioHellScene(ExpeditionTable::RowData* pRow)
{
	if (NULL == pRow)
	{
		return 0;
	}
	if(TEAM_TYPE_BIOHELL == pRow->Type && !pRow->RandomSceneIDs.empty())
	{
		return GetRandomScene(pRow->RandomSceneIDs[0]);
	}
	return 0;
}

bool ExpeditionConfigMgr::NeedInTs(int expID)
{
	auto pConf = GetExpeditionConf(expID);
	if(pConf)
	{
		return (pConf->isCross);
	}
	return false;
}

UINT32 ExpeditionConfigMgr::GetType2Count(int type)
{
	auto pConf = GetTeamType2ExpConf(type);
	if(pConf)
	{
		return pConf->MaxCount;
	}
	return 0;
}

UINT32 ExpeditionConfigMgr::GetExpOpenTime(int expID, bool isBackFlow, UINT32 backFlowLevel, UINT32 serverOpenTime)
{
	UINT32 configTime = 0;
	ExpeditionTable::RowData *pRow = GetExpeditionConf(expID);
	if(pRow)
	{
		UINT32 tday= 0;
		UINT32 thour = 0; 
		if(isBackFlow)
		{
			size_t n = pRow->BackServerOpenTime.size();
			if(n > 0)
			{
				UINT32 level = isBackFlow;
				tday = pRow->BackServerOpenTime[n-1][1];
				thour = pRow->BackServerOpenTime[n-1][2];
				for(size_t i = 0; i < n; ++i)
				{
					if(level <= pRow->BackServerOpenTime[i][0])
					{
						tday = pRow->BackServerOpenTime[i][1];
						thour = pRow->BackServerOpenTime[i][2];
						break;	
					}
				}
			}
		}
		else
		{
			tday = pRow->ServerOpenTime[0];
			thour = pRow->ServerOpenTime[1];
		}
		configTime = (tday * 24 + thour) * 3600;
	}

	return UINT32(XCommon::GetDayBeginTime(serverOpenTime, false) + configTime);
}

