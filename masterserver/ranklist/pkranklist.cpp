#include "pch.h"
#include "pkranklist.h"
#include "role/rolesummarymgr.h"
#include "pb/project.pb.h"
#include "ranklistmgr.h"
#include "table/ProfessionMgr.h"
#include "account/punishmgr.h"
#include "table/globalconfig.h"
//#include "rewardmgr.h"
//#include "designationMgr.h"

#define PKRANKCOUNT 5000

PkRankInfo::PkRankInfo(UINT64 roleid)
:CRankInfo(roleid)
{
}

PkRankInfo::~PkRankInfo()
{

}

void PkRankInfo::SetData(UINT32 point, UINT32 ctime, const KKSG::PkRankExtraData& extraData)
{
	m_point = point;
	m_time = ctime;
	m_extraData = extraData;
}

void PkRankInfo::Reset()
{
	m_point = 0;
}

void PkRankInfo::RankInfoToKKSG(KKSG::RankData* rankdata) const
{
	CRoleSummary* role = CRoleSummaryMgr::Instance()->GetRoleSummary(GetUniqueId());
	if (NULL != role)
	{
		rankdata->set_roleid(GetUniqueId());
		rankdata->set_rolename(role->GetName());
		rankdata->set_rolelevel(role->GetLevel());
		rankdata->set_pkpoint(m_point);
		rankdata->set_profession(role->GetProfession());
		rankdata->set_titleid(role->GetTitleID());
		rankdata->set_is_vip(role->IsQQVip());
		rankdata->set_is_svip(role->IsQQSVip());
		rankdata->set_starttype(role->GetStartUpType());
        *rankdata->mutable_pkextradata() = m_extraData;
	}
}

void PkRankInfo::RankInfoToDBKKSG(KKSG::RankData* rankdata) const
{
	rankdata->set_roleid(GetUniqueId());
	rankdata->set_pkpoint(m_point);
	rankdata->set_time(m_time);
	*rankdata->mutable_pkextradata() = m_extraData;
}

void PkRankInfo::DBKKSGToRankInfo(const KKSG::RankData* rankdata)
{
	SetUniqueId(rankdata->roleid());
	m_point = rankdata->pkpoint();
	m_time = rankdata->time();
	m_extraData = rankdata->pkextradata();
}

void PkRankInfo::InfoPrint()
{
	SSInfo << " role = " << GetUniqueId() << " point = " << m_point << " time = " << m_time << END;
	SSInfo << " joincount = " << m_extraData.joincount() << " winrate = " << m_extraData.winrate() << " continuewin = " << m_extraData.continuewin() << END;
}

//////////////////////////////////////////////////////////////////////
void PkRealTimeDb::PackDBData(KKSG::RankList* ranklist)
{
	PkRealTimeRankList* pPkRanklist = static_cast<PkRealTimeRankList*>(m_ranklist);
	std::vector<PkRankInfo*> rankinfo;
	pPkRanklist->GetRankInfo(rankinfo);

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

void PkRealTimeDb::UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank)
{
	PkRealTimeRankList* ranklist = static_cast<PkRealTimeRankList*>(m_ranklist);
	PkRankInfo pkinfo(rankdata.roleid());
	pkinfo.DBKKSGToRankInfo(&rankdata);
	ranklist->OnChange(pkinfo.GetUniqueId(), pkinfo.m_point, pkinfo.m_time, pkinfo.m_extraData);
}

bool PkRealTimeDb::BeforeSaveToDB()
{
	PkRealTimeRankList* ranklist = static_cast<PkRealTimeRankList*>(m_ranklist);
	if(ranklist->IsChanged())
	{
		ranklist->SetIsChanged(false);
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
PkRealTimeRankList::PkRealTimeRankList(int type)
:CRankList(type)
{	
	RegisterDb(new PkRealTimeDb);

	SaveToDBEveryPeriod* policy = new SaveToDBEveryPeriod;	
	policy->SetSaveInterval(GetUpdateInterval());
	m_db->SetPolicy(policy);

	m_ischanged = false;
}

PkRealTimeRankList::~PkRealTimeRankList()
{
	Clear();
}

void PkRealTimeRankList::Clear()
{
	std::vector<PkRankInfo*> pkrankinfo;
	m_rankTree.GetAllKey(pkrankinfo);
	for(auto i = pkrankinfo.begin(); i != pkrankinfo.end(); i++)
	{
		delete *i;
	}

	m_rankTree.Clear();
	SetIsChanged(true);
}

UINT32 PkRealTimeRankList::GetNeedRankCount()
{
	return PKRANKCOUNT;
}

UINT32 PkRealTimeRankList::GetRank(UINT64 roleid)
{
	return m_rankTree.GetRankByID(roleid);
}

UINT32 PkRealTimeRankList::GetSize()
{
	return m_rankTree.GetSize();
}

const PkRankInfo* PkRealTimeRankList::GetInfoByRank(UINT32 rank)
{
	PkRankInfo* rankinfo;
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

UINT32 PkRealTimeRankList::GetPkPoint(UINT64 roleId)
{
	PkRankInfo* rankinfo;
	UINT32 ret = m_rankTree.GetKeyByID(roleId, rankinfo);
	if (ret != INVALID_RANK)
	{
		return rankinfo->m_point;
	}
	return GetGlobalConfig().PkBasePoint;
}

void PkRealTimeRankList::RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession)
{
	std::vector<PkRankInfo*> vec;
	m_rankTree.GetSomeTopKey(count, vec);

	UINT32 vecSize = vec.size();
	for(UINT32 i = 0; i < vecSize; i++)
	{
		PkRankInfo* rankinfo = vec[i];
		KKSG::RankData temprankdata;
		rankinfo->RankInfoToKKSG(&temprankdata);
		if (0 == profession || 
			CProfessionMgr::Instance()->GetBasicProfession(temprankdata.profession()) == (int)profession)
		{
			KKSG::RankData* rankdata = rankdatas->add_rankdata();
			rankdata->CopyFrom(temprankdata);
		}
	}
}

void PkRealTimeRankList::RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank)
{
	const PkRankInfo* rankinfo = GetInfoByRank(rank);
	if (NULL != rankinfo)
	{
		rankinfo->RankInfoToKKSG(rankdata);
	}
}

void PkRealTimeRankList::OnChange(UINT64 roleid, UINT32 point, UINT32 time, const KKSG::PkRankExtraData& extraData)
{
	if(CPunishMgr::Instance()->IsPunish(roleid, KKSG::PUNISH_USER_PK_RANK))
	{
		return;
	}
	PkRankInfo* rankinfo;
	UINT32 ret = m_rankTree.GetKeyByID(roleid, rankinfo);
	if(ret == INVALID_RANK)
	{
		if(0 == point)
		{
			return;
		}
		PkRankInfo* rankinfo = new PkRankInfo(roleid);
		rankinfo->SetData(point, time, extraData);
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
		if(0 == point)
		{
			delete rankinfo;
			return;
		}
		rankinfo->SetData(point, time, extraData);
		InsertKey(rankinfo);
	}
	m_ischanged = true;
}

void PkRealTimeRankList::InsertKey(PkRankInfo* rankinfo)
{
	if(NULL == rankinfo)
	{
		LogWarn("Insert a NULL pointer in PkRealTimeRankList");
		return;
	}
	UINT64 roleid = rankinfo->GetUniqueId();
	m_rankTree.Insert(rankinfo, roleid);
}

void PkRealTimeRankList::GetRankInfo(std::vector<PkRankInfo*>& vec)
{
	m_rankTree.GetAllKey(vec);
}

void PkRealTimeRankList::DeleteRole(UINT64 roleid)
{
	PkRankInfo* rankinfo;
	UINT32 ret = m_rankTree.GetKeyByID(roleid, rankinfo);
	if(ret != INVALID_RANK)
	{
		m_rankTree.Erase(roleid);
		delete rankinfo;
	}
}

int PkRealTimeRankList::GetUpdateInterval()
{
	if(GetType() == KKSG::PkRank2v2)
	{
		return PK2v2UpdateInterval;
	}
	return PkUpdateInterVal;
}

void PkRealTimeRankList::InfoPrint()
{
	std::vector<PkRankInfo*> vec;
	GetRankInfo(vec);
	SSInfo << " size = " << vec.size() << END;
	for(size_t i = 0; i < vec.size(); ++i)
	{
		vec[i]->InfoPrint();
	}
}

void PkRealTimeRankList::ASGetTopInfo(UINT32 count, std::vector<PkRankInfo*>& infoVec)
{
	if(count > GetSize())
	{
		count = GetSize();
	}
	m_rankTree.GetSomeTopKey(count, infoVec);
}

void PkRealTimeRankList::ASGetTopRank(UINT32 count, std::vector<ASPair>& rankVec)
{
	if(count > GetSize())
	{
		count = GetSize();
	}
	std::vector<PkRankInfo*> vec;
	m_rankTree.GetSomeTopKey(count, vec);
	for(size_t i = 0; i < vec.size(); ++i)
	{
		ASPair tmp;
		tmp.first = (i+1);
		tmp.second = vec[i]->GetUniqueId();
		rankVec.push_back(tmp);
	}
}

LastWeekPkRankList::LastWeekPkRankList(int type)
	:PkRealTimeRankList(type)
{

}

LastWeekPkRankList::~LastWeekPkRankList()
{

}

int LastWeekPkRankList::GetUpdateInterval()
{
	return LastWeek_PkUpdateInterval;
}

UINT32 LastWeekPkRankList::GetNeedRankCount()
{
	return GetGlobalConfig().ArenaStarRankLimit;
}

void LastWeekPkRankList::ASUpdateLastWeekRankInfo(ArenaStarRankInterface* pCurRank)
{
	if(NULL == pCurRank)
	{
		return;
	}
	Clear();
	std::vector<PkRankInfo*> vec;
	pCurRank->ASGetTopInfo(GetNeedRankCount(), vec);
	for(size_t i = 0; i < vec.size(); ++i)
	{
		PkRankInfo* pNewInfo = new PkRankInfo(*vec[i]);
		InsertKey(pNewInfo);
	}
    SetIsChanged(true);
}
