#include "pch.h"
#include "scenevsbase.h"
#include "pk/ptcg2c_pkpreparentf.h"
#include "scene.h"
#include "unit/unit.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "scenepk.h"
#include "scenecustompk.h"
#include "scenepvp.h"
#include "scenegmf.h"
#include "sceneskycity.h"
#include "scenereswar.h"
#include "scenegpr.h"
#include "scenehorserace.h"
#include "sceneswitch.h"
#include "unit/dummyrolemanager.h"
#include "unit/attributewatcher.h"
#include "unit/dummyrole.h"
#include "gamelogic/robotgenerator.h"
#include "gamelogic/rolesummarymgr.h"
#include "scenemanager.h"
#include "component/XSkillComponent.h"
#include "pvp/ptcg2c_pvpbattlekill.h"
#include "sceneinvfight.h"
#include "sceneherobattle.h"
#include "scenecastle.h"
#include "sceneleague.h"
#include "scenepktwo.h"
#include "scenecustompktwo.h"
#include "scenemobabattle.h"
#include "bigmeleefight.h"
#include "battlefieldfight.h"
#include "config.h"
#include "sceneweekend4v4horseracing.h"
#include "sceneweekend4v4crazybomb.h"
#include "sceneweekend4v4livechallenge.h"
#include "sceneweekend4v4ghostaction.h"
#include "sceneweekend4v4monsterfight.h"
#include "sceneweekend4v4duck.h"
#include "scene/scenesurvive.h"

#define VS_ENTER_TIMEOUT (20 + PREPARE_TIMEOUT) 

SceneVsBase::SceneVsBase(Scene* scene)
{
	m_scene = scene;

	m_stateStartTime = time_t(0);
	m_IsEnd = false;
	m_lockNum = 0;
	m_lastVsUpdateTime = time_t(0);
}

SceneVsBase::~SceneVsBase()
{
	for(size_t i = 0; i < m_usedRobotSummary.size(); ++i)
	{
		RobotGenerator::Instance()->FreeRoleSummary(m_usedRobotSummary[i]);
	}
	m_usedRobotSummary.clear();
}

void SceneVsBase::Fight321Ntf(int flag)
{
	if(NULL == m_scene)
	{
		return ;
	}

	PtcG2C_PkPrepareNtf ntf;
	ntf.m_Data.set_beginorend(flag);
	m_scene->Broadcast(ntf, NULL);
	
	const std::list<Role*>& players = m_scene->GetAllRoles();
	for (auto i = players.begin(); i != players.end(); ++i)
	{
		MakePuppet(*i, (0 == flag));
	}

	std::vector<DummyRole *> dummys;
	m_scene->GetDummyRoles(dummys);
	for(size_t i = 0; i < dummys.size(); ++i)
	{
		MakePuppet(dummys[i], (0 == flag));		
	}
}

void SceneVsBase::MakePuppet(Unit* pUnit, bool puppet, bool notify/* = true*/)
{
	if(NULL == pUnit)
	{
		return ;
	}

	//if(IsPuppet(pUnit->GetID()) == puppet)
	//{
	//	return;//忽略
	//}

	//SetPuppet(pUnit->GetID(), puppet);

	if(pUnit->IsRole())
	{
		pUnit->MakePuppet(puppet, notify);
	}
	else if(pUnit->IsDummyRole())
	{
		//SSWarn << " name = " << ((DummyRole*)pUnit)->GetName() << " puppet = " << (int)(puppet) << END;

		((DummyRole*)(pUnit))->EnableAI(!puppet);
	}
}

void SceneVsBase::AddBuff(UINT64 roleID, int buffid, int bufflevel)
{
	Role* pRole = GetRole(roleID);
	if(pRole && pRole->GetXObject())
	{
		XAddBuffActionArgs args;
		args.singleBuff.buffID = buffid;
		args.singleBuff.BuffLevel = bufflevel;
		args.Caster = roleID;
		pRole->GetXObject()->DispatchAction(&args);

		//SSDebug << __FUNCTION__ << " username = " << pRole->GetName() << " buffid = " << buffid << " bufflevel = " << bufflevel << END;
	}
}

bool SceneVsBase::Init(const KKSG::SceneSwitchData& roData)
{
	//sub-class to do
	return true;
}

bool SceneVsBase::Init(const KKSG::CreateBattleParam& roParam)
{
	//sub-class to do
	return true;
}

void SceneVsBase::VsOnEvent(const VsEvent& event)
{
	SSInfo << __FUNCTION__ << " sceneid = " << GetScene()->GetSceneID() << " roleid = " << event.roleid << " type = " << event.type << END;
	if(IsEnd())
	{
		EndOnEvent(event);
		return ;
	}
	if(Locked())//VsUpdate 禁止触发 VsOnEvent 
	{
		return;
	}

	OnEvent(event);

	SetLastEvent(event);
	VsUpdate(true);
	ClearLastEvent();
}

bool SceneVsBase::VsUpdate(bool vsevent/* = false*/)
{
	if(IsEnd())
	{
		return true;//结束
	}
	if(Locked())
	{
		return false;
	}
	AddLock();

	time_t now = time(NULL);
	if(now >= m_lastVsUpdateTime + GetUpInterval() || vsevent)
	{
		m_lastVsUpdateTime = now;
		Update();
	}

	ReduceLock();

	return false;
}

SceneVsBase* SceneVsBase::CreateVsHandler(Scene* pScene)
{
	switch(pScene->GetSceneType())
	{
	case KKSG::SCENE_PK:
		{
			return (new ScenePk(pScene));
		}
		break;
	case KKSG::SCENE_PKTWO:
		{
			return (new ScenePkTwo(pScene));
		}
		break;
	case KKSG::SCENE_CUSTOMPKTWO:
		{
			return (new SceneCustomPkTwo(pScene));
		}
		break;
	case KKSG::SCENE_CUSTOMPK:
		{
			return (new SceneCustomPk(pScene));
		}
		break;
	case KKSG::SCENE_MOBA:
		{
			return (new SceneMobaBattle(pScene));
		}
		break;
	case KKSG::SCENE_PVP:
		{
			return (new ScenePvp(pScene));
		}
		break;
	case KKSG::SCENE_GMF:
		{
			return (new SceneGMF(pScene));
		}
		break;
	case KKSG::SKYCITY_FIGHTING:
		{
			return (new SceneSkyCity(pScene));
		}
		break;
	case KKSG::SCENE_GPR:
		{
			return (new SceneGPR(pScene));
		}
		break;
	case KKSG::SCENE_RESWAR_PVP:
		{
			return (new SceneResWar(pScene));
		}
		break;
	case KKSG::SCENE_INVFIGHT:
		{
			return (new SceneInvFigtht(pScene));
		}
		break;
	case KKSG::SCENE_HORSE_RACE:
		{
			return (new SceneHorseRace(pScene));
		}
		break;
	case KKSG::SCENE_HEROBATTLE:
		{
			return (new SceneHeroBattle(pScene));
		}
		break;
	case KKSG::SCENE_CASTLE_WAIT:
	case KKSG::SCENE_CASTLE_FIGHT:
		{
			return (new SceneCastle(pScene));
		}
		break;
	case KKSG::SCENE_LEAGUE_BATTLE:
		{
			return new SceneLeague(pScene);
		}
		break;
    case KKSG::SCENE_WEEKEND4V4_MONSTERFIGHT:
        {
            return new SceneWeekEnd4v4MonsterFight(pScene);
        }
        break;
    case KKSG::SCENE_WEEKEND4V4_GHOSTACTION:
        {
            return new SceneWeekEnd4v4GhostAction(pScene);
        }
        break;
    case KKSG::SCENE_WEEKEND4V4_DUCK:
        {
            return new SceneWeekEnd4v4Duck(pScene);
        }
        break;
    case KKSG::SCENE_WEEKEND4V4_LIVECHALLENGE:
        {
            return new SceneWeekEnd4v4LiveChallenge(pScene);
        }
        break;
    case KKSG::SCENE_WEEKEND4V4_CRAZYBOMB:
        {
            return new SceneWeekEnd4v4CrazyBomb(pScene);
        }
        break;
    case KKSG::SCENE_WEEKEND4V4_HORSERACING:
        {
            return new SceneWeekEnd4v4HorseRacing(pScene);
        }
        break;
	case KKSG::SCENE_BIGMELEE_FIGHT:
		{
			return new SceneBMFight(pScene);
		}
	case KKSG::SCENE_BATTLEFIELD_FIGHT:
		{
			return new SceneBFFight(pScene);
		}
		break;
	case KKSG::SCENE_SURVIVE:
		{
			return new SceneSurvive(pScene);
		}
		break;
	default:{}
	}
	return NULL;
}

void SceneVsBase::SetState(VsStateType type)
{
	if (type == m_statemgr.GetCurrentState()->GetStateType())
	{
		return;
	}

	switch(type)
	{
	case VS_STATE_WAIT_LOAD:
		{
			m_statemgr.SetState(&VsWaitLoad::GlobalVsWaitLoad);
		}
		break;
	case VS_STATE_PREPARE321:
		{
			m_statemgr.SetState(&VsPrepare321::GlobalVsPrepare321);
		}
		break;
	case VS_STATE_END:
		{
			m_statemgr.SetState(&VsEnd::GlobalVsEnd);
		}
		break;
	case VS_STATE_IN_FIGHT:
		{
			m_statemgr.SetState(&VsInFight::GlobalVsInFight);
		}
		break;
	case VS_STATE_AFTER_FIGHT:
		{
			m_statemgr.SetState(&VsAfterFight::GlobalVsAfterFight);
		}
		break;
	case VS_STATE_READY_GMF:
		{
			m_statemgr.SetState(&VsReadyGmf::GlobalVsReadyGmf);
		}
		break;
	case VS_STATE_READY_LEAGUE:
		{
			m_statemgr.SetState(&VsReadyLeague::GlobalVsReadyLeague);
		}
		break;
	default:
		break;
	}
}

void SceneVsBase::OnEnterWaitLoad()
{
	// sub-class do
}

VsStateType SceneVsBase::IsAllLoad()
{
	// sub-class do
	return VS_STATE_DUMMY;
}

VsStateType SceneVsBase::OnLoadTimeOut()
{
	// sub-class do or base-calss do
	return VS_STATE_PREPARE321;//默认进入倒计时状态
}

UINT32 SceneVsBase::GetLoadWaitTime()
{
	return VS_ENTER_TIMEOUT;
}

void SceneVsBase::OnStateEnd()
{
	// sub-class do
}

void SceneVsBase::OnEnterFight()
{
	// sub-class do
}

VsStateType SceneVsBase::CheckFightEnd()
{
	// sub-class do
	return VS_STATE_DUMMY;
}

void SceneVsBase::OnEnterP321()
{
	// sub-class do
}

//默认调用
VsStateType SceneVsBase::CheckP321End()
{
	if(GetStateStartTime() +  GetGlobalConfig().PkPrepareTime <= time(NULL))//这个时间统一
	{
		return VS_STATE_IN_FIGHT;
	}
	return VS_STATE_DUMMY;
}

void SceneVsBase::ChangeWatcherState(Role* pRole, bool watcher)
{
	if(NULL == pRole)
	{
		return ;
	}

	if(pRole->IsLeavingScene())
	{
		return ;
	}
	
	if(pRole->GetCurrScene() != GetScene())
	{
		return ;
	}

	if(pRole->IsWatcher() == watcher)
	{
		return ;
	}

	//SSDebug << __FUNCTION__ << " : role = " << pRole->GetName() << " flag = " << watcher << END;

	if(GetLastEventRoleID() == pRole->GetID())
	{
		SSInfo << " last event role change scene role = " << pRole->GetID() << " watcher = " << watcher << END;
		KKSG::SceneSwitchData data;
		if(watcher)
		{
			data.add_watchers(pRole->GetID());
		}
		CSceneSwitch::EnterScene(pRole, GetScene()->GetSceneTemplateID(), GetScene()->GetSceneID(), data, GSConfig::Instance()->IsCrossGS(), GSConfig::Instance()->GetLine());
	}
	else
	{
		pRole->SetChangeSameScene(true);
		SceneManager::Instance()->EnterScene(pRole, GetScene(), watcher);
	}
}

void SceneVsBase::OnEnterAfterFight()
{
	//sub-class do
}

VsStateType SceneVsBase::CheckAfterFightEnd()
{
	//sub-class do
	return VS_STATE_DUMMY;
}

void SceneVsBase::TranInitPosition(UINT64 unitID)
{
	EnterPosition pos = GetInitPos(unitID);
	Unit* pUnit = GetUnit(unitID);
	if(NULL == pUnit)
	{
		return ;
	}
	XEntity* entity = dynamic_cast<XEntity*>(pUnit->GetXObject());
	if(entity)
	{
		entity->TransferLocation(pos.position, pos.face);
	}
}

void SceneVsBase::ForbidSkill(UINT64 unitID, bool forbid)
{
	return ;//wait return
	Role* pRole = RoleManager::Instance()->FindByRoleID(unitID);
	if(pRole)
	{
		MakePuppet(pRole, forbid);
		return;
	}
	DummyRole* pDummy = DummyRoleManager::Instance()->Find(unitID);
	if(pDummy)
	{
		MakePuppet(pDummy, forbid);
	}
}

EnterPosition SceneVsBase::GetInitPos(UINT64 unitID)
{
	EnterPosition pos;
	UINT32 group = GetGroup(unitID);
	SceneInfo* pSinfo =	m_scene->GetSceneInfo();
	if(NULL == pSinfo)
	{
		return pos;
	}
	bool posrand = true;
	if (SceneConfig::Instance()->IsPosRandExcept(GetScene()->GetSceneType()))
	{
		posrand = false;
	}

	if (GetScene()->GetSceneType() == KKSG::SCENE_BIGMELEE_FIGHT)//随机出生点
	{
		int idx = pSinfo->GetRandStartPos(pos.position);
		if (idx!=-1)
		{
			float desFace = pSinfo->GetStartFace(idx);
			pos.face = desFace;
		}
	}else
	{
		Vector3 desPos = pSinfo->GetStartPos((UINT32)group, posrand);
		float desFace = pSinfo->GetStartFace((UINT32)group);
		pos.position = desPos;
		pos.face = desFace;
	}

	return pos;
}

Unit* SceneVsBase::GetUnit(UINT64 unitID)
{
	return GetScene()->FindSceneUnit(unitID);
}

Role* SceneVsBase::GetRole(UINT64 roleID)
{
	return GetScene()->FindSceneRole(roleID);
}

void SceneVsBase::SetPuppet(UINT64 roleid, bool puppet)
{
	m_role_puppet[roleid] = puppet;
}

bool SceneVsBase::IsPuppet(UINT64 roleid)
{
	auto it = m_role_puppet.find(roleid);
	if(it == m_role_puppet.end())
	{
		return false;
	}
	return (it->second);
}

void SceneVsBase::AddUsedRobotSummary(RoleSummary* summary)
{
	if(summary)
	{
		m_usedRobotSummary.push_back(summary);
	}
}

DummyRole* SceneVsBase::CreateRobot(const FilterRoleData& filter, UINT32 lookupid, UINT32 robotgroup, int dummytype)
{
	if(0 == filter.qwRoleID)
	{
		SSError << " create robot null" <<END;
		return NULL;
	}
	
	RoleSummary* summary = RobotConfig::Instance()->CreateRobotSummary(filter, lookupid); 
	if(NULL == summary)
	{
		SSError << "donot find robot summary confid = " << lookupid << END;
		return NULL;
	}

	UINT32 fightGroup = (UINT32)(KKSG::FightEnemy); //和怪物同一战线

	auto sinfo = GetScene()->GetSceneInfo();
	if(NULL == sinfo)
	{
		return NULL;
	}
	EnterSceneInfo entersceneinfo = SceneConfig::Instance()->GetEnterSceneInfo(KKSG::ENTER_SCENE_SWITCH, GetScene()->GetSceneTemplateID());
	Vector3 pos = sinfo->GetStartPos(robotgroup, true);
	float face = sinfo->GetStartFace(robotgroup);
	DummyRole* pDummy = DummyRoleManager::Instance()->Create(summary, pos, face, fightGroup, entersceneinfo, DummyType(dummytype));
	if(NULL == pDummy)
	{
		return NULL;
	}

	MakePuppet(pDummy, true);

	AddUsedRobotSummary(summary);

	GetScene()->AddUnitAndNotify(pDummy);

	return pDummy; 
}

UINT64 SceneVsBase::GetFightGroupUId(UINT64 roleid)
{
	return (UINT64)(GetGroup(roleid));
}

bool SceneVsBase::IsInWaitLoaded()
{
	return (VS_STATE_WAIT_LOAD == GetCurrentStateType());
}

int SceneVsBase::GetCurrentStateType()
{
	if(NULL == m_statemgr.GetCurrentState())
	{
		return VS_STATE_DUMMY;
	}
	return m_statemgr.GetCurrentState()->GetStateType();
}


void SceneVsBase::EndSkills(const std::vector<UINT64>& unitids)
{
	for(size_t i = 0; i < unitids.size(); ++i)
	{
		Unit* pUnit = GetUnit(unitids[i]);	
		if(pUnit && pUnit->GetXObject() && pUnit->GetXObject()->SkillComponent())
		{
			pUnit->GetXObject()->SkillComponent()->EndSkill();
			pUnit->ClearMobs();
		}
	}
}

void SceneVsBase::SkillCoolDown(const std::vector<UINT64>& roleids)
{
	for(size_t i = 0; i < roleids.size(); ++i)
	{
		Unit* pUnit = GetUnit(roleids[i]);
		if(pUnit && pUnit->IsRole())
		{
			((Role*)(pUnit))->SkillCoolDown();
		}
	}
}

void SceneVsBase::MakeSkillCDAtLaunch(const std::vector<UINT64>& roleids)
{
	for(size_t i = 0; i < roleids.size(); ++i)
    {
        Role* pRole = GetRole(roleids[i]);
        if(pRole)
        {
            pRole->SkillCDAtLaunch();
        }
    }
}

RoleSummary* SceneVsBase::GetUsedRoleSummary(UINT64 uid)
{
	for(size_t i = 0; i < m_usedRobotSummary.size(); ++i)
	{
		if(m_usedRobotSummary[i]->qwRobotID == uid)
		{
			return m_usedRobotSummary[i];
		}
	}
	return NULL;
}

void SceneVsBase::VsKillNotify(UINT64 deadID, UINT32 reviveTime, int notifyType)
{
	Unit* deadUnit = GetUnit(deadID);
	if(NULL == deadUnit)
	{
		return;
	}
	UINT64 killID = deadUnit->GetKillerID();
	Unit* killUnit = GetUnit(killID);
	if(NULL == killUnit)
	{
		return;
	}
	PtcG2C_PvpBattleKill killntf;
	killntf.m_Data.set_killid(killID);
	killntf.m_Data.set_deadid(deadID);
	killntf.m_Data.set_revivetime(reviveTime);
	UINT32 contiKillCount = 1;
	if(killUnit->IsRole())
	{
		CombatStatistics& combat = ((Role*)(killUnit))->Statistics();
		contiKillCount = combat.GetKillContinueCount();
	}
	else if(killUnit->IsDummyRole())
	{
		CombatStatistics& combat = ((DummyRole*)(killUnit))->Statistics();
		contiKillCount = combat.GetKillContinueCount();
	}
	killntf.m_Data.set_contikillcount(contiKillCount);

	std::vector<UINT64> assistIds;
	if (deadUnit->IsRole())
	{
		CombatStatistics& combat = ((Role*)(deadUnit))->Statistics();
		assistIds = combat.GetAssistRoleIDs();
	}
	else if (deadUnit->IsDummyRole())
	{
		CombatStatistics& combat = ((DummyRole*)(deadUnit))->Statistics();
		assistIds = combat.GetAssistRoleIDs();
	}
	for (auto it = assistIds.begin(); it != assistIds.end(); ++it)
	{
		killntf.m_Data.add_assitids(*it);
	}

	switch(notifyType)
	{
	case 0:
		{
			GetScene()->Broadcast(killntf);			
		}
		break;
	case 1:
		{
			if(killUnit->IsRole())
			{
				((Role*)(killUnit))->Send(killntf);
			}
		}
		break;
	default:
		break;
	}
}

void SceneVsBase::PreEnterScene(Role* role)
{
	if (!role->IsWatcher())
	{
		if (SceneConfig::Instance()->IsHasReadyScene(GetScene()->GetSceneType()))
		{
			///> 进场景先不让动
			MakePuppet(role, true, false);
		}
	}

	OnEnterBeforeNtf(role);
}

void SceneVsBase::PreRoleLoginReconnect(Role* role)
{
	OnRoleReconnectBeforeNtf(role);
}

//有可能多次调用
void SceneVsBase::SceneReady()
{
	if (SceneConfig::Instance()->IsHasReadyScene(GetScene()->GetSceneType()))
	{
		if (!GetScene()->IsSceneReady())
		{
			GetScene()->SetSceneReady();
			GetScene()->OnSceneReady();
		}
	}
}

void SceneVsBase::SetLastEvent(const VsEvent& event)
{
	m_lastEvent = event;
}

void SceneVsBase::ClearLastEvent()
{
	VsEvent tmp;
	m_lastEvent = tmp;
}
