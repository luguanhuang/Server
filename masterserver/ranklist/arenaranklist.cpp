#include "pch.h"
#include "arenaranklist.h"
#include "arena/arenamgr.h"
#include "role/rolesummarymgr.h"
#include "global/GlobalConfigDBBuild.h"
#include "table/globalconfig.h"
#include "network/dblink.h"
#include "util/XCommon.h"
#include "ranklistmgr.h"
#include "table/ArenaNpcTable.h"
#include "ranklist/ptcm2d_saveranklistchangetodb.h"
#include "account/punishmgr.h"
#include "event/eventmgr.h"

CArenaRankInfo::CArenaRankInfo(UINT64 roleid)
:CRankInfo(roleid)
{
	Reset();
}

CArenaRankInfo::~CArenaRankInfo()
{

}

void CArenaRankInfo::Reset()
{

}

void CArenaRankInfo::RankInfoToKKSG(KKSG::RankData* rankdata) const
{
	if (NULL == rankdata || 0 == GetUniqueId())
	{
		return;
	}
	if (ArenaMgr::Instance()->IsArenaNpc(GetUniqueId()))
	{
		const ArenaNpcInfo* pNpcInfo = ArenaMgr::Instance()->GetArenaNpcInfo(GetUniqueId());
		if (NULL == pNpcInfo)
		{
			SSWarn<<"pNpcInfo is NULL"<<END;
			return;
		}
		rankdata->set_roleid(0);
		rankdata->set_rolename(pNpcInfo->szNpcName);
		rankdata->set_rolelevel(ArenaMgr::Instance()->GetNpcBattleLevel(rankdata->rank()));
		rankdata->set_is_vip(false);
		rankdata->set_is_svip(false);
		rankdata->set_starttype(KKSG::StartUp_Normal);
	}
	else
	{
		CRoleSummary* pRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(GetUniqueId());
		if (NULL == pRoleSummary)
		{
			SSWarn<<"pRoleSummary is NULL"<<END;
			return;
		}
		rankdata->set_roleid(GetUniqueId());
		rankdata->set_rolename(pRoleSummary->GetName());
		rankdata->set_rolelevel(pRoleSummary->GetLevel());
		rankdata->set_is_vip(pRoleSummary->IsQQVip());
		rankdata->set_is_svip(pRoleSummary->IsQQSVip());
		rankdata->set_starttype(pRoleSummary->GetStartUpType());
	}
}

void CArenaRankInfo::RankInfoToDBKKSG(KKSG::RankData* rankdata) const
{
	if (NULL == rankdata)
	{
		return;
	}
	rankdata->set_roleid(GetUniqueId());
}

void CArenaRankInfo::DBKKSGToRankInfo(const KKSG::RankData* rankdata)
{
	if (NULL == rankdata)
	{
		return;
	}
	SetUniqueId(rankdata->roleid());
}

/////////////////////////////////////////////////////////////////////////
void CArenaRealTimeDb::PackDBData(KKSG::RankList* ranklist)
{
	const std::vector<CRankInfo*>* rankinfos = &(static_cast<CArenaRealTimeRankList*>(m_ranklist))->m_rankinfos;
	for (auto i = rankinfos->begin(); i != rankinfos->end(); ++i)
	{
		CRankInfo* rankinfo = *i;
		if (NULL == rankinfo)
		{
			continue;
		}
		KKSG::RankData* rankdata = ranklist->add_rankdata();
		rankinfo->RankInfoToDBKKSG(rankdata);
	}
}

void CArenaRealTimeDb::UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank)
{
	CArenaRealTimeRankList* ranklist = static_cast<CArenaRealTimeRankList*>(m_ranklist);
	const std::vector<CRankInfo*>* rankinfos = &ranklist->m_rankinfos;
	if (rankinfos->size() < rank)
	{
		return;
	}
	CRankInfo* rankinfo = (*rankinfos)[rank - 1];
	if (NULL == rankinfo)
	{
		return;
	}
	ranklist->SetRank(rankinfo->GetUniqueId(), INVALID_RANK);
	ranklist->m_outsideRobot.insert(rankinfo->GetUniqueId()); //不在排行榜里的机器人id
	rankinfo->DBKKSGToRankInfo(&rankdata);
	ranklist->SetRank(rankinfo->GetUniqueId(), rank);
}

bool CArenaRealTimeDb::BeforeSaveToDB()
{ 
	CArenaRealTimeRankList* ranklist = static_cast<CArenaRealTimeRankList*>(m_ranklist);
	if (ranklist->m_roleid2changed.empty())
	{
		return false;
	}
	//PtcM2D_SaveRankListChangeToDB ptc;
	//ptc.m_Data.set_ranktype(ranklist->GetType());

	//KKSG::RankList* changed = ptc.m_Data.mutable_ranklistchange();
	//for (auto i = ranklist->m_roleid2changed.begin(); i != ranklist->m_roleid2changed.end(); ++i)
	//{
	//	KKSG::RankData* pRankData = changed->add_rankdata();
	//	pRankData->set_roleid(i->first);
	//	pRankData->set_rank(i->second);
	//}
	//DBLink::Instance()->SendTo(ptc);
		
	ranklist->m_roleid2changed.clear();
	
	return true;
}

CArenaRealTimeRankList::CArenaRealTimeRankList(int type)
:CRankList(type)
{ 
	if (type > KKSG::RankeType_MAX)
	{
		SSWarn<<"rank type:%d"<<type<<END;
		return;
	}
	TimeMark timemark("new real time arena rank list");

	//ArenaNpcTable* npcs = ArenaMgr::Instance()->GetNpcTable();
	//m_rankinfos.reserve(npcs->Count());
	//m_roleid2rank.rehash(npcs->Count());

	//UINT32 rank = 0;
	//for (auto i = npcs->m_vecNpcInfo.begin(); i != npcs->m_vecNpcInfo.end(); ++i)
	//{
	//	AddRankInfo((*i)->qwNpcId, ++rank);
	//}

	//RegisterDb(new CArenaRealTimeDb);

	//SaveToDBEveryPeriod* policy = new SaveToDBEveryPeriod;	
	//policy->SetSaveInterval(ArenaUpdateInterVal);
	//m_db->SetPolicy(policy);

	ArenaMgr::Instance()->SetRankList(this); 
}

CArenaRealTimeRankList::~CArenaRealTimeRankList()
{
	for (auto i = m_rankinfos.begin(); i != m_rankinfos.end(); ++i)
	{
		if (NULL != *i)
		{
			delete *i;
			*i = NULL;
		}
	}
}

void CArenaRealTimeRankList::RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession)
{
	count = count > m_rankinfos.size() ? m_rankinfos.size() : count;
	for (UINT32 i = 0; i < count; ++i)
	{
		if (NULL == m_rankinfos[i])
		{
			continue;
		}
		KKSG::RankData* pRankData = rankdatas->add_rankdata();
		pRankData->set_rank(i + 1);
		m_rankinfos[i]->RankInfoToKKSG(pRankData);
	}
}

void CArenaRealTimeRankList::RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank)
{
	if (rank > m_rankinfos.size())
	{
		return;
	}
	if (NULL == m_rankinfos[rank - 1])
	{
		return;
	}
	m_rankinfos[rank - 1]->RankInfoToKKSG(rankdata);
}

UINT32 CArenaRealTimeRankList::GetNextEmptyPos(UINT64 roleid)
{
	return INVALID_RANK;

	if(CPunishMgr::Instance()->IsPunish(roleid, KKSG::PUNISH_USER_ARENA_RANK))
	{
		return INVALID_RANK;
	}
	UINT32 offset = convert<UINT32>(CGlobalConfigDBBuild::Instance()->GetConfigValue(eArenaRankInit));
	UINT32 rank = offset + (UINT32)( GetGlobalConfig().ArenaRankBegin);
	if (m_rankinfos.size() < rank)
	{
		return INVALID_RANK;
	}
	CGlobalConfigDBBuild::Instance()->SetConfigValue(eArenaRankInit, ToString(offset + 1));
	CGlobalConfigDBBuild::Instance()->UpdateToDb(eArenaRankInit);

	CRankInfo* rankinfo = m_rankinfos[rank - 1];
	if (SwapPos(roleid, INVALID_RANK, rankinfo->GetUniqueId(), rank))
	{
		return rank;
	}
	else
	{
		return INVALID_RANK;
	}
}

bool CArenaRealTimeRankList::SwapPos(UINT64 roleid1, UINT32 rank1, UINT64 roleid2, UINT32 rank2)
{
	return false;

	CRankInfo* rankinfo1 = NULL;
	CRankInfo* rankinfo2 = NULL;
	if (m_rankinfos.size() >= rank1)
	{
		rankinfo1 = m_rankinfos[rank1 -1];
	}
	if (m_rankinfos.size() >= rank2)
	{
		rankinfo2 = m_rankinfos[rank2 -1];
	}

	if (NULL == rankinfo1 && NULL == rankinfo2)
	{
		return false;
	}
	else if (NULL == rankinfo1 && NULL != rankinfo2)
	{
		rankinfo2->SetUniqueId(roleid1);
		SetRank(roleid1, rank2);
		SetRank(roleid2, INVALID_RANK);
		SetChanged(roleid1, rank2);
		if(ArenaMgr::Instance()->IsArenaNpc(roleid2))
		{
			m_outsideRobot.insert(roleid2);
		}
	}
	else if (NULL != rankinfo1 && NULL == rankinfo2)
	{
		rankinfo1->SetUniqueId(roleid2);
		SetRank(roleid2, rank1);
		SetRank(roleid1, INVALID_RANK);
		SetChanged(roleid2, rank1);
		if(ArenaMgr::Instance()->IsArenaNpc(roleid1))
		{
			m_outsideRobot.insert(roleid1);
		}
	}
	else
	{
		rankinfo1->SetUniqueId(roleid2);
		SetRank(roleid2, rank1);
		rankinfo2->SetUniqueId(roleid1);
		SetRank(roleid1, rank2);
		SetChanged(roleid1, rank2);	
		SetChanged(roleid2, rank1);	
	}
	SetTimeStamp();
	return true;
}

void CArenaRealTimeRankList::DeleteRole(UINT64 roleid)
{
	return;

	if(m_roleid2rank.find(roleid) == m_roleid2rank.end())
	{
		return;
	}
	
	UINT32 rank = m_roleid2rank[roleid];
	if(INVALID_RANK == rank)
	{
		return;
	}

	if(m_outsideRobot.empty())
	{
		LogError("There must be have a outside ranklist robot when the size of player in ranklist >= 1");
		assert(false);
	}
	UINT64 robotid = *m_outsideRobot.begin();
	m_outsideRobot.erase(robotid);
	SwapPos(roleid, rank, robotid, INVALID_RANK);
}

/////////////////////////////////////////////////////////////////////
void CArenaRankDb::PackDBData(KKSG::RankList* ranklist)
{
	const std::vector<CRankInfo*>* rankinfos = &((CArenaRealTimeRankList*)m_ranklist)->m_rankinfos;
	for (auto i = rankinfos->begin(); i != rankinfos->end(); ++i)
	{
		CRankInfo* rankinfo = *i;
		if (NULL == rankinfo)
		{
			continue;
		}
		KKSG::RankData* rankdata = ranklist->add_rankdata();
		rankinfo->RankInfoToDBKKSG(rankdata);
	}
}

void CArenaRankDb::UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank)
{
	CArenaRankList* ranklist = (CArenaRankList*)m_ranklist;
	const std::vector<CRankInfo*>* rankinfos = &ranklist->m_rankinfos;
	if (rankinfos->size() < rank)
	{
		return;
	}
	CRankInfo* rankinfo = (*rankinfos)[rank - 1];
	if (NULL == rankinfo)
	{
		return;
	}
	ranklist->SetRank(rankinfo->GetUniqueId(), INVALID_RANK);
	rankinfo->DBKKSGToRankInfo(&rankdata);
	ranklist->SetRank(rankinfo->GetUniqueId(), rank);
}

bool CArenaRankDb::BeforeSaveToDB()
{
	CArenaRealTimeRankList* realtimeranklist = 
		(CArenaRealTimeRankList*)CRankListMgr::Instance()->GetRankList(KKSG::RealTimeArenaRank);
	if (NULL == realtimeranklist)
	{
		return false;
	}

	CArenaRankList* ranklist = (CArenaRankList*)m_ranklist;
	std::unordered_map<UINT64, UINT32>* roleid2rank = &ranklist->m_roleid2rank;
	for (auto i = roleid2rank->begin(); i != roleid2rank->end(); ++i)
	{
		i->second = INVALID_RANK;
	}

	const std::vector<CRankInfo*>* rankinfos = &realtimeranklist->m_rankinfos;
	UINT32 rank = 0;
	for (auto i = rankinfos->begin(); i != rankinfos->end(); ++i)
	{
		UINT64 roleid = (*i)->GetUniqueId();
		ranklist->ReplaceRankInfo(roleid, ++rank);
		ArenaMgr::Instance()->GiveRankReward(roleid, rank);

		if ((roleid >> 60) == KKSG::Category_Role && rank <= 1000)
		{
			EventMgr::Instance()->AddEvent(roleid, ACH_EVENT_OFFSET + ACHIEVE_COM_TYPE_ARENA_RANK, rank);
			if (rank == 1)
			{
				EventMgr::Instance()->AddEvent(roleid, DESIGNATION_COM_TYPE_ARENA_RANK, rank);
			}
		}
	}
		
	ranklist->SetTimeStamp();

	return true;
}

CArenaRankList::CArenaRankList(int type)
:CRankList(type)
{
	if (type > KKSG::RankeType_MAX)
	{
		SSWarn<<"rank type:"<<type<<END;
		return;
	}
	TimeMark stTimeMark("new arena rank list");
	
	//ArenaNpcTable* npcs = ArenaMgr::Instance()->GetNpcTable();
	//m_rankinfos.reserve(npcs->Count());
	//m_roleid2rank.rehash(npcs->Count());

	//UINT32 rank = 0;
	//for (auto i = npcs->m_vecNpcInfo.begin(); i != npcs->m_vecNpcInfo.end(); ++i)
	//{
	//	AddRankInfo((*i)->qwNpcId, ++rank);
	//}

	//RegisterDb(new CArenaRankDb);

	//SaveToDBAtSpecialTime* policy = new SaveToDBAtSpecialTime;
	//policy->SetSaveTime( GetGlobalConfig().RankUpdateTime[0], GetGlobalConfig().RankUpdateTime[1]);
	//m_db->SetPolicy(policy);
}

CArenaRankList::~CArenaRankList()
{
	for (auto i = m_rankinfos.begin(); i != m_rankinfos.end(); ++i)
	{
		if (NULL != *i)
		{
			delete *i;
			*i = NULL;
		}
	}
}

void CArenaRankList::RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 professsion)
{
	count = count > m_rankinfos.size() ? m_rankinfos.size() : count;
	for (UINT32 i = 0; i < count; ++i)
	{
		if (NULL == m_rankinfos[i])
		{
			continue;
		}
		KKSG::RankData* pRankData = rankdatas->add_rankdata();
		pRankData->set_rank(i + 1);
		m_rankinfos[i]->RankInfoToKKSG(pRankData);
	}
}

void CArenaRankList::RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank)
{
	if (rank > m_rankinfos.size())
	{
		return;
	}
	if (NULL == m_rankinfos[rank - 1])
	{
		return;
	}
	m_rankinfos[rank - 1]->RankInfoToKKSG(rankdata);
}