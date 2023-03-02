#include "pch.h"
#include "tshowvoteranklist.h"
#include "pb/project.pb.h"
#include "util/timespecificmgr.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "util/XCommon.h"
#include "table/FashionRankTable.h"
#include "foreach.h"
//#include "gamelogic/tshowrecord.h"
//#include "tshow/ptcg2c_tshowvoterankntf.h"

TShowRankInfo::TShowRankInfo(UINT64 roleid)
:CRankInfo(roleid)
{
	m_tshowvotecount = 0;
	m_time = 0;
	m_timestamp = 0;
}

TShowRankInfo::~TShowRankInfo()
{

}

void TShowRankInfo::Reset()
{

}

void TShowRankInfo::RankInfoToKKSG(KKSG::RankData* pRankData) const
{
	if (NULL == pRankData || 0 == GetUniqueId())
	{
		return;
	}
	pRankData->set_roleid(GetUniqueId());
	pRankData->set_tshowvotecount(m_tshowvotecount);
	CRoleSummary* pRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(GetUniqueId());
	if (NULL == pRoleSummary)
	{
		return;
	}
	pRankData->set_rolename(pRoleSummary->GetName());
}

void TShowRankInfo::RankInfoToDBKKSG(KKSG::RankData* pRankData) const
{
	if (NULL == pRankData)
	{
		return;
	}
	pRankData->set_roleid(GetUniqueId());
	pRankData->set_tshowvotecount(m_tshowvotecount);
	pRankData->set_intervaltimestamp(m_timestamp);
	pRankData->set_time(m_time);
}

void TShowRankInfo::DBKKSGToRankInfo(const KKSG::RankData* pRankData)
{
	if (NULL == pRankData)
	{
		return;
	}
	SetUniqueId(pRankData->roleid());
	m_tshowvotecount = pRankData->tshowvotecount();
	m_time = pRankData->time();
	m_timestamp = pRankData->intervaltimestamp();
}
//////////////////////////////////////////////////////////////////////////
void TShowRankDb::PackDBData(KKSG::RankList* ranklist)
{
	std::vector<TShowRankInfo>* rankinfos = &(static_cast<TShowRankList*>(m_ranklist))->GetRankInfo();
	for (auto i = rankinfos->begin(); i != rankinfos->end(); ++i)
	{
		CRankInfo* rankinfo = &(*i);
		if (NULL == rankinfo)
		{
			continue;
		}
		KKSG::RankData* rankdata = ranklist->add_rankdata();
		rankinfo->RankInfoToDBKKSG(rankdata);
	}

	std::set<TShowRankInfo*, TShowCompare>& currankinfos = (static_cast<TShowRankList*>(m_ranklist))->GetCurRankInfo();
	for (auto i = currankinfos.begin(); i != currankinfos.end(); ++i)
	{
		CRankInfo* rankinfo = (*i);
		if (NULL == rankinfo)
		{
			continue;
		}
		KKSG::RankData* rankdata = ranklist->add_rankdata();
		rankinfo->RankInfoToDBKKSG(rankdata);
	}
}

void TShowRankDb::UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank)
{
	TShowRankList* ranklist = static_cast<TShowRankList*>(m_ranklist);
	std::vector<TShowRankInfo>* rankinfos = &ranklist->GetRankInfo();

	TShowRankInfo rankinfo(rankdata.roleid());
	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(rankdata.roleid());
	if (!summary)
	{
		return;
	}
	rankinfo.DBKKSGToRankInfo(&rankdata);
	if (rankinfo.m_timestamp)
	{
		bool bPush = true;
		foreach(i in (*rankinfos))
		{
			if (i->GetUniqueId() == rankinfo.GetUniqueId())
			{
				bPush = false;
				break;
			}
		}
		if(bPush)
			rankinfos->push_back(rankinfo);
	}
	else
	{
		ranklist->OnChange(rankinfo.GetUniqueId(), rankinfo.m_tshowvotecount, rankinfo.m_time);
	}
}

void TShowRankDb::OnReceiveFromDB()
{
	TShowRankList* ranklist = static_cast<TShowRankList*>(m_ranklist);
	std::set<TShowRankInfo*, TShowCompare>& rankinfos = ranklist->GetCurRankInfo();

	if (!rankinfos.empty())
	{
		if (!XCommon::IsWeekSame((*rankinfos.begin())->m_time, time(NULL)))
		{
			ranklist->Update();
		}
	}
}

bool TShowRankDb::BeforeSaveToDB()
{
	return true;
}


//////////////////////////////////////////////////////////////////////
TShowRankList::TShowRankList(UINT32 type)
:CRankList(type)
{
	TimeSpecific time(0, 0, 0);
	m_timehandle = CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&TShowRankList::OnTimer, this));

	RegisterDb(new TShowRankDb);
}

TShowRankList::~TShowRankList()
{
	Clear();
	CTimeSpecificMgr::Instance()->UnRegisterEvent(m_timehandle);
}

void TShowRankList::SaveToDb()
{
	m_db->SendToDB();
	SetTimeStamp();
}

void TShowRankList::Clear()
{
	for (auto i = m_rankinfos.begin(); i != m_rankinfos.end(); ++i)
	{
		delete *i;
	}
	m_rankinfos.clear();
}

UINT32 TShowRankList::GetNeedRankCount()
{
	return TSHOWRANKCOUNT;
}

UINT32 TShowRankList::GetRank(UINT64 roleid)
{
	UINT32 rank = 0;
	for (auto i = m_rankinfos.begin(); i != m_rankinfos.end(); ++i)
	{
		++rank;
		if ((*i)->GetUniqueId() == roleid)
		{
			return rank;
		}
	}
	return INVALID_RANK;
}

UINT32 TShowRankList::GetSize()
{
	return m_rankinfos.size();
}

void TShowRankList::RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession)
{
	UINT32 tempcount = 0;
	for (auto i = m_rankinfos.begin(); i != m_rankinfos.end(); ++i)
	{
		if (++tempcount > count)
		{
			break;
		}
		TShowRankInfo* rankinfo = *i;
		KKSG::RankData* rankdata = rankdatas->add_rankdata();
		rankinfo->RankInfoToKKSG(rankdata);
	}
}

void TShowRankList::RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank)
{
	UINT32 count = 0;
	for (auto i = m_rankinfos.begin(); i != m_rankinfos.end(); ++i)
	{
		if (++count == rank)
		{
			TShowRankInfo* rankinfo = *i;
			rankinfo->RankInfoToKKSG(rankdata);
		}
	}
}

void TShowRankList::OnChange(UINT64 roleid, UINT32 tshowvotecount, UINT32 time)
{
	for (auto i = m_rankinfos.begin(); i != m_rankinfos.end(); ++i)
	{
		if ((*i)->GetUniqueId() == roleid)
		{
			delete *i;
			m_rankinfos.erase(i);
			break;
		}
	}
	TShowRankInfo* rankinfo = new TShowRankInfo(roleid);
	rankinfo->m_tshowvotecount = tshowvotecount;
	rankinfo->m_time = time;
	
	m_rankinfos.insert(rankinfo);

	if (m_rankinfos.size() > TSHOWRANKCOUNT)
	{
		auto i = m_rankinfos.end();
		--i;
		delete *i;
		m_rankinfos.erase(i);
	}
}

bool TShowRankList::OnTimer()
{
	time_t nowtime = time(NULL);
	tm* time = localtime(&nowtime);
	if (1 == time->tm_wday)
	{
		Update();
	}
	else
	{
		SaveToDb();
	}
	return true;
}

void TShowRankList::Update()
{
	time_t nowtime = (int)time(0);
	int i = 0;
	m_lastrankinfos.clear();
	for (auto iter = m_rankinfos.begin(); iter != m_rankinfos.end() && i < TSHOWLASTRANKCOUNT; ++iter, ++i)
	{
		bool bPush = true;
		foreach(i in m_lastrankinfos)
		{
			if (i->GetUniqueId() == (*iter)->GetUniqueId())
			{
				bPush = false;
				break;
			}
		}
		if(!bPush)
			continue;
		m_lastrankinfos.push_back(**iter);
		m_lastrankinfos.back().m_timestamp = (UINT32)nowtime;
	}

	for (auto iter = m_rankinfos.begin(); iter != m_rankinfos.end(); ++iter)
	{
		GiveReward((*iter)->GetUniqueId());
	}

	Clear();
	SaveToDb();
}

UINT32 TShowRankList::GetLastRankInfo(int count, KKSG::TShowTopListRes& res)
{
	if (!m_db->IsFinishLoad())
	{
		return KKSG::ERR_FAILED;
	}
	/*time_t now = time(0);
	if (!m_lastrankinfos.empty())
	{
		if (!XCommon::IsWeekSame(m_lastrankinfos[0].m_time, now))
		{
			Update();
		}
	}*/
	auto iter = m_lastrankinfos.begin();
	auto end = m_lastrankinfos.end();
	for (int i = 0; i < count && iter != end; ++i, ++iter)
	{
		CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary((iter)->GetUniqueId());
		if (!summary)
		{
			continue;
		}
		KKSG::RoleBriefInfo* brief = res.add_rolesinfo();
		brief->set_type((KKSG::RoleType)summary->GetProfession());
		brief->set_roleid((iter)->GetUniqueId());
		brief->set_name(summary->GetName());
		brief->set_level(summary->GetLevel());
		const KKSG::RoleSummaryStored& roleData = summary->GetData();
		brief->mutable_fashion()->CopyFrom(roleData.fashion());
		//for (int i = 0; i < FashionCount; ++i)
		//{
		//	brief->add_fashion(summary->oFashion.GetFashionPos(i));
		//}
	}

	return KKSG::ERR_SUCCESS;
}

void TShowRankList::GiveReward(UINT64 roleID)
{
	/*UINT32 rank = GetRank(roleID);
	if (rank == INVALID_RANK)
	{
		return;
	}

	std::vector<ItemDesc> vecItem;
	vecItem.clear();
	TShowConfig::Instance()->GetFashionRankReward(rank, vecItem);

	std::vector<std::string> vecParam1;
	vecParam1.push_back(ToString(rank));
	CRewardMgr::Instance()->PushOnceReward(roleID, KKSG::RewardFashionPowerRank, vecParam1, vecItem);
	*/
}

UINT32 TShowRankList::GetLastRank(UINT64 roleID)
{
	for (UINT32 i = 0; i < m_lastrankinfos.size(); ++i)
	{
		if (m_lastrankinfos[i].GetUniqueId() == roleID)
		{
			return ++i;
		}
	}

	return 0;
}

void TShowRankList::SendTShowVoteRank(CRoleSummary* role)
{
}

