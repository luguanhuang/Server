#include "pch.h"
#include "towerranklist.h"
#include "pb/project.pb.h"
#include "role/rolesummarymgr.h"
#include "util/timespecificmgr.h"
#include "util/XCommon.h"
#include "util/gametime.h"
#include "account/punishmgr.h"

TowerRankInfo::TowerRankInfo(std::vector<UINT64>& roleids)
{
	m_towerHardLevel = 0;
	m_towerFloor = 0;
	m_bestTime = 0;
	m_time = 0;
	for (size_t i = 0; i < roleids.size(); ++i)
	{
		m_roleIDs.push_back(roleids[i]);
	}
}

TowerRankInfo::~TowerRankInfo()
{

}

void TowerRankInfo::Reset()
{

}

void TowerRankInfo::RankInfoToKKSG(KKSG::RankData* pRankData) const
{
	if (NULL == pRankData || m_roleIDs.empty())
	{
		return;
	}
	for (size_t i = 0; i < m_roleIDs.size(); ++i)
	{
		CRoleSummary* pRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(m_roleIDs[i]);
		if (NULL == pRoleSummary)
		{
			continue;
		}
		pRankData->add_roleids(m_roleIDs[i]);
		pRankData->add_rolenames(pRoleSummary->GetName());
		pRankData->add_titleids(pRoleSummary->GetTitleID());
		pRankData->set_starttype(pRoleSummary->GetStartUpType());
	}
	if (pRankData->roleids_size())
	{
		pRankData->set_towerhardlevel(m_towerHardLevel);
		pRankData->set_towerfloor(m_towerFloor);
		pRankData->set_towerthroughtime(m_bestTime);
	}
}

void TowerRankInfo::RankInfoToDBKKSG(KKSG::RankData* pRankData) const
{
	if (NULL == pRankData)
	{
		return;
	}
	for (size_t i = 0; i < m_roleIDs.size(); ++i)
	{
		CRoleSummary* pRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(m_roleIDs[i]);
		if (NULL == pRoleSummary)
		{
			continue;
		}
		pRankData->add_roleids(m_roleIDs[i]);
	}
	if (!pRankData->roleids_size())
	{
		return;
	}

	pRankData->set_towerhardlevel(m_towerHardLevel);
	pRankData->set_towerfloor(m_towerFloor);
	pRankData->set_towerthroughtime(m_bestTime);
	pRankData->set_time(m_time);
}

void TowerRankInfo::DBKKSGToRankInfo(const KKSG::RankData* pRankData)
{
	if (NULL == pRankData)
	{
		return;
	}
	if (!pRankData->roleids_size())
	{
		return;
	}	
	/*for (int i = 0; i < pRankData->roleids_size(); ++i)
	{
		m_roleIDs.push_back(pRankData->roleids(i));
	}*/

	m_time = pRankData->time();
	m_towerHardLevel = pRankData->towerhardlevel();
	m_towerFloor = pRankData->towerfloor();
	m_bestTime = pRankData->towerthroughtime();
}
//////////////////////////////////////////////////////////////////////////
void TowerRankDb::PackDBData(KKSG::RankList* ranklist)
{
	/*std::vector<TowerRankInfo>* rankinfos = &((TowerRankList*)m_ranklist)->GetRankInfo();
	for (auto i = rankinfos->begin(); i != rankinfos->end(); ++i)
	{
		CRankInfo* rankinfo = &(*i);
		if (NULL == rankinfo)
		{
			continue;
		}
		KKSG::RankData* rankdata = ranklist->add_rankdata();
		rankinfo->RankInfoToDBKKSG(rankdata);
	}*/

	std::set<TowerRankInfo*, TowerCompare>& currankinfos = (static_cast<TowerRankList*>(m_ranklist))->GetCurRankInfo();
	for (auto i = currankinfos.begin(); i != currankinfos.end(); ++i)
	{
		TowerRankInfo* rankinfo = (*i);
		if (NULL == rankinfo)
		{
			continue;
		}
		KKSG::RankData* rankdata = ranklist->add_rankdata();
		rankinfo->RankInfoToDBKKSG(rankdata);
	}
}

void TowerRankDb::UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank)
{
	TowerRankList* ranklist = static_cast<TowerRankList*>(m_ranklist);

	std::vector<UINT64> roleids;
	for (int i = 0; i < rankdata.roleids_size(); ++i)
	{
		CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(rankdata.roleids(i));
		if (!summary)
		{
			continue;
		}
		roleids.push_back(rankdata.roleids(i));
	}
	if (roleids.empty())
	{
		return;
	}
	TowerRankInfo rankinfo(roleids);
	rankinfo.DBKKSGToRankInfo(&rankdata);
	ranklist->OnChange(rankinfo.m_roleIDs, rankinfo.m_towerHardLevel, rankinfo.m_towerFloor, rankinfo.m_bestTime, rankinfo.m_time);
}

bool TowerRankDb::BeforeSaveToDB()
{
	TowerRankList* ranklist = static_cast<TowerRankList*>(m_ranklist);
	if(ranklist->IsChanged())
	{
		ranklist->SetIsChanged(false);
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
TowerRankList::TowerRankList(UINT32 type)
:CRankList(type)
{	
	RegisterDb(new TowerRankDb);

	SaveToDBEveryPeriod* policy = new SaveToDBEveryPeriod;	
	policy->SetSaveInterval(TowerUpdateInterVal);
	m_db->SetPolicy(policy);

	m_ischanged = false;
}

TowerRankList::~TowerRankList()
{
	Clear();
}

void TowerRankList::Clear()
{
	for (auto i = m_rankinfos.begin(); i != m_rankinfos.end(); ++i)
	{
		delete *i;
	}
	m_rankinfos.clear();
}

UINT32 TowerRankList::GetNeedRankCount()
{
	return TOWERRANKCOUNT;
}

UINT32 TowerRankList::GetRank(UINT64 roleid)
{
	UINT32 rank = 0;
	for (auto i = m_rankinfos.begin(); i != m_rankinfos.end(); ++i)
	{
		++rank;
		for (size_t j = 0; j < (*i)->GetRoleIDs().size(); ++j)
		{
			if ((*i)->GetRoleIDs()[j] == roleid)
			{
				return rank;
			}
		}
	}
	return INVALID_RANK;
}

UINT32 TowerRankList::GetSize()
{
	return m_rankinfos.size();
}

void TowerRankList::RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession)
{
	UINT32 tempcount = 0;
	for (auto i = m_rankinfos.begin(); i != m_rankinfos.end(); ++i)
	{
		if (++tempcount > count)
		{
			break;
		}
		TowerRankInfo* rankinfo = *i;
		KKSG::RankData* rankdata = rankdatas->add_rankdata();
		rankinfo->RankInfoToKKSG(rankdata);
	}
}

void TowerRankList::RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank)
{
	UINT32 count = 0;
	for (auto i = m_rankinfos.begin(); i != m_rankinfos.end(); ++i)
	{
		if (++count == rank)
		{
			TowerRankInfo* rankinfo = *i;
			rankinfo->RankInfoToKKSG(rankdata);
		}
	}
}

void TowerRankList::OnChange(std::vector<UINT64>& roleids, UINT32 towerHardLevel, UINT32 towerFloor, UINT32 useTime, UINT32 time)
{
	for(auto i = roleids.begin(); i != roleids.end(); i++)
	{
		if(CPunishMgr::Instance()->IsPunish(*i, KKSG::PUNISH_USER_TOWER))
		{
			return;
		}
	}	
	for (auto i = m_rankinfos.begin(); i != m_rankinfos.end(); ++i)
	{
		bool found = true;
		if ((*i)->GetRoleIDs().size() != roleids.size())
		{
			continue;
		}
		for (size_t j = 0; j < roleids.size(); ++j)
		{
			bool onefound = false;
			for (size_t k = 0; k < (*i)->GetRoleIDs().size(); ++k)
			{
				if ((*i)->GetRoleIDs()[k] == roleids[j])
				{
					onefound = true;
					break;
				}
			}
			if (!onefound)
			{
				found = false;
				break;
			}
		}
		if (found)
		{
			delete *i;
			m_rankinfos.erase(i);
			break;
		}
	}
	TowerRankInfo* rankinfo = new TowerRankInfo(roleids);
	rankinfo->m_towerHardLevel = towerHardLevel;
	rankinfo->m_towerFloor = towerFloor;
	rankinfo->m_bestTime = useTime;
	rankinfo->m_time = time;
	
	m_rankinfos.insert(rankinfo);

	if (m_rankinfos.size() > TOWERRANKCOUNT)
	{
		auto i = m_rankinfos.end();
		--i;
		delete *i;
		m_rankinfos.erase(i);
	}

	m_ischanged = true;
}

void TowerRankList::DeleteRole(UINT64 roleid)
{
	auto iter = m_rankinfos.begin();
	while(iter != m_rankinfos.end())
	{
		bool isfind = false;
		for(auto i = (*iter)->GetRoleIDs().begin(); i != (*iter)->GetRoleIDs().end(); i++)
		{
			if(roleid == *i)
			{
				isfind = true;
				break;
			}
		}
		if(isfind)
		{
			m_rankinfos.erase(iter++);
			continue;
		}
		iter++;
	}
}