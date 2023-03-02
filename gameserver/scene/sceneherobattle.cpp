#include "pch.h"
#include "sceneherobattle.h"
#include "unit/rolemanager.h"
#include "herobattle/ptcg2c_herobattlecanusehero.h"
#include "herobattle/ptcg2c_herobattleteammsgntf.h"
#include "herobattle/ptcg2c_herobattlesyncntf.h"
#include "herobattle/ptcg2c_herobattleincirclentf.h"
#include "herobattle/ptcg2c_herobattleovertime.h"
#include "herobattle/ptcg2c_herobattleteamrolentf.h"
#include "herobattle/ptcg2c_closechooseherontf.h"
#include "herobattle/ptcg2c_herobattletipsntf.h"
#include "gamelogic/herobattlerecord.h"
#include "gamelogic/bagtransition.h"
#include "config/itemdropconfig.h"
#include "live/livemanager.h"
#include "gamelogic/notice.h"
#include "gamelogic/noticemgr.h"
#include <cmath>
#include "util/gametime.h"
#include "mail/mailsender.h"
#include "mail/mailconfig.h"
#include "battle/ptcg2m_reportbadplayer2ms.h"
#include "network/mslink.h"
#include "utility/UtilMath.h"
#include "herobattle/ptcg2c_herobattleancientpower.h"
#include "mobabattle/ptcg2c_mobasignalbroadcast.h"
#include "gamelogic/goalaward.h"

const UINT32 NoBodyDestroyTime = 60 * 60 * 2;

const UINT32 OneFrameTime = 100;

SceneHeroBattle::SceneHeroBattle(Scene* pScene):SceneVsBase(pScene), m_datamgr(pScene), m_fightTeam1(GetTeam1()), m_fightTeam2(GetTeam2())
{
	m_lastUpdateTime = 0;

	m_occupant = 0;

	m_lootTeam = 0;
	m_lootProgress = 0;
	m_lootRate = 0;

	m_lastAddPointTime = 0;	

	m_startOverTime = 0;
	m_overTimeCount = 0;
	m_lastAddAncientPowerTime = 0;

	m_state = HeroBattleState_Normal;

	m_mvpid = 0;
	m_loseMvpid = 0;

	m_noOneTime = time(NULL);

	m_isSendNotice = false;
}

SceneHeroBattle::~SceneHeroBattle()
{

}

bool SceneHeroBattle::Init(const KKSG::CreateBattleParam& roParam)
{
	for (int i = 0; i < roParam.herobattle().roleinfo_size(); ++i)
	{
		const KKSG::PvpRoleInfo& data = roParam.herobattle().roleinfo(i);
	
		if (m_roleData.find(data.roleid()) != m_roleData.end())
		{
			LogError("same role [%llu] enter HeroBattle scene", data.roleid());
			continue;
		}
		auto iter = m_roleData.insert(std::make_pair(data.roleid(), HeroBattleRoleData(data)));
		if (data.camp() == GetTeam1())
		{
			m_fightTeam1.teamMember.push_back(data.roleid());
		}
		else if(data.camp() == GetTeam2())
		{
			m_fightTeam2.teamMember.push_back(data.roleid());
		}
		else
		{
			iter.first->second.info.set_camp(GetTeam1());
			LogError("role [%llu] teamid is not %u or %u", data.roleid(), GetTeam1(), GetTeam2());
		}
	}
	m_fightTeam1.eloPoint = roParam.herobattle().elopoint1();
	m_fightTeam2.eloPoint = roParam.herobattle().elopoint2();

	LogInfo("HeroBattle game start, elopoint %.0lf:%.0lf", m_fightTeam1.eloPoint, m_fightTeam2.eloPoint);

	InitState();

	HeroBattleMapCenterConf* centerConf = HeroBattleMgr::Instance()->GetHeroMapCenterConf(GetScene()->GetSceneTemplateID());
	if (centerConf == NULL)
	{
		LogError("HeroBattle sceneid can't find in HeroBattleMapCenter.txt, sceneid: %u", GetScene()->GetSceneTemplateID());
		m_centerConf.SceneID = 0;
	}
	else
	{
		m_centerConf = *centerConf;
	}

	LiveManager::Instance()->CreateLiveInfo(KKSG::LIVE_HEROBATTLE, GetScene()->GetSceneID(), 0, 0, 0);

	return true;
}

void SceneHeroBattle::InitState()
{
	m_statemgr.Init(this, &VsWaitLoad::GlobalVsWaitLoad);
	m_statemgr.GetCurrentState()->OnEnter(this);
}

void SceneHeroBattle::OnEnterBeforeNtf(Role* role)
{
	///> 隐身
	role->MakeInvisible(true, false);
}

UINT32 SceneHeroBattle::GetGroup(UINT64 roleid)
{
	auto iter = m_roleData.find(roleid);
	if (iter != m_roleData.end())
	{
		return iter->second.info.camp();
	}
	return GetTeam1();
}

bool SceneHeroBattle::OnEvent(const VsEvent& event)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(event.roleid);
	if (pRole == NULL)
	{
		return false;
	}
	if (pRole->IsWatcher())
	{
		if (event.type == VS_EVENT_ROLE_LOADED)
		{
			_SyncAllDataToClient(pRole);
		}
		return true;
	}

	if (m_roleData.find(event.roleid) == m_roleData.end())
	{
		LogError("Can't find role [%llu] in HeroBattle", event.roleid);
		return false;
	}

	HeroBattleRoleData& data = m_roleData[event.roleid];

	switch (event.type)
	{
	case VS_EVENT_ROLE_LOADED:
		{
			if (m_battleUniqueTag.empty())
			{
				TRoleLog log(pRole);
				m_battleUniqueTag = log.GetTransTag();
			}

			m_loadmgr.AddRole(event.roleid);
			m_datamgr.AddUnit(event.roleid, data.info.camp());

			data.enterGameTime = TimeUtil::GetMilliSecond();
			data.canChooseHero = true;
			data.freeWeekHero = pRole->Get<CHeroBattleRecord>()->GetFreeWeekHero();
			data.experienceHero = pRole->Get<CHeroBattleRecord>()->GetExperienceHero();

			m_noOneTime = 0;	///> 场景内有人

			_SyncAllDataToClient(pRole);
			_SendGameNotice(pRole, HeroBattleGameNotice1);

			LogDebug("role [%llu] is loaded", event.roleid);
			break;
		}
	case VS_EVENT_ROLE_QUIT:
		{
			PrintHeroTLog(data, pRole, true);

			///> 当输。扣elo分数
			double eloPoint = pRole->Get<CHeroBattleRecord>()->GetELOPoint();
			double newELOPoint, otherELO = data.info.camp() == GetTeam1() ? m_fightTeam2.eloPoint : m_fightTeam1.eloPoint;

			newELOPoint = eloPoint + 1.0 * GetGlobalConfig().HeroBattleELOK * (0.0 - 1.0 / (1.0 + pow(10, (otherELO - eloPoint) / 400.0)));

			pRole->Get<CHeroBattleRecord>()->SetELOPoint(newELOPoint);
			LogInfo("role [%llu] runaway, HeroBattle elopoint, %.2lf -> %.2lf", pRole->GetID(), eloPoint, newELOPoint);

			THeroBattleRoundVar var;
			var.m_elo = eloPoint;
			var.m_newElo = newELOPoint;
			var.m_result = TX_BATTLE_RUNAWAY;
			var.m_killNum = m_datamgr.GetUnitKillCount(pRole->GetID());
			var.m_deadNum = m_datamgr.GetUnitDeadCount(pRole->GetID());
			pRole->Get<CHeroBattleRecord>()->PrintHeroBattleRoundTLog(var, m_battleUniqueTag);

			pRole->Get<CHeroBattleRecord>()->RunAway(var.m_killNum);

			m_loadmgr.DelRole(event.roleid);
			m_datamgr.OnUnitCombatStop(event.roleid);

			if (m_loadmgr.GetLoadedRoleCount() == 0)
			{
				m_noOneTime = time(NULL);		///> 场景内无人
			}

			break;
		}
	case VS_EVENT_ROLE_DIE:
		{
			PrintHeroTLog(data, pRole, false);

			_AddRoleDie(event.roleid);
			_SendTeamDataToClient();
			_SendTeamRoleToClient();

			data.canChooseHero = true;

			break;
		}
	case VS_EVENT_ROLE_REVIVE:
		{
			_RoleRevive(data, true);
			break;
		}
	default:
		{
			break;
		}
	}

	return true;
}

void SceneHeroBattle::Update()
{
	m_statemgr.Update();

	if (m_noOneTime != 0 && m_noOneTime + NoBodyDestroyTime <= time(NULL))
	{
		SetState(VS_STATE_END);
		LogError("HeroBattle, it shouldn't be print, game end when nobody in more than %d seconds", NoBodyDestroyTime);
		return;
	}

	if (GetCurrentStateType() != VS_STATE_IN_FIGHT)
	{
		return;
	}

	if (GetStateStartTime() + 60 < time(NULL) && m_isSendNotice == false)
	{
		m_isSendNotice = true;
		_SendGameNoticeToAll(HeroBattleGameNotice4);
	}
	
	UINT64 nowTime = TimeUtil::GetMilliSecond();
	if (m_lastUpdateTime + OneFrameTime > nowTime)
	{
		return;
	}

	m_lastUpdateTime = nowTime;

	_CheckRoleRevive(nowTime);
	_CheckChooseHero(nowTime);

	if (m_occupant != 0 && m_startOverTime != 0 && m_startOverTime + GetGlobalConfig().HeroBattleOverTime * 1000 <= nowTime)
	{
		///> 加时结束，因为如果被对方占领加时就置零，所以当前的occupant就是分数99进入加时的那个队伍
		if (_CheckLastPointCondition(m_occupant))
		{
			_AddOnePoint(true);
			SetState(VS_STATE_END);
			return;
		}
		LogDebug("Check last point failed");
		_ChangeOverTimeState(true); ///> 再次开始加时
	}
	if (m_occupant != 0 && m_startOverTime == 0 && m_lastAddPointTime + GetGlobalConfig().HeroBattleSecond2OnePoint * 1000 <= nowTime)
	{
		m_lastAddPointTime = nowTime;
		_AddOnePoint();
		if (_CheckEnd())
		{
			SetState(VS_STATE_END);
			return;
		}
	}

	switch (m_state)
	{
	case HeroBattleState_Normal:
		{
			_OnTickNormalState();
			break;
		}
	case HeroBattleState_Loot:
		{
			_OnTickLootState();
			break;
		}
	case HeroBattleState_InFight:
		{
			_OnTickInFightState();
			break;
		}
	default:
		{
			break;
		}
	}

	if (m_lastAddAncientPowerTime + 1000 <= nowTime)
	{
		m_lastAddAncientPowerTime = nowTime;
		_UpdateAncientPowerPerSecond();
	}
}

VsStateType SceneHeroBattle::IsAllLoad()
{
	///> 只要有一个人进来，就进入infight状态
	for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		if(m_loadmgr.HaveRole(i->first))
		{
			return VS_STATE_IN_FIGHT;
		}
	}
	return VS_STATE_DUMMY;
}

VsStateType SceneHeroBattle::OnLoadTimeOut()
{
	return VS_STATE_IN_FIGHT;
}

VsStateType SceneHeroBattle::CheckFightEnd()
{
	if (m_occupant == GetTeam1() || m_occupant == GetTeam2()) ///> 已经被占领了
	{
		return VS_STATE_DUMMY;
	}
	if (GetStateStartTime() + GetGlobalConfig().HeroBattleTimeLimitIfNoOccupant <= time(NULL))
	{
		LogWarn("No occupant and Game end");
		return VS_STATE_END;
	}
	return VS_STATE_DUMMY;
}

void SceneHeroBattle::OnStateEnd()
{
	LogDebug("HeroBattle game end, GG");

	_MakeEndGameData();

	GetScene()->SetDontDestory(false);
	SetEnd();
}

void SceneHeroBattle::OnRoleReconnect(Role* pRole)
{
	if (pRole == NULL)
	{
		return;
	}
	LogInfo("role [%llu] reconnected in HeroBattle", pRole->GetID());

	_SyncAllDataToClient(pRole);
}

void SceneHeroBattle::OnHurt(Unit* pTargetUnit, Unit* pCastUnit, double value)
{
	if (GetCurrentStateType() != VS_STATE_IN_FIGHT)
	{
		return;
	}
	if (pTargetUnit->IsRole())
	{
		auto iter = m_roleData.find(pTargetUnit->GetID());
		if (iter != m_roleData.end())
		{
			double maxHP = pTargetUnit->GetAttr(TOTAL_MaxHP);
			if (UtilMath::Isdoublezero(maxHP))
			{
				return;
			}
			double percent = value * 100 / pTargetUnit->GetAttr(TOTAL_MaxHP);
			_AddAncientPower(iter->second, 3, percent);
		}
	}
}

KKSG::ErrorCode SceneHeroBattle::SetHero(UINT64 roleid, UINT32 heroid)
{	
	if (GetCurrentStateType() != VS_STATE_IN_FIGHT)
	{
		return KKSG::ERR_CANTCHOOSEHERO;
	}

	if (!m_loadmgr.HaveRole(roleid))
	{
		LogError("role [%llu] not in this scene, strange error", roleid);
		return KKSG::ERR_UNKNOWN;
	}
	if (m_roleData.find(roleid) == m_roleData.end())
	{
		LogError("role [%llu] is in m_loadmgr but not in roleData, what happen??", roleid);
		return KKSG::ERR_UNKNOWN;
	}

	Role* pRole = GetRole(roleid);
	if (pRole == NULL)
	{
		LogError("role [%llu] is NULL", roleid);
		return KKSG::ERR_UNKNOWN;
	}

	if (HeroBattleMgr::Instance()->GetHeroConf(heroid) == NULL)
	{
		LogWarn("heroid:%u in not in OverWatch.txt", heroid);
		return KKSG::ERR_HERO_INVALID;
	}

	HeroBattleRoleData& data = m_roleData[roleid];

	if (!pRole->Get<CHeroBattleRecord>()->IsHaveHero(heroid))
	{
		bool isHave = false;	
		for (auto i = data.freeWeekHero.begin(); i != data.freeWeekHero.end(); ++i)
		{
			if (heroid == *i)
			{
				isHave = true;
				break;
			}
		}
		for (auto i = data.experienceHero.begin(); i != data.experienceHero.end(); ++i)
		{
			if (heroid == i->first)
			{
				isHave = true;
				break;
			}
		}
		if (!isHave)
		{
			return KKSG::ERR_HERO_INVALID;
		}
	}

	bool alreadyChosen = false;
	for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		if (!m_loadmgr.HaveRole(i->first)) 
		{
			continue;
		}
		if (i->first == roleid)
		{
			continue;
		}
		if (i->second.info.camp() == data.info.camp() && i->second.heroid == heroid)
		{
			alreadyChosen = true;
			break;
		}
	}
	if (GetGlobalConfig().HeroBattleCanChooseSame != 1 && alreadyChosen)
	{
		return KKSG::ERR_HERO_ALREADY_CHOSEN;
	}

	if (data.canChooseHero)
	{
		data.heroid = heroid;	
		if (data.state == 0)
		{
			///> 第一次进场景的变身
			data.state = 1;
			data.canChooseHero = false;
			///> 变身
			pRole->Transform(data.heroid, UseHeroType_HeroBattle);
			///> 能动
			pRole->MakePuppet(false);
			///> 现身
			pRole->MakeInvisible(false);

			_SendGameNotice(pRole, HeroBattleGameNotice2);

			ResetHeroVar(data);
			data.preHero = heroid;
		}

		_SendTeamRoleToClient();
		return KKSG::ERR_SUCCESS;
	}
	return KKSG::ERR_CANTCHOOSEHERO;
}

void SceneHeroBattle::FillResultAndReward(KKSG::HeroBattleResult* result, UINT64 roleId)
{
	if (GetCurrentStateType() != VS_STATE_END)
	{
		return;
	}
	if (result == NULL)
	{
		return;
	}
	auto iter = m_roleData.find(roleId);
	if (iter == m_roleData.end())
	{
		return;
	}
	result->set_mvpid(m_mvpid);
	result->set_mvpheroid(iter->second.heroid);
	result->set_teamid(iter->second.info.camp());
	result->set_losemvpid(m_loseMvpid);
	result->set_kda(iter->second.kda);

	Role* pRole = GetRole(roleId);
	if (pRole == NULL)
	{
		return;
	}

	THeroBattleRoundVar var;
	var.m_killNum = m_datamgr.GetUnitKillCount(pRole->GetID());
	var.m_deadNum = m_datamgr.GetUnitDeadCount(pRole->GetID());
	if (!_CheckEnd())
	{
		///> 平局
		result->set_over(KKSG::HeroBattleOver_Draw);
		var.m_elo = var.m_newElo = pRole->Get<CHeroBattleRecord>()->GetELOPoint();
		var.m_result = TX_BATTLE_DRAW;
	}
	else
	{
		UINT32 winTeam = m_fightTeam1.point >= 100 ? GetTeam1() : GetTeam2();
		double eloPoint = pRole->Get<CHeroBattleRecord>()->GetELOPoint();
		double newELOPoint, otherELO = iter->second.info.camp() == GetTeam1() ? m_fightTeam2.eloPoint : m_fightTeam1.eloPoint;
		if (iter->second.info.camp() == winTeam)
		{
			result->set_over(KKSG::HeroBattleOver_Win);
			newELOPoint = eloPoint + 1.0 * GetGlobalConfig().HeroBattleELOK * (1.0 - 1.0 / (1.0 + pow(10, (otherELO - eloPoint) / 400.0)));
			var.m_result = TX_BATTLE_SUCCESS;
		}
		else
		{
			result->set_over(KKSG::HeroBattleOver_Lose);
			newELOPoint = eloPoint + 1.0 * GetGlobalConfig().HeroBattleELOK * (0.0 - 1.0 / (1.0 + pow(10, (otherELO - eloPoint) / 400.0)));
			var.m_result = TX_BATTLE_FAIL;
		}
		pRole->Get<CHeroBattleRecord>()->SetELOPoint(newELOPoint);
		LogInfo("role [%llu], HeroBattle elopoint, %.2lf -> %.2lf", pRole->GetID(), eloPoint, newELOPoint);
		var.m_elo = eloPoint;
		var.m_newElo = newELOPoint;
		var.m_IsMvp = m_mvpid == pRole->GetID();
	}
	pRole->Get<CHeroBattleRecord>()->PrintHeroBattleRoundTLog(var, m_battleUniqueTag);
	if (result->over() == KKSG::HeroBattleOver_Win)
	{
		BagGiveItemTransition give(pRole);
		give.SetReason(ItemFlow_HeroBattle, ItemFlow_HeroBattle_Reward);

		if (pRole->Get<CHeroBattleRecord>()->GetTodaySpCount() < GetGlobalConfig().HeroBattleSpecialNum
			&& HeroBattleMgr::Instance()->IsInSpecialOpenTime())
		{
			pRole->Get<CHeroBattleRecord>()->AddTodaySpCount();
			for (UINT32 i = 0; i < GetGlobalConfig().HeroBattleSpecialReward.size(); ++i)
			{
				UINT32 dropid = GetGlobalConfig().HeroBattleSpecialReward[i];
				const ItemDesc* desc = ItemDropConfig::Instance()->GetRandomDrop(dropid);
				if (desc == NULL)
				{
					continue;
				}
				KKSG::ItemBrief* brief = result->add_dayjoinreward();
				desc->ToKKSG(brief);
				give.GiveItem(*desc);
			}
		}

		for (UINT32 i = 0; i < GetGlobalConfig().HeroBattleReward.size(); ++i)
		{
			UINT32 dropid = GetGlobalConfig().HeroBattleReward[i];
			const ItemDesc* desc = ItemDropConfig::Instance()->GetRandomDrop(dropid);
			if (desc == NULL)
			{
				continue;
			}
			KKSG::ItemBrief* brief = result->add_winreward();
			desc->ToKKSG(brief);
			give.GiveItem(*desc);
		}

		UINT32 rate = pRole->Get<CHeroBattleRecord>()->GetTodayBigRewardRate();
		UINT32 random = XRandom::randInt(1, 100 + 1);
		if (random <= rate)
		{
			pRole->Get<CHeroBattleRecord>()->AddTodayBigRewardCount();
			ItemDesc desc(GetGlobalConfig().HeroBattleBigReward[0], GetGlobalConfig().HeroBattleBigReward[1], GetGlobalConfig().HeroBattleBigReward[2] > 0);
			KKSG::ItemBrief* brief = result->add_winreward();
			desc.ToKKSG(brief);
			give.GiveItem(desc);

			Notice* notice = new Notice(HeroBattleBigRewardNotice);
			notice->Add(pRole);
			notice->SendRoleGuild(pRole);
		}

		give.NotifyClient();
	}
}

UINT32 SceneHeroBattle::GetWinTeam()
{
	if (!_CheckEnd())
	{
		return 0;
	}
	return m_fightTeam1.point >= 100 ? GetTeam1() : GetTeam2();
}

UINT64 SceneHeroBattle::GetMvpID()
{
	return m_mvpid;
}

void SceneHeroBattle::ResetHeroVar(HeroBattleRoleData& roledata)
{
	UINT64 roleid = roledata.roleid;
	roledata.lastKillNum = m_datamgr.GetUnitKillCount(roleid);
	roledata.lastDamage = m_datamgr.GetUnitTotalDamage(roleid);
	roledata.lastHeroReviveTime = (UINT32)time(NULL);
}

void SceneHeroBattle::PrintHeroTLog(HeroBattleRoleData& roledata, Role* pRole, bool isEnd)
{
	if (pRole == NULL)
	{
		return;
	}
	if (roledata.nextReviveTime != 0) ///> 死亡期不应该打tlog
	{
		return;
	}
	UINT64 roleid = roledata.roleid;
	THeroBattleResultFlow oLog(pRole);
	oLog.SetTransTag(m_battleUniqueTag);

	oLog.m_UseHeroID = roledata.heroid;
	oLog.m_KillNum = m_datamgr.GetUnitKillCount(roleid) - roledata.lastKillNum;
	oLog.m_Damage = m_datamgr.GetUnitTotalDamage(roleid) - roledata.lastDamage;
	oLog.m_LifeTime = (UINT32)time(NULL) - roledata.lastHeroReviveTime;
	oLog.m_Order = roledata.heroOrder;
	oLog.m_IsChange = roledata.isChangeHero;
	oLog.m_IsDead = !isEnd;

	oLog.Do();
}

bool SceneHeroBattle::IsWinRole(UINT64 roleid)
{
	auto iter = m_roleData.find(roleid);
	if (iter == m_roleData.end())
	{
		return false;
	}
	return iter->second.info.camp() == GetWinTeam();
}

void SceneHeroBattle::GMEnd(Role* pRole)
{
	if (pRole == NULL || GetCurrentStateType() != VS_STATE_IN_FIGHT)
	{
		return;
	}
	auto iter = m_roleData.find(pRole->GetID());
	if (iter == m_roleData.end())
	{
		return;
	}
	if (iter->second.info.camp() == GetTeam1())
	{
		m_fightTeam1.point = 100;
		m_occupant = GetTeam1();
	}
	else
	{
		m_fightTeam2.point = 100;
		m_occupant = GetTeam2();
	}
	m_mvpid = pRole->GetID();
	SetState(VS_STATE_END);
}

void SceneHeroBattle::_OnTickNormalState()
{
	bool isChange = _UpdateRoleInCircle();
	UINT32 teamid = _GetSameTeamInCircle();
	if (teamid != m_occupant && teamid != 0 && teamid != (UINT32)-1)
	{
		///> 开始抢夺
		m_lootTeam = teamid;
		m_lootProgress = 0;
		_RecalLootRate();	

		_SetHeroBattleState(HeroBattleState_Loot);

		_SendSyncDataToClient();
		isChange = true;
	}
	else if (teamid == (UINT32)-1 && m_occupant != 0)
	{
		///> 有occupant且圈内双方队玩家都存在，认为进入非occupant一方的Infight状态
		m_lootTeam = m_occupant == GetTeam1() ? GetTeam2() : GetTeam1();
		m_lootProgress = 0;
		m_lootRate = 0;	

		_SetHeroBattleState(HeroBattleState_InFight);

		_SendSyncDataToClient();
		isChange = true;
	}
	if (isChange) _SendInCircleToClient();
}

void SceneHeroBattle::_OnTickLootState()
{
	if (m_lootTeam == 0)
	{
		LogError("In loot state but lootTeam = 0");
	}
	if (m_lootTeam != 0)
	{
		m_lootProgress += m_lootRate;
		if (m_lootProgress >= 100.0)
		{
			if (m_occupant == 0)
			{
				///> 第一次占领
				_SendGameNoticeToAll(HeroBattleGameNotice3);
			}

			m_occupant = m_lootTeam;
			_ClearLootData();

			_SendSyncDataToClient();

			_AddOnePoint(true); ///> 占领后立马加一分(为了防止2个99分互相抢夺导致游戏时间不可控）
			if (_CheckEnd())
			{
				SetState(VS_STATE_END);
				return;
			}

			if (m_startOverTime != 0) ///> 如果当前处于加时需要置零
			{
				_ChangeOverTimeState(false);
			}

			_SetHeroBattleState(HeroBattleState_Normal);
		}
		else
		{
			///> 改变状态
			bool isChange = _UpdateRoleInCircle();
			UINT32 teamid = _GetSameTeamInCircle();
			if (teamid == (UINT32)-1)
			{
				_SetHeroBattleState(HeroBattleState_InFight);

				_SendSyncDataToClient();
				isChange = true;
			}
			else if (teamid == m_lootTeam)
			{
				_RecalLootRate();

				_SendSyncDataToClient();
			}
			else if (teamid == 0 || m_occupant != 0)
			{
				///> 没有人 or occupant在圈内, 进入normal状态
				_ClearLootData();	

				_SetHeroBattleState(HeroBattleState_Normal);

				_SendSyncDataToClient();
				isChange = true;
			}
			else
			{
				///> occupant还未出现，teamid为对方，进入对方Loot状态
				m_lootTeam = teamid;
				m_lootProgress = 0;
				_RecalLootRate();	

				_SetHeroBattleState(HeroBattleState_Loot);

				_SendSyncDataToClient();
				isChange = true;
			}
			if (isChange) _SendInCircleToClient();
		}
	}
}

void SceneHeroBattle::_OnTickInFightState()
{
	bool isChange = _UpdateRoleInCircle();
	UINT32 teamid = _GetSameTeamInCircle();
	if (teamid == (UINT32)-1)
	{
		///> 两方人都在
	}
	else if (teamid == m_lootTeam)
	{
		///> 重新进入抢夺状态
		_RecalLootRate();

		_SetHeroBattleState(HeroBattleState_Loot);

		_SendSyncDataToClient();
		isChange = true;	
	}
	else if (teamid == 0 || m_occupant != 0)
	{
		///> 没有人 or occupant在圈内, 进入normal状态
		_ClearLootData();	

		_SetHeroBattleState(HeroBattleState_Normal);

		_SendSyncDataToClient();
		isChange = true;
	}
	else
	{
		///> occupant还未出现，teamid为对方，进入对方Loot状态
		m_lootTeam = teamid;
		m_lootProgress = 0;
		_RecalLootRate();	

		_SetHeroBattleState(HeroBattleState_Loot);

		_SendSyncDataToClient();
		isChange = true;
	}
	if (isChange) _SendInCircleToClient();
}

void SceneHeroBattle::_SetHeroBattleState(HeroBattleState state)
{
	if (m_state == state)
	{
		return;
	}
	m_state = state;

	switch (m_state)
	{
	case HeroBattleState_Normal:
		{
			break;
		}
	case HeroBattleState_Loot:
		{
			_SendTipsToTeam(HeroBattleTips_StartLoot, m_lootTeam);
			_SendTipsToTeam(HeroBattleTips_Enemy_StartLoot, m_lootTeam == GetTeam1() ? GetTeam2() : GetTeam1());
			break;
		}
	case HeroBattleState_InFight:
		{
			_SendTipsToTeam(HeroBattleTips_StartInFight);
			break;
		}
	default:
		{
			break;
		}
	}
}

void SceneHeroBattle::_AddRoleDie(UINT64 roleid)
{
	auto iter = m_roleData.find(roleid);
	if (iter == m_roleData.end())
	{
		LogError("role [%llu] not in HeroBattle", roleid);
		return;
	}
	iter->second.nextReviveTime = TimeUtil::GetMilliSecond() + GetGlobalConfig().HeroBattleReviveTime * 1000;
	VsKillNotify(roleid, GetGlobalConfig().HeroBattleReviveTime, 0);
}

void SceneHeroBattle::_CheckRoleRevive(UINT64 nowTime)
{
	if (GetCurrentStateType() != VS_STATE_IN_FIGHT)
	{
		return;
	}

	for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		if (!m_loadmgr.HaveRole(i->first)) 
		{
			continue;
		}
		if (i->second.nextReviveTime != 0 && i->second.nextReviveTime <= nowTime)
		{
			_RoleRevive(i->second, false);	
		}
	}
}

void SceneHeroBattle::_RoleRevive(HeroBattleRoleData& data, bool isBeRevive)
{
	Role* pRole = GetRole(data.roleid);
	if (pRole != NULL && pRole->IsDead())
	{
		data.nextReviveTime = 0;
		data.canChooseHero = false;

		///> 拉位置
		if (!isBeRevive)
		{
			TranInitPosition(data.roleid);
		}
		///> 复活
		pRole->Revive();
		///> 变身
		pRole->Transform(data.heroid, UseHeroType_HeroBattle);

		PtcG2C_CloseChooseHeroNtf oPtc;
		pRole->Send(oPtc);

		ResetHeroVar(data);

		++data.heroOrder;
		if (data.preHero == data.heroid)
		{
			data.isChangeHero = false;
		}
		else
		{
			data.isChangeHero = true;
		}

		data.preHero = data.heroid;
	}
	LogDebug("role [%llu] revive in HeroBattle", data.roleid);
}

void SceneHeroBattle::_CheckChooseHero(UINT64 nowTime)
{
	if (GetCurrentStateType() != VS_STATE_IN_FIGHT)
	{
		return;
	}

	for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		if (!m_loadmgr.HaveRole(i->first))
		{
			continue;
		}
		///> 进入选英雄阶段，一段时间没选英雄，随机选择英雄
		if (i->second.state == 0 && i->second.canChooseHero && i->second.enterGameTime + GetGlobalConfig().HeroBattleChooseHeroTime * 1000 <= nowTime)
		{
			Role* pRole = GetRole(i->first);
			if (pRole == NULL)
			{
				LogError("role [%llu] is null", pRole->GetID());
				continue;
			}

			std::set<UINT32> exceptHero;
			for (auto j = m_roleData.begin(); j != m_roleData.end(); ++j)
			{
				if (j->second.info.camp() == i->second.info.camp() && j->second.heroid != 0)
				{
					exceptHero.insert(j->second.heroid);
				}
			}
			KKSG::ErrorCode errorCode = SetHero(pRole->GetID(), pRole->Get<CHeroBattleRecord>()->GetRandomHero(i->second.freeWeekHero, i->second.experienceHero, exceptHero));
			if (errorCode != KKSG::ERR_SUCCESS)
			{
				LogError("role [%llu] set hero error [%d]", pRole->GetID(), errorCode);
				continue;
			}
			PtcG2C_CloseChooseHeroNtf oPtc;
			pRole->Send(oPtc);	
		}
	}
}

void SceneHeroBattle::_SendGameNotice(Role* pRole, CNoticeType type)
{
	Notice notice(type);
	notice.SendSelf(pRole);
}

void SceneHeroBattle::_SendGameNoticeToAll(CNoticeType type)
{
	for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		if (!m_loadmgr.HaveRole(i->first)) 
		{
			continue;
		}
		Role* pRole = GetRole(i->first);
		if (pRole == NULL)
		{
			continue;
		}
		_SendGameNotice(pRole, type);
	}
}

UINT32 SceneHeroBattle::_GetSameTeamInCircle()
{
	UINT32 teamid = 0;
	for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		if (!m_loadmgr.HaveRole(i->first)) 
		{
			continue;
		}
		Role* pRole = GetRole(i->first);
		if (pRole == NULL)
		{
			continue;
		}
		if (!pRole->IsDead() && i->second.isInCircle)
		{
			if (teamid == 0)
			{
				teamid = i->second.info.camp();
			}
			else if (teamid != i->second.info.camp())
			{
				return (UINT32)-1; ///> 圈里有不同玩家
			}
		}
	}
	return teamid;
}

UINT32 SceneHeroBattle::_GetTeamRoleNumInCircle(UINT32 teamid)
{
	UINT32 num = 0;
	for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		if (!m_loadmgr.HaveRole(i->first)) 
		{
			continue;
		}
		Role* pRole = GetRole(i->first);
		if (pRole == NULL)
		{
			continue;
		}
		if (!pRole->IsDead() && i->second.isInCircle && i->second.info.camp() == teamid)
		{
			num++;
		}
	}
	return num;
}

bool SceneHeroBattle::_UpdateRoleInCircle()
{
	bool isChange = false;
	for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		if (!m_loadmgr.HaveRole(i->first)) 
		{
			continue;
		}
		bool isInCircle = _IsInCircle(i->first);
		if (i->second.isInCircle != isInCircle)
		{
			i->second.isInCircle = isInCircle;
			isChange = true;
		}
	}
	return isChange;
}

bool SceneHeroBattle::_IsInCircle(UINT64 roleid)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(roleid);
	if (pRole == NULL)
	{
		LogError("role [%llu] pointer in HeroBattle is NULL");
		return false;
	}
	if (m_centerConf.SceneID == 0) ///> 不存在的
	{
		return false;
	}
	const Vector3& v3 = pRole->GetXObject()->GetPosition_p();
	if (m_centerConf.CenterType == 1)
	{
		float dis = Vector3::Distance(v3, Vector3(m_centerConf.Center[0], v3.y, m_centerConf.Center[2]));
		return dis <= m_centerConf.Param[0];
	}
	else if (m_centerConf.CenterType == 2)
	{
		return std::fabs(v3.x - m_centerConf.Center[0]) <= m_centerConf.Param[0] / 2.0
			&& std::fabs(v3.z - m_centerConf.Center[2]) <= m_centerConf.Param[1] / 2.0;
	}
	return false;
}

void SceneHeroBattle::_RecalLootRate()
{
	UINT32 num = _GetTeamRoleNumInCircle(m_lootTeam);
	if (num == 0)
	{
		return;
	}
	if (GetGlobalConfig().HeroBattleLootRate.size() < num)
	{
		num = GetGlobalConfig().HeroBattleLootRate.size();
	}
	m_lootRate = 1.0 * GetGlobalConfig().HeroBattleLootRate[num - 1] * OneFrameTime / 1000.0;
}

bool SceneHeroBattle::_CheckLastPointCondition(UINT32 teamid)
{
	UINT32 tmpteamid = _GetSameTeamInCircle();
	return tmpteamid == 0 || tmpteamid == teamid;
}

bool SceneHeroBattle::_CheckEnd()
{
	return m_fightTeam1.point >= 100 || m_fightTeam2.point >= 100;
}

void SceneHeroBattle::_SendCanUseHeroToClient(Role* pRole)
{
	if (pRole == NULL || m_roleData.find(pRole->GetID()) == m_roleData.end())
	{
		return;
	}
	PtcG2C_HeroBattleCanUseHero oPtc;

	const std::unordered_set<UINT32>& hero = pRole->Get<CHeroBattleRecord>()->GetHero();
	for (auto i = hero.begin(); i != hero.end(); ++i)
	{
		oPtc.m_Data.add_havehero(*i);
	}
	
	HeroBattleRoleData& data = m_roleData[pRole->GetID()];
	for (auto i = data.freeWeekHero.begin(); i != data.freeWeekHero.end(); ++i)
	{
		oPtc.m_Data.add_freehero(*i);
	}
	for (auto i = data.experienceHero.begin(); i != data.experienceHero.end(); ++i)
	{
		oPtc.m_Data.add_experiencehero(i->first);
	}

	oPtc.m_Data.set_leftchoosetime(GetGlobalConfig().HeroBattleChooseHeroTime);

	pRole->Send(oPtc);
}

void SceneHeroBattle::_SendTeamRoleToClient(Role* pRole)
{
	PtcG2C_HeroBattleTeamRoleNtf oPtc;
	oPtc.m_Data.set_team1(GetTeam1());
	oPtc.m_Data.set_team2(GetTeam2());
	for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		if (!m_loadmgr.HaveRole(i->first))
		{
			continue;
		}
		Role* role = GetRole(i->first);
		if (role == NULL)
		{
			continue;
		}
		KKSG::HeroBattleTeamMember* member;
		if (i->second.info.camp() == GetTeam1())
		{
			member = oPtc.m_Data.add_members1();
		}
		else 
		{
			member = oPtc.m_Data.add_members2();
		}
		member->set_uid(i->first);
		member->set_heroid(i->second.heroid);
		member->set_name(i->second.info.name());
		member->set_killnum(role->Statistics().GetKillCount());
		member->set_deathnum(role->Statistics().GetDeathCount());
		member->set_assitnum(role->Statistics().GetAssistNum());
	}

	if (pRole == NULL) GetScene()->Broadcast(oPtc);
	else pRole->Send(oPtc);
}

void SceneHeroBattle::_SendTeamDataToClient(Role* pRole)
{
	PtcG2C_HeroBattleTeamMsgNtf oPtc;

	KKSG::HeroBattleTeamData* data1 = oPtc.m_Data.add_teamdata();
	data1->set_teamid(GetTeam1());
	data1->set_headcount(m_datamgr.GetGroupKillCount(GetTeam1()));
	data1->set_point(m_fightTeam1.point);

	KKSG::HeroBattleTeamData* data2 = oPtc.m_Data.add_teamdata();
	data2->set_teamid(GetTeam2());
	data2->set_headcount(m_datamgr.GetGroupKillCount(GetTeam2()));
	data2->set_point(m_fightTeam2.point);

	if (pRole == NULL) GetScene()->Broadcast(oPtc);
	else pRole->Send(oPtc);
}

void SceneHeroBattle::_SendSyncDataToClient(Role* pRole)
{
	PtcG2C_HeroBattleSyncNtf oPtc;
	oPtc.m_Data.set_occupant(m_occupant);
	oPtc.m_Data.set_lootteam(m_lootTeam);
	oPtc.m_Data.set_lootprogress(m_lootProgress);
	oPtc.m_Data.set_isinfight(m_state == HeroBattleState_InFight);

	if (pRole == NULL) GetScene()->Broadcast(oPtc);
	else pRole->Send(oPtc);
}

void SceneHeroBattle::_SendInCircleToClient(Role* pRole)
{
	PtcG2C_HeroBattleInCircleNtf oPtc;
	for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		if (!m_loadmgr.HaveRole(i->first)) 
		{
			continue;
		}
		if (i->second.isInCircle)
		{
			oPtc.m_Data.add_roleincircle(i->first);
		}
	}

	if (pRole == NULL) GetScene()->Broadcast(oPtc);
	else pRole->Send(oPtc);
}

void SceneHeroBattle::_SyncAllDataToClient(Role* pRole)
{
	if (pRole == NULL)
	{
		return;
	}
	if (!pRole->IsWatcher()) _SendCanUseHeroToClient(pRole);
	_SendTeamRoleToClient(pRole);
	_SendTeamDataToClient(pRole);
	_SendSyncDataToClient(pRole);
	_SendInCircleToClient(pRole);
	_SendAncientPowerToClient(pRole);

	UINT64 nowTime = TimeUtil::GetMilliSecond();
	PtcG2C_HeroBattleOverTime oTimePtc;
	if (m_startOverTime == 0 || m_startOverTime + GetGlobalConfig().HeroBattleOverTime * 1000 <= nowTime)
	{
		oTimePtc.m_Data.set_millisecond(0);
	}
	else
	{
		oTimePtc.m_Data.set_millisecond(m_startOverTime + GetGlobalConfig().HeroBattleOverTime * 1000 - nowTime);
	}
	pRole->Send(oTimePtc);
}

void SceneHeroBattle::_SendAncientPowerToClient(Role* pRole)
{
	PtcG2C_HeroBattleAncientPower oPtc;
	for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		oPtc.m_Data.add_roleids(i->first);
		oPtc.m_Data.add_ancientpower(i->second.ancientPower);
	}
	if (pRole == NULL) GetScene()->Broadcast(oPtc);
	else pRole->Send(oPtc);
}

void SceneHeroBattle::_AddOnePoint(bool force)
{
	if (m_occupant == 0)
	{
		return;
	}
	if (m_occupant == GetTeam1())
	{
		if (force || m_fightTeam1.point != 99) m_fightTeam1.point++; 
		else
		{
			if (_CheckLastPointCondition(GetTeam1()))
			{
				m_fightTeam1.point++;
			}
			else
			{
				_ChangeOverTimeState(true);
			}
		}
	}
	else
	{
		if (force || m_fightTeam2.point != 99) m_fightTeam2.point++;
		else
		{
			if (_CheckLastPointCondition(GetTeam2()))
			{
				m_fightTeam2.point++;
			}
			else
			{
				_ChangeOverTimeState(true);
			}
		}
	}
	_SendTeamDataToClient();
}

void SceneHeroBattle::_ChangeOverTimeState(bool isStart)
{
	PtcG2C_HeroBattleOverTime oPtc;
	if (isStart)
	{
		m_overTimeCount++;
		m_startOverTime = TimeUtil::GetMilliSecond();
		oPtc.m_Data.set_millisecond(GetGlobalConfig().HeroBattleOverTime * 1000);
		LogDebug("start overtime");

		_SendTipsToTeam(HeroBattleTips_StartOverTime);
		if (m_overTimeCount == 1)
		{
			_SendGameNoticeToAll(HeroBattleGameNotice5);
		}
	}
	else
	{
		m_startOverTime = 0;
		oPtc.m_Data.set_millisecond(0);
		LogDebug("end overtime");
	}
		
	GetScene()->Broadcast(oPtc);
}

void SceneHeroBattle::_ClearLootData()
{
	m_lootTeam = 0;
	m_lootProgress = 0;
	m_lootRate = 0;
}

void SceneHeroBattle::_CalcuKDA()
{
	for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		const CombatStatistics* stat = GetCombatStatistics(i->first);
		if (stat == NULL)
		{
			continue;
		}
		UINT32 tmpkill = stat->GetKillCount();
		UINT32 tmpdead = stat->GetDeathCount();
		UINT32 tmpassist = stat->GetAssistNum();
		i->second.kda = (tmpkill * 2 + tmpassist) / (tmpdead + 1.0f);
	}
}

void SceneHeroBattle::_MakeEndGameData()
{
	_CalcuKDA();
	bool isDraw = false;
	UINT32 winTeam = 0;
	if (!_CheckEnd())
	{
		///> 没有一组分数到达100，目前只会出现的情况是没有人占领，到时间平局
		isDraw = true;
		if (m_mvpid == 0)				///> 不等于0是用gm指令
		{	
			m_mvpid = _GetMVPID();
			winTeam = GetRoleCamp(m_mvpid);
		}
	}
	else 
	{
		winTeam = m_fightTeam1.point >= 100 ? GetTeam1() : GetTeam2();
		if (m_mvpid == 0)						///> 不等于0是用gm指令	
		{
			m_mvpid = _GetMVPID(winTeam);
		}
	}

	if (winTeam == GetTeam1())
	{
		m_loseMvpid = _GetMVPID(GetTeam2());
	}
	else
	{
		m_loseMvpid = _GetMVPID(GetTeam1());
	}
	
	KKSG::HeroBattleOneGame teamGame;
	teamGame.set_mvpid(m_mvpid);

	for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		const HeroBattleRoleData& data = i->second;
		KKSG::RoleSmallInfo info;
		info.set_roleid(data.info.roleid());
		info.set_rolelevel(data.info.level());
		info.set_rolename(data.info.name());
		info.set_roleprofession(data.info.profession());
		if (data.info.camp() == GetTeam1())
		{
			teamGame.add_team1()->CopyFrom(info);
		}
		else
		{
			teamGame.add_team2()->CopyFrom(info);
		}
	}

	for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		if (!m_loadmgr.HaveRole(i->first))
		{
			continue;
		}
		Role* pRole = GetRole(i->first);
		if (pRole == NULL)
		{
			continue;
		}

		if (isDraw) teamGame.set_over(KKSG::HeroBattleOver_Draw);
		else
		{
			teamGame.set_over(i->second.info.camp() == winTeam ? KKSG::HeroBattleOver_Win : KKSG::HeroBattleOver_Lose);
		}

		pRole->Get<CHeroBattleRecord>()->AddGameRecord(teamGame, m_datamgr.GetUnitKillCount(i->first));

		PrintHeroTLog(i->second, pRole, true);
	}

	// 再比赛中获得己方MVP的玩家，举报计数-3，最低为0
	if (m_mvpid)
	{
		PtcG2M_ReportBadPlayer2Ms ptc;
		ptc.m_Data.set_roleid(m_mvpid);
		ptc.m_Data.set_scenetype(this->GetScene()->GetSceneType());
		int score = -GetGlobalConfig().ReportHeroBattleMvpDelNum;
		ptc.m_Data.set_score(score);
		ptc.m_Data.set_ismvp(true);
		MSLink::Instance()->SendTo(m_mvpid, ptc);
	}

    // 目标奖励
    {
        std::map<UINT32, double> teamdamage;
        for (auto itor = m_roleData.begin(); itor != m_roleData.end(); ++itor)
        {
            const CombatStatistics* stat = GetCombatStatistics(itor->first);
            if (stat == NULL)
            {
                continue;
            }
            teamdamage[itor->second.info.camp()] += stat->GetTotalDamage();
        }


        for (auto itor = m_roleData.begin(); itor != m_roleData.end(); ++itor)
        {
            const CombatStatistics* stat = GetCombatStatistics(itor->first);
            if (stat == NULL)
            {
                continue;
            }
            
            GoalAwardsConfig::Instance()->SetValue(itor->first, GOAL_AWARD_ID_101, (double)stat->GetKillCount(), 0);
            GoalAwardsConfig::Instance()->SetValue(itor->first, GOAL_AWARD_ID_102, (double)stat->GetAssistNum(), 0);
            GoalAwardsConfig::Instance()->SetValue(itor->first, GOAL_AWARD_ID_103, teamdamage[itor->second.info.camp()], 0);

            GoalAwardsConfig::Instance()->SetValue(itor->first, GOAL_AWARD_ID_104, (double)stat->GetKillCount(), 0);
            GoalAwardsConfig::Instance()->SetValue(itor->first, GOAL_AWARD_ID_105, (double)stat->GetAssistNum(), 0);
            GoalAwardsConfig::Instance()->SetValue(itor->first, GOAL_AWARD_ID_106, stat->GetTotalDamage(), 0);

            if(m_mvpid == itor->first)
            {
                GoalAwardsConfig::Instance()->SetValue(itor->first, GOAL_AWARD_ID_107, 1.f, 0);
            }

            if(!isDraw && itor->second.info.camp() == winTeam)
            {
                GoalAwardsConfig::Instance()->SetValue(itor->first, GOAL_AWARD_ID_108, 1.f, 0);
            }
        }
    }
}

UINT64 SceneHeroBattle::_GetMVPID(UINT32 teamid)
{
	UINT64 mvpID = 0;
	float mvpkda = 0.0f;
	double mvpdamage = 0.0;

	for(auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		if (teamid != 0 && i->second.info.camp() != teamid)
		{
			continue;
		}

		const CombatStatistics* stat = GetCombatStatistics(i->first);
		if (stat == NULL)
		{
			continue;
		}
		double tmpdamage = stat->GetTotalDamage();	
		float kda = i->second.kda;
		bool isBetter = false;
		if (mvpID == 0 || kda > mvpkda)
		{
			isBetter = true;
		}
		else if (kda == mvpkda)
		{
			if (tmpdamage > mvpdamage || (tmpdamage == mvpdamage && XRandom::randInt(0, 1000) < 500))
			{
				isBetter = true;
			}
		}
		if (isBetter)
		{
			mvpID = i->first;
			mvpkda = kda;
			mvpdamage = tmpdamage;
		}
	}

	return mvpID;
}

void SceneHeroBattle::_SendTipsToTeam(HeroBattleTipsEnum en, UINT32 teamid)
{
	for(auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		if (!m_loadmgr.HaveRole(i->first))
		{
			continue;
		}
		if (teamid != (UINT32)-1 && i->second.info.camp() != teamid)
		{
			continue;
		}
		Role* pRole = GetRole(i->first);
		if (pRole == NULL)
		{
			continue;
		}
		PtcG2C_HeroBattleTipsNtf oPtc;
		oPtc.m_Data.set_id((UINT32)en);
		pRole->Send(oPtc);
	}
}

UINT32 SceneHeroBattle::GetRoleCamp(UINT64 roleid)
{
	auto iter = m_roleData.find(roleid);	
	if (iter == m_roleData.end())
	{
		return 0;
	}
	return iter->second.info.camp();
}

void SceneHeroBattle::AddExperienceHero(UINT64 roleid, UINT32 heroid, UINT32 endTime)
{
	auto iter = m_roleData.find(roleid);
	if (iter == m_roleData.end())
	{
		return;
	}
	iter->second.experienceHero[heroid] = endTime;
}

const CombatStatistics* SceneHeroBattle::GetCombatStatistics(UINT64 roleId)
{
	return m_datamgr.GetStatistics(roleId, GetScene());
}

HeroBattleRoleData* SceneHeroBattle::GetRoleData(UINT64 roleId)
{
	auto it = m_roleData.find(roleId);
	if (it == m_roleData.end())
	{
		return NULL;
	}
	return &it->second;
}

KKSG::ErrorCode SceneHeroBattle::CheckReportPlayer(UINT64 roleId, UINT64 badRoleId, int& num)
{
	if (GetCurrentStateType() != VS_STATE_END)
	{
		return KKSG::ERR_FAILED;
	}
	num = 1;
	HeroBattleRoleData* pData1 = GetRoleData(roleId);
	if (pData1 == NULL)
	{
		return KKSG::ERR_FAILED;
	}
	HeroBattleRoleData* pData2 = GetRoleData(badRoleId);
	if (pData2 == NULL)
	{
		return KKSG::ERR_FAILED;
	}
	if (pData2->reportIds.find(roleId) != pData2->reportIds.end())
	{
		return KKSG::ERR_ALREADY_REPORTED;
	}
	pData2->reportIds.insert(roleId);

	const MailConf* conf = MailConfig::Instance()->GetMailConf(MailConf_HeroReportPlayerBack);
	if (conf)
	{
		std::string mailcontent = MailConfig::Instance()->ReplaceString("%s", pData2->info.name(), conf->m_content);
		MailSender sender;
		sender.SetContent(Mail_System, conf->m_title, mailcontent);
		sender.Send(roleId);
	}

	// 1. 对评分大于等于2.0的玩家举报无效
	if (pData2->kda >= GetGlobalConfig().ReportHeroBattleInvalidKda)
	{
		num = 0;
		return KKSG::ERR_SUCCESS;
	}
	// 2.失败方举报胜利方无效
	UINT32 winCamp = GetWinTeam();
	if (GetRoleCamp(roleId) != winCamp && GetRoleCamp(badRoleId) == winCamp)
	{
		num = 0;
		return KKSG::ERR_SUCCESS;
	}
	// 3.失败方玩家举报举报己方评分最低的玩家举报数+1
	if (GetRoleCamp(roleId) != winCamp && GetRoleCamp(badRoleId) != winCamp)
	{
		UINT32 loseCamp = GetRoleCamp(roleId);
		float kda = 0.0f;
		UINT64 leastKdaRoleId = 0;
		for (auto it = m_roleData.begin(); it != m_roleData.end(); ++it)
		{
			HeroBattleRoleData& hr = it->second;
			if (hr.info.camp() != loseCamp)
			{
				continue;
			}
			if (leastKdaRoleId == 0 || hr.kda < kda)
			{
				leastKdaRoleId = hr.roleid;
				kda = hr.kda;
			}
		}
		if (badRoleId == leastKdaRoleId)
		{
			num += GetGlobalConfig().ReportHeroBattleLeastNum;
		}
	}
	// 4.玩家被举报，己方击杀数大于等于2，且自己击杀和助攻都为0的玩家，举报数+2（视为挂机或者消极比赛）
	const CombatStatistics* pStat = m_datamgr.GetStatistics(badRoleId, GetScene());
	if (pStat && pStat->GetKillCount() == 0 && pStat->GetAssistNum() == 0)
	{
		UINT32 badCamp = GetRoleCamp(badRoleId);
		UINT32 totalKill = 0;
		for (auto it = m_roleData.begin(); it != m_roleData.end(); ++it)
		{
			HeroBattleRoleData& hr = it->second;
			if (hr.info.camp() != badCamp)
			{
				continue;
			}
			totalKill += m_datamgr.GetUnitKillCount(hr.roleid);
		}
		if (totalKill >= GetGlobalConfig().ReportHeroCampTotalKillNum)
		{
			num += GetGlobalConfig().ReportHeroCampTotalAddNum;
		}
	}	
	
	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode SceneHeroBattle::SelectHeroAncientPower(Role* pRole, UINT32 selectPower)
{
	if (pRole == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}
	auto iter = m_roleData.find(pRole->GetID());
	if (iter == m_roleData.end())
	{
		return KKSG::ERR_UNKNOWN;
	}
	HeroBattleRoleData& data = iter->second;
	if (data.heroid == 0)
	{
		LogWarn("role [%llu] in HeroBattle not select hero, but want to select ancient power", pRole->GetID());
		return KKSG::ERR_FAILED;
	}	
	HeroBattleAncientConf* conf = HeroBattleMgr::Instance()->GetAncientPowerConf(data.heroid);
	if (conf == NULL)
	{
		LogWarn("heroid [%u] can't find in HeroBattleAncientPower.txt", data.heroid);
		return KKSG::ERR_UNKNOWN;
	}
	if (pRole->IsDead())
	{
		return KKSG::ERR_ANCIENTPOWER_DEAD;
	}
	if (data.ancientPower < 100.0)
	{
		return KKSG::ERR_ANCIENTPOWER_LESSPOINT;
	}

	std::vector<UINT32>* addBuff;
	switch (selectPower)
	{
	case 1:
		{
			addBuff = &conf->Buff1;
			break;
		}
	case 2:
		{
			addBuff = &conf->Buff2;
			break;
		}
	case 3:
		{
			addBuff = &conf->Buff3;
			break;
		}
	default:
		{
			return KKSG::ERR_FAILED;
		}
	}

	data.ancientPower = 0;
	for (auto i = addBuff->begin(); i != addBuff->end(); ++i)
	{
		XAddBuffActionArgs args;
		args.singleBuff.buffID = *i;
		args.singleBuff.BuffLevel = 1;
		args.Caster = pRole->GetID();
		pRole->GetXObject()->DispatchAction(&args);
	}

	return KKSG::ERR_SUCCESS;
}

void SceneHeroBattle::SendToTeamRole(UINT32 teamid, const CProtocol& oPtc)
{
	for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		if (i->second.info.camp() != teamid)
		{
			continue;	
		}
		Role* pRole = GetRole(i->first);
		if (pRole == NULL)
		{
			continue;
		}
		pRole->Send(oPtc);
	}
}

KKSG::ErrorCode SceneHeroBattle::SendSignal(Role* pRole, UINT32 type)
{
	if (GetCurrentStateType() != VS_STATE_IN_FIGHT)
	{
		return KKSG::ERR_UNKNOWN;
	}
	if (pRole == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}
	auto iter = m_roleData.find(pRole->GetID());
	if (iter == m_roleData.end())
	{
		return KKSG::ERR_UNKNOWN;
	}

	HeroBattleRoleData& data = iter->second;

	UINT64 nowTime = TimeUtil::GetMilliSecond();
	if (data.signalCount + 1 > GetGlobalConfig().MobaSignaTimelLimit[0])
	{
		if (data.firstSendSignalTime + GetGlobalConfig().MobaSignaTimelLimit[1] * 1000 >= nowTime)
		{
			return KKSG::ERR_MOBA_SIGNAL_TOOFAST;
		}
		data.signalCount = 1;
		data.firstSendSignalTime = nowTime;
	}
	else
	{
		++data.signalCount;
	}

	PtcG2C_MobaSignalBroadcast oPtc;
	oPtc.m_Data.set_type(type);
	oPtc.m_Data.set_uid(pRole->GetID());
	const Vector3& vec3 = pRole->GetXObject()->GetPosition_p();
	UINT32 posxz = (int)(vec3.z * 100 + 0.5f);
	posxz |= ((int)(vec3.x * 100 + 0.5f) << 16);
	oPtc.m_Data.set_posxz(posxz);
	SendToTeamRole(data.info.camp(), oPtc);

	return KKSG::ERR_SUCCESS;
}

void SceneHeroBattle::_UpdateAncientPowerPerSecond()
{
	for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		if (!m_loadmgr.HaveRole(i->first))
		{
			continue;
		}
		Role* pRole = GetRole(i->first);
		if (pRole == NULL || pRole->IsDead())
		{
			continue;
		}
		_AddAncientPower(i->second, 1);
		if (_IsInCircle(i->first))
		{
			_AddAncientPower(i->second, 2);
		}
	}
	_SendAncientPowerToClient();
}

void SceneHeroBattle::_AddAncientPower(HeroBattleRoleData& data, UINT32 type, double percent)
{
	HeroBattleAncientConf* conf = HeroBattleMgr::Instance()->GetAncientPowerConf(data.heroid);
	if (conf == NULL)
	{
		return;
	}
	if (type == 1)
	{
		data.ancientPower += conf->AddPowerPerSecond;
	}
	else if (type == 2)
	{
		data.ancientPower += conf->AddPowerInCircle;
	}
	else if (type == 3)
	{
		data.ancientPower += conf->AddPowerBeHit * percent;
	}
	if (data.ancientPower > 100.0)
	{
		data.ancientPower = 100.0;
	}
}