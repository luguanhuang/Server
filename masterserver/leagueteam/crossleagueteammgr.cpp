#include "pch.h"
#include "crossleagueteammgr.h"
#include "timeutil.h"
#include "util.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/crossleagueranklist.h"
#include "other/serverinfomgr.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "role/crossrolesummarymgr.h"
#include "leagueteammgr.h"
#include "common/ptcm2k_reportrolesummary2world.h"
#include "network/worldlink.h"
#include "config.h"

CrossLeagueTeamMgr::CrossLeagueTeamMgr()
{

}

CrossLeagueTeamMgr::~CrossLeagueTeamMgr()
{

}

bool CrossLeagueTeamMgr::Init()
{
	CTimerMgr::Instance()->SetTimer(this, 0, 5 * 60 * 1000, -1, __FILE__, __LINE__);	
	return true;
}

void CrossLeagueTeamMgr::Uninit()
{

}

void CrossLeagueTeamMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	// 频率一定要控制, 否则world会爆炸
	if (m_changedRoleIds.empty())
	{
		return;
	}

	PtcM2K_ReportRoleSummary2World ntf;
	UINT32 i = 0;
	while (m_changedRoleIds.empty() == false && i < 5)
	{
		UINT64 roleId = m_changedRoleIds.pop_front();
		CRoleSummary* su = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);
		if (su == NULL)
		{
			continue;
		}
		ntf.m_Data.add_rsus()->CopyFrom(su->GetData());
		i++;
	}
	WorldLink::Instance()->SendTo(ntf);
}

void CrossLeagueTeamMgr::UpdateTeam(const KKSG::CrossLeagueTeamData& data)
{
	auto it = m_teams.find(data.id());
	if (it != m_teams.end())
	{
		it->second.LoadFromData(data);
		UpdateTeamRank(it->second);
	}
	else
	{
		CrossLeagueTeam& team = m_teams[data.id()];
		team.LoadFromData(data);
		UpdateTeamRank(team);

		for (auto it = team.m_memberIds.begin(); it != team.m_memberIds.end(); ++it)
		{
			SetRoleTeamId(*it, team.m_id);
		}
	}

	// 是本服的战队, 把跨服的数据同步到本服战队身上, 方便使用
	LeagueTeam* pLTeam = LeagueTeamMgr::Instance()->GetLeagueTeamById(data.id());
	if (pLTeam)
	{
		pLTeam->SyncCrossData(data);
	}
}

void CrossLeagueTeamMgr::UpdateTeamRank(CrossLeagueTeam& team)
{
	// 没打过的不上榜
	if (team.m_totalNum == 0)
	{
		return;
	}

	CrossLeagueRankList* ranklist = CRankListMgr::Instance()->GetCrossLeagueRankList();
	ranklist->OnChange(team.m_id, team.m_score, team.GetWinRate(), team.m_time);
}

bool CrossLeagueTeamMgr::HasTeam(UINT64 id)
{
	return m_teams.find(id) != m_teams.end();
}

CrossLeagueTeam* CrossLeagueTeamMgr::GetTeam(UINT64 id)
{
	auto it = m_teams.find(id);
	if (it == m_teams.end())
	{
		return NULL;
	}
	return &it->second;
}

CrossLeagueTeam* CrossLeagueTeamMgr::GetTeamByRoleId(UINT64 roleId)
{
	UINT64 teamId = GetRoleTeamId(roleId);
	if (teamId == 0)
	{
		return NULL;
	}
	return GetTeam(teamId);
}

void CrossLeagueTeamMgr::SetRoleTeamId(UINT64 roleId, UINT64 teamId)
{
	m_roleId2leagueTeamId[roleId] = teamId;
}

void CrossLeagueTeamMgr::RemoveRoleTeamId(UINT64 roleId)
{
	m_roleId2leagueTeamId.erase(roleId);
}

UINT64 CrossLeagueTeamMgr::GetRoleTeamId(UINT64 roleId)
{
	auto it = m_roleId2leagueTeamId.find(roleId);
	if (it == m_roleId2leagueTeamId.end())
	{
		return 0;
	}
	return it->second;
}

void CrossLeagueTeamMgr::FillLeagueTeamDetail(CrossLeagueTeam* pLTeam, KKSG::LeagueTeamDetail& data)
{
	if (pLTeam == NULL)
	{
		return;
	}
	data.set_league_teamid(pLTeam->m_id);
	data.set_teamname(pLTeam->m_name);
	data.set_serverid(pLTeam->m_serverId);
	data.set_servername(ZServerInfoMgr::Instance()->GetServerName(pLTeam->m_serverId));
	data.set_score(pLTeam->m_score);

	for (auto it = pLTeam->m_memberIds.begin(); it != pLTeam->m_memberIds.end(); ++it)
	{
		CRoleSummary* su = CrossRoleSummaryMgr::Instance()->GetRoleSummary(*it);
		if (su == NULL)
		{
			continue;
		}
		FillLeagueMemberDetail(su, *data.add_members());
	}
}

void CrossLeagueTeamMgr::FillLeagueMemberDetail(CRoleSummary* su, KKSG::LeagueTeamMemberDetail& data)
{
	if (su == NULL)
	{
		return;
	}
	su->FillRoleOutLookBrief(*data.mutable_brief());
	data.set_pkpoint(su->GetPkPoint());
}

void CrossLeagueTeamMgr::OnSeasonEnd()
{
	m_teams.clear();
	m_roleId2leagueTeamId.clear();
}

void CrossLeagueTeamMgr::OnRoleSummaryChanged(UINT64 roleId)
{
	if (!IsCrossLeagueMember(roleId))
	{
		return;
	}
	m_changedRoleIds.push_back(roleId);
}

bool CrossLeagueTeamMgr::IsCrossLeagueMember(UINT64 roleId)
{
	UINT64 teamId = GetRoleTeamId(roleId);
	return teamId == 0 ? false : true;
}

void CrossLeagueTeamMgr::BroadAllSelfServerMember(const CProtocol& ptc)
{
	for (auto it = m_teams.begin(); it != m_teams.end(); ++it)
	{
		if (it->second.m_serverId != MSConfig::Instance()->GetServerID())
		{
			continue;
		}
		LeagueTeam* pLTeam = LeagueTeamMgr::Instance()->GetLeagueTeamById(it->second.m_id);
		if (pLTeam == NULL)
		{
			continue;
		}
		pLTeam->BroadCast(ptc);
	}
}
