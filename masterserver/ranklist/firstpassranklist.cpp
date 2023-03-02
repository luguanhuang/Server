#include "pch.h"
#include "firstpassranklist.h"
#include "pb/project.pb.h"
#include "role/rolesummarymgr.h"
#include "util/timespecificmgr.h"
#include "util/XCommon.h"
#include "ranklistmgr.h"


FirstPassInfo::FirstPassInfo()
{
	m_time = 0;
	m_usecTime = 0;
	m_commendNum = 0;
	m_starLevel = 0;
}

FirstPassInfo::~FirstPassInfo()
{

}

void FirstPassInfo::Reset()
{

}

void FirstPassInfo::RankInfoToKKSG(KKSG::RankData* pRankData) const
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
		pRankData->set_is_vip(pRoleSummary->IsQQVip());
		pRankData->set_is_svip(pRoleSummary->IsQQSVip());
		pRankData->set_starttype(pRoleSummary->GetStartUpType());
		pRankData->set_starlevel(m_starLevel);
	}
	if (pRankData->roleids_size())
	{
		pRankData->set_time(m_time);
	}
}

void FirstPassInfo::RankInfoToDBKKSG(KKSG::RankData* pRankData) const
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

	pRankData->set_time(m_time);
	pRankData->set_usectime(m_usecTime);
	pRankData->set_commendnum(m_commendNum);
	pRankData->set_starlevel(m_starLevel);
}

void FirstPassInfo::DBKKSGToRankInfo(const KKSG::RankData* pRankData)
{
	if (NULL == pRankData)
	{
		return;
	}
	if (!pRankData->roleids_size())
	{
		return;
	}
	for (int i = 0; i < pRankData->roleids_size(); ++i)
	{
		m_roleIDs.push_back(pRankData->roleids(i));
	}

	m_time = pRankData->time();
	m_usecTime = pRankData->usectime();
	m_commendNum = pRankData->commendnum();
	m_starLevel = pRankData->starlevel();
}
//////////////////////////////////////////////////////////////////////////
void FirstPassRankDb::PackDBData(KKSG::RankList* ranklist)
{
	/*std::vector<FirstPassInfo>* rankinfos = &((FirstPassRankList*)m_ranklist)->GetRankInfo();
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

	std::set<FirstPassInfo*, FirstPassCompare>& currankinfos = (static_cast<FirstPassRankList*>(m_ranklist))->GetCurRankInfo();
	for (auto i = currankinfos.begin(); i != currankinfos.end(); ++i)
	{
		FirstPassInfo* rankinfo = (*i);
		if (NULL == rankinfo)
		{
			continue;
		}
		KKSG::RankData* rankdata = ranklist->add_rankdata();
		rankinfo->RankInfoToDBKKSG(rankdata);
	}
}

void FirstPassRankDb::UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank)
{
	FirstPassRankList* ranklist = static_cast<FirstPassRankList*>(m_ranklist);

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
	FirstPassInfo rankinfo;
	rankinfo.DBKKSGToRankInfo(&rankdata);
	ranklist->OnChange(rankinfo.m_roleIDs, rankinfo.m_starLevel, rankinfo.m_time, rankinfo.m_usecTime);
}

bool FirstPassRankDb::BeforeSaveToDB()
{
	FirstPassRankList* ranklist = static_cast<FirstPassRankList*>(m_ranklist);
	if (ranklist->IsChanged())
	{
		ranklist->SetIsChanged(false);
		return true;
	}
	return false;
}


//////////////////////////////////////////////////////////////////////
FirstPassRankList::FirstPassRankList(UINT32 type, UINT32 serverid)
:CRankList(type, serverid)
{
	RegisterDb(new FirstPassRankDb);

	SaveToDBEveryPeriod* policy = new SaveToDBEveryPeriod;	
	policy->SetSaveInterval(XRandom::randInt(250, 300));
	m_db->SetPolicy(policy);

	m_ischanged = false;
}

FirstPassRankList::~FirstPassRankList()
{
	Clear();
}

void FirstPassRankList::AddCommend(int num)
{
	auto begin = m_rankinfos.begin();
	if (begin != m_rankinfos.end())
	{
		(*begin)->m_commendNum += num;
	}
}

int FirstPassRankList::GetCommend()
{
	auto begin = m_rankinfos.begin();
	if (begin != m_rankinfos.end())
	{
		return (*begin)->m_commendNum;
	}

	return 0;
}

FirstPassInfo* FirstPassRankList::GetRankInfo(int rank)
{
	auto iter = m_rankinfos.begin();
	if (iter != m_rankinfos.end())
	{
		return (*iter);
	}
	return NULL;
}

void FirstPassRankList::Clear()
{
	for (auto i = m_rankinfos.begin(); i != m_rankinfos.end(); ++i)
	{
		delete *i;
	}
	m_rankinfos.clear();
}

void FirstPassRankList::RemoveAll()
{
	Clear();
	SetIsChanged(true);
	GetDbHandler()->SendToDB();
}

UINT32 FirstPassRankList::GetNeedRankCount()
{
	return CLIENTFIRSTPASSRANKCOUNT;
}

UINT32 FirstPassRankList::GetRank(UINT64 roleid)
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

UINT32 FirstPassRankList::GetSize()
{
	return m_rankinfos.size();
}

UINT32 FirstPassRankList::GetRank(std::vector<UINT64>& roleids, int starLevel)
{
	int rank = 0;
	for (auto i = m_rankinfos.begin(); i != m_rankinfos.end(); ++i, ++rank)
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
			//m_rankinfos.erase(i);
			//break;
			if ((*i)->m_starLevel >= starLevel)
			{
				return (rank + 1);
			}
			return INVALID_RANK;
		}
	}

	return INVALID_RANK;
}

void FirstPassRankList::EraseRank(std::vector<UINT64>& roleids)
{
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
			return;
		}
	}
}

void FirstPassRankList::RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession)
{
	UINT32 tempcount = 0;
	KKSG::RankData tempRankData;
	for (auto i = m_rankinfos.begin(); i != m_rankinfos.end(); ++i)
	{
		if (++tempcount > count)
		{
			break;
		}
		FirstPassInfo* rankinfo = *i;
		tempRankData.Clear();
		rankinfo->RankInfoToKKSG(&tempRankData);
		if (tempRankData.roleids_size())
		{
			KKSG::RankData* rankdata = rankdatas->add_rankdata();
			rankdata->CopyFrom(tempRankData);
		}
	}
}

void FirstPassRankList::RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank)
{
	UINT32 count = 0;
	KKSG::RankData tempRankData;
	for (auto i = m_rankinfos.begin(); i != m_rankinfos.end(); ++i)
	{
		if (++count == rank)
		{
			FirstPassInfo* rankinfo = *i;
			tempRankData.Clear();
			rankinfo->RankInfoToKKSG(&tempRankData);
			if (tempRankData.roleids_size())
			{
				rankdata->CopyFrom(tempRankData);
			}
		}
	}
}

int FirstPassRankList::OnChange(std::vector<UINT64>& roleids, UINT32 starLevel, UINT32 time, UINT32 usecTime)
{
	if (roleids.empty())
	{
		return INVALID_RANK;
	}

	UINT32 rank = GetRank(roleids, starLevel);
	if (rank != INVALID_RANK)
	{
		return INVALID_RANK;
	}

	EraseRank(roleids);

	FirstPassInfo* rankinfo = new FirstPassInfo;
	rankinfo->m_roleIDs = roleids;
	rankinfo->m_time = time;
	rankinfo->m_usecTime = usecTime;
	rankinfo->m_starLevel = starLevel;
	
	m_rankinfos.insert(rankinfo);

	if (m_rankinfos.size() > FIRSTPASSRANKCOUNT)
	{
		auto i = m_rankinfos.end();
		--i;
		delete *i;
		m_rankinfos.erase(i);
	}

	rank = GetRank(roleids, starLevel);

	m_ischanged = true;

	return rank;
}

