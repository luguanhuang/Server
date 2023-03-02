#include "pch.h"
#include "skycraftranklist.h"

#include "skyteam/skyteam.h"
#include "skyteam/skyteammgr.h"
#include "config.h"
#include "table/globalconfig.h"

#define SKYCRAFT_RANKCOUNT 5000

SkyCraftRankInfo::SkyCraftRankInfo(UINT64 stId)
:CRankInfo(stId)
{
	Reset();
}

void SkyCraftRankInfo::Reset()
{
	m_point = 0;
	m_winRate = 0.0f;
	m_time = 0;
}

void SkyCraftRankInfo::SetData(UINT32 point, float winRate, UINT32 time)
{
	m_point = point;
	m_winRate = winRate;
	m_time = time;
}

void SkyCraftRankInfo::RankInfoToKKSG(KKSG::RankData* rankdata) const
{
	ToRankKKSG(*rankdata->mutable_scdata());
}

void SkyCraftRankInfo::RankInfoToDBKKSG(KKSG::RankData* rankdata) const
{
	rankdata->set_time(m_time);
	KKSG::SkyCraftRankData* data = rankdata->mutable_scdata();
	data->set_stid(GetUniqueId());
	data->set_point(m_point);
	data->set_winrate(m_winRate);
}

void SkyCraftRankInfo::DBKKSGToRankInfo(const KKSG::RankData* rankdata)
{
	m_time = rankdata->time();
	const KKSG::SkyCraftRankData& data = rankdata->scdata();
	SetUniqueId(data.stid());
	m_point = data.point();
	m_winRate = data.winrate();
}

void SkyCraftRankInfo::ToRankKKSG(KKSG::SkyCraftRankData& data) const
{
	data.set_stid(GetUniqueId());
	data.set_point(m_point);
	data.set_winrate(m_winRate);

	SkyTeam* pSTeam = SkyTeamMgr::Instance()->GetSkyTeamById(GetUniqueId());
	if (pSTeam)
	{
		data.set_teamname(pSTeam->GetName());
		data.set_winnum(pSTeam->GetTeamRecord().GetTotalWinNum());
	}
}

void SkyCraftRankDb::PackDBData(KKSG::RankList* ranklist)
{
	SkyCraftRankList* sranklist = static_cast<SkyCraftRankList*>(m_ranklist);
	std::vector<SkyCraftRankInfo*> vec;
	sranklist->GetRankInfo(vec);
	for (auto i = vec.begin(); i != vec.end(); ++i)
	{
		SkyCraftRankInfo* rankinfo = *i;
		if (NULL == rankinfo)
		{
			continue;
		}
		KKSG::RankData* rankdata = ranklist->add_rankdata();
		rankinfo->RankInfoToDBKKSG(rankdata);
	}
}

void SkyCraftRankDb::UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank)
{
	SkyCraftRankList* sranklist = static_cast<SkyCraftRankList*>(m_ranklist);
	SkyCraftRankInfo* rankinfo = new SkyCraftRankInfo(rankdata.scdata().stid());
	rankinfo->DBKKSGToRankInfo(&rankdata);
	sranklist->InsertKey(rankinfo);
}

bool SkyCraftRankDb::BeforeSaveToDB()
{
	SkyCraftRankList* sranklist = static_cast<SkyCraftRankList*>(m_ranklist);
	if (sranklist->IsChanged())
	{
		sranklist->SetIsChanged(false);
		return true;
	}
	return false;
}

SkyCraftRankList::SkyCraftRankList(int type)
:CRankList(type)
,m_ischanged(false)
{
	RegisterDb(new SkyCraftRankDb);
	SaveToDBEveryPeriod* policy = new SaveToDBEveryPeriod;	
	policy->SetSaveInterval(GetUpdateInterval());
	m_db->SetPolicy(policy);
}

SkyCraftRankList::~SkyCraftRankList()
{
	Clear();
}

void SkyCraftRankList::Clear()
{
	std::vector<SkyCraftRankInfo*> allrank;
	m_rankTree.GetAllKey(allrank);
	for (auto it = allrank.begin(); it != allrank.end(); ++it)
	{
		delete *it;
	}
	m_rankTree.Clear();
}

void SkyCraftRankList::RemoveAll()
{
	Clear();
	SetIsChanged(true);
	GetDbHandler()->SendToDB();
}

UINT32 SkyCraftRankList::GetNeedRankCount()
{
	return SKYCRAFT_RANKCOUNT;
}

UINT32 SkyCraftRankList::GetRank(UINT64 stId)
{
	return m_rankTree.GetRankByID(stId);
}

UINT32 SkyCraftRankList::GetSize()
{
	return m_rankTree.GetSize();
}

const SkyCraftRankInfo* SkyCraftRankList::GetInfoByRank(UINT32 rank)
{
	SkyCraftRankInfo* rankinfo = NULL;
	UINT32 ret = m_rankTree.GetKeyByRank(rank, rankinfo);
	if (ret != INVALID_RANK)
	{
		return rankinfo;
	}
	else
	{
		return NULL;
	}
}

void SkyCraftRankList::RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession)
{
	std::vector<SkyCraftRankInfo*> vec;
	m_rankTree.GetSomeTopKey(count, vec);

	UINT32 vecSize = vec.size();
	for (UINT32 i = 0; i < vecSize; i++)
	{
		SkyCraftRankInfo* rankinfo = vec[i];
		KKSG::RankData* rankdata = rankdatas->add_rankdata();
		rankinfo->RankInfoToKKSG(rankdata);
		rankdata->set_rank(i + 1);
	}
}

void SkyCraftRankList::RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank)
{
	const SkyCraftRankInfo* rankinfo = GetInfoByRank(rank);
	if (NULL != rankinfo)
	{
		rankinfo->RankInfoToKKSG(rankdata);
	}
}

void SkyCraftRankList::NewRankInfoToClient(KKSG::RankData* rankdata, UINT64 uid)
{
	KKSG::SkyCraftRankData* sc = rankdata->mutable_scdata();
	sc->set_stid(uid);
	SkyTeam* pSTeam = SkyTeamMgr::Instance()->GetSkyTeamById(uid);
	if (pSTeam)
	{
		sc->set_teamname(pSTeam->GetName());
		sc->set_point(pSTeam->GetScore());
		sc->set_winnum(pSTeam->GetTeamRecord().GetTotalWinNum());
		sc->set_winrate(pSTeam->GetTeamRecord().GetTotalWinRate());
	}
}

void SkyCraftRankList::InsertKey(SkyCraftRankInfo* rankinfo)
{
	if (NULL == rankinfo)
	{
		return;
	}
	m_rankTree.Insert(rankinfo, rankinfo->GetUniqueId());
}

void SkyCraftRankList::OnChange(UINT64 stId, UINT32 point, float winRate, UINT32 time)
{
	SkyCraftRankInfo* rankinfo = NULL;
	UINT32 ret = m_rankTree.GetKeyByID(stId, rankinfo);
	if (ret == INVALID_RANK)
	{
		SkyCraftRankInfo* rankinfo = new SkyCraftRankInfo(stId);
		rankinfo->SetData(point, winRate, time);
		InsertKey(rankinfo);

		// É¾µôÄ©Î²µÄ
		if (m_rankTree.GetSize() > GetNeedRankCount())
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
		m_rankTree.Erase(stId);
		if(0 == point)
		{
			delete rankinfo;
			return;
		}
		rankinfo->SetData(point, winRate, time);
		InsertKey(rankinfo);
	}
	m_ischanged = true;
}

void SkyCraftRankList::GetRankInfo(std::vector<SkyCraftRankInfo*>& vec)
{
	m_rankTree.GetAllKey(vec);
}

void SkyCraftRankList::GetTopNRankInfo(UINT32 count, std::vector<SkyCraftRankInfo*>& vec)
{
	m_rankTree.GetSomeTopKey(count, vec);
}

void SkyCraftRankList::DelSkyTeam(UINT64 id)
{
	SkyCraftRankInfo* rankinfo;
	UINT32 ret = m_rankTree.GetKeyByID(id, rankinfo);
	if (ret != INVALID_RANK)
	{
		m_rankTree.Erase(id);
		delete rankinfo;
	}
}

int SkyCraftRankList::GetUpdateInterval()
{
	return SkyCraftRankInterval;
}