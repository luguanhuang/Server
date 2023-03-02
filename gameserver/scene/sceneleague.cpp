#include "pch.h"
#include "sceneleague.h"
#include "leaguebattlebase.h"
#include "unit/rolemanager.h"
#include "globalconfig.h"
#include "timeutil.h"
#include "scenevsbase.h"
#include "scenebase.h"
#include "scene.h"
#include "leaguebattle/ptcg2c_leaguebattleoneresultnft.h"
#include "leaguebattle/ptcg2c_leaguebattleresultntf.h"
#include "leaguebattle/ptcg2c_leaguebattleloadinfontf.h"
#include "leaguebattle/ptcg2c_leaguebattlestatentf.h"
#include "leaguebattle/ptcg2c_leaguebattlebasedatantf.h"
#include "leaguebattle/rpcg2k_leaguerankupdateg2k.h"
#include "live/liveinfo.h"
#include "live/livemanager.h"
#include "network/mslink.h"
#include "leaguebattle/rpcg2m_leaguerankupdateg2m.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/rolemisc.h"
#include "gamelogic/bagtransition.h"
#include "network/worldlink.h"
#include "utility/tlogger.h"

SceneLeague::SceneLeague(Scene* scene)
:SceneVsBase(scene)
,m_initLoaded(false)
,m_readyEndBefore(false)
,m_readyEnd(false)
,m_roleId1(0)
,m_roleId2(0)
,m_winId(0)
,m_failId(0)
,m_winHpPercent(0.0f)
,m_failHpPercent(0.0f)
,m_updateRankDelay(false)
{

	m_timer = CTimerMgr::Instance()->SetTimer(this, 0, 5 * 1000, -1, __FILE__, __LINE__);	
}

SceneLeague::~SceneLeague()
{
	CTimerMgr::Instance()->KillTimer(m_timer);
}

void SceneLeague::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	NotifyBattleState();
}

bool SceneLeague::Init(const KKSG::CreateBattleParam& roParam)
{
	m_room.LoadFromKKSG(roParam.lbinfo());
	InitState();
	return true;
}

void SceneLeague::InitState()
{
	m_statemgr.Init(this, &VsWaitLoad::GlobalVsWaitLoad);
	m_statemgr.GetCurrentState()->OnEnter(this);
}

UINT32 SceneLeague::GetGroup(UINT64 roleid)
{
	return m_room.GetGroup(roleid);
}

UINT64 SceneLeague::GetFightGroupUId(UINT64 roleid)
{
	// 准备阶段结束之前不能打起来
	if (!m_readyEnd)
	{
		return UINT64(-1);
	}
	return (UINT64)GetGroup(roleid);
}

void SceneLeague::OnEnterBeforeNtf(Role* role)
{
	// 切观战不发送loading协议
	if (m_room.IsChangingWatch(role->GetID()))
	{
		return;
	}
	NotifyLoadInfo(role);
}

void SceneLeague::OnRoleReconnect(Role* pRole)
{
	if (pRole == NULL)
	{
		return;
	}
	LogInfo("role[%llu] reconnected", pRole->GetID());
	NotifyLoadInfo(pRole);
}

void SceneLeague::NotifyLoadInfo(Role* pRole)
{
	if (pRole == NULL)
	{
		return;
	}
	PtcG2C_LeagueBattleLoadInfoNtf ntf;
	ntf.m_Data.set_type(m_room.GetBattleType());
	m_room.FillRedTeamData(*ntf.m_Data.mutable_team1());
	m_room.FillBlueTeamData(*ntf.m_Data.mutable_team2());
	pRole->Send(ntf);
}

void SceneLeague::OnEnterWaitLoad()
{
	LogDebug("Enter Wait Load");
}

VsStateType SceneLeague::IsAllLoad()
{
	// 第一次加载完成在第一个玩家load的时候直接设置，这里不用IsAllLoad来控制
	if (!m_initLoaded)
	{
		return VS_STATE_DUMMY;
	}

	if (m_roleId1 && !m_room.IsRoleLoad(m_roleId1))
	{
		return VS_STATE_DUMMY;
	}
	if (m_roleId2 && !m_room.IsRoleLoad(m_roleId2))
	{
		return VS_STATE_DUMMY;
	}
	return VS_STATE_PREPARE321;
}

VsStateType SceneLeague::OnLoadTimeOut()
{
	// 第一次进场景加载超时
	if (!m_initLoaded)
	{
		m_initLoaded = true;
		return VS_STATE_READY_LEAGUE;
	}

	return VS_STATE_PREPARE321;
}

void SceneLeague::OnReadyStart()
{
	m_readyEnd = false;
	LogDebug("Ready Start");
}

VsStateType SceneLeague::CheckReadyEnd()
{
	UINT32 leftTime = GetReadyLeftTime();
	// 准备阶段结束前5秒, 通知不能进入了
	if (leftTime <= 5 && m_readyEndBefore == false)
	{
		m_room.NotifyReadyEndToMs(GetScene()->GetSceneID());	
		m_readyEndBefore = true;
	}

	if (leftTime != 0)
	{
		return VS_STATE_DUMMY;
	}

	// 准备阶段结束的时候判断上阵，上阵为空则直接结束
	m_readyEnd = true;

	bool flag = SetFightRole();
	if (!flag)
	{
		return VS_STATE_END;
	}

	return VS_STATE_WAIT_LOAD;
}

void SceneLeague::GMSetReady()
{
	auto cur = m_statemgr.GetCurrentState();
	if(NULL == cur || VS_STATE_READY_LEAGUE != cur->GetStateType())
	{
		return ;
	}
	UINT32 readyTime = 0;
	if (m_room.IsRaceMatch())
	{
		readyTime = GetGlobalConfig().LeagueBattleReadyTime;
	}
	else
	{
		readyTime = GetGlobalConfig().LeagueBattleReadyTimeForEighth;
	}

	SetStateStartTime(TimeUtil::GetTime() - readyTime + 1);
}

void SceneLeague::OnReadyEnd()
{
	LogDebug("Ready End");

	LiveInfo* info = LiveManager::Instance()->CreateLiveInfo(KKSG::LIVE_LEAGUEBATTLE, GetScene()->GetSceneID(), 0, 0, 0);
	if (info)
	{
		m_room.NotifyLiveInfoToMsOrWorld(GetScene()->GetSceneID(), info->m_liveID);
	}
}

void SceneLeague::OnEnterP321()
{
	LogDebug("OnEnter 321");
	TranInitPosition(m_roleId1);
	TranInitPosition(m_roleId2);

	std::vector<Role*> roles;
	Role* pRole = RoleManager::Instance()->FindByRoleID(m_roleId1);
	if (pRole) roles.push_back(pRole);
	pRole = RoleManager::Instance()->FindByRoleID(m_roleId2);
	if (pRole) roles.push_back(pRole);

	// 切线
	GetScene()->ChangeLine(roles);

	// FightGroup
	for (auto i = roles.begin(); i != roles.end(); ++i)
	{
		Role* role = *i;
		UINT32 group = GetScene()->GetFightGroup(role); // 最终会调用到GetFightGroupUId
		role->ChangeFightGroup(group);					// 改变fightgroup让打起来
		SSDebug << __FUNCTION__ << " name = " << role->GetName() << " group = " << group << END;
	}

	// 更新上阵状态和战斗阶段
	NotifyBaseData();
	NotifyBattleState();
}

void SceneLeague::OnEnterFight()
{
	LogDebug("OnEnter Fight");
	Role* pRole = RoleManager::Instance()->FindByRoleID(m_roleId1);
	if (pRole) pRole->SkillCDAtLaunch();
	pRole = RoleManager::Instance()->FindByRoleID(m_roleId2);
	if (pRole) pRole->SkillCDAtLaunch();

	NotifyBattleState();
}

VsStateType SceneLeague::CheckFightEnd()
{
	bool fail = CheckRoleFailed(m_roleId1);
	if (fail)
	{
		m_failId = m_roleId1;
		m_winId = m_roleId2;
		return VS_STATE_AFTER_FIGHT;
	}
	fail = CheckRoleFailed(m_roleId2);
	if (fail)
	{
		m_failId = m_roleId2;
		m_winId = m_roleId1;
		return VS_STATE_AFTER_FIGHT;
	}

	if (GetFightLeftTime() == 0)
	{
		// 超时血量少的失败
		m_failId = GetHpPrecentLessID();
		if (m_failId == m_roleId1)
		{
			m_winId = m_roleId2;
		}
		else if (m_failId == m_roleId2)
		{
			m_winId = m_roleId1;	
		}

		return VS_STATE_AFTER_FIGHT;
	}
	return VS_STATE_DUMMY;
}

void SceneLeague::OnEnterAfterFight()
{
	LogDebug("Enter After Fight");
	// 玩家停止动作
	std::vector<UINT64> roleids;
	roleids.push_back(m_roleId1);
	roleids.push_back(m_roleId2);
	EndSkills(roleids);
	MakePuppet(GetUnit(m_roleId1), true);
	MakePuppet(GetUnit(m_roleId2), true);

	// 推送状态
	NotifyBattleState();
	// 计算双方血量
	UpdateHp();
	// 清掉输的人
	RemoveFailRole();

	// 推送小结算
	NotifyOneBattleResult();
}

void SceneLeague::RemoveFailRole()
{
	if (m_failId == m_roleId1)	
	{
		m_roleId1 = 0;
	}
	else if (m_failId == m_roleId2)
	{
		m_roleId2 = 0;
	}
	else
	{
		LogError("failed id[%llu] invalid", m_failId);
	}
	// 设置role状态
	m_room.SetRoleState(m_failId, KKSG::LBRoleState_Failed);
	LeagueBattleRole* br = m_room.GetBattleRole(m_winId);
	if (br)
	{
		br->m_killNum++;
	}
}

VsStateType SceneLeague::CheckAfterFightEnd()
{
	if (GetAfterFightLeftTime() > 0)
	{
		return VS_STATE_DUMMY;
	}

	bool flag = SetFightRole();
	if (!flag)
	{
		return VS_STATE_END;
	}

	return VS_STATE_WAIT_LOAD;
}

void SceneLeague::OnStateEnd()
{
	SSDebug << __FUNCTION__ << END;
	GetScene()->SetDontDestory(false);

	LiveManager::Instance()->RemoveLiveInfo(KKSG::LIVE_LEAGUEBATTLE, GetScene()->GetSceneID(), 0, 0, 0);

	NotifyBattleState();

	// 更新胜利队伍，计算积分变化
	UpdateWinLeague();
	m_room.UpdateScore(m_winLeagueId);

	DoTxLog();
	// 去MS或者world更新排名
	if (BeginRankUpdate())
	{
		m_updateRankDelay = true;
	}
	// 直接推送结算，结束
	else
	{
		NotifyBattleResult();
		SetEnd();
		GetScene()->SetDontDestory(false);
	}
}

bool SceneLeague::BeginRankUpdate()
{
	if (m_room.GetBattleType() == KKSG::LeagueBattleType_RacePoint)
	{
		if (MSLink::Instance()->GetConnID() == INVALID_CONN_ID)
		{
			return false;
		}
		RpcG2M_LeagueRankUpdateG2M* rpc = RpcG2M_LeagueRankUpdateG2M::CreateRpc();
		rpc->m_oArg.set_sceneid(GetScene()->GetSceneID());
		rpc->m_oArg.set_win_teamid(m_winLeagueId);

		{
			auto& room = m_room.GetRedTeam();
			rpc->m_oArg.add_league_teamid(room.m_leagueTeamId);
			rpc->m_oArg.add_point(room.m_newScore);
			rpc->m_oArg.add_total_num(room.m_totalNum);
			rpc->m_oArg.add_win_num(room.m_winNum);
		}
		{
			auto& room = m_room.GetBlueTeam();
			rpc->m_oArg.add_league_teamid(room.m_leagueTeamId);
			rpc->m_oArg.add_point(room.m_newScore);
			rpc->m_oArg.add_total_num(room.m_totalNum);
			rpc->m_oArg.add_win_num(room.m_winNum);
		}

		MSLink::Instance()->SendTo(*rpc);
		return true;
	}
	else if (m_room.GetBattleType() == KKSG::LeagueBattleType_CrossRacePoint)
	{
		if (!WorldLink::Instance()->IsConnected())
		{
			return false;
		}

		RpcG2K_LeagueRankUpdateG2K* rpc = RpcG2K_LeagueRankUpdateG2K::CreateRpc();
		rpc->m_oArg.set_sceneid(GetScene()->GetSceneID());
		rpc->m_oArg.set_win_teamid(m_winLeagueId);

		{
			auto& room = m_room.GetRedTeam();
			rpc->m_oArg.add_league_teamid(room.m_leagueTeamId);
			rpc->m_oArg.add_point(room.m_newScore);
			rpc->m_oArg.add_total_num(room.m_totalNum);
			rpc->m_oArg.add_win_num(room.m_winNum);
		}
		{
			auto& room = m_room.GetBlueTeam();
			rpc->m_oArg.add_league_teamid(room.m_leagueTeamId);
			rpc->m_oArg.add_point(room.m_newScore);
			rpc->m_oArg.add_total_num(room.m_totalNum);
			rpc->m_oArg.add_win_num(room.m_winNum);
		}

		WorldLink::Instance()->SendTo(*rpc);
		return true;
	}
	return false;
}

void SceneLeague::EndRankUpdate(const KKSG::LeagueRankUpdateG2MRes* res)
{
	m_updateRankDelay = false;
	do 
	{
		if (res == NULL)
		{
			break;
		}
		if (res->league_teamid_size() != res->lastrank_size() || res->league_teamid_size() != res->currank_size())
		{
			break;
		}
		m_room.m_allRank = res->allrank();
		for (int i = 0; i < res->league_teamid_size(); ++i)
		{
			LeagueBattleTeam* team = m_room.GetBattleTeamById(res->league_teamid(i));
			if (team == NULL)
			{
				continue;
			}
			team->m_oldRank = res->lastrank(i);			
			team->m_newRank = res->currank(i);
			// 新上榜
			if (team->m_oldRank == INVALID_RANK && team->m_newRank != INVALID_RANK)
			{
				team->m_deltaRank = res->allrank() - team->m_newRank;
			}
			// 被挤下榜
			else if (team->m_oldRank != INVALID_RANK && team->m_newRank == INVALID_RANK)
			{
				team->m_deltaRank = team->m_oldRank - res->allrank();
			}
			else
			{
				team->m_deltaRank = team->m_oldRank - team->m_newRank;
			}
		}
	} while (0);

	NotifyBattleResult();
	SetEnd();
}

void SceneLeague::Update()
{
	if (m_updateRankDelay)
	{
		return;
	}

	if (m_readyEnd && VS_STATE_AFTER_FIGHT != GetCurrentStateType())
	{
		m_room.CheckSwitchWatcher(this);
	}

	m_statemgr.Update();//会改变 m_IsEnd 状态 所以顺序不能变
}

bool SceneLeague::OnEvent(const VsEvent& event)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(event.roleid);
	if(NULL == pRole)
	{
		return true;
	}

	if(pRole->IsWatcher())
	{
		SSDebug << "league battle watcher name = " << pRole->GetName() << " event = " << event.type << END;
	}
	else
	{
		if(VS_EVENT_ROLE_LOADED == event.type)
		{
			OnRoleLoad(pRole);
		}
		else if(VS_EVENT_ROLE_DIE == event.type)
		{
			OnRoleDie(pRole);
		}
	}

	if (VS_EVENT_ROLE_LOADED == event.type)
	{
		OnRoleEnter(pRole);
	}
	else if(VS_EVENT_ROLE_QUIT == event.type)
	{
		OnRoleQuit(pRole);
	}

	return true;
}

void SceneLeague::OnRoleEnter(Role* pRole)
{
	NotifyBaseData(pRole);
	NotifyBattleState(pRole);
	m_room.RemoveChangingWatch(pRole->GetID());
}

void SceneLeague::OnRoleLoad(Role* pRole)
{
	if (pRole == NULL)
	{
		return;
	}
	LogDebug("role[%llu] loaded", pRole->GetID());

	// 进来第一个人设置成准备状态
	if (!m_initLoaded)
	{
		SetState(VS_STATE_READY_LEAGUE);
		m_initLoaded = true;
	}

	m_room.AddRoleLoad(pRole->GetID());

	// 准备阶段结束之后进来先不能动，在update中切观战
	if (m_readyEnd)
	{
		MakePuppet(pRole, true);
	}
}

void SceneLeague::OnRoleQuit(Role* pRole)
{
	if (pRole == NULL)
	{
		return;
	}
	LogDebug("role[%llu] quit", pRole->GetID());
	m_room.RemoveRoleLoad(pRole->GetID());

	MakePuppet(pRole, false);

	// 在观战或者准备阶段，要下阵
	if (pRole->IsWatcher() || !m_readyEnd)
	{
		// 尝试下阵
		SetRoleDown(pRole);
	}
}

void SceneLeague::OnRoleDie(Role* pRole)
{
	if (pRole == NULL)
	{
		return;
	}
	LogDebug("role[%llu] dead", pRole->GetID());
}

int SceneLeague::SetRoleUp(Role* pRole)
{
	LeagueBattleTeam* team = m_room.GetBattleTeam(pRole->GetID());
	if (team == NULL)
	{
		return KKSG::ERR_LEAGUE_HAS_NO_TEAM;
	}
	UINT64 roleId = pRole->GetID();
	if (team->GetBattleRoleSize() >= team->GetMemberSize())
	{
		return KKSG::ERR_LEAGUE_ROLE_ALREADY_UP;
	}
	if (team->HasBattleRole(roleId))
	{
		return KKSG::ERR_SUCCESS;	// 忽略
	}
	team->AddBattleRole(roleId);

	NotifyBaseData();

	return KKSG::ERR_SUCCESS;
}

int SceneLeague::SetRoleDown(Role* pRole)
{
	LeagueBattleTeam* team = m_room.GetBattleTeam(pRole->GetID());
	if (team == NULL)
	{
		return KKSG::ERR_LEAGUE_HAS_NO_TEAM;
	}
	UINT64 roleId = pRole->GetID();
	if (!team->HasBattleRole(roleId))
	{
		return KKSG::ERR_SUCCESS;	//忽略请求
	}
	KKSG::LeagueBattleRoleState state = team->GetBattleRoleState(roleId);
	if (state == KKSG::LBRoleState_Fighting)
	{
		return KKSG::ERR_LEAGUE_ROLE_FIGHTING;
	}
	if (state != KKSG::LBRoleState_Waiting)
	{
		return KKSG::ERR_LEAGUE_ROLE_ALREADY_BATTLE;
	}
	team->RemoveBattleRole(roleId);

	NotifyBaseData();

	return KKSG::ERR_SUCCESS;
}

UINT32 SceneLeague::GetReadyLeftTime()
{
	auto cur = m_statemgr.GetCurrentState();
	if(NULL == cur || VS_STATE_READY_LEAGUE != cur->GetStateType())
	{
		return 0;
	}
	UINT32 now = TimeUtil::GetTime();
	UINT32 endTime = 0;
	if (m_room.IsRaceMatch())
	{
		endTime = GetStateStartTime() + GetGlobalConfig().LeagueBattleReadyTime;
	}
	else
	{
		endTime = GetStateStartTime() + GetGlobalConfig().LeagueBattleReadyTimeForEighth;
	}
	return endTime > now ? (endTime - now) : 0;
}

UINT32 SceneLeague::GetFightLeftTime()
{
	auto cur = m_statemgr.GetCurrentState();
	if (NULL == cur || VS_STATE_IN_FIGHT != cur->GetStateType())
	{
		return 0;
	}
	UINT32 now = TimeUtil::GetTime();
	UINT32 endTime = 0;
	if (m_room.IsRaceMatch())
	{
		endTime = GetStateStartTime() + GetGlobalConfig().LeagueBattleOneFightTime;
	}
	else
	{
		endTime = GetStateStartTime() + GetGlobalConfig().LeagueBattleOneFightTimeForEighth;
	}
	return endTime > now ? (endTime - now) : 0;
}

UINT32 SceneLeague::GetAfterFightLeftTime()
{
	UINT32 now = TimeUtil::GetTime();
	UINT32 endTime = GetStateStartTime() + GetGlobalConfig().LeagueBattleFightAfterTime;
	return endTime > now ? (endTime - now) : 0;
}

KKSG::LeagueBattleFightState SceneLeague::GetBattleState(UINT32& leftTime)
{
	leftTime = 5;
	auto cur = m_statemgr.GetCurrentState();
	if (cur == NULL)
	{
		return KKSG::LBFight_None;
	}

	switch (cur->GetStateType())
	{
	case VS_STATE_READY_LEAGUE:
		{
			leftTime = GetReadyLeftTime();
			return KKSG::LBFight_Wait;
		}
	case VS_STATE_WAIT_LOAD:
	case VS_STATE_PREPARE321:
		{
			leftTime = 0;
			return KKSG::LBFight_Fight;
		}
	case VS_STATE_IN_FIGHT:
		{
			leftTime = GetFightLeftTime();
			return KKSG::LBFight_Fight;
		}
	case VS_STATE_AFTER_FIGHT:
		{
			leftTime = GetAfterFightLeftTime();
			return KKSG::LBFight_Result;
		}
	case VS_STATE_END:
		{
			return KKSG::LBFight_Result;
		}
	default:
		LogError("invalid state: %d", cur->GetStateType());
	}

	return KKSG::LBFight_None;
}

bool SceneLeague::SetFightRole()
{
	if (m_roleId1 == 0)
	{
		m_roleId1 = m_room.GetRedTeam().GetNextBattleRoleId();
	}
	if (m_roleId2 == 0)
	{
		m_roleId2 = m_room.GetBlueTeam().GetNextBattleRoleId();	
	}

	if (m_roleId1 == 0 || m_roleId2 == 0)
	{
		return false;
	}

	// 切战斗
	Role* pRole = RoleManager::Instance()->FindByRoleID(m_roleId1);
	if (pRole && pRole->IsWatcher())
	{
		ChangeWatcherState(pRole, false);
	}

	pRole = RoleManager::Instance()->FindByRoleID(m_roleId2);
	if (pRole && pRole->IsWatcher())
	{
		ChangeWatcherState(pRole, false);
	}

	ClearFailInfo();
	NotifyBaseData();
	return true;
}

bool SceneLeague::CheckRoleFailed(UINT64 roleId)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(roleId);
	if (NULL == pRole || pRole->IsDead())
	{
		return true;
	}
	if (!m_room.IsRoleLoad(roleId))
	{
		return true;
	}
	return false;
}

void SceneLeague::ClearFailInfo()
{
	m_failId = 0;
	m_winId = 0;
	m_failHpPercent = 0;
	m_winHpPercent = 0;
}

UINT64 SceneLeague::GetHpPrecentLessID()
{
	Role* pRole11 = RoleManager::Instance()->FindByRoleID(m_roleId1);
	Role* pRole22 = RoleManager::Instance()->FindByRoleID(m_roleId2);

	if(NULL == pRole11)
	{
		return m_roleId1;
	}
	else if(NULL == pRole22)
	{
		return m_roleId2;
	}

	if(pRole11->GetPreciseHpPercent() < pRole22->GetPreciseHpPercent())
	{
		return m_roleId1;
	}
	if(pRole11->GetPreciseHpPercent() > pRole22->GetPreciseHpPercent())
	{
		return m_roleId2;
	}

	// 随机
	int n = XRandom::randInt(0, 100);
	if (n < 50)
	{
		return m_roleId1;
	}
	return m_roleId2;
}

void SceneLeague::UpdateHp()
{
	if (m_winId)
	{
		Role* pRole = RoleManager::Instance()->FindByRoleID(m_winId);
		if (pRole)
		{
			m_winHpPercent = pRole->GetPreciseHpPercent();
		}
		else
		{
			m_winHpPercent = 0.0f;
		}
	}
	if (m_failId)
	{
		Role* pRole = RoleManager::Instance()->FindByRoleID(m_failId);
		if (pRole)
		{
			m_failHpPercent = pRole->GetPreciseHpPercent();
		}
		else
		{
			m_failHpPercent = 0.0f;
		}
	}
}

void SceneLeague::UpdateWinLeague()
{
	m_winId = 0;
	if (m_roleId1 && m_roleId2 == 0)
	{
		m_winId = m_roleId1;
	}
	else if (m_roleId1 == 0 && m_roleId2)
	{
		m_winId = m_roleId2;	
	}

	if (m_winId)
	{
		if (m_room.GetRedTeam().HasMember(m_winId))
		{
			m_winLeagueId = m_room.GetRedTeam().m_leagueTeamId;
			m_loseLeagueId = m_room.GetBlueTeam().m_leagueTeamId;
		}
		else
		{
			m_winLeagueId = m_room.GetBlueTeam().m_leagueTeamId;
			m_loseLeagueId = m_room.GetRedTeam().m_leagueTeamId;
		}
	}
	else
	{
		LeagueBattleTeam& red = m_room.GetRedTeam();
		LeagueBattleTeam& blue = m_room.GetBlueTeam();
		if (red.m_oldRank < blue.m_oldRank)
		{
			m_winLeagueId = red.m_leagueTeamId;
			m_loseLeagueId = blue.m_leagueTeamId;
		}
		else
		{	
			m_winLeagueId = blue.m_leagueTeamId;
			m_loseLeagueId = red.m_leagueTeamId;
		}
	}

	LogInfo("win leagueId: [%llu] lose leagueId: [%llu]", m_winLeagueId, m_loseLeagueId);
}

void SceneLeague::NotifyOneBattleResult()
{
	NotifyBaseData();
	PtcG2C_LeagueBattleOneResultNft ntf;
	m_room.FillRoleBrief(m_winId, *ntf.m_Data.mutable_winrole());
	m_room.FillRoleBrief(m_failId, *ntf.m_Data.mutable_loserole());
	ntf.m_Data.set_winhppercent(m_winHpPercent);
	ntf.m_Data.set_losehppercent(m_failHpPercent);
	GetScene()->Broadcast(ntf);

	LogDebug("win = %s, lose = %s", ntf.m_Data.winrole().name().c_str(), ntf.m_Data.loserole().name().c_str());
}

void SceneLeague::NotifyBattleResult()
{
	LogInfo("Result");
	PtcG2C_LeagueBattleResultNtf ntf;
	ntf.m_Data.set_type(m_room.GetBattleType());
	m_room.FillTeamResultDataById(m_winLeagueId, *ntf.m_Data.mutable_winteam());
	m_room.FillTeamResultDataById(m_loseLeagueId, *ntf.m_Data.mutable_loseteam());

	LeagueBattleTeam* winTeam = m_room.GetBattleTeamById(m_winLeagueId);
	LeagueBattleTeam* loseTeam = m_room.GetBattleTeamById(m_loseLeagueId);

	if (winTeam == NULL || loseTeam == NULL)
	{
		LogError("impossible");
		return;
	}

	std::set<UINT64> roleIds;
	for (size_t i = 0; i < winTeam->m_allmembers.size(); ++i)
	{
		UINT64 roleId = winTeam->m_allmembers[i];	
		Role* pRole = RoleManager::Instance()->FindByRoleID(roleId);
		if (pRole == NULL)
		{
			continue;
		}
		ntf.m_Data.mutable_winteam()->set_honorpoint(0);
		if (m_room.GetBattleType() == KKSG::LeagueBattleType_RacePoint || m_room.GetBattleType() == KKSG::LeagueBattleType_CrossRacePoint)
		{
			if (pRole->Get<RoleMisc>()->GetDailyLBNum() < GetGlobalConfig().LeagueBattleRaceReward[0])
			{
				BagGiveItemTransition trans(pRole);
				trans.SetReason(ItemFlow_LeagueBattle, ItemFlow_LeagueBattleRace);
				trans.GiveItem(ARENA_COIN, GetGlobalConfig().LeagueBattleRaceReward[1]);
				trans.NotifyClient();
				ntf.m_Data.mutable_winteam()->set_honorpoint(GetGlobalConfig().LeagueBattleRaceReward[1]);		
				pRole->Get<RoleMisc>()->AddDailyLBNum();
			}
		}
		pRole->Send(ntf);
		roleIds.insert(roleId);
	}

	for (size_t i = 0; i < loseTeam->m_allmembers.size(); ++i)
	{
		UINT64 roleId = loseTeam->m_allmembers[i];	
		Role* pRole = RoleManager::Instance()->FindByRoleID(roleId);
		if (pRole == NULL)
		{
			continue;
		}
		ntf.m_Data.mutable_loseteam()->set_honorpoint(0);
		if (m_room.GetBattleType() == KKSG::LeagueBattleType_RacePoint || m_room.GetBattleType() == KKSG::LeagueBattleType_CrossRacePoint)
		{
			if (pRole->Get<RoleMisc>()->GetDailyLBNum() < GetGlobalConfig().LeagueBattleRaceReward[0])
			{
				BagGiveItemTransition trans(pRole);
				trans.SetReason(ItemFlow_LeagueBattle, ItemFlow_LeagueBattleRace);
				trans.GiveItem(ARENA_COIN, GetGlobalConfig().LeagueBattleRaceReward[1]);
				trans.NotifyClient();
				ntf.m_Data.mutable_loseteam()->set_honorpoint(GetGlobalConfig().LeagueBattleRaceReward[1]);
				pRole->Get<RoleMisc>()->AddDailyLBNum();
			}
		}
		pRole->Send(ntf);
		roleIds.insert(roleId);
	}

	ntf.m_Data.mutable_winteam()->set_honorpoint(0);
	ntf.m_Data.mutable_loseteam()->set_honorpoint(0);
	auto& watchers = GetScene()->GetWatchers();
	for (auto it = watchers.begin(); it != watchers.end(); ++it)
	{
		Role* pRole = it->second;
		if (pRole == NULL)
		{
			continue;
		}
		if (roleIds.find(pRole->GetID()) != roleIds.end())
		{
			continue;
		}
		pRole->Send(ntf);	
	}

	// 通知MS结果
	m_room.NotifyEndToMs(GetScene()->GetSceneID(), m_winLeagueId);
}

void SceneLeague::NotifyBattleState(Role* pRole)
{
	PtcG2C_LeagueBattleStateNtf ntf;
	UINT32 leftTime;
	auto state = GetBattleState(leftTime);
	ntf.m_Data.set_state(state);
	ntf.m_Data.set_lefttime(leftTime);
	if (pRole)
	{
		pRole->Send(ntf);
	}
	else
	{
		GetScene()->Broadcast(ntf);
	}
}

void SceneLeague::NotifyBaseData(Role* pRole)
{
	PtcG2C_LeagueBattleBaseDataNtf ntf;
	m_room.FillRedTeamUpData(*ntf.m_Data.mutable_team1());
	m_room.FillBlueTeamUpData(*ntf.m_Data.mutable_team2());
	if (pRole)
	{
		pRole->Send(ntf);
	}
	else
	{
		GetScene()->Broadcast(ntf);
	}
}

bool SceneLeague::IsFightRole(UINT64 roleId)
{
	if (m_roleId1 == roleId)
	{
		return true;
	}
	if (m_roleId2 == roleId)
	{
		return true;
	}
	return false;
}

void SceneLeague::OnBeforeChangeWather(Role* pRole)
{
	MakePuppet(pRole, false);
}

void SceneLeague::DoTxLog()
{
	LeagueBattleTeam* winTeam = m_room.GetBattleTeamById(m_winLeagueId);
	LeagueBattleTeam* loseTeam = m_room.GetBattleTeamById(m_loseLeagueId);
	if (NULL == winTeam || NULL == loseTeam)
	{
		return;
	}
	TLeagueBattleFlow oLog;
	oLog.m_winLeagueId = m_winLeagueId;
	oLog.m_loseLeagueId = m_loseLeagueId;
	std::ostringstream ss;
	INT32 count = 0;
	for (auto i = winTeam->m_allmembers.begin(); i != winTeam->m_allmembers.end(); ++i)
	{
		auto it = m_room.m_briefs.find(*i);
		if (it != m_room.m_briefs.end())
		{
			if (count) ss << "#";
			ss << it->second.serverid() << "=" << it->second.account() << "=" << it->second.roleid() << "=" << it->second.name() << "=" << it->second.level() << "=" << it->second.ppt() << "=" << it->second.profession();
			count ++;
		}
	}
	oLog.m_strWinInfo = ss.str();
	std::ostringstream s;
	count = 0;
	for (auto i = loseTeam->m_allmembers.begin(); i != loseTeam->m_allmembers.end(); ++i)
	{
		auto it = m_room.m_briefs.find(*i);
		if (it != m_room.m_briefs.end())
		{
			if (count) s << "#";
			s << it->second.serverid() << "="  << it->second.account() << "=" << it->second.roleid() << "=" << it->second.name() << "=" << it->second.level() << "=" << it->second.ppt() << "=" << it->second.profession();
			count ++;
		}
	}
	oLog.m_strLoseInfo = s.str();
	oLog.Do();
}