#include "pch.h"
#include "skycraftmatchmgr.h"
#include "timeutil.h"
#include "skyteam/skyteam.h"
#include "skyteam/skyteammgr.h"
#include "team/teammgr.h"
#include "team/team.h"
#include "team/teamforceop.h"
#include "team/teammember.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "other/matchhandler.h"
#include "skycraft/ptcm2c_skycraftmatchntf.h"
#include "scene/sceneswitch.h"
#include "scene/scene.h"
#include "table/globalconfig.h"
#include "tablemgr/skycrafttablemgr.h"

bool SkyCraftMatchMgr::Init()
{
	CTimerMgr::Instance()->SetTimer(this, 0, 3000, -1, __FILE__, __LINE__);
	return true;
}

void SkyCraftMatchMgr::Uninit()
{
}

void SkyCraftMatchMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	// TODO 可以只在特定日期匹配
	DoMatch();
	DoTimeOut();
	DoCheckDelay();
}

int SkyCraftMatchMgr::StartMatch(UINT64 stId, Team* team)
{
	SkyTeam* pSTeam = SkyTeamMgr::Instance()->GetSkyTeamById(stId);
	if (pSTeam == NULL)
	{
		return KKSG::ERR_SKY_HAS_NO_TEAM;
	}
	if (pSTeam->GetTeamRecord().GetTodayNum() >= GetGlobalConfig().SkyCraftDailyNum)
	{
		return KKSG::ERR_SKY_TEAM_NO_DAILY_NUM;
	}

	KKSG::SkyTeamState state = pSTeam->GetState();
	if (state == KKSG::SkyTeamState_Match)
	{
		return KKSG::ERR_SKY_TEAM_IN_MATCH;
	}
	if (state == KKSG::SkyTeamState_Battle)
	{
		return KKSG::ERR_SKY_TEAM_IN_BATTLE;
	}
	if (state != KKSG::SkyTeamState_Idle)
	{
		return KKSG::ERR_FAILED;
	}

	if (IsInMatch(stId))
	{
		return KKSG::ERR_SKY_TEAM_IN_MATCH;
	}

	SkyMatchUnit unit;
	unit.stId = stId;
	unit.teamId = team->GetID();
	unit.score = pSTeam->GetScore();
	unit.startTime = TimeUtil::GetTime();

	int ret = Start(unit);
	if (ret != KKSG::ERR_SUCCESS)
	{
		return ret;
	}

	pSTeam->SetState(KKSG::SkyTeamState_Match);
	pSTeam->SetMatchTeamId(team->GetID());

	NotifyStartMatch(team);
	LogInfo("start match skyteam[%llu]", stId);
	return KKSG::ERR_SUCCESS;
}

int SkyCraftMatchMgr::CancelMatch(Team* team)
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
	SkyTeam* pSTeam = SkyTeamMgr::Instance()->GetSkyTeamByRoleId(roleId);
	if (pSTeam == NULL)
	{
		return KKSG::ERR_FAILED;
	}
	SkyMatchUnit* unit = GetMatchUnit(pSTeam->GetId());
	// 客户端容错， 没在匹配中也允许取消
	if (unit == NULL)
	{
		NotifyStopMatch(team);
		pSTeam->SetState(KKSG::SkyTeamState_Idle);
		return KKSG::ERR_SUCCESS;
	}

	if (unit->teamId != team->GetID())
	{
		return KKSG::ERR_FAILED;	
	}
	NotifyStopMatch(team);
	pSTeam->SetState(KKSG::SkyTeamState_Idle);

	LogInfo("cancel match skyteam[%llu]", pSTeam->GetId());

	Cancel(pSTeam->GetId());
	return KKSG::ERR_SUCCESS;
}

void SkyCraftMatchMgr::ForceCancelMatch(UINT64 stId)
{
	SkyMatchUnit* unit = GetMatchUnit(stId);
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
	handler.Data().stId = stId;
	handler.Stop(team, KKSG::KMT_SKYCRAFT);
	// 最终会调用到CancelMatch
}

void SkyCraftMatchMgr::OnMatchSuccess(SkyMatchUnit& a, SkyMatchUnit& b)
{
	Team* pTeamA = TeamMgr::Instance()->FindTeam(a.teamId);
	Team* pTeamB = TeamMgr::Instance()->FindTeam(b.teamId);
	NotifyStopMatch(pTeamA);
	NotifyStopMatch(pTeamB);

	if (pTeamA == NULL || pTeamB == NULL)
	{
		LogWarn("Match Success, but team null, skyteam[%llu] team[%u] skyteam[%llu] team[%u]",
				a.stId, a.teamId, b.stId, b.teamId);
		return;
	}
	SkyTeam* pSTeamA = SkyTeamMgr::Instance()->GetSkyTeamById(a.stId);
	SkyTeam* pSTeamB = SkyTeamMgr::Instance()->GetSkyTeamById(b.stId);

	if (pSTeamA == NULL || pSTeamB == NULL)
	{
		LogWarn("Match Success, but skyteam null, skyteam[%llu] team[%u] skyteam[%llu] team[%u]",
				a.stId, a.teamId, b.stId, b.teamId);
		return;
	}

	LogInfo("Match Success, skyteam[%llu] team[%u] skyteam[%llu] team[%u]",
			a.stId, a.teamId, b.stId, b.teamId);

	// 设置队伍状态
	TeamForceOp op;
	op.SetInBattleState(pTeamA->GetID());
	op.SetInBattleState(pTeamB->GetID());

	HMatchHandler handler;
	handler.Done(pTeamA, KKSG::KMT_SKYCRAFT);
	handler.Done(pTeamB, KKSG::KMT_SKYCRAFT);

	// 存一下匹配信息
	UINT32 id = NewUid();
	FightGroup& group = m_matchedGroup[id];
	group.teamA = a;
	group.teamB = b;

	// 建立战场
	KKSG::CreateBattleParam param;
	KKSG::SkyCraftInfo& info = *param.mutable_scinfo();
	info.set_uid(id);
	info.set_type(KKSG::SCT_RacePoint);
	pSTeamA->FillBattleData(*info.mutable_team1());
	pSTeamB->FillBattleData(*info.mutable_team2());

	CSceneCreator::Instance()->CreateBattleScene(GetGlobalConfig().SkyCraftSceneId, this, param);
}

void SkyCraftMatchMgr::OnMatchTimeOut(SkyMatchUnit& unit)
{
	Team* team = TeamMgr::Instance()->FindTeam(unit.teamId);
	HMatchHandler handler;
	handler.Done(team, KKSG::KMT_SKYCRAFT);
	NotifyMatchTimeout(team);

	SkyTeam* pSTeam = SkyTeamMgr::Instance()->GetSkyTeamById(unit.stId);
	if (pSTeam)
	{
		pSTeam->SetState(KKSG::SkyTeamState_Idle);
	}
}

void SkyCraftMatchMgr::OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res)
{
	UINT32 uid = roParam.scinfo().uid();
	auto it = m_matchedGroup.find(uid);
	if (it == m_matchedGroup.end())
	{
		LogError("No Match Id[%u]", uid);
		return;
	}
	FightGroup& group = it->second;

	SkyTeam* pSTeamA = SkyTeamMgr::Instance()->GetSkyTeamById(group.teamA.stId);
	SkyTeam* pSTeamB = SkyTeamMgr::Instance()->GetSkyTeamById(group.teamB.stId);

	if (pSTeamA == NULL || pSTeamB == NULL || scene == NULL)
	{
		LogError("Failed, skyteam [%llu] skyteam [%llu]", group.teamA.stId, group.teamB.stId);
		if (pSTeamA) pSTeamA->SetState(KKSG::SkyTeamState_Idle);
		if (pSTeamB) pSTeamB->SetState(KKSG::SkyTeamState_Idle);
		m_matchedGroup.erase(it);
		return;
	}

	LogInfo("Create Battle Success, skyteam[%llu], skyteam[%llu], sceneid[%u]",
		group.teamA.stId, group.teamB.stId, scene->GetID());

	pSTeamA->SetState(KKSG::SkyTeamState_Idle);
	pSTeamB->SetState(KKSG::SkyTeamState_Idle);
	pSTeamA->SetBattleStart(scene->GetID(), scene->GetLine());
	pSTeamB->SetBattleStart(scene->GetID(), scene->GetLine());

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

void SkyCraftMatchMgr::NotifyStartMatch(Team* team)
{
	if (team == NULL)
	{
		return;
	}
	PtcM2C_SkyCraftMatchNtf ntf;
	ntf.m_Data.set_type(KKSG::SCMN_Start);
	team->BroadCast(ntf);
}

void SkyCraftMatchMgr::NotifyStopMatch(Team* team)
{
	if (team == NULL)
	{
		return;
	}
	PtcM2C_SkyCraftMatchNtf ntf;
	ntf.m_Data.set_type(KKSG::SCMN_Stop);
	team->BroadCast(ntf);
}

void SkyCraftMatchMgr::NotifyMatchTimeout(Team* team)
{
	if (team == NULL)
	{
		return;
	}
	PtcM2C_SkyCraftMatchNtf ntf;
	ntf.m_Data.set_type(KKSG::SCMN_Timeout);
	team->BroadCast(ntf);
}

UINT32 SkyCraftMatchMgr::NewUid()
{
	return ++m_uid;
}

int SkyCraftMatchMgr::Start(SkyMatchUnit& unit)
{
	if (IsInMatch(unit.stId))
	{
		return KKSG::ERR_SKY_TEAM_IN_MATCH;
	}
	if (IsInDelay(unit.stId))
	{
		return KKSG::ERR_SKY_TEAM_IN_MATCH;
	}

	unit.startTime = TimeUtil::GetTime();

	m_allUnits.insert(std::make_pair(unit.stId, unit));
	m_sortuUnits[std::make_pair(unit.score, unit.stId)] = unit.startTime;

	return KKSG::ERR_SUCCESS;
}

void SkyCraftMatchMgr::Cancel(UINT64 stId)
{
	auto it = m_allUnits.find(stId);
	if (it == m_allUnits.end())
	{
		return ;
	}
	SkyMatchUnit& unit = it->second;
	m_sortuUnits.erase(std::make_pair(unit.score, unit.stId));
	m_allUnits.erase(it);
}

bool SkyCraftMatchMgr::IsInMatch(UINT64 stId)
{
	return m_allUnits.find(stId) != m_allUnits.end();
}

SkyMatchUnit* SkyCraftMatchMgr::GetMatchUnit(UINT64 stId)
{
	auto it = m_allUnits.find(stId);
	if (it == m_allUnits.end())
	{
		return NULL;
	}
	return &it->second;
}

void SkyCraftMatchMgr::GetAllMatchSkyTeamIds(std::vector<UINT64>& stIds)
{
	stIds.clear();
	for (auto it = m_allUnits.begin(); it != m_allUnits.end(); ++it)
	{
		stIds.push_back(it->first);
	}
}

void SkyCraftMatchMgr::DoMatch()
{
	if (m_sortuUnits.size() < 2)
	{
		return;
	}
	auto it = m_sortuUnits.begin();
	auto itNext = it;
	auto itTemp = it;
	for (; it != m_sortuUnits.end(); )
	{
		itTemp = it;
		itNext = ++it;
		it = itTemp;
		if (itNext == m_sortuUnits.end())
		{
			break;
		}
		if (!IsFit(it->first.first, it->second, itNext->first.first, itNext->second))
		{
			it++;
			continue;
		}
		auto itUnit1 = m_allUnits.find(it->first.second);
		auto itUnit2 = m_allUnits.find(itNext->first.second);
		if (itUnit1 == m_allUnits.end() || itUnit2 == m_allUnits.end())
		{
			it++;
			LogError("impossible");
			continue;
		}
		OnMatchSuccess(itUnit1->second, itUnit2->second);

		AddToDelay(itUnit1->first);
		AddToDelay(itUnit2->first);

		m_allUnits.erase(itUnit1);
		m_allUnits.erase(itUnit2);
		m_sortuUnits.erase(it++);
		m_sortuUnits.erase(it++);
	}
}

void SkyCraftMatchMgr::DoTimeOut()
{
	UINT32 now = TimeUtil::GetTime();
	UINT32 maxScore = 0;
	UINT32 maxTime = 0;
	for (auto it = m_sortuUnits.begin(); it != m_sortuUnits.end(); )
	{
		UINT32 score = it->first.first;
		UINT32 startTime = it->second;
		UINT32 delta = (now > startTime) ? (now - startTime) : 0;
		if (score > maxScore)
		{
			maxTime = SkyCraftTableMgr::Instance()->GetMaxTime(score, maxScore);
		}

		if (delta < maxTime)
		{
			it++;
			continue;
		}

		auto it2 = m_allUnits.find(it->first.second);
		if (it2 == m_allUnits.end())
		{
			m_sortuUnits.erase(it++);
			LogError("impossible");
			continue;
		}
		OnMatchTimeOut(it2->second);
		m_allUnits.erase(it2);
		m_sortuUnits.erase(it++);
	}
}

bool SkyCraftMatchMgr::IsFit(UINT32 score1, UINT32 time1, UINT32 score2, UINT32 time2)
{
	UINT32 delta = (score1 < score2) ? (score2 - score1) : (score1 - score2);
	if (delta == 0)
	{
		return true;
	}

	UINT32 now = TimeUtil::GetTime();
	UINT32 deltaTime1 = (now > time1) ? (now - time1) : 0;
	UINT32 deltaTime2 = (now > time2) ? (now - time2) : 0;
	UINT32 span1 = SkyCraftTableMgr::Instance()->GetSpan(score1, deltaTime1);
	if (delta <= span1)
	{
		LogDebug("Match Success, score1: %u time1: %u, score2: %u time2: %u", score1, deltaTime1, score2, deltaTime2);
		return true;
	}

	UINT32 span2 = SkyCraftTableMgr::Instance()->GetSpan(score2, deltaTime2);
	if (delta <= span2)
	{
		LogDebug("Match Success, score1: %u time1: %u, score2: %u time2: %u", score1, deltaTime1, score2, deltaTime2);
		return true;
	}

	return false;
}

void SkyCraftMatchMgr::DoCheckDelay()
{
	UINT32 now = TimeUtil::GetTime();
	for (auto it = m_delayUnits.begin(); it != m_delayUnits.end();)
	{
		if (now >= it->second)
		{
			LogDebug("Erase Delay[%llu]", it->first);
			m_delayUnits.erase(it++);
		}
		else
		{
			it++;
		}
	}
}

void SkyCraftMatchMgr::AddToDelay(UINT64 ltId)
{
	UINT32 now = TimeUtil::GetTime();
	m_delayUnits[ltId] = now + 10;
}

void SkyCraftMatchMgr::DeleteFromDelay(UINT64 ltId)
{
	m_delayUnits.erase(ltId);
}

bool SkyCraftMatchMgr::IsInDelay(UINT64 ltId)
{
	return m_delayUnits.find(ltId) == m_delayUnits.end() ? false : true;
}
