#include "pch.h"
#include "leaguebattlematchmgr.h"
#include "leagueteam/leagueteammgr.h"
#include "team/team.h"
#include "league/ptcm2c_leaguebattlestartmatchntf.h"
#include "league/ptcm2c_leaguebattlestopmatchntf.h"
#include "league/ptcm2c_leaguebattlematchtimeoutntf.h"
#include "team/teammgr.h"
#include "scene/scenecreate.h"
#include "table/globalconfig.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "scene/sceneswitch.h"
#include "scene/scene.h"
#include "team/teammember.h"
#include "tablemgr/leaguematchtablemgr.h"
#include "util/gametime.h"
#include "timeutil.h"
#include "pb/project.pb.h"
#include "team/teamforceop.h"
#include "other/matchhandler.h"
#include "leaguebattle/leagueseasonmgr.h"
#include "league/ptcm2k_leaguematchworldntf.h"
#include "config.h"
#include "network/worldlink.h"
#include "leagueteam/crossleagueteammgr.h"
#include "cross/crosszonestatus.h"

LeagueBattleMatchMgr::LeagueBattleMatchMgr()
{
}

LeagueBattleMatchMgr::~LeagueBattleMatchMgr()
{
}

bool LeagueBattleMatchMgr::Init()
{
	CTimerMgr::Instance()->SetTimer(this, 0, 3000, -1, __FILE__, __LINE__);
	return true;
}

void LeagueBattleMatchMgr::Uninit()
{
}

void LeagueBattleMatchMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	// 跨服积分赛阶段这里不匹配
	KKSG::LeagueBattleTimeState state = LeagueSeasonMgr::Instance()->GetTodayState();
	if (state == KKSG::LBTS_CrossPointRace)
	{
		return;
	}

	DoMatch();
	DoTimeOut();
	DoCheckDelay();
}

int LeagueBattleMatchMgr::StartMatch(UINT64 leagueTeamId, Team* team)
{
	LeagueTeam* pLTeam = LeagueTeamMgr::Instance()->GetLeagueTeamById(leagueTeamId);
	if (pLTeam == NULL)
	{
		return KKSG::ERR_LEAGUE_TEAM_NOT_EXIST;
	}
	KKSG::LeagueTeamState state = pLTeam->GetState();
	if (state == KKSG::LeagueTeamState_Match)
	{
		return KKSG::ERR_LEAGUE_TEAM_IN_MATCH;
	}
	if (state == KKSG::LeagueTeamState_Battle)
	{
		return KKSG::ERR_LEAGUE_TEAM_IN_BATTLE;
	}
	if (state != KKSG::LeagueTeamState_Idle)
	{
		return KKSG::ERR_FAILED;
	}

	if (IsInMatch(leagueTeamId))
	{
		return KKSG::ERR_LEAGUE_TEAM_IN_MATCH;
	}

	bool isCrossMatch = LeagueSeasonMgr::Instance()->IsCrossPointRaceDay();
	if (isCrossMatch)
	{
		// 检查是否有跨服匹配资格
		if (CrossLeagueTeamMgr::Instance()->GetTeam(leagueTeamId) == NULL)
		{
			return KKSG::ERR_LEAGUE_TEAM_CANNOT_CROSS;
		}
		// 跨服区不可用
		if (!CrossZoneStatus::Instance()->IsCrossZoneUsable())
		{
			return KKSG::ERR_CROSS_ZONE_UNUSABLE;
		}
	}

	LeagueMatchUnit unit;
	unit.leagueTeamId = leagueTeamId;
	unit.teamId = team->GetID();
	unit.score = pLTeam->GetScore();
	unit.startTime = TimeUtil::GetTime();

	int ret = Start(unit);
	if (ret != KKSG::ERR_SUCCESS)
	{
		return ret;
	}

	pLTeam->SetState(KKSG::LeagueTeamState_Match);
	pLTeam->SetMatchTeamId(team->GetID());

	// notify Wolrd 匹配
	if (isCrossMatch)
	{
		PtcM2K_LeagueMatchWorldNtf ntf;
		ntf.m_Data.set_type(KKSG::MTWT_ADD);
		ntf.m_Data.set_serverid(MSConfig::Instance()->GetServerID());
		ntf.m_Data.set_ltid(leagueTeamId);
		ntf.m_Data.set_teamid(team->GetID());
		pLTeam->FillLeagueTeamBattleData(*ntf.m_Data.mutable_ltdata());
		WorldLink::Instance()->SendTo(ntf);
	}

	NotifyStartMatch(team);
	LogInfo("start match leagueteam[%llu]", leagueTeamId);
	return KKSG::ERR_SUCCESS;
}

int LeagueBattleMatchMgr::CancelMatch(Team* team)
{
	if (team == NULL)
	{
		return KKSG::ERR_FAILED;
	}
	UINT64 roleId = team->GetLeader();
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleId);
	if (pRole == NULL)
	{
		return KKSG::ERR_FAILED;
	}
	LeagueTeam* leagueTeam = LeagueTeamMgr::Instance()->GetLeagueTeamByRoleId(roleId);
	if (leagueTeam == NULL)
	{
		return KKSG::ERR_FAILED;	
	}

	LeagueMatchUnit* unit = GetMatchUnit(leagueTeam->GetId());
	if (unit == NULL)
	{
		return KKSG::ERR_LEAGUE_TEAM_NOT_IN_MATCH;
	}

	if (unit->teamId != team->GetID())
	{
		return KKSG::ERR_FAILED;	
	}

	// 通知World取消匹配
	if (LeagueSeasonMgr::Instance()->IsCrossPointRaceDay())
	{
		PtcM2K_LeagueMatchWorldNtf ntf;
		ntf.m_Data.set_type(KKSG::MTWT_DEL);
		ntf.m_Data.set_serverid(MSConfig::Instance()->GetServerID());
		ntf.m_Data.set_ltid(unit->leagueTeamId);
		ntf.m_Data.set_teamid(unit->teamId);
		WorldLink::Instance()->SendTo(ntf);
	}

	NotifyStopMatch(team);
	leagueTeam->SetState(KKSG::LeagueTeamState_Idle);
	LogInfo("cancel match leagueteam[%llu]", leagueTeam->GetId());

	Cancel(leagueTeam->GetId());
	return KKSG::ERR_SUCCESS;
}

void LeagueBattleMatchMgr::ForceCancelMatch(UINT64 leagueTeamId)
{
	LeagueMatchUnit* unit = GetMatchUnit(leagueTeamId);
	if (unit == NULL)
	{
		return;
	}
	Team* team = TeamMgr::Instance()->FindTeam(unit->teamId);
	if (team == NULL)
	{
		return;
	}

	HMatchHandler handler;
	handler.Data().leagueTeamId = leagueTeamId;
	handler.Stop(team, KKSG::KMT_LEAGUE);
	// 最终会调用到CancelMatch
}

void LeagueBattleMatchMgr::OnCrossZoneUnusable()
{
	if (!LeagueSeasonMgr::Instance()->IsCrossPointRaceDay())
	{
		return;
	}

	std::vector<UINT64> ltIds;
	GetMatchLeagueIds(ltIds);
	for (auto it = ltIds.begin(); it != ltIds.end(); ++it)
	{
		ForceCancelMatch(*it);
	}
}

void LeagueBattleMatchMgr::OnMatchSuccess(LeagueMatchUnit& a, LeagueMatchUnit& b)
{
	Team* pTeamA = TeamMgr::Instance()->FindTeam(a.teamId);
	Team* pTeamB = TeamMgr::Instance()->FindTeam(b.teamId);
	NotifyStopMatch(pTeamA);
	NotifyStopMatch(pTeamB);

	if (pTeamA == NULL || pTeamB == NULL)
	{
		LogWarn("Match Success, but team null, leagueteam[%llu] team[%u] leagueteam[%llu] team[%u]",
				a.leagueTeamId, a.teamId, b.leagueTeamId, b.teamId);
		return;
	}
	LeagueTeam* leagueTeamA = LeagueTeamMgr::Instance()->GetLeagueTeamById(a.leagueTeamId);
	LeagueTeam* leagueTeamB = LeagueTeamMgr::Instance()->GetLeagueTeamById(b.leagueTeamId);
	if (leagueTeamA == NULL || leagueTeamB == NULL)
	{
		LogWarn("Match Success, but leagueteam null, leagueteam[%llu] team[%u] leagueteam[%llu] team[%u]",
				a.leagueTeamId, a.teamId, b.leagueTeamId, b.teamId);
		return;
	}

	LogInfo("Match Success, leagueteam[%llu] team[%u] leagueteam[%llu] team[%u]",
			a.leagueTeamId, a.teamId, b.leagueTeamId, b.teamId);

	// 设置队伍状态
	TeamForceOp op;
	op.SetInBattleState(pTeamA->GetID());
	op.SetInBattleState(pTeamB->GetID());
	HMatchHandler handler;
	handler.Done(pTeamA, KKSG::KMT_LEAGUE);
	handler.Done(pTeamB, KKSG::KMT_LEAGUE);

	// 存一下匹配信息
	UINT32 id = NewUid();
	FightGroup& group = m_matchedGroup[id];
	group.teamA = a;
	group.teamB = b;

	// 建立战场
	KKSG::CreateBattleParam param;
	KKSG::LeagueBattleInfo& info = *param.mutable_lbinfo();
	info.set_uid(id);
	info.set_type(KKSG::LeagueBattleType_RacePoint);
	leagueTeamA->FillLeagueTeamBattleData(*info.mutable_team1());
	leagueTeamB->FillLeagueTeamBattleData(*info.mutable_team2());

	CSceneCreator::Instance()->CreateBattleScene(GetGlobalConfig().LeagueBattleSceneId, this, param);
}

void LeagueBattleMatchMgr::OnMatchTimeOut(LeagueMatchUnit& unit)
{
	Team* team = TeamMgr::Instance()->FindTeam(unit.teamId);
	HMatchHandler handler;
	handler.Done(team, KKSG::KMT_LEAGUE);
	NotifyMatchTimeout(team);
	LeagueTeam* lt = LeagueTeamMgr::Instance()->GetLeagueTeamById(unit.leagueTeamId);
	if (lt)
	{
		lt->SetState(KKSG::LeagueTeamState_Idle);
	}
}

void LeagueBattleMatchMgr::TimeOutFromWorld(UINT64 ltId, UINT32 teamId)
{
	LeagueMatchUnit* unit = GetMatchUnit(ltId);
	if (unit == NULL)
	{
		return;
	}
	OnMatchTimeOut(*unit);

	Cancel(ltId);
}

void LeagueBattleMatchMgr::MatchSuccessFromWorld(UINT64 ltId, UINT32 teamId, UINT32 gsLine, UINT32 sceneId)
{
	Team* pTeam = TeamMgr::Instance()->FindTeam(teamId);
	NotifyStopMatch(pTeam);
	Cancel(ltId);

	LeagueTeam* pLTeam = LeagueTeamMgr::Instance()->GetLeagueTeamById(ltId);

	if (pLTeam == NULL || pTeam == NULL)
	{
		return;
	}

	LogInfo("Match Success From World, ltid[%llu] teamid[%u]", ltId, teamId);

	// 设置队伍状态
	TeamForceOp op;
	op.SetInBattleState(pTeam->GetID());
	HMatchHandler handler;
	handler.Done(pTeam, KKSG::KMT_LEAGUE);

	pLTeam->SetState(KKSG::LeagueTeamState_Battle);
	pLTeam->SetBattleStart(sceneId, gsLine, true);

	// 切场景
	KKSG::SceneSwitchData switchData;
	const std::list<CTeamMember*>& members = pTeam->GetMember();
	for (auto it = members.begin(); it != members.end(); ++it)
	{
		CRole* role = CRoleManager::Instance()->GetByRoleID((*it)->m_qwRoleID);
		if (role) CSceneSwitch::EnterScene(role, sceneId, switchData, gsLine, true, GetGlobalConfig().LeagueBattleSceneId);
	}
}

void LeagueBattleMatchMgr::OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res)
{
	auto it = m_matchedGroup.find(roParam.lbinfo().uid());
	if (it == m_matchedGroup.end())
	{
		LogError("No Match Id[%u]", roParam.lbinfo().uid());
		return;
	}
	FightGroup& group = it->second;

	LeagueTeam* leagueTeamA = LeagueTeamMgr::Instance()->GetLeagueTeamById(group.teamA.leagueTeamId);
	LeagueTeam* leagueTeamB = LeagueTeamMgr::Instance()->GetLeagueTeamById(group.teamB.leagueTeamId);
	if (leagueTeamA == NULL || leagueTeamB == NULL || scene == NULL)
	{
		LogError("Failed, leagueteam [%llu] leagueteam [%llu]", group.teamA.leagueTeamId, group.teamB.leagueTeamId);
		if (leagueTeamA) leagueTeamA->SetState(KKSG::LeagueTeamState_Idle);
		if (leagueTeamB) leagueTeamB->SetState(KKSG::LeagueTeamState_Idle);
		m_matchedGroup.erase(it);
		return;
	}

	LogInfo("Create Battle Success, leagueteam[%llu], leagueteam[%llu], sceneid[%u]",
		group.teamA.leagueTeamId, group.teamB.leagueTeamId, scene->GetID());

	leagueTeamA->SetState(KKSG::LeagueTeamState_Battle);
	leagueTeamB->SetState(KKSG::LeagueTeamState_Battle);
	leagueTeamA->SetBattleStart(scene->GetID(), scene->GetLine(), false);
	leagueTeamB->SetBattleStart(scene->GetID(), scene->GetLine(), false);

	Team* teamA = TeamMgr::Instance()->FindTeam(group.teamA.teamId);
	Team* teamB = TeamMgr::Instance()->FindTeam(group.teamB.teamId);

	// 切场景
	KKSG::SceneSwitchData switchData;
	if (teamA)
	{
		const std::list<CTeamMember*>& members = teamA->GetMember();
		for (auto it = members.begin(); it != members.end(); ++it)
		{
			CRole* role = CRoleManager::Instance()->GetByRoleID((*it)->m_qwRoleID);
			if (role != NULL) CSceneSwitch::EnterScene(role, scene->GetID(), switchData);
		}
	}
	if (teamB)
	{
		const std::list<CTeamMember*>& members = teamB->GetMember();
		for (auto it = members.begin(); it != members.end(); ++it)
		{
			CRole* role = CRoleManager::Instance()->GetByRoleID((*it)->m_qwRoleID);
			if (role != NULL) CSceneSwitch::EnterScene(role, scene->GetID(), switchData);
		}
	}

	m_matchedGroup.erase(it);
}

void LeagueBattleMatchMgr::NotifyStartMatch(Team* team)
{
	if (team == NULL)
	{
		return;
	}
	PtcM2C_LeagueBattleStartMatchNtf ntf;
	team->BroadCast(ntf);
}

void LeagueBattleMatchMgr::NotifyStopMatch(Team* team)
{
	if (team == NULL)
	{
		return;
	}
	PtcM2C_LeagueBattleStopMatchNtf ntf;
	team->BroadCast(ntf);
}

void LeagueBattleMatchMgr::NotifyMatchTimeout(Team* team)
{
	if (team == NULL)
	{
		return;
	}
	PtcM2C_LeagueBattleMatchTimeoutNtf ntf;
	team->BroadCast(ntf);
}

