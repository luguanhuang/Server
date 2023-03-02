#include "pch.h"
#include "leagueranklist.h"
#include "leagueteam/leagueteammgr.h"
#include "config.h"
#include "table/globalconfig.h"

#define LEAGUERANKCOUNT 5000

LeagueRankInfo::LeagueRankInfo(UINT64 leagueTeamId)
:CRankInfo(leagueTeamId)
{
	Reset();
}

void LeagueRankInfo::Reset()
{
	m_time = 0;
	m_rankData.Clear();
	m_roleids.clear();
}

void LeagueRankInfo::RankInfoToKKSG(KKSG::RankData* rankdata) const
{
	ToRankKKSG(*rankdata->mutable_leagueinfo());
}

void LeagueRankInfo::RankInfoToDBKKSG(KKSG::RankData* rankdata) const
{
	rankdata->set_time(m_time);
	KKSG::LeagueRankData* data = rankdata->mutable_leagueinfo();
	*data = m_rankData;
	data->set_league_teamid(GetUniqueId());//放在最后

	for(auto it = m_roleids.begin(); it != m_roleids.end(); ++it)
	{
		rankdata->add_roleids(*it);
	}
}

void LeagueRankInfo::DBKKSGToRankInfo(const KKSG::RankData* rankdata)
{
	m_time = rankdata->time();
	const KKSG::LeagueRankData& data = rankdata->leagueinfo();
	SetUniqueId(data.league_teamid());
	m_rankData = data;

	for(int i = 0; i < rankdata->roleids_size(); ++i)
	{
		m_roleids.push_back(rankdata->roleids(i));
	}
}

void LeagueRankInfo::ToRankKKSG(KKSG::LeagueRankData& data) const
{
	data = m_rankData;
	data.set_league_teamid(GetUniqueId());
	data.set_serverid(MSConfig::Instance()->GetServerID());
	data.set_servername(MSConfig::Instance()->GetName());

	FillTeamData(data);
}

void LeagueRankInfo::FillTeamData(KKSG::LeagueRankData& data) const
{
	//teamid历史唯一
	LeagueTeam* team = LeagueTeamMgr::Instance()->GetLeagueTeamById(GetUniqueId());
	if (team == NULL)
	{
		return;	
	}
	data.set_teamname(team->GetName());
	data.set_winnum(team->GetLeagueTeamBattleRecord().GetTotalWinNum());
}

void LeagueRankInfo::SetData(UINT32 point, float winRate, UINT32 ctime, UINT32 continueWin, UINT32 joinCount)
{
	m_time = ctime;
	m_rankData.set_point(point);
	m_rankData.set_winrate(winRate);
	m_rankData.set_continuewin(continueWin);
	m_rankData.set_joincount(joinCount);
}

void LeagueRankInfo::InfoPrint()
{
	SSInfo << " team = " << GetUniqueId() << " point = " << m_rankData.point() << " time = " << m_time << END;
	SSInfo << " total = " << m_rankData.joincount() << " winrate = " << m_rankData.winrate() << " continuewin = " << m_rankData.continuewin() << END;
	SSInfo << " winnum = " << m_rankData.winnum() << " teamname = " << m_rankData.teamname() << END;
}


void LeagueRankDb::PackDBData(KKSG::RankList* ranklist)
{
	LeagueRankList* lranklist = static_cast<LeagueRankList*>(m_ranklist);
	std::vector<LeagueRankInfo*> vec;
	lranklist->GetRankInfo(vec);
	for (auto i = vec.begin(); i != vec.end(); ++i)
	{
		LeagueRankInfo* rankinfo = *i;
		if (NULL == rankinfo)
		{
			continue;
		}
		KKSG::RankData* rankdata = ranklist->add_rankdata();
		rankinfo->RankInfoToDBKKSG(rankdata);
	}
}

void LeagueRankDb::UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank)
{
	LeagueRankList* lranklist = static_cast<LeagueRankList*>(m_ranklist);
	LeagueRankInfo rankinfo(rankdata.leagueinfo().league_teamid());
	rankinfo.DBKKSGToRankInfo(&rankdata);
	if(lranklist->GetType() == KKSG::LastWeek_LeagueTeamRank)//竞技场名人堂(有额外数据 不能OnChange)
	{
		auto pNewInfo = new LeagueRankInfo(rankinfo);	
		lranklist->InsertKey(pNewInfo);
	}
	else
	{
		lranklist->OnChange(rankinfo.GetUniqueId(), rankinfo.m_rankData.point(), rankinfo.m_rankData.winrate(), rankinfo.m_time, rankinfo.m_rankData.continuewin(), rankinfo.m_rankData.joincount());
	}
}

bool LeagueRankDb::BeforeSaveToDB()
{
	LeagueRankList* lranklist = static_cast<LeagueRankList*>(m_ranklist);
	if(lranklist->IsChanged())
	{
		lranklist->SetIsChanged(false);
		return true;
	}
	return false;
}

LeagueRankList::LeagueRankList(int type)
:CRankList(type)
,m_ischanged(false)
{
	RegisterDb(new LeagueRankDb);
	SaveToDBEveryPeriod* policy = new SaveToDBEveryPeriod;	
	policy->SetSaveInterval(GetUpdateInterval());
	m_db->SetPolicy(policy);
}

LeagueRankList::~LeagueRankList()
{
	Clear();
}

void LeagueRankList::Clear()
{
	std::vector<LeagueRankInfo*> allrank;
	m_rankTree.GetAllKey(allrank);
	for (auto it = allrank.begin(); it != allrank.end(); ++it)
	{
		delete *it;
	}
	m_rankTree.Clear();
}

void LeagueRankList::RemoveAll()
{
	Clear();
	SetIsChanged(true);
	GetDbHandler()->SendToDB();
}

UINT32 LeagueRankList::GetNeedRankCount()
{
	return LEAGUERANKCOUNT;
}

UINT32 LeagueRankList::GetRank(UINT64 leagueTeamId)
{
	return m_rankTree.GetRankByID(leagueTeamId);
}

UINT32 LeagueRankList::GetSize()
{
	return m_rankTree.GetSize();
}

const LeagueRankInfo* LeagueRankList::GetInfoByRank(UINT32 rank)
{
	LeagueRankInfo* rankinfo = NULL;
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

void LeagueRankList::RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession)
{
	std::vector<LeagueRankInfo*> vec;
	m_rankTree.GetSomeTopKey(count, vec);

	UINT32 vecSize = vec.size();
	for (UINT32 i = 0; i < vecSize; i++)
	{
		LeagueRankInfo* rankinfo = vec[i];
		KKSG::RankData* rankdata = rankdatas->add_rankdata();
		rankinfo->RankInfoToKKSG(rankdata);
		rankdata->set_rank(i + 1);
	}
}

void LeagueRankList::RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank)
{
	const LeagueRankInfo* rankinfo = GetInfoByRank(rank);
	if (NULL != rankinfo)
	{
		rankinfo->RankInfoToKKSG(rankdata);
	}
}

void LeagueRankList::NewRankInfoToClient(KKSG::RankData* rankdata, UINT64 uid)
{
	KKSG::LeagueRankData* ldata = rankdata->mutable_leagueinfo();
	ldata->set_league_teamid(uid);
	LeagueTeam* lt = LeagueTeamMgr::Instance()->GetLeagueTeamById(uid);
	if (lt)
	{
		ldata->set_serverid(MSConfig::Instance()->GetServerID());
		ldata->set_servername(MSConfig::Instance()->GetName());
		ldata->set_teamname(lt->GetName());
		ldata->set_point(lt->GetScore());
		ldata->set_winnum(lt->GetLeagueTeamBattleRecord().GetTotalWinNum());
		ldata->set_winrate(lt->GetLeagueTeamBattleRecord().GetTotalWinRate());
	}
}

void LeagueRankList::InsertKey(LeagueRankInfo* rankinfo)
{
	if (NULL == rankinfo)
	{
		return;
	}
	m_rankTree.Insert(rankinfo, rankinfo->GetUniqueId());
}

void LeagueRankList::OnChange(UINT64 leagueTeamId, UINT32 point, float winRate, UINT32 time, UINT32 continueWin, UINT32 joinCount)
{
	LeagueRankInfo* rankinfo = NULL;
	UINT32 ret = m_rankTree.GetKeyByID(leagueTeamId, rankinfo);
	if (ret == INVALID_RANK)
	{
		LeagueRankInfo* rankinfo = new LeagueRankInfo(leagueTeamId);
		rankinfo->SetData(point, winRate, time, continueWin, joinCount);
		InsertKey(rankinfo);

		// 删掉末尾的
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
		m_rankTree.Erase(leagueTeamId);
		if(0 == point)
		{
			delete rankinfo;
			return;
		}
		rankinfo->SetData(point, winRate, time, continueWin, joinCount);
		InsertKey(rankinfo);
	}
	m_ischanged = true;
}

void LeagueRankList::GetRankInfo(std::vector<LeagueRankInfo*>& vec)
{
	m_rankTree.GetAllKey(vec);
}

void LeagueRankList::GetTopNRankInfo(UINT32 count, std::vector<LeagueRankInfo*>& vec)
{
	m_rankTree.GetSomeTopKey(count, vec);
}

void LeagueRankList::DelLeagueTeam(UINT64 id)
{
	LeagueRankInfo* rankinfo;
	UINT32 ret = m_rankTree.GetKeyByID(id, rankinfo);
	if (ret != INVALID_RANK)
	{
		m_rankTree.Erase(id);
		delete rankinfo;
	}
}

int LeagueRankList::GetUpdateInterval()
{
	return LeagueTeamRankInterval;
}

void LeagueRankList::InfoPrint()
{
	std::vector<LeagueRankInfo*> vec;
	GetRankInfo(vec);
	SSInfo << " size = " << vec.size() << END;
	for(size_t i = 0; i < vec.size(); ++i)
	{
		vec[i]->InfoPrint();
	}
}

void LeagueRankList::ASGetTopInfo(UINT32 count, std::vector<LeagueRankInfo*>& infoVec)
{
	if(count > GetSize())
	{
		count = GetSize();
	}
	m_rankTree.GetSomeTopKey(count, infoVec);
}

void LeagueRankList::ASGetTopRank(UINT32 count, std::vector<ASPair>& rankVec)
{
	SSWarn << " should not call this func" << END;
}

LastWeekLeagueRankList::LastWeekLeagueRankList(int type)
	:LeagueRankList(type)
{

}

LastWeekLeagueRankList::~LastWeekLeagueRankList()
{

}

int LastWeekLeagueRankList::GetUpdateInterval()
{
	return LastWeek_LeagueTeamRankInterval;
}

UINT32 LastWeekLeagueRankList::GetNeedRankCount()
{
	return GetGlobalConfig().ArenaStarRankLimit;
}

UINT32 LastWeekLeagueRankList::ASGetTopViewRank()
{
	return 1;
}

bool LastWeekLeagueRankList::ASUpdateLastWeekRankInfoLeague(LeagueRankList* pCurRank, UINT64 leagueTeamID)
{
	if(NULL == pCurRank)
	{
		return false;
	}

	const LeagueRankInfo* rankinfo = pCurRank->GetInfoByRank(pCurRank->GetRank(leagueTeamID));
	if(NULL == rankinfo)
	{
		SSError << "find info failed teamid = " << leagueTeamID << END;
		return false;
	}

	Clear();
    SetIsChanged(true);

	LeagueRankInfo* pNewInfo = new LeagueRankInfo(*rankinfo);
	//名人堂需要一些额外的数据
	pNewInfo->FillTeamData(pNewInfo->m_rankData);
	auto pTeam = LeagueTeamMgr::Instance()->GetLeagueTeamById(leagueTeamID);
	if(pTeam)
	{
		const auto& allMembers = pTeam->GetAllMember();
		for(size_t i = 0; i < allMembers.size(); ++i)
		{
			pNewInfo->m_roleids.push_back(allMembers[i].roleId);
		}
	}
	else
	{
		SSWarn << " find league team failed id = " << leagueTeamID << END;
	}

	InsertKey(pNewInfo);
	return true;
}

void LastWeekLeagueRankList::ASGetTopRank(UINT32 count, std::vector<ASPair>& rankVec)
{
	//直接只取第一名
	UINT32 rank = 1;
	const auto pInfo = GetInfoByRank(rank);
	if(NULL == pInfo)
	{
		SSWarn << " get rank info failed " << END;
		return;
	}
	for(auto it = pInfo->m_roleids.begin(); it != pInfo->m_roleids.end(); ++it)
	{
		rankVec.push_back(ASPair(rank, *it));
	}
}
