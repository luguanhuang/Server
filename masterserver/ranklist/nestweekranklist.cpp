#include "pch.h"
#include "nestweekranklist.h"
#include "role/rolesummarymgr.h"
#include "table/globalconfig.h"

#define NESTWEEKRANKCOUNT 5000 

NestWeekRankInfo::NestWeekRankInfo(NWRKey& roles)
{
	Reset();
	m_roles = roles;
}

NestWeekRankInfo::~NestWeekRankInfo()
{

}

void NestWeekRankInfo::Reset()
{
	m_leastCostT = 0;
	m_lastPassT = 0;
	m_roles.clear();
}

void NestWeekRankInfo::RankInfoToKKSG(KKSG::RankData* rankdata) const
{
	if(rankdata)
	{
		for(size_t i = 0; i < m_roles.size(); ++i)
		{
			CRoleSummary* pRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(m_roles[i]);
			if(pRoleSummary)
			{
				rankdata->add_roleids(m_roles[i]);
				rankdata->add_rolenames(pRoleSummary->GetName());
				rankdata->add_titleids(pRoleSummary->GetTitleID());
			}
		}
		rankdata->set_usetime(m_leastCostT);
	}
}

void NestWeekRankInfo::RankInfoToDBKKSG(KKSG::RankData* rankdata) const
{
	if(rankdata)
	{
		for(size_t i = 0; i < m_roles.size(); ++i)
		{
			rankdata->add_roleids(m_roles[i]);
		}
		rankdata->set_usetime(m_leastCostT);
		rankdata->set_time(m_lastPassT);
	}
}

void NestWeekRankInfo::DBKKSGToRankInfo(const KKSG::RankData* rankdata)
{
	Reset();	
	if(rankdata)
	{
		for(int i = 0; i < rankdata->roleids_size(); ++i)
		{
			m_roles.push_back(rankdata->roleids(i));
		}
		m_leastCostT = rankdata->usetime();
		m_lastPassT = rankdata->time();
	}
}

void NestWeekRankInfo::SetData(UINT32 leastCostT, UINT32 lastPassT)
{
	m_leastCostT = leastCostT;
	m_lastPassT = lastPassT;
}

void NestWeekRankInfo::InfoPrint()
{
	SSInfo << __FUNCTION__ << " cost = " << m_leastCostT << " pass = " << m_lastPassT << END;
	for(size_t i = 0; i < m_roles.size(); ++i)
	{
		SSInfo << __FUNCTION__ << " role = " << m_roles[i] << END;
	}
}

bool NestWeekRankInfo::HaveRole(UINT64 roleID)
{
	for(size_t i = 0; i < m_roles.size(); ++i)
	{
		if(m_roles[i] == roleID)
		{
			return true;
		}
	}
	return false;
}

void NestWeekDB::PackDBData(KKSG::RankList* ranklist)
{
	if(NULL == ranklist)
	{
		return ;
	}
	NestWeekRankList* nwlist = static_cast<NestWeekRankList*>(m_ranklist);
	if(NULL == nwlist)
	{
		return ;
	}
	std::vector<NestWeekRankInfo*> rankinfo;
	nwlist->GetAllInfo(rankinfo);	
	for(size_t i = 0; i < rankinfo.size(); ++i)
	{
		if(rankinfo[i])
		{
			rankinfo[i]->RankInfoToDBKKSG(ranklist->add_rankdata());
		}
	}
}

void NestWeekDB::UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank)
{
	NestWeekRankList* nwlist = static_cast<NestWeekRankList*>(m_ranklist);
	if(NULL == nwlist)
	{
		return ;
	}
	NWRKey roles;
	for(int i = 0; i < rankdata.roleids_size(); ++i)
	{
		roles.push_back(rankdata.roleids(i));
	}
	nwlist->OnChange(roles, rankdata.usetime(), rankdata.time());
}

bool NestWeekDB::BeforeSaveToDB()
{
	NestWeekRankList* nwlist = static_cast<NestWeekRankList*>(m_ranklist);
	if(NULL == nwlist)
	{
		return false;
	}
	if(nwlist->IsChanged())
	{
		nwlist->SetChanged(false);
		return true;
	}
	return false;
}

NestWeekRankList::NestWeekRankList(int type)
:CRankList(type)
{
	RegisterDb(new NestWeekDB);

	SaveToDBEveryPeriod* policy = new SaveToDBEveryPeriod;
	policy->SetSaveInterval(GetUpdateInterval());
	m_db->SetPolicy(policy);

	SetChanged(false);
}

NestWeekRankList::~NestWeekRankList()
{
	Clear();
}

void NestWeekRankList::Clear()
{
	std::vector<NestWeekRankInfo*> rankinfo;
	m_rankTree.GetAllKey(rankinfo);
	for(size_t i = 0; i < rankinfo.size(); ++i)
	{
		delete rankinfo[i];
	}
	m_rankTree.Clear();
	SetChanged(true);
}

UINT32 NestWeekRankList::GetNeedRankCount()
{
	return NESTWEEKRANKCOUNT;
}

UINT32 NestWeekRankList::GetRank(UINT64 roleid)
{
	//待优化
	std::vector<NestWeekRankInfo*> allinfo;
	GetAllInfo(allinfo);
	for(size_t i = 0; i < allinfo.size(); ++i)
	{
		if(allinfo[i] && allinfo[i]->HaveRole(roleid))
		{
			return (i+1);
		}
	}
	return INVALID_RANK;
}

UINT32 NestWeekRankList::GetSize()
{
	return m_rankTree.GetSize();
}

void NestWeekRankList::RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 professsion /*= 0*/)
{
	std::vector<NestWeekRankInfo*> rankinfo;
	m_rankTree.GetSomeTopKey(count, rankinfo);
	for(size_t i = 0; i < rankinfo.size(); ++i)
	{
		if(rankinfo[i])
		{
			rankinfo[i]->RankInfoToKKSG(rankdatas->add_rankdata());
		}
	}
}

void NestWeekRankList::RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank)
{
	NestWeekRankInfo* info = NULL;
	m_rankTree.GetKeyByRank(rank, info);
	if(info)
	{
		info->RankInfoToKKSG(rankdata);
	}
}

void NestWeekRankList::OnChange(NWRKey& roles, UINT32 costtime, UINT32 passtime)
{
	if(roles.empty())
	{
		return ;
	}
	if(0 == costtime)
	{
		return;
	}
	std::sort(roles.begin(), roles.end());//排序

	NestWeekRankInfo* rankinfo = NULL;
	UINT32 ret = m_rankTree.GetKeyByID(roles, rankinfo);
	if(ret == INVALID_RANK)//新的
	{
		rankinfo = new NestWeekRankInfo(roles);
	}
	else if(rankinfo)//已有
	{
		if(rankinfo->LeastCostT() <= costtime)	
		{
			return ;//无需更新
		}
		m_rankTree.Erase(roles);
	}

	if(rankinfo)
	{
		rankinfo->SetData(costtime, passtime);
		m_rankTree.Insert(rankinfo, rankinfo->GetKey());
	}

	if(GetSize() > GetNeedRankCount())
	{
		DeleteLast();		
	}

	SetChanged(true);	
}

void NestWeekRankList::DeleteLast()
{
	if(GetSize() == 0)
	{
		return ;
	}
	NestWeekRankInfo* info = NULL;
	UINT32 ret = m_rankTree.GetKeyByRank(GetSize(), info);
	if(INVALID_RANK != ret && info)
	{
		m_rankTree.Erase(info->GetKey());
		delete info;
		info = NULL;
	}
}

void NestWeekRankList::GetAllInfo(std::vector<NestWeekRankInfo*>& vec)
{
	m_rankTree.GetAllKey(vec);
}

void NestWeekRankList::InfoPrint()
{
	std::vector<NestWeekRankInfo*> rankinfo;
	GetAllInfo(rankinfo);
	SSInfo << " size = " << rankinfo.size() << END;
	for(size_t i = 0; i < rankinfo.size(); ++i)
	{
		rankinfo[i]->InfoPrint();
	}
}

int NestWeekRankList::GetUpdateInterval()
{
	return NestWeekUpdateInterval;

}

void NestWeekRankList::ASGetTopInfo(UINT32 count, std::vector<NestWeekRankInfo*>& infoVec)
{
	if(count > GetSize())
	{
		count = GetSize();
	}
	m_rankTree.GetSomeTopKey(count, infoVec);
}

void NestWeekRankList::ASGetTopRank(UINT32 count, std::vector<ASPair>& rankVec)
{
	if(count > GetSize())
	{
		count = GetSize();
	}
	std::unordered_set<UINT64> tmpset;
	std::vector<NestWeekRankInfo*> vec;
	m_rankTree.GetSomeTopKey(count, vec);
	for(size_t i = 0; i < vec.size(); ++i)
	{
		UINT32 rank = (i + 1);
		const auto& roles = vec[i]->GetKey();
		for(size_t j = 0; j < roles.size(); ++j)
		{
			UINT64 roleID = roles[j];
			if(tmpset.find(roleID) != tmpset.end())
			{
				continue;
			}
			tmpset.insert(roleID);
			ASPair tmp(rank, roleID);
			rankVec.push_back(tmp);
		}
	}
}

LastWeekNestWeekRankList::LastWeekNestWeekRankList(int type)
	:NestWeekRankList(type)
{

}

LastWeekNestWeekRankList::~LastWeekNestWeekRankList()
{

}

int LastWeekNestWeekRankList::GetUpdateInterval()
{
	return LastWeek_NestWeekUpdateInterval;
}

void LastWeekNestWeekRankList::AddRankInfo(NestWeekRankInfo* info)
{
	if(NULL == info)
	{
		return;
	}
	if(INVALID_RANK != m_rankTree.GetRankByID(info->GetKey()))
	{
		SSWarn << " add team repeat " << END;
		info->InfoPrint();
		return ;
	}

	NestWeekRankInfo* pNewInfo = new NestWeekRankInfo(*info);
	m_rankTree.Insert(pNewInfo, pNewInfo->GetKey());

	SetChanged(true);
}

UINT32 LastWeekNestWeekRankList::GetNeedRankCount()
{
	return GetGlobalConfig().ArenaStarRankLimit;
}

void LastWeekNestWeekRankList::ASUpdateLastWeekRankInfo(ArenaStarRankInterface* pCurRank)
{
	if(NULL == pCurRank)
	{
		return;
	}
	Clear();
	std::vector<NestWeekRankInfo*> vec;
	pCurRank->ASGetTopInfo(GetNeedRankCount(), vec);
	for(size_t i = 0; i < vec.size(); ++i)
	{
		NestWeekRankInfo* pNewInfo = new NestWeekRankInfo(*vec[i]);
		m_rankTree.Insert(pNewInfo, pNewInfo->GetKey());
	}
    SetChanged(true);
}

UINT32 LastWeekNestWeekRankList::ASGetTopViewRank()
{
	return 1;
}
