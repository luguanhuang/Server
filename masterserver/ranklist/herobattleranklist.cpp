#include "pch.h"
#include "herobattleranklist.h"
#include "role/rolesummarymgr.h"
#include "pb/project.pb.h"
#include "ranklistmgr.h"
#include "table/ProfessionMgr.h"
#include "account/punishmgr.h"
#include "table/globalconfig.h"

#define HERORANKCOUNT 3000

HeroRankInfo::HeroRankInfo(UINT64 roleid):CRankInfo(roleid)
{

}

HeroRankInfo::~HeroRankInfo()
{

}

void HeroRankInfo::Reset()
{
	m_rankData.set_elopoint(0);
}

void HeroRankInfo::RankInfoToKKSG(KKSG::RankData* rankdata) const
{
	CRoleSummary* role = CRoleSummaryMgr::Instance()->GetRoleSummary(GetUniqueId());
	if (role != NULL)
	{
		rankdata->set_roleid(GetUniqueId());
		rankdata->set_strroleid(ToString(GetUniqueId()));
		rankdata->set_rolename(role->GetName());
	}
	KKSG::HeroRankData* herodata = rankdata->mutable_heroinfo();
	*herodata = m_rankData;
}

void HeroRankInfo::RankInfoToDBKKSG(KKSG::RankData* rankdata) const
{
	rankdata->set_roleid(GetUniqueId());
	rankdata->set_time(m_time);
	KKSG::HeroRankData* herodata = rankdata->mutable_heroinfo();
	*herodata = m_rankData;
}

void HeroRankInfo::DBKKSGToRankInfo(const KKSG::RankData* rankdata)
{
	SetUniqueId(rankdata->roleid());
	m_time = rankdata->time();
	m_rankData = rankdata->heroinfo();
}

void HeroRankInfo::InfoPrint()
{
	SSInfo << " role = " << GetUniqueId() << " time = " << m_time << " elopoint =  " << m_rankData.elopoint() << END;
	SSInfo << " totol = " << m_rankData.totalnum() << " win = " << m_rankData.winnum() << " continuewin = " << m_rankData.continuewinnum() << " maxkill = " << m_rankData.maxkillnum() << END;
}

//////////////////////////////////////////////////////////////////////
void HeroBattleRankDb::PackDBData(KKSG::RankList* ranklist)
{
	HeroBattleRankList* pRanklist = static_cast<HeroBattleRankList*>(m_ranklist);
	std::vector<HeroRankInfo*> rankinfo;
	pRanklist->GetRankInfo(rankinfo);

	for (auto i = rankinfo.begin(); i != rankinfo.end(); ++i)
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

void HeroBattleRankDb::UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank)
{
	HeroBattleRankList* pRanklist = static_cast<HeroBattleRankList*>(m_ranklist);
	HeroRankInfo info(rankdata.roleid());
	info.DBKKSGToRankInfo(&rankdata);
	pRanklist->OnChange(info.GetUniqueId(), info.m_time, info.m_rankData);
}

bool HeroBattleRankDb::BeforeSaveToDB()
{
	HeroBattleRankList* ranklist = static_cast<HeroBattleRankList*>(m_ranklist);
	if (ranklist->IsChanged())
	{
		ranklist->SetIsChanged(false);
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
HeroBattleRankList::HeroBattleRankList(int type):CRankList(type)
{	
	RegisterDb(new HeroBattleRankDb);

	SaveToDBEveryPeriod* policy = new SaveToDBEveryPeriod;	
	policy->SetSaveInterval(GetUpdateInterval());
	m_db->SetPolicy(policy);

	m_ischanged = false;
}

HeroBattleRankList::~HeroBattleRankList()
{
	Clear();
}

void HeroBattleRankList::Clear()
{
	std::vector<HeroRankInfo*> rankinfo;
	m_rankTree.GetAllKey(rankinfo);
	for(auto i = rankinfo.begin(); i != rankinfo.end(); i++)
	{
		delete *i;
	}

	m_rankTree.Clear();
}

void HeroBattleRankList::RemoveAll()
{
	Clear();
	SetIsChanged(true);
	GetDbHandler()->SendToDB();
}

UINT32 HeroBattleRankList::GetNeedRankCount()
{
	return HERORANKCOUNT;
}

UINT32 HeroBattleRankList::GetRank(UINT64 roleid)
{
	return m_rankTree.GetRankByID(roleid);
}

UINT32 HeroBattleRankList::GetSize()
{
	return m_rankTree.GetSize();
}

const HeroRankInfo* HeroBattleRankList::GetInfoByRank(UINT32 rank)
{
	HeroRankInfo* rankinfo;
	UINT32 ret = m_rankTree.GetKeyByRank(rank, rankinfo);
	if (ret != INVALID_RANK)
	{
		return rankinfo;
	}
	return NULL;
}

void HeroBattleRankList::RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession)
{
	std::vector<HeroRankInfo*> vec;
	m_rankTree.GetSomeTopKey(count, vec);

	for (UINT32 i = 0; i < vec.size(); i++)
	{
		HeroRankInfo* rankinfo = vec[i];
		KKSG::RankData* rankdata = rankdatas->add_rankdata();
		rankinfo->RankInfoToKKSG(rankdata);
	}
}

void HeroBattleRankList::RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank)
{
	const HeroRankInfo* rankinfo = GetInfoByRank(rank);
	if (rankinfo != NULL)
	{
		rankinfo->RankInfoToKKSG(rankdata);
	}
}

void HeroBattleRankList::OnChange(UINT64 roleid, UINT32 time, const KKSG::HeroRankData& rankData)
{
	/*if(CPunishMgr::Instance()->IsPunish(roleid, KKSG::PUNISH_USER_PK_RANK))
	{
		return;
	}*/
	if (rankData.totalnum() == 0)
	{
		return;
	}
	HeroRankInfo* rankinfo;
	UINT32 ret = m_rankTree.GetKeyByID(roleid, rankinfo);
	if (ret == INVALID_RANK)
	{
		rankinfo = new HeroRankInfo(roleid);
		rankinfo->m_time = time;
		rankinfo->m_rankData = rankData;
		InsertKey(rankinfo);

		if (m_rankTree.GetSize() > GetNeedRankCount())
		{
			UINT32 ret = m_rankTree.GetKeyByRank(m_rankTree.GetSize(), rankinfo);
			if (ret != INVALID_RANK)
			{
				m_rankTree.Erase(rankinfo->GetUniqueId());
				delete rankinfo;
			}
		}
	}
	else
	{
		m_rankTree.Erase(roleid);
		rankinfo->m_time = time;
		rankinfo->m_rankData = rankData;
		InsertKey(rankinfo);
	}
	m_ischanged = true;
}

void HeroBattleRankList::InsertKey(HeroRankInfo* rankinfo)
{
	if (rankinfo == NULL)
	{
		LogWarn("Insert a NULL pointer in HeroBattleRankList");
		return;
	}
	if (rankinfo->m_rankData.totalnum() == 0)
	{
		return;
	}
	UINT64 roleid = rankinfo->GetUniqueId();
	m_rankTree.Insert(rankinfo, roleid);
}

void HeroBattleRankList::GetRankInfo(std::vector<HeroRankInfo*>& vec)
{
	m_rankTree.GetAllKey(vec);
}

int HeroBattleRankList::GetUpdateInterval()
{
	return HeroBattleUpdateInterval;
}

void HeroBattleRankList::InfoPrint()
{
	std::vector<HeroRankInfo*> vec;
	GetRankInfo(vec);
	SSInfo << " size = " << vec.size() << END;
	for(size_t i = 0; i < vec.size(); ++i)
	{
		vec[i]->InfoPrint();
	}
}

void HeroBattleRankList::ASGetTopInfo(UINT32 count, std::vector<HeroRankInfo*>& infoVec)
{
	if(count > GetSize())
	{
		count = GetSize();
	}
	m_rankTree.GetSomeTopKey(count, infoVec);
}

void HeroBattleRankList::ASGetTopRank(UINT32 count, std::vector<ASPair>& rankVec)
{
	if(count > GetSize())
	{
		count = GetSize();
	}
	std::vector<HeroRankInfo*> vec;
	m_rankTree.GetSomeTopKey(count, vec);
	for(size_t i = 0; i < vec.size(); ++i)
	{
		ASPair tmp;
		tmp.first = (i+1);
		tmp.second = vec[i]->GetUniqueId();
		rankVec.push_back(tmp);
	}
}

LastWeekHeroBattleRankList::LastWeekHeroBattleRankList(int type)
	:HeroBattleRankList(type)
{

}

LastWeekHeroBattleRankList::~LastWeekHeroBattleRankList()
{

}

int LastWeekHeroBattleRankList::GetUpdateInterval()
{
	return LastWeek_HeroBattleUpdateInterval;
}

UINT32 LastWeekHeroBattleRankList::GetNeedRankCount()
{
	return GetGlobalConfig().ArenaStarRankLimit;
}

void LastWeekHeroBattleRankList::ASUpdateLastWeekRankInfo(ArenaStarRankInterface* pCurRank)
{
	if(NULL == pCurRank)
	{
		return;
	}
	Clear();
	std::vector<HeroRankInfo*> vec;
	pCurRank->ASGetTopInfo(GetNeedRankCount(), vec);
	for(size_t i = 0; i < vec.size(); ++i)
	{
		HeroRankInfo* pNewInfo = new HeroRankInfo(*vec[i]);
		InsertKey(pNewInfo);
	}	
	SetIsChanged(true);
}
