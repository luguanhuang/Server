#include "pch.h"
#include "petranklist.h"
#include "role/rolesummarymgr.h"
#include "role/rolesummary.h"

#define PETRANKCOUNT 3000

PetRankInfo::PetRankInfo(UINT64 petuid)
:CRankInfo(petuid)
{

}

PetRankInfo::~PetRankInfo()
{

}

void PetRankInfo::Reset()
{
	m_powerpoint = 0;
}

void PetRankInfo::RankInfoToKKSG(KKSG::RankData* rankdata) const
{
	CRoleSummary* pRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(m_roleid);
	if(NULL != pRoleSummary)
	{
		rankdata->set_roleid(m_roleid);
		rankdata->set_rolename(pRoleSummary->GetName());
		rankdata->set_powerpoint(m_powerpoint);
		rankdata->set_petid(m_petid);
		rankdata->set_petuid(GetUniqueId());
		rankdata->set_is_vip(pRoleSummary->IsQQVip());
		rankdata->set_is_svip(pRoleSummary->IsQQSVip());
		rankdata->set_starttype(pRoleSummary->GetStartUpType());
	}
}

void PetRankInfo::RankInfoToDBKKSG(KKSG::RankData* rankdata) const
{
	rankdata->set_petuid(GetUniqueId());
	rankdata->set_roleid(m_roleid);
	rankdata->set_powerpoint(m_powerpoint);
	rankdata->set_time(m_time);
	rankdata->set_petid(m_petid);
}

void PetRankInfo::DBKKSGToRankInfo(const KKSG::RankData* rankdata)
{
	SetUniqueId(rankdata->petuid());
	m_roleid = rankdata->roleid();
	m_powerpoint =  rankdata->powerpoint();
	m_time = rankdata->time();
	m_petid = rankdata->petid();
}

///////////////////////////////////////////////////////////////////////////////////////////////
void PetRankDb::PackDBData(KKSG::RankList* ranklist)
{
	PetRankList* pPetRankList = static_cast<PetRankList*>(m_ranklist);
	std::vector<PetRankInfo*> rankinfos;
	pPetRankList->GetRankInfo(rankinfos);

	for(auto i = rankinfos.begin(); i != rankinfos.end(); i++)
	{
		CRankInfo* rankinfo = *i;
		if(NULL == rankinfo)
		{
			continue;
		}
		KKSG::RankData* rankdata = ranklist->add_rankdata();
		rankinfo->RankInfoToDBKKSG(rankdata);
	}
}

void PetRankDb::UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank)
{
	PetRankList* ranklist = static_cast<PetRankList*>(m_ranklist);
	PetRankInfo rankinfo(rankdata.petuid());
	rankinfo.DBKKSGToRankInfo(&rankdata);
	ranklist->OnChange(rankinfo.GetUniqueId(), rankinfo.m_petid, rankinfo.m_roleid, rankinfo.m_powerpoint, rankinfo.m_time);
}

bool PetRankDb::BeforeSaveToDB()
{
	PetRankList* ranklist = static_cast<PetRankList*>(m_ranklist);
	if (ranklist->IsChanged())
	{
		ranklist->SetIsChanged(false);
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////
PetRankList::PetRankList(int type)
:CRankList(type)
{
	RegisterDb(new PetRankDb);

	SaveToDBEveryPeriod* policy = new SaveToDBEveryPeriod;
	policy->SetSaveInterval(PetUpdateInterVal);
	m_db->SetPolicy(policy);

	m_ischanged = false;
}

PetRankList::~PetRankList()
{
	Clear();
}

void PetRankList::Clear()
{
	std::vector<PetRankInfo*> rankinfos;
	m_rankTree.GetAllKey(rankinfos);
	for(auto i = rankinfos.begin(); i != rankinfos.end(); i++)
	{
		delete *i;
	}

	m_rankTree.Clear();
	m_roleToPets.clear();
}

UINT32 PetRankList::GetNeedRankCount()
{
	return PETRANKCOUNT;
}

UINT32 PetRankList::GetRank(UINT64 roleid)
{
	if(m_roleToPets.find(roleid) == m_roleToPets.end())
	{
		return INVALID_RANK;
	}

	if(m_roleToPets[roleid].empty())
	{
		return INVALID_RANK;
	}

	PetRankInfo* bestPet = *(m_roleToPets[roleid].begin());
	if(bestPet == NULL)
	{
		return INVALID_RANK;
	}

	return m_rankTree.GetRankByID(bestPet->GetUniqueId());
}

UINT32 PetRankList::GetSize()
{
	return m_rankTree.GetSize();
}

const PetRankInfo* PetRankList::GetInfoByRank(UINT32 rank)
{
	PetRankInfo* rankinfo;
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

void PetRankList::RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession)
{
	std::vector<PetRankInfo*> vec;
	m_rankTree.GetSomeTopKey(count, vec);

	UINT32 vecSize = vec.size();
	for(UINT32 i = 0; i < vecSize; i++)
	{
		PetRankInfo* rankinfo = vec[i];
		KKSG::RankData* rankdata = rankdatas->add_rankdata();
		rankinfo->RankInfoToKKSG(rankdata);
	}
}

void PetRankList::RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank)
{
	const PetRankInfo* rankinfo = GetInfoByRank(rank);
	if(NULL != rankinfo)
	{
		rankinfo->RankInfoToKKSG(rankdata);
	}
}

//petid为0代表删除petuid的坐骑
void PetRankList::OnChange(UINT64 petuid, UINT32 petid, UINT64 roleid, UINT32 powerpoint, UINT32 time)
{
	PetRankInfo* rankinfo;
	UINT32 ret = m_rankTree.GetKeyByID(petuid, rankinfo);
	if(0 == petid) //delete
	{
		if(ret != INVALID_RANK)
		{
			EraseKey(rankinfo);
			delete rankinfo;
		}
		return;
	}
	if(INVALID_RANK == ret)
	{
		rankinfo = new PetRankInfo(petuid);
		rankinfo->m_roleid = roleid;
		rankinfo->m_powerpoint = powerpoint;
		rankinfo->m_time = time;
		rankinfo->m_petid = petid;
		InsertKey(rankinfo);

		if(m_rankTree.GetSize() > GetNeedRankCount())
		{
			UINT32 ret = m_rankTree.GetKeyByRank(m_rankTree.GetSize(), rankinfo);
			if(ret != INVALID_RANK)
			{
				EraseKey(rankinfo);
				delete rankinfo;
			}
		}
	}
	else
	{
		EraseKey(rankinfo);
		rankinfo->m_roleid = roleid;
		rankinfo->m_powerpoint = powerpoint;
		rankinfo->m_time = time;
		rankinfo->m_petid = petid;
		InsertKey(rankinfo);
	}

	m_ischanged = true;
}

void PetRankList::InsertKey(PetRankInfo* rankinfo)
{
	if(NULL == rankinfo)
	{
		LogWarn("Insert a NULL pointer in PetRankList");
		return;
	}
	UINT64 petuid = rankinfo->GetUniqueId();
	m_rankTree.Insert(rankinfo, petuid);

	UINT64 roleid = rankinfo->m_roleid;
	m_roleToPets[roleid].insert(rankinfo);
}

void PetRankList::EraseKey(PetRankInfo* rankinfo)
{
	if(NULL == rankinfo)
	{
		LogWarn("Erase a NULL pointer in PetRankList");
		return;
	}
	UINT64 petuid = rankinfo->GetUniqueId();
	UINT64 roleid = rankinfo->m_roleid;

	bool canFind = true;
	if(!m_rankTree.CanFind(petuid))
	{
		canFind = false;
	}
	m_rankTree.Erase(petuid);
	
	UINT32 ret = m_roleToPets[roleid].erase(rankinfo);

	if((ret == 1 && !canFind) || (ret == 0 && canFind))
	{
		assert(false);
	}
}

void PetRankList::GetRankInfo(std::vector<PetRankInfo*>& vec)
{
	m_rankTree.GetAllKey(vec);
}