#include "pch.h"
#include "biohelllrankList.h"
#include "role/rolesummarymgr.h"
#include "table/globalconfig.h"
#include "event/eventmgr.h"

#define BIOHELLLRANKCOUNT 5000 

BioHelllRankInfo::BioHelllRankInfo(NBIOKey& roles)
{
	Reset();
	m_roles = roles;
}

BioHelllRankInfo::~BioHelllRankInfo()
{

}

void BioHelllRankInfo::Reset()
{
	m_leastCostT = 0;
	m_lastPassT = 0;
	m_roles.clear();
}

void BioHelllRankInfo::RankInfoToKKSG(KKSG::RankData* rankdata) const
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

void BioHelllRankInfo::RankInfoToDBKKSG(KKSG::RankData* rankdata) const
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

void BioHelllRankInfo::DBKKSGToRankInfo(const KKSG::RankData* rankdata)
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

void BioHelllRankInfo::SetData(UINT32 leastCostT, UINT32 lastPassT)
{
	m_leastCostT = leastCostT;
	m_lastPassT = lastPassT;
}

void BioHelllRankInfo::InfoPrint()
{
	SSInfo << __FUNCTION__ << " cost = " << m_leastCostT << " pass = " << m_lastPassT << END;
	for(size_t i = 0; i < m_roles.size(); ++i)
	{
		SSInfo << __FUNCTION__ << " role = " << m_roles[i] << END;
	}
}

bool BioHelllRankInfo::HaveRole(UINT64 roleID)
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

void BioHelllDB::PackDBData(KKSG::RankList* ranklist)
{
	if(NULL == ranklist)
	{
		return ;
	}
	BioHelllRankList* nwlist = static_cast<BioHelllRankList*>(m_ranklist);
	if(NULL == nwlist)
	{
		return ;
	}
	std::vector<BioHelllRankInfo*> rankinfo;
	nwlist->GetAllInfo(rankinfo);	
	for(size_t i = 0; i < rankinfo.size(); ++i)
	{
		if(rankinfo[i])
		{
			rankinfo[i]->RankInfoToDBKKSG(ranklist->add_rankdata());
		}
	}
}

void BioHelllDB::UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank)
{
	BioHelllRankList* nwlist = static_cast<BioHelllRankList*>(m_ranklist);
	if(NULL == nwlist)
	{
		return ;
	}
	NBIOKey roles;
	for(int i = 0; i < rankdata.roleids_size(); ++i)
	{
		roles.push_back(rankdata.roleids(i));
	}
	nwlist->OnChange(roles, rankdata.usetime(), rankdata.time());
}

bool BioHelllDB::BeforeSaveToDB()
{
	BioHelllRankList* nwlist = static_cast<BioHelllRankList*>(m_ranklist);
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

BioHelllRankList::BioHelllRankList(int type)
:CRankList(type)
{
	RegisterDb(new BioHelllDB);
	SaveToDBEveryPeriod* policy = new SaveToDBEveryPeriod;
	policy->SetSaveInterval(GetUpdateInterval());
	m_db->SetPolicy(policy);

	SetChanged(false);
}

BioHelllRankList::~BioHelllRankList()
{
	Clear();
}

void BioHelllRankList::Clear()
{
	std::vector<BioHelllRankInfo*> rankinfo;
	m_rankTree.GetAllKey(rankinfo);
	for(size_t i = 0; i < rankinfo.size(); ++i)
	{
		delete rankinfo[i];
	}
	m_rankTree.Clear();
	SetChanged(true);
}

UINT32 BioHelllRankList::GetNeedRankCount()
{
	return BIOHELLLRANKCOUNT;
}

UINT32 BioHelllRankList::GetRank(UINT64 roleid)
{
	//待优化
	std::vector<BioHelllRankInfo*> allinfo;
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

UINT32 BioHelllRankList::GetSize()
{
	return m_rankTree.GetSize();
}

void BioHelllRankList::RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 professsion /*= 0*/)
{
	std::vector<BioHelllRankInfo*> rankinfo;
	m_rankTree.GetSomeTopKey(count, rankinfo);
	for(size_t i = 0; i < rankinfo.size(); ++i)
	{
		if(rankinfo[i])
		{
			rankinfo[i]->RankInfoToKKSG(rankdatas->add_rankdata());
		}
	}
}

void BioHelllRankList::RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank)
{
	BioHelllRankInfo* info = NULL;
	m_rankTree.GetKeyByRank(rank, info);
	if(info)
	{
		info->RankInfoToKKSG(rankdata);
	}
}

void BioHelllRankList::OnChange(NBIOKey& roles, UINT32 costtime, UINT32 passtime)
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

	BioHelllRankInfo* rankinfo = NULL;
	UINT32 ret = m_rankTree.GetKeyByID(roles, rankinfo);
	if(ret == INVALID_RANK)//新的
	{
		rankinfo = new BioHelllRankInfo(roles);
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

void BioHelllRankList::DeleteLast()
{
	if(GetSize() == 0)
	{
		return ;
	}
	BioHelllRankInfo* info = NULL;
	UINT32 ret = m_rankTree.GetKeyByRank(GetSize(), info);
	if(INVALID_RANK != ret && info)
	{
		m_rankTree.Erase(info->GetKey());
		delete info;
		info = NULL;
	}
}

void BioHelllRankList::GetAllInfo(std::vector<BioHelllRankInfo*>& vec)
{
	m_rankTree.GetAllKey(vec);
}

void BioHelllRankList::InfoPrint()
{
	std::vector<BioHelllRankInfo*> rankinfo;
	GetAllInfo(rankinfo);
	SSInfo << " size = " << rankinfo.size() << END;
	for(size_t i = 0; i < rankinfo.size(); ++i)
	{
		rankinfo[i]->InfoPrint();
	}
}

void BioHelllRankList::GiveReward()
{
	std::vector<BioHelllRankInfo*> vec;
	GetAllInfo(vec);
	std::map<UINT64,UINT32> mapDesig;
	for(size_t i = 0; i < vec.size(); ++i)
	{
		if(vec[i])
		{
			for(auto iter = vec[i]->GetKey().begin(); iter!=vec[i]->GetKey().end(); iter++)
			{
				auto itFind = mapDesig.find(*iter);
				if (itFind==mapDesig.end())
				{
					LogInfo("BIOHELLL-----nRoleID=%llu nRank=%d",*iter,i+1);
					mapDesig.insert(std::pair<UINT64,UINT32>(*iter,i+1));
					EventMgr::Instance()->AddEvent(*iter, DESIGNATION_COM_TYPE_BIOHELLL,i+1);
				}
			}
		}
	}
	Clear();
}

int BioHelllRankList::GetUpdateInterval()
{
	return BioHelllpdateInterval;

}

void BioHelllRankList::ASGetTopInfo(UINT32 count, std::vector<BioHelllRankInfo*>& infoVec)
{
	if(count > GetSize())
	{
		count = GetSize();
	}
	m_rankTree.GetSomeTopKey(count, infoVec);
}
