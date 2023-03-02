#include "pch.h"
#include "leaguebattlebase.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "sceneleague.h"
#include "timeutil.h"
#include "gamelogic/LeagueMatchParamMgr.h"
#include "leaguebattle/ptcg2m_leaguebattleupdatedatag2m.h"
#include "network/mslink.h"
#include "servermgr/servermgr.h"
#include "config.h"
#include "network/worldlink.h"
#include "leaguebattle/ptcg2m_leagueeliminationupdateg2m.h"
#include "leaguebattle/ptcg2k_leagueeliminationupdateg2k.h"

LeagueBattleTeam::LeagueBattleTeam()
:m_serverId(0)
,m_leagueTeamId(0)
,m_oldScore(0)
,m_newScore(0)
,m_deltaScore(0)
,m_oldRank(0)
,m_newRank(0)
,m_deltaRank(0)
,m_totalNum(0)
,m_winNum(0)
{

}

void LeagueBattleTeam::LoadFromKKSG(const KKSG::LeagueBattleTeamData& data)
{
	m_serverId = data.serverid();
	m_serverName = data.servername();
	m_leagueTeamId = data.league_teamid();
	m_name = data.name();
	m_oldScore = data.score();
	m_oldRank = data.rank();
	m_totalNum = data.total_num();
	m_winNum = data.total_win();

	m_newScore = m_oldScore;
	m_newRank = m_oldRank;

	for (int i = 0; i < data.members_size(); ++i)
	{
		UINT64 roleId = data.members(i).roleid();
		if (!HasMember(roleId))
		{
			m_allmembers.push_back(roleId);
		}
	}
}

void LeagueBattleTeam::FillTeamBrief(KKSG::LeagueBattleTeamBrief& data)
{
	data.set_league_teamid(m_leagueTeamId);
	data.set_serverid(m_serverId);
	data.set_name(m_name);
}

void LeagueBattleTeam::FillTeamData(KKSG::LeagueBattleTeamData& data)
{
	data.set_serverid(m_serverId);
	data.set_servername(m_serverName);
	data.set_league_teamid(m_leagueTeamId);
	data.set_name(m_name);
	data.set_score(m_oldScore);
	data.set_rank(m_oldRank);
	data.set_total_num(m_totalNum);
	data.set_total_win(m_winNum);
}

void LeagueBattleTeam::FillBattleEndDataToMS(KKSG::LeagueBattleUpdateDataG2M& data, UINT64 winLeagueId, LeagueBattleTeam& other)
{
	UINT32 now = TimeUtil::GetTime();
	data.set_league_teamid(m_leagueTeamId);
	KKSG::LeagueBattleOneRecord* record = data.mutable_record();
	record->set_other_teamid(other.m_leagueTeamId);
	record->set_other_teamname(other.m_name);
	record->set_serverid(other.m_serverId);
	record->set_servername(other.m_serverName);
	record->set_score_change(m_deltaScore);
	record->set_time(now);
	if (winLeagueId == m_leagueTeamId)
	{
		record->set_result(KKSG::PkResult_Win);
	}
	else
	{
		record->set_result(KKSG::PkResult_Lose);
	}
}

bool LeagueBattleTeam::HasMember(UINT64 roleId)
{
	for (size_t i = 0; i < m_allmembers.size(); ++i)
	{
		if (m_allmembers[i] == roleId)
		{
			return true;
		}
	}
	return false;
}

bool LeagueBattleTeam::HasBattleRole(UINT64 roleId)
{
	for (auto it = m_battleRoles.begin(); it != m_battleRoles.end(); ++it)
	{
		if (it->m_roleId == roleId)
		{
			return true;
		}
	}
	return false;
}

void LeagueBattleTeam::AddBattleRole(UINT64 roleId)
{
	if (HasBattleRole(roleId))
	{
		return;
	}
	LeagueBattleRole role;
	role.m_roleId = roleId;
	role.m_state = KKSG::LBRoleState_Waiting;
	role.m_killNum = 0;
	m_battleRoles.push_back(role);
}

LeagueBattleRole* LeagueBattleTeam::GetBattleRole(UINT64 roleId)
{
	for (auto it = m_battleRoles.begin(); it != m_battleRoles.end(); ++it)
	{
		if (it->m_roleId == roleId)
		{
			return &(*it);
		}
	}
	return NULL;
}

KKSG::LeagueBattleRoleState LeagueBattleTeam::GetBattleRoleState(UINT64 roleId)
{
	for (auto it = m_battleRoles.begin(); it != m_battleRoles.end(); ++it)
	{
		if (it->m_roleId == roleId)
		{
			return it->m_state;
		}
	}
	return KKSG::LBRoleState_None;
}

UINT64 LeagueBattleTeam::GetNextBattleRoleId()
{
	for (auto it = m_battleRoles.begin(); it != m_battleRoles.end(); ++it)
	{
		if (it->m_state != KKSG::LBRoleState_Waiting)
		{
			continue;
		}
		it->m_state = KKSG::LBRoleState_Fighting;
		return it->m_roleId;
	}
	return 0;
}

void LeagueBattleTeam::RemoveBattleRole(UINT64 roleId)
{
	for (auto it = m_battleRoles.begin(); it != m_battleRoles.end(); ++it)
	{
		if (it->m_roleId == roleId)
		{
			m_battleRoles.erase(it);
			break;
		}
	}
}

void LeagueBattleTeam::UpdateScore(bool isWin, UINT32 otherScore)
{
	int delta = otherScore - m_oldScore;

	float expect = 1/(1 + pow(10, ((float)delta/(float)400)));
	float W = isWin ? 1.0f : 0.0f;
	float K = (float)LeagueMatchParamMgr::Instance()->GetK(m_oldScore, isWin);
	m_deltaScore = (int)(K * (W - expect));

	int temp = m_oldScore + m_deltaScore;
	if (temp < 0)
	{
		temp = 0;
	}
	m_newScore = temp;

	m_totalNum++;
	if (isWin)
	{
		m_winNum++;
	}
}

void LeagueBattleRoom::LoadFromKKSG(const KKSG::LeagueBattleInfo& info)
{
	m_battleType = info.type();
	m_roomId = info.uid();
	m_teamRed.LoadFromKKSG(info.team1());
	m_teamBlue.LoadFromKKSG(info.team2());
	
	for (size_t i = 0; i < info.team1().members_size(); ++i)
	{
		const KKSG::LeagueBattleRoleBrief& brief = info.team1().members(i);
		m_briefs[brief.roleid()] = brief;
	}
	for (size_t i = 0; i < info.team2().members_size(); ++i)
	{
		const KKSG::LeagueBattleRoleBrief& brief = info.team2().members(i);
		m_briefs[brief.roleid()] = brief;
	}
}

UINT32 LeagueBattleRoom::GetGroup(UINT64 roleId)
{
	if (m_teamRed.HasMember(roleId))
	{
		return LeagueGroup_Red;
	}
	if (m_teamBlue.HasMember(roleId))
	{
		return LeagueGroup_Blue;
	}
	return LeagueGroup_None;
}

bool LeagueBattleRoom::IsRoleLoad(UINT64 roleId)
{
	for (size_t i = 0; i < m_loadedRole.size(); ++i)
	{
		if (roleId == m_loadedRole[i])
		{
			return true;
		}
	}
	return false;
}

void LeagueBattleRoom::AddRoleLoad(UINT64 roleId)
{
	if (IsRoleLoad(roleId))
	{
		return;
	}
	m_loadedRole.push_back(roleId);
}

void LeagueBattleRoom::RemoveRoleLoad(UINT64 roleId)
{
	for (auto it = m_loadedRole.begin(); it != m_loadedRole.end(); ++it)	
	{
		if (*it == roleId)
		{
			m_loadedRole.erase(it);
			break;
		}
	}
}

LeagueBattleTeam* LeagueBattleRoom::GetBattleTeam(UINT64 roleId)
{
	if (m_teamRed.HasMember(roleId))
	{
		return &m_teamRed;
	}
	if (m_teamBlue.HasMember(roleId))
	{
		return &m_teamBlue;
	}
	return NULL;
}

LeagueBattleTeam* LeagueBattleRoom::GetBattleTeamById(UINT64 leagueTeamId)
{
	if (m_teamRed.m_leagueTeamId == leagueTeamId)
	{
		return &m_teamRed;
	}
	if (m_teamBlue.m_leagueTeamId == leagueTeamId)
	{
		return &m_teamBlue;
	}
	return NULL;
}

LeagueBattleRole* LeagueBattleRoom::GetBattleRole(UINT64 roleId)
{
	LeagueBattleRole* role = m_teamRed.GetBattleRole(roleId);
	if (role)
	{
		return role;
	}
	role = m_teamBlue.GetBattleRole(roleId);
	return role;
}

void LeagueBattleRoom::SetRoleState(UINT64 roleId, KKSG::LeagueBattleRoleState state)
{
	LeagueBattleRole* role = m_teamRed.GetBattleRole(roleId);
	if (role)
	{
		role->m_state = state;
		return;
	}
	role = m_teamBlue.GetBattleRole(roleId);
	if (role)
	{
		role->m_state = state;
		return;
	}
}

void LeagueBattleRoom::FillTeamBrief(UINT64 roleId, KKSG::LeagueBattleTeamBrief& team)
{
	if (m_teamRed.HasMember(roleId))
	{
		m_teamRed.FillTeamBrief(team);
	}
	else if (m_teamBlue.HasMember(roleId))
	{
		m_teamBlue.FillTeamBrief(team);
	}
}

void LeagueBattleRoom::FillRoleBrief(UINT64 roleId, KKSG::LeagueBattleRoleBrief& info)
{
	auto it = m_briefs.find(roleId);
	if (it == m_briefs.end())
	{
		return;
	}
	info.CopyFrom(it->second);
}

void LeagueBattleRoom::FillTeamData(UINT64 roleId, KKSG::LeagueBattleTeamData& data)
{
	if (m_teamRed.HasMember(roleId))
	{
		FillTeamData(&m_teamRed, data);
	}
	else if (m_teamBlue.HasMember(roleId))
	{
		FillTeamData(&m_teamBlue, data);
	}
}

void LeagueBattleRoom::FillTeamData(LeagueBattleTeam* team, KKSG::LeagueBattleTeamData& data)
{
	if (team == NULL)
	{
		return;
	}
	team->FillTeamData(data);
	for (auto it = team->m_allmembers.begin(); it != team->m_allmembers.end(); ++it)
	{
		auto it2 = m_briefs.find(*it);	
		if (it2 == m_briefs.end())
		{
			continue;
		}
		KKSG::LeagueBattleRoleBrief* brief = data.add_members();
		brief->CopyFrom(it2->second);
	}
}

void LeagueBattleRoom::FillRedTeamData(KKSG::LeagueBattleTeamData& data)
{
	FillTeamData(&m_teamRed, data);
}

void LeagueBattleRoom::FillBlueTeamData(KKSG::LeagueBattleTeamData& data)
{
	FillTeamData(&m_teamBlue, data);
}

void LeagueBattleRoom::FillTeamUpData(LeagueBattleTeam* team, KKSG::LeagueBattleOneTeam& data)
{
	data.set_league_teamid(team->m_leagueTeamId);
	data.set_name(team->m_name);
	data.set_score(team->m_oldScore);
	data.set_serverid(team->m_serverId);

	UINT32 index = 0;
	for (auto it = team->m_battleRoles.begin(); it != team->m_battleRoles.end(); ++it)
	{
		index++;
		auto it2 = m_briefs.find(it->m_roleId);
		if (it2 == m_briefs.end())
		{
			continue;
		}
		KKSG::LeagueBattleOneRole* one = data.add_members();
		one->mutable_basedata()->CopyFrom(it2->second);
		one->set_state(it->m_state);
		one->set_index(index);
	}
}

void LeagueBattleRoom::FillRedTeamUpData(KKSG::LeagueBattleOneTeam& data)
{
	FillTeamUpData(&m_teamRed, data);
}

void LeagueBattleRoom::FillBlueTeamUpData(KKSG::LeagueBattleOneTeam& data)
{
	FillTeamUpData(&m_teamBlue, data);
}

void LeagueBattleRoom::FillTeamResultDataById(UINT64 leagueId, KKSG::LeagueBattleResultTeam& data)
{
	if (m_teamRed.m_leagueTeamId == leagueId)
	{
		FillTeamResultData(&m_teamRed, data);
	}
	else if (m_teamBlue.m_leagueTeamId == leagueId)
	{
		FillTeamResultData(&m_teamBlue, data);
	}
}

void LeagueBattleRoom::FillTeamResultData(LeagueBattleTeam* team, KKSG::LeagueBattleResultTeam& data)
{
	data.set_league_teamid(team->m_leagueTeamId);
	data.set_name(team->m_name);
	data.set_serverid(team->m_serverId);
	data.set_servername(team->m_serverName);
	data.set_rank(team->m_newRank);
	data.set_score(team->m_newScore);
	data.set_score_change(team->m_deltaScore);
	data.set_rank_change(team->m_deltaRank);

	for (size_t i = 0; i < team->m_allmembers.size(); ++i)
	{
		UINT64 roleId = team->m_allmembers[i];
		auto it = m_briefs.find(roleId);
		if (it == m_briefs.end())
		{
			continue;
		}
		KKSG::LeagueBattleResultRole* mem = data.add_members();
		mem->mutable_basedata()->CopyFrom(it->second);

		LeagueBattleRole* role = team->GetBattleRole(roleId);
		if (role)
		{
			mem->set_killnum(role->m_killNum);
			mem->set_is_up(true);
		}
		else
		{
			mem->set_killnum(0);
			mem->set_is_up(false);
		}
	}
}

void LeagueBattleRoom::CheckSwitchWatcher(SceneLeague* battle)
{
	UINT32 now = TimeUtil::GetTime();
	if (now == m_lastCheckTime)
	{
		return;
	}
	m_lastCheckTime = now;

	std::vector<UINT64> roleIds;
	for (auto it = m_loadedRole.begin(); it != m_loadedRole.end(); )
	{
		UINT64 roleId = *it;
		if (battle->IsFightRole(roleId))
		{
			it++;
			continue;
		}
		else
		{
			roleIds.push_back(roleId);
			it = m_loadedRole.erase(it);
		}
	}

	for(size_t i = 0; i < roleIds.size(); ++i)
	{
		Role* pRole = RoleManager::Instance()->FindByRoleID(roleIds[i]);
		if(pRole)
		{
			AddChangingWatch(pRole->GetID());
			battle->OnBeforeChangeWather(pRole);
			battle->ChangeWatcherState(pRole, true);
		}
	}
}

void LeagueBattleRoom::UpdateScore(UINT64 winId)
{
	// 积分赛才有分数变化
	if (KKSG::LeagueBattleType_RacePoint != m_battleType && KKSG::LeagueBattleType_CrossRacePoint != m_battleType)
	{
		return;
	}
	if (winId == 0)
	{
		return;
	}
	if (m_teamRed.m_leagueTeamId == winId)
	{
		m_teamRed.UpdateScore(true, m_teamBlue.m_oldScore);	
		m_teamBlue.UpdateScore(false, m_teamRed.m_oldScore);
	}
	if (m_teamBlue.m_leagueTeamId == winId)
	{
		m_teamRed.UpdateScore(false, m_teamBlue.m_oldScore);	
		m_teamBlue.UpdateScore(true, m_teamRed.m_oldScore);
	}
}

void LeagueBattleRoom::NotifyReadyEndToMs(UINT32 sceneId)
{
	PtcG2M_LeagueBattleUpdateDataG2M ntfRed;
	ntfRed.m_Data.set_sceneid(sceneId);
	ntfRed.m_Data.set_is_ready_end(true);
	ntfRed.m_Data.set_league_teamid(m_teamRed.m_leagueTeamId);

	PtcG2M_LeagueBattleUpdateDataG2M ntfBlue;
	ntfBlue.m_Data.set_sceneid(sceneId);
	ntfBlue.m_Data.set_is_ready_end(true);
	ntfBlue.m_Data.set_league_teamid(m_teamBlue.m_leagueTeamId);

	// 非跨服
	if (!GSConfig::Instance()->IsCrossGS())
	{
		MSLink::Instance()->SendTo(ntfRed);
		MSLink::Instance()->SendTo(ntfBlue);
	}
	// 跨服
	else
	{
		ServerMgr::Instance()->SendTo(m_teamRed.m_serverId, ntfRed);
		ServerMgr::Instance()->SendTo(m_teamBlue.m_serverId, ntfBlue);
	}
}

void LeagueBattleRoom::NotifyEndToMs(UINT32 sceneId, UINT64 winTeamId)
{
	// 1.通知每个战队的MS结果记录 2.通知MS或者World，淘汰赛结果

	// 通知各自MS战斗结束，更新对战记录，不更新分数(分数在updaterank里更新)
	// TeamRed
	PtcG2M_LeagueBattleUpdateDataG2M ntfRed;
	ntfRed.m_Data.set_sceneid(sceneId);
	ntfRed.m_Data.set_is_battle_end(true);
	ntfRed.m_Data.mutable_record()->set_type(m_battleType);
	m_teamRed.FillBattleEndDataToMS(ntfRed.m_Data, winTeamId, m_teamBlue);

	// TeamBlue
	PtcG2M_LeagueBattleUpdateDataG2M ntfBlue;
	ntfBlue.m_Data.set_sceneid(sceneId);
	ntfBlue.m_Data.set_is_battle_end(true);
	ntfBlue.m_Data.mutable_record()->set_type(m_battleType);
	m_teamBlue.FillBattleEndDataToMS(ntfBlue.m_Data, winTeamId, m_teamRed);

	if (!GSConfig::Instance()->IsCrossGS())
	{
		MSLink::Instance()->SendTo(ntfRed);
		MSLink::Instance()->SendTo(ntfBlue);
	}
	else
	{
		ServerMgr::Instance()->SendTo(m_teamRed.m_serverId, ntfRed);
		ServerMgr::Instance()->SendTo(m_teamBlue.m_serverId, ntfBlue);
	}


	// 本服淘汰赛结果
	if (m_battleType == KKSG::LeagueBattleType_Eliminate)
	{
		PtcG2M_LeagueEliminationUpdateG2M ntf;
		ntf.m_Data.set_roomid(m_roomId);
		ntf.m_Data.set_sceneid(sceneId);
		ntf.m_Data.set_win_leagueid(winTeamId);
		MSLink::Instance()->SendTo(ntf);
	}
	// 跨服淘汰赛结果
	else if (m_battleType == KKSG::LeagueBattleType_CrossEliminate)
	{
		PtcG2K_LeagueEliminationUpdateG2K ntf;
		ntf.m_Data.set_roomid(m_roomId);
		ntf.m_Data.set_sceneid(sceneId);
		ntf.m_Data.set_win_leagueid(winTeamId);
		WorldLink::Instance()->SendTo(ntf);
	}
}

void LeagueBattleRoom::NotifyLiveInfoToMsOrWorld(UINT32 sceneId, UINT32 liveId)
{
	if (m_battleType == KKSG::LeagueBattleType_Eliminate)
	{
		PtcG2M_LeagueEliminationUpdateG2M ntf;
		ntf.m_Data.set_roomid(m_roomId);
		ntf.m_Data.set_sceneid(sceneId);
		ntf.m_Data.set_liveid(liveId);
		MSLink::Instance()->SendTo(ntf);
	}
	else if (m_battleType == KKSG::LeagueBattleType_CrossEliminate)
	{
		PtcG2K_LeagueEliminationUpdateG2K ntf;
		ntf.m_Data.set_roomid(m_roomId);
		ntf.m_Data.set_sceneid(sceneId);
		ntf.m_Data.set_liveid(liveId);
		WorldLink::Instance()->SendTo(ntf);
	}
}
