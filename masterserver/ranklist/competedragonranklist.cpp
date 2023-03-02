#include "pch.h"
#include "competedragonranklist.h"
#include "role/rolesummarymgr.h"
#include "table/globalconfig.h"
#include "event/eventmgr.h"
#include "mail/maildata.h"
#include "mail/mailconfig.h"
#include "mail/mailmgr.h"
#include "competedragon/competedragonconfig.h"

#define COMPETEDRAGONRANKCOUNT 5000 

CompeteDragonRankInfo::CompeteDragonRankInfo(CDRKey& roles)
{
	Reset();
	m_roles = roles;
}

CompeteDragonRankInfo::~CompeteDragonRankInfo()
{

}

void CompeteDragonRankInfo::Reset()
{
	m_leastCostT = 0;
	m_lastPassT = 0;
	m_roles.clear();
}

void CompeteDragonRankInfo::RankInfoToKKSG(KKSG::RankData* rankdata) const
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

void CompeteDragonRankInfo::RankInfoToDBKKSG(KKSG::RankData* rankdata) const
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

void CompeteDragonRankInfo::DBKKSGToRankInfo(const KKSG::RankData* rankdata)
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

void CompeteDragonRankInfo::SetData(UINT32 leastCostT, UINT32 lastPassT)
{
	m_leastCostT = leastCostT;
	m_lastPassT = lastPassT;
}

void CompeteDragonRankInfo::InfoPrint()
{
	SSInfo << __FUNCTION__ << " cost = " << m_leastCostT << " pass = " << m_lastPassT << END;
	for(size_t i = 0; i < m_roles.size(); ++i)
	{
		SSInfo << __FUNCTION__ << " role = " << m_roles[i] << END;
	}
}

bool CompeteDragonRankInfo::HaveRole(UINT64 roleID)
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

void CompeteDragonDB::PackDBData(KKSG::RankList* ranklist)
{
	if(NULL == ranklist)
	{
		return ;
	}
	CompeteDragonRankList* cdlist = static_cast<CompeteDragonRankList*>(m_ranklist);
	if(NULL == cdlist)
	{
		return ;
	}
	std::vector<CompeteDragonRankInfo*> rankinfo;
	cdlist->GetAllInfo(rankinfo);	
	for(size_t i = 0; i < rankinfo.size(); ++i)
	{
		if(rankinfo[i])
		{
			rankinfo[i]->RankInfoToDBKKSG(ranklist->add_rankdata());
		}
	}
}

void CompeteDragonDB::UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank)
{
	CompeteDragonRankList* cdlist = static_cast<CompeteDragonRankList*>(m_ranklist);
	if(NULL == cdlist)
	{
		return ;
	}
	CDRKey roles;
	for(int i = 0; i < rankdata.roleids_size(); ++i)
	{
		roles.push_back(rankdata.roleids(i));
	}
	cdlist->OnChange(roles, rankdata.usetime(), rankdata.time());
}

bool CompeteDragonDB::BeforeSaveToDB()
{
	CompeteDragonRankList* cdlist = static_cast<CompeteDragonRankList*>(m_ranklist);
	if(NULL == cdlist)
	{
		return false;
	}
	if(cdlist->IsChanged())
	{
		cdlist->SetChanged(false);
		return true;
	}
	return false;
}

CompeteDragonRankList::CompeteDragonRankList(int type)
:CRankList(type)
{
	RegisterDb(new CompeteDragonDB);

	SaveToDBEveryPeriod* policy = new SaveToDBEveryPeriod;
	policy->SetSaveInterval(GetUpdateInterval());
	m_db->SetPolicy(policy);

	SetChanged(false);
}

CompeteDragonRankList::~CompeteDragonRankList()
{
	Clear();
}

void CompeteDragonRankList::Clear()
{
	std::vector<CompeteDragonRankInfo*> rankinfo;
	m_rankTree.GetAllKey(rankinfo);
	for(size_t i = 0; i < rankinfo.size(); ++i)
	{
		delete rankinfo[i];
	}
	m_rankTree.Clear();
	SetChanged(true);
}

UINT32 CompeteDragonRankList::GetNeedRankCount()
{
	return COMPETEDRAGONRANKCOUNT;
}

UINT32 CompeteDragonRankList::GetRank(UINT64 roleid)
{
	//待优化
	std::vector<CompeteDragonRankInfo*> allinfo;
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

UINT32 CompeteDragonRankList::GetSize()
{
	return m_rankTree.GetSize();
}

void CompeteDragonRankList::RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 professsion /*= 0*/)
{
	std::vector<CompeteDragonRankInfo*> rankinfo;
	m_rankTree.GetSomeTopKey(count, rankinfo);
	for(size_t i = 0; i < rankinfo.size(); ++i)
	{
		if(rankinfo[i])
		{
			rankinfo[i]->RankInfoToKKSG(rankdatas->add_rankdata());
		}
	}
}

void CompeteDragonRankList::RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank)
{
	CompeteDragonRankInfo* info = NULL;
	m_rankTree.GetKeyByRank(rank, info);
	if(info)
	{
		info->RankInfoToKKSG(rankdata);
	}
}

void CompeteDragonRankList::OnChange(CDRKey& roles, UINT32 costtime, UINT32 passtime)
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

	CompeteDragonRankInfo* rankinfo = NULL;
	UINT32 ret = m_rankTree.GetKeyByID(roles, rankinfo);
	if(ret == INVALID_RANK)//新的
	{
		rankinfo = new CompeteDragonRankInfo(roles);
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

void CompeteDragonRankList::DeleteLast()
{
	if(GetSize() == 0)
	{
		return ;
	}
	CompeteDragonRankInfo* info = NULL;
	UINT32 ret = m_rankTree.GetKeyByRank(GetSize(), info);
	if(INVALID_RANK != ret && info)
	{
		m_rankTree.Erase(info->GetKey());
		delete info;
		info = NULL;
	}
}

void CompeteDragonRankList::OnTwoWeekEvent()
{
	std::set<UINT64> rewardRoles;
	UINT32 nRank = 0;
	std::vector<CompeteDragonRankInfo*> allInfos;
	GetAllInfo(allInfos);
	for (auto i = allInfos.begin(); i != allInfos.end(); ++i)
	{
		CompeteDragonRankInfo* rankinfo = (*i);
		if (NULL == rankinfo)
			continue;
		if (rankinfo->GetKey().empty())
			continue;
		++nRank;
		auto data = CompeteDragonConfig::Instance()->GetCompeteDragonRankData(nRank);
		std::vector<ItemDesc> items;
		if (data)
		{
			for (auto i = data->reward.begin(); i != data->reward.end(); ++i)
			{
				ItemDesc temp(i->seq[0], i->seq[1]);
				items.push_back(temp);
			}
		}
		for (auto iter = rankinfo->GetKey().begin(); iter != rankinfo->GetKey().end(); ++iter)
		{
			if (rewardRoles.find(*iter) != rewardRoles.end())
			{
				continue;
			}
			EventMgr::Instance()->AddEvent(*iter, DESIGNATION_COM_TYPE_COMPETEDRAGONRANK,data->desigation);
			if (!items.empty())
			{
				CMailMgr::Instance()->SendMail(*iter, MailConf_CompeteDragon, items);
			}
			rewardRoles.insert(*iter);
		}
	}

	Clear();
}

void CompeteDragonRankList::GetAllInfo(std::vector<CompeteDragonRankInfo*>& vec)
{
	m_rankTree.GetAllKey(vec);
}

void CompeteDragonRankList::InfoPrint()
{
	std::vector<CompeteDragonRankInfo*> rankinfo;
	GetAllInfo(rankinfo);
	SSInfo << " size = " << rankinfo.size() << END;
	for(size_t i = 0; i < rankinfo.size(); ++i)
	{
		rankinfo[i]->InfoPrint();
	}
}

int CompeteDragonRankList::GetUpdateInterval()
{
	return CompeteDragonUpdateInterval;

}

