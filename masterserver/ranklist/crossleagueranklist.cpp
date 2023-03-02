#include "pch.h"
#include "crossleagueranklist.h"
#include "leagueteam/crossleagueteammgr.h"
#include "config.h"
#include "other/serverinfomgr.h"

#define CROSS_LEAGUERANKCOUNT 5000

CrossLeagueRankInfo::CrossLeagueRankInfo(UINT64 leagueTeamId)
:CRankInfo(leagueTeamId)
{
	Reset();
}

void CrossLeagueRankInfo::Reset()
{
	m_point = 0;
	m_winRate = 0.0f;
	m_time = 0;
}

void CrossLeagueRankInfo::RankInfoToKKSG(KKSG::RankData* rankdata) const
{
	ToRankKKSG(*rankdata->mutable_leagueinfo());
}

void CrossLeagueRankInfo::RankInfoToDBKKSG(KKSG::RankData* rankdata) const
{
	rankdata->set_time(m_time);
	KKSG::LeagueRankData* data = rankdata->mutable_leagueinfo();
	data->set_point(m_point);
	data->set_league_teamid(GetUniqueId());
	data->set_winrate(m_winRate);
}

void CrossLeagueRankInfo::DBKKSGToRankInfo(const KKSG::RankData* rankdata)
{
	m_time = rankdata->time();
	const KKSG::LeagueRankData& data = rankdata->leagueinfo();
	SetUniqueId(data.league_teamid());
	m_point = data.point();
	m_winRate = data.winrate();
}

void CrossLeagueRankInfo::ToRankKKSG(KKSG::LeagueRankData& data) const
{
	data.set_league_teamid(GetUniqueId());
	data.set_point(m_point);
	data.set_winrate(m_winRate);
	CrossLeagueTeam* pLTeam =  CrossLeagueTeamMgr::Instance()->GetTeam(GetUniqueId());
	if (pLTeam == NULL)
	{
		return;	
	}
	data.set_serverid(pLTeam->m_serverId);
	data.set_servername(ZServerInfoMgr::Instance()->GetServerName(pLTeam->m_serverId));
	data.set_teamname(pLTeam->m_name);
	data.set_winnum(pLTeam->m_winNum);
}

void CrossLeagueRankDb::PackDBData(KKSG::RankList* ranklist)
{
	CrossLeagueRankList* lranklist = static_cast<CrossLeagueRankList*>(m_ranklist);
	std::vector<CrossLeagueRankInfo*> vec;
	lranklist->GetRankInfo(vec);
	for (auto i = vec.begin(); i != vec.end(); ++i)
	{
		CrossLeagueRankInfo* rankinfo = *i;
		if (NULL == rankinfo)
		{
			continue;
		}
		KKSG::RankData* rankdata = ranklist->add_rankdata();
		rankinfo->RankInfoToDBKKSG(rankdata);
	}
}

void CrossLeagueRankDb::UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank)
{
	CrossLeagueRankList* lranklist = static_cast<CrossLeagueRankList*>(m_ranklist);
	CrossLeagueRankInfo rankinfo(rankdata.leagueinfo().league_teamid());
	rankinfo.DBKKSGToRankInfo(&rankdata);
	lranklist->OnChange(rankinfo.GetUniqueId(), rankinfo.m_point, rankinfo.m_winRate, rankinfo.m_time);
}

bool CrossLeagueRankDb::BeforeSaveToDB()
{
	CrossLeagueRankList* lranklist = static_cast<CrossLeagueRankList*>(m_ranklist);
	if(lranklist->IsChanged())
	{
		lranklist->SetIsChanged(false);
		return true;
	}
	return false;
}

CrossLeagueRankList::CrossLeagueRankList(int type)
:CRankList(type)
,m_ischanged(false)
{
	RegisterDb(new CrossLeagueRankDb);
	SaveToDBEveryPeriod* policy = new SaveToDBEveryPeriod;	
	policy->SetSaveInterval(CrossLeagueTeamRankInterval);
	m_db->SetPolicy(policy);
}

CrossLeagueRankList::~CrossLeagueRankList()
{
	Clear();
}

void CrossLeagueRankList::Clear()
{
	std::vector<CrossLeagueRankInfo*> allrank;
	m_rankTree.GetAllKey(allrank);
	for (auto it = allrank.begin(); it != allrank.end(); ++it)
	{
		delete *it;
	}
	m_rankTree.Clear();
}

void CrossLeagueRankList::RemoveAll()
{
	Clear();
	SetIsChanged(true);
	GetDbHandler()->SendToDB();
}

UINT32 CrossLeagueRankList::GetNeedRankCount()
{
	return CROSS_LEAGUERANKCOUNT;
}

UINT32 CrossLeagueRankList::GetRank(UINT64 leagueTeamId)
{
	return m_rankTree.GetRankByID(leagueTeamId);
}

UINT32 CrossLeagueRankList::GetSize()
{
	return m_rankTree.GetSize();
}

const CrossLeagueRankInfo* CrossLeagueRankList::GetInfoByRank(UINT32 rank)
{
	CrossLeagueRankInfo* rankinfo = NULL;
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

void CrossLeagueRankList::RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession)
{
	std::vector<CrossLeagueRankInfo*> vec;
	m_rankTree.GetSomeTopKey(count, vec);

	UINT32 vecSize = vec.size();
	for (UINT32 i = 0; i < vecSize; i++)
	{
		CrossLeagueRankInfo* rankinfo = vec[i];
		KKSG::RankData* rankdata = rankdatas->add_rankdata();
		rankinfo->RankInfoToKKSG(rankdata);
		rankdata->set_rank(i + 1);
	}
}

void CrossLeagueRankList::RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank)
{
	const CrossLeagueRankInfo* rankinfo = GetInfoByRank(rank);
	if (NULL != rankinfo)
	{
		rankinfo->RankInfoToKKSG(rankdata);
	}
}

void CrossLeagueRankList::NewRankInfoToClient(KKSG::RankData* rankdata, UINT64 uid)
{
	KKSG::LeagueRankData* ldata = rankdata->mutable_leagueinfo();
	ldata->set_league_teamid(uid);
	CrossLeagueTeam* lt = CrossLeagueTeamMgr::Instance()->GetTeam(uid);
	if (lt)
	{
		ldata->set_serverid(lt->m_serverId);
		ldata->set_servername(ZServerInfoMgr::Instance()->GetServerName(lt->m_serverId));
		ldata->set_teamname(lt->m_name);
		ldata->set_point(lt->m_score);
		ldata->set_winnum(lt->m_winNum);
		ldata->set_winrate(lt->GetWinRate());
	}
}

void CrossLeagueRankList::InsertKey(CrossLeagueRankInfo* rankinfo)
{
	if (NULL == rankinfo)
	{
		return;
	}
	m_rankTree.Insert(rankinfo, rankinfo->GetUniqueId());
}

void CrossLeagueRankList::OnChange(UINT64 leagueTeamId, UINT32 point, float winRate, UINT32 time)
{
	CrossLeagueRankInfo* rankinfo = NULL;
	UINT32 ret = m_rankTree.GetKeyByID(leagueTeamId, rankinfo);
	if (ret == INVALID_RANK)
	{
		CrossLeagueRankInfo* rankinfo = new CrossLeagueRankInfo(leagueTeamId);
		rankinfo->m_point = point;
		rankinfo->m_winRate = winRate;
		rankinfo->m_time = time;
		InsertKey(rankinfo);

		// É¾µôÄ©Î²µÄ
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
		m_rankTree.Erase(leagueTeamId);
		if(0 == point)
		{
			delete rankinfo;
			return;
		}
		rankinfo->m_point = point;
		rankinfo->m_winRate = winRate;
		rankinfo->m_time = time;
		InsertKey(rankinfo);
	}
	m_ischanged = true;
}

void CrossLeagueRankList::GetRankInfo(std::vector<CrossLeagueRankInfo*>& vec)
{
	m_rankTree.GetAllKey(vec);
}

void CrossLeagueRankList::GetTopNRankInfo(UINT32 count, std::vector<CrossLeagueRankInfo*>& vec)
{
	m_rankTree.GetSomeTopKey(count, vec);
}

void CrossLeagueRankList::DelLeagueTeam(UINT64 id)
{
	CrossLeagueRankInfo* rankinfo;
	UINT32 ret = m_rankTree.GetKeyByID(id, rankinfo);
	if (ret != INVALID_RANK)
	{
		m_rankTree.Erase(id);
		delete rankinfo;
	}
}
