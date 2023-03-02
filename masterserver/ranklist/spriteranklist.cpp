#include "pch.h"
#include "spriteranklist.h"
#include "role/rolesummarymgr.h"
#include "role/rolesummary.h"

#define SPRITERANKCOUNT 3000

SpriteRankInfo::SpriteRankInfo(UINT64 roleid)
:CRankInfo(roleid)
{
	
}

SpriteRankInfo::~SpriteRankInfo()
{

}

void SpriteRankInfo::Reset()
{
	m_powerpoint = 0;
}

void SpriteRankInfo::RankInfoToKKSG(KKSG::RankData* rankdata) const
{
	CRoleSummary* pRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(GetUniqueId());
	if(NULL != pRoleSummary)
	{
		rankdata->set_roleid(GetUniqueId());
		rankdata->set_rolename(pRoleSummary->GetName());
		rankdata->set_powerpoint(m_powerpoint);
		rankdata->set_titleid(pRoleSummary->GetTitleID());
		rankdata->set_is_vip(pRoleSummary->IsQQVip());
		rankdata->set_is_svip(pRoleSummary->IsQQSVip());
		rankdata->set_starttype(pRoleSummary->GetStartUpType());
	}
}

void SpriteRankInfo::RankInfoToDBKKSG(KKSG::RankData* rankdata) const
{
	rankdata->set_roleid(GetUniqueId());
	rankdata->set_powerpoint(m_powerpoint);
	rankdata->set_time(m_time);
}

void SpriteRankInfo::DBKKSGToRankInfo(const KKSG::RankData* rankdata)
{
	SetUniqueId(rankdata->roleid());
	m_powerpoint = rankdata->powerpoint();
	m_time = rankdata->time();
}

///////////////////////////////////////////////////////////////////////////
void SpriteRankDb::PackDBData(KKSG::RankList* ranklist)
{
	SpriteRankList* pSpriteRankList = static_cast<SpriteRankList*>(m_ranklist);
	std::vector<SpriteRankInfo*> rankinfos;
	pSpriteRankList->GetRankInfo(rankinfos);

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

void SpriteRankDb::UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank)
{
	SpriteRankList* pSpriteRankList = static_cast<SpriteRankList*>(m_ranklist);
	SpriteRankInfo rankinfo(rankdata.roleid());
	rankinfo.DBKKSGToRankInfo(&rankdata);
	pSpriteRankList->OnChange(rankinfo.GetUniqueId(), rankinfo.m_powerpoint, rankinfo.m_time);
}

bool SpriteRankDb::BeforeSaveToDB()
{
	SpriteRankList* pSpriteRankList = static_cast<SpriteRankList*>(m_ranklist);
	if(pSpriteRankList->IsChanged())
	{
		pSpriteRankList->SetIsChanged(false);
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////
SpriteRankList::SpriteRankList(int type)
:CRankList(type)
{
	RegisterDb(new SpriteRankDb);

	SaveToDBEveryPeriod* policy = new SaveToDBEveryPeriod;
	policy->SetSaveInterval(SpriteUpdateInterVal);
	m_db->SetPolicy(policy);

	m_ischanged = false;
}

SpriteRankList::~SpriteRankList()
{
	Clear();
}

void SpriteRankList::Clear()
{
	std::vector<SpriteRankInfo*> rankinfos;
	m_rankTree.GetAllKey(rankinfos);
	for(auto i = rankinfos.begin(); i != rankinfos.end(); i++)
	{
		delete *i;
	}

	m_rankTree.Clear();
}

UINT32 SpriteRankList::GetNeedRankCount()
{
	return SPRITERANKCOUNT;
}

UINT32 SpriteRankList::GetRank(UINT64 roleid)
{
	return m_rankTree.GetRankByID(roleid);
}

UINT32 SpriteRankList::GetSize()
{
	return m_rankTree.GetSize();
}

const SpriteRankInfo* SpriteRankList::GetInfoByRank(UINT32 rank)
{
	SpriteRankInfo* rankinfo;
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

void SpriteRankList::RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession)
{
	std::vector<SpriteRankInfo*> vec;
	m_rankTree.GetSomeTopKey(count, vec);

	UINT32 vecSize = vec.size();
	for(UINT32 i = 0; i < vecSize; i++)
	{
		SpriteRankInfo* rankinfo = vec[i];
		KKSG::RankData* rankdata = rankdatas->add_rankdata();
		rankinfo->RankInfoToKKSG(rankdata);
	}
}

void SpriteRankList::RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank)
{
	const SpriteRankInfo* rankinfo = GetInfoByRank(rank);
	if(NULL != rankinfo)
	{
		rankinfo->RankInfoToKKSG(rankdata);
	}
}

void SpriteRankList::OnChange(UINT64 roleid, UINT32 powerpoint, UINT32 time)
{
	SpriteRankInfo* rankinfo;
	UINT32 ret = m_rankTree.GetKeyByID(roleid, rankinfo);
	if(0 == powerpoint) //身上没有精灵，从排行榜删除
	{
		if(ret != INVALID_RANK)
		{
			m_rankTree.Erase(rankinfo->GetUniqueId());
			delete rankinfo;
		}
		return;
	}
	if(INVALID_RANK == ret)
	{
		rankinfo = new SpriteRankInfo(roleid);
		rankinfo->m_powerpoint = powerpoint;
		rankinfo->m_time = time;
		InsertKey(rankinfo);

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
		rankinfo->m_powerpoint = powerpoint;
		rankinfo->m_time = time;
		InsertKey(rankinfo);
	}

	m_ischanged = true;
}

void SpriteRankList::InsertKey(SpriteRankInfo* rankinfo)
{
	if(NULL == rankinfo)
	{
		LogWarn("Insert a NULL pointer in SpriteRankList");
		return;
	}
	UINT64 roleid = rankinfo->GetUniqueId();
	m_rankTree.Insert(rankinfo, roleid);
}

void SpriteRankList::GetRankInfo(std::vector<SpriteRankInfo*>& vec)
{
	m_rankTree.GetAllKey(vec);
}