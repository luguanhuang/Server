#include "pch.h"
#include "military_ranklist.h"
#include "role/rolesummarymgr.h"
#include "pb/project.pb.h"
#include "ranklistmgr.h"
#include "table/ProfessionMgr.h"
#include "account/punishmgr.h"
#include "table/globalconfig.h"
#include "militaryrank/ptcm2g_militaryrankchange.h"
#include "role/rolemanager.h"

#define MILITARYRANKCOUNT 5000

MilitaryRankInfo::MilitaryRankInfo(UINT64 roleid)
	:CRankInfo(roleid)
{

}

MilitaryRankInfo::~MilitaryRankInfo()
{

}

void MilitaryRankInfo::Reset()
{
	military_exploit_ = 0;
	military_rank_ = 0;
	total_num_ = 0;
}

void MilitaryRankInfo::RankInfoToKKSG(KKSG::RankData* rankdata) const
{
	CRoleSummary* role = CRoleSummaryMgr::Instance()->GetRoleSummary(GetUniqueId());
	if (NULL != role)
	{
		rankdata->set_roleid(GetUniqueId());
		rankdata->set_rolename(role->GetName());
		rankdata->set_time(time_);
		KKSG::MilitaryRankData* military = rankdata->mutable_military_info();
		military->set_military_exploit(military_exploit_);
		military->set_military_rank(military_rank_);
		//military->set_total_num(total_num_);
	}
}

void MilitaryRankInfo::RankInfoToDBKKSG(KKSG::RankData* rankdata) const
{
	rankdata->set_roleid(GetUniqueId());
	rankdata->set_time(time_);
	KKSG::MilitaryRankData* military = rankdata->mutable_military_info();
	military->set_military_exploit(military_exploit_);
	military->set_military_rank(military_rank_);
	military->set_total_num(total_num_);
}

void MilitaryRankInfo::DBKKSGToRankInfo(const KKSG::RankData* rankdata)
{
	SetUniqueId(rankdata->roleid());
	if (rankdata->has_military_info())
	{
		military_exploit_ = rankdata->military_info().military_exploit();
		military_rank_ = rankdata->military_info().military_rank();
		total_num_ = rankdata->military_info().total_num();
	}	
	time_ = rankdata->time();
}

//////////////////////////////////////////////////////////////////////
void MilitaryDb::PackDBData(KKSG::RankList* ranklist)
{
	MilitaryRankList* militaryRanklist = static_cast<MilitaryRankList*>(m_ranklist);
	std::vector<MilitaryRankInfo*> rankinfo;
	militaryRanklist->GetRankInfo(rankinfo);

	for (auto i = rankinfo.begin(); i != rankinfo.end(); ++i)
	{
		MilitaryRankInfo* rankinfo = *i;
		if (NULL == rankinfo)
		{
			continue;
		}
		KKSG::RankData* rankdata = ranklist->add_rankdata();
		rankinfo->RankInfoToDBKKSG(rankdata);
	}
}

void MilitaryDb::UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank)
{
	MilitaryRankList* ranklist = static_cast<MilitaryRankList*>(m_ranklist);
	MilitaryRankInfo* military_info = new MilitaryRankInfo(rankdata.roleid());
	military_info->DBKKSGToRankInfo(&rankdata);
	ranklist->InsertKey(military_info);
}

bool MilitaryDb::BeforeSaveToDB()
{
	MilitaryRankList* ranklist = static_cast<MilitaryRankList*>(m_ranklist);
	if(ranklist->IsChanged())
	{
		ranklist->SetIsChanged(false);
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
MilitaryRankList::MilitaryRankList(int type)
	:CRankList(type)
{	
	RegisterDb(new MilitaryDb);

	SaveToDBEveryPeriod* policy = new SaveToDBEveryPeriod;	
	policy->SetSaveInterval(MilitaryUpdateInterval);
	m_db->SetPolicy(policy);

	m_ischanged = false;
}

MilitaryRankList::~MilitaryRankList()
{
	Clear();
}

void MilitaryRankList::Clear()
{
	std::vector<MilitaryRankInfo*> military_rankinfo;
	m_rankTree.GetAllKey(military_rankinfo);
	for(auto i = military_rankinfo.begin(); i != military_rankinfo.end(); i++)
	{
		delete *i;
	}

	m_rankTree.Clear();
}

UINT32 MilitaryRankList::GetNeedRankCount()
{
	return MILITARYRANKCOUNT;
}

UINT32 MilitaryRankList::GetRank(UINT64 roleid)
{
	return m_rankTree.GetRankByID(roleid);
}

UINT32 MilitaryRankList::GetSize()
{
	return m_rankTree.GetSize();
}

const MilitaryRankInfo* MilitaryRankList::GetInfoByRank(UINT32 rank)
{
	MilitaryRankInfo* rankinfo;
	UINT32 ret = m_rankTree.GetKeyByRank(rank, rankinfo);
	if(ret != INVALID_RANK)
	{
		return rankinfo;
	}
	else 
	{
		return NULL;
	}
}

void MilitaryRankList::RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession)
{
	std::vector<MilitaryRankInfo*> vec;
	m_rankTree.GetSomeTopKey(count, vec);

	for (UINT32 i = 0; i < vec.size(); i++)
	{
		MilitaryRankInfo* rankinfo = vec[i];
		KKSG::RankData* rankdata = rankdatas->add_rankdata();
		rankinfo->RankInfoToKKSG(rankdata);
	}
}

void MilitaryRankList::RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank)
{
	const MilitaryRankInfo* rankinfo = GetInfoByRank(rank);
	if (NULL != rankinfo)
	{
		rankinfo->RankInfoToKKSG(rankdata);
	}
}

void MilitaryRankList::OnChange(
	UINT64 roleid,
	UINT32 military_exploit, 
	UINT32 military_rank,
	UINT32 total_num,
	UINT32 time)
{
	MilitaryRankInfo* rankinfo;
	UINT32 ret = m_rankTree.GetKeyByID(roleid, rankinfo);
	if(ret == INVALID_RANK)
	{
		if(0 == military_exploit)
		{
			return;
		}
		MilitaryRankInfo* rank_info = new MilitaryRankInfo(roleid);
		rank_info->military_exploit_ = military_exploit;
		rank_info->military_rank_ = military_rank;
		rank_info->total_num_ = total_num;
		rank_info->time_ = time;
		InsertKey(rank_info);

		if(m_rankTree.GetSize() > GetNeedRankCount())
		{
			UINT32 ret = m_rankTree.GetKeyByRank(m_rankTree.GetSize(), rankinfo);
			if(ret != INVALID_RANK)
			{
				m_rankTree.Erase(rankinfo->GetUniqueId());
				delete rankinfo;
			}
		}
	}
	else
	{
		m_rankTree.Erase(roleid);
		if(0 == military_exploit)
		{
			delete rankinfo;
			return;
		}
		rankinfo->military_exploit_ = military_exploit;
		rankinfo->military_rank_ = military_rank;
		rankinfo->total_num_ = total_num;
		rankinfo->time_ = time;
		InsertKey(rankinfo);
	}

	// 调整军阶
	AdjustMilitaryRank(roleid,military_exploit,military_rank);

	m_ischanged = true;
}

void MilitaryRankList::InsertKey(MilitaryRankInfo* rankinfo)
{
	if(NULL == rankinfo)
	{
		LogWarn("Insert a NULL pointer in PkRealTimeRankList");
		return;
	}
	UINT64 roleid = rankinfo->GetUniqueId();
	m_rankTree.Insert(rankinfo, roleid);
}

void MilitaryRankList::GetRankInfo(std::vector<MilitaryRankInfo*>& vec)
{
	m_rankTree.GetAllKey(vec);
}

void MilitaryRankList::DeleteRole(UINT64 roleid)
{
	MilitaryRankInfo* rankinfo;
	UINT32 ret = m_rankTree.GetKeyByID(roleid, rankinfo);
	if(ret != INVALID_RANK)
	{
		m_rankTree.Erase(roleid);
		delete rankinfo;
	}
}


void MilitaryRankList::AdjustMilitaryRank(
	const UINT64 role_id,
	const UINT32 military_exploit, 
	const UINT32 military_rank)
{
	if (military_exploit >= GetGlobalConfig().TopRankMilitaryExploit)
	{
		bool need_update = false;
		UINT32 old_military_rank = military_rank;
		UINT32 new_rank = GetRank(role_id);
		UINT32 new_military_rank = GetMilitaryRankByRank(new_rank);
		// 如果进阶了
		while (new_military_rank > old_military_rank)
		{
			need_update = true;

			UINT32 edge_rank = GetMinRankByMilitary(old_military_rank+1) +1 ;
			MilitaryRankInfo* edge_rank_info = NULL;
			UINT32 ret = m_rankTree.GetKeyByRank(edge_rank, edge_rank_info);
			if(ret != INVALID_RANK)
			{
				if (old_military_rank+1 == edge_rank_info->military_rank_)
				{
					// 这个人需要降军阶	
					UpdateMilitaryRank(edge_rank_info->GetUniqueId(),old_military_rank);
				}
			}
			++old_military_rank;
		}

		// 更新自己军阶 
		if (need_update)
		{
			UpdateMilitaryRank(role_id,new_military_rank);
		}
	}
}

UINT32 MilitaryRankList::GetMilitaryRankByRank(UINT32 rank)
{
	auto it = GetGlobalConfig().MilitaryRank.begin();
	auto it_end = GetGlobalConfig().MilitaryRank.end();
	for (; it != it_end; ++it)
	{
		if ( rank >= it->seq[1]  && rank <= it->seq[2])
		{
			return it->seq[0];
		}
	}

	return 0;
}


UINT32 MilitaryRankList::GetMinRankByMilitary(UINT32 military_rank)
{
	auto it = GetGlobalConfig().MilitaryRank.begin();
	auto it_end = GetGlobalConfig().MilitaryRank.end();
	for (; it != it_end; ++it)
	{
		if ( military_rank == it->seq[0]  )
		{
			return it->seq[2];
		}
	}

	return 0;
}


// 通知军衔变化
void MilitaryRankList::UpdateMilitaryRank(UINT64 role_id,UINT32 military_rank)
{	
	MilitaryRankInfo* rankinfo;
	UINT32 ret = m_rankTree.GetKeyByID(role_id, rankinfo);
	if(ret != INVALID_RANK)
	{		
		UINT32 new_military_exploit = rankinfo->military_exploit_;	
		UINT32 new_total_num  =	rankinfo->total_num_ ;
		UINT32 new_time  = rankinfo->time_ ;
		m_rankTree.Erase(role_id);

		rankinfo->military_rank_ = military_rank;
		rankinfo->military_exploit_ = new_military_exploit;					
		rankinfo->total_num_ = new_total_num;
		rankinfo->time_ = new_time;
		InsertKey(rankinfo);
	}

	CRole* role = CRoleManager::Instance()->GetByRoleID(role_id);	
	if (NULL != role)
	{
		PtcM2G_MilitaryRankChange ptc;
		ptc.m_Data.set_role_id(role_id);
		ptc.m_Data.set_military_rank(military_rank);
		role->SendMsgToGS(ptc);
	}
}

UINT32  MilitaryRankList::GetMilitaryInfoByRoleId(UINT64 role_id)
{
	MilitaryRankInfo* rankinfo;
	UINT32 ret = m_rankTree.GetKeyByID(role_id, rankinfo);
	if(ret != INVALID_RANK)
	{
		return rankinfo->military_rank_;
	}
	return 1;
}