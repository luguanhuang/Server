#include "pch.h"
#include <time.h>
#include "math/XVector3.h"
#include "scene/ptcg2c_deathnotify.h"
#include "scene/scene.h"
#include "entity/XEntity.h"
#include "component/XBuffComponent.h"
#include "buff/XBuffTraveller.h"
#include "attributewatcher.h"
#include "table/globalconfig.h"
#include "rolemanager.h"
#include "enemymanager.h"
#include "foreach.h"
#include "table/CombatParamMgr.h"
#include "battle/ptcg2c_revivenotify.h"
#include "pb/project.pb.h"
#include "dummyrolemanager.h"
#include "gamelogic/fightgroup.h"
#include "gamelogic/killerwatcher.h"
#include "buff/XBuff.h"
#include "scene/scenemortuary.h"
#include "battle/ptcg2c_clientonlybuffnotify.h"
#include "battle/ptcg2c_fightgroupchangentf.h"
#include "component/XAIComponent.h"
#include "util/gettimeofday.h"
#include "scene/securitystatistics.h"
#include "battle/ptcg2c_specialstatentf.h"
#include "entity/XEntity.h"
#include "component/XSkillComponent.h"
#include "scene/scenedragon.h"

Unit::Unit()
{
	m_uID = 0;
	m_IsDead = false;
	m_IsDeprecated = false;
	m_huge = false;
	m_level = 0;
	m_summongroup = 0;
	m_boundRadius = 0.5f;
	m_boundRadiusOffset = Vector3::zero;
	m_boundHeight = 0.5f;
	m_pObject = NULL;
	m_currScene = NULL;
	m_collider = false;
	m_ignore_dynamic_collide = false;
	m_combatAttr = new CombatAttribute();
	m_skillLevelMgr = NULL;
	m_fightgroup = KKSG::FightNeutral;
	m_scale = 1;
	m_delay = 50; // 默认客户端是50ms延迟
	m_IsArmorBroken = false;
	m_uPresentID = 0;
	m_uTemplateID = 0;
	m_hostid = 0;
	m_finalHostid = 0;
	m_floatingMin = 0;
	m_floatingMax = 0;
	m_killer = new KillerWatcher();
	m_aiTime = 0;
	m_ai = NULL;
	m_behavior = NULL;
	m_aiData = NULL;
	m_securityStatistics = NULL;
	m_specialstate = 0;
	m_combatUseLevel = 0;
	m_bBlockAddHP = false;

	XDelegate d = XDelegate::registerMethod<Unit, &Unit::OnAttributeRegenerate>(this);
	m_TimerToken = CTimerMgr::Instance()->SetTimer(d, NULL, NULL, 1000, -1, __FILE__, __LINE__);

	m_cyclesync = 0;
	m_lastsync = 0;
}

Unit::~Unit()
{
	CTimerMgr::Instance()->KillTimer(m_TimerToken);
	delete m_combatAttr;
	delete m_killer;
}

KKSG::EntityCategory Unit::GetUnitType(UINT64 uID)
{
	return (KKSG::EntityCategory)((uID >> UnitTypeOffset)&UnitBitMask);
}

void Unit::EnterSceneResetAttribute()
{
	double hpMax = m_combatAttr->GetAttr(TOTAL_MaxHP);
	m_combatAttr->SetAttr(BASIC_CurrentHP, hpMax);
	double mpMax = m_combatAttr->GetAttr(TOTAL_MaxMP);
	m_combatAttr->SetAttr(BASIC_CurrentMP, mpMax);

	AttributeWatcher watcher(this);
	watcher.SetAttr(PERCENT_CurrentHP, m_combatAttr->GetAttr(PERCENT_MaxHP) * 100);
	watcher.CheckChangedAttributeAndNotify(false);

	m_killer->Reset();
}

void Unit::EnterScene(Scene *scene)
{
	m_reviveinfo.Clear();

	m_currScene = scene;
	if(m_pObject) m_pObject->OnEnterScene(scene);
	OnEnterScene(scene);

	if (NULL != m_currScene && NULL != m_currScene->GetSceneGroup())
	{
		m_currScene->GetSceneGroup()->AddUnit(this);
	}
}

void Unit::LeaveScene()
{
	if(m_pObject) m_pObject->OnLeaveScene();
	OnLeaveScene();

	///> 离开场景清理阵营关系数据，必须最后处理
	if (NULL != m_currScene && NULL != m_currScene->GetSceneGroup())
	{
		m_currScene->GetSceneGroup()->RemoveUnit(this);
	}
	//ResetFightGroup();

	ResetSpecialState();

	m_currScene = NULL;
}

void Unit::SetAttr(CombatAttrDef attrDef, double value)
{
	m_combatAttr->SetAttr(attrDef, value);

	if (m_pObject)
	{
		XAttributeChangedEventArgs args;
		args.AttrID = attrDef;
		m_pObject->DispatchAction(&args);
	}
}

void Unit::AddAttr(CombatAttrDef attrDef, double value)
{
	double actualValue = m_combatAttr->AddAttr(attrDef, value);

	if (m_pObject)
	{
		XAttributeChangedEventArgs args;
		args.AttrID = attrDef;
		m_pObject->DispatchAction(&args);
	}

	if (m_securityStatistics != NULL)
		m_securityStatistics->OnAttributeChange(attrDef, actualValue);
}

void Unit::CheckAttrState()
{
	if (m_IsDead)
		return;

	if (GetAttr(BASIC_CurrentHP) < 0)
	{
		m_combatAttr->SetAttr(BASIC_CurrentHP, 0);
	}

	if (GetAttr(BASIC_CurrentMP) < 0)
	{
		m_combatAttr->SetAttr(BASIC_CurrentMP, 0);
	}

	if (GetAttr(BASIC_CurrentSuperArmor) < 0)
	{
		m_combatAttr->SetAttr(BASIC_CurrentSuperArmor, 0);
	}

	if (GetAttr(BASIC_CurrentXULI) < 0)
	{
		m_combatAttr->SetAttr(BASIC_CurrentXULI, 0);
	}
}

void Unit::PackDeathNotfiy(KKSG::DeathInfo* info)
{
	if (NULL == m_currScene)
	{
		return;
	}
	info->set_killer(GetExecutionerID());
	info->set_uid(GetID());
	info->set_showslowcamera(HasDeathSlowCamera());
	if (IsRole())
	{
		SceneMortuary* mortuary = m_currScene->GetMortuary();
		info->set_revivecount(mortuary->GetReviveLimitCount(m_uID));
		if (KKSG::SCENE_DRAGON == m_currScene->GetSceneType())
		{
			if (NULL != m_currScene->GetSceneDragon() && m_currScene->GetSceneDragon()->CanRevive())
			{
				info->set_costrevivecount(mortuary->GetItemReviveCount(m_uID));
			}
			else
			{
				info->set_costrevivecount(m_currScene->GetSceneInfo()->m_pConf->ReviveNumb);
				info->set_revivecount(m_currScene->GetSceneInfo()->m_pConf->ReviveNumb);
			}
		}
		else
		{
			info->set_costrevivecount(mortuary->GetItemReviveCount(m_uID));
		}
		info->set_type(KKSG::ReviveType(m_reviveinfo.type));
	}
}

void Unit::SendDeathNotfiy()
{
	if (m_currScene != NULL)
	{
		PtcG2C_DeathNotify oPtc;
		PackDeathNotfiy(&oPtc.m_Data);
		m_currScene->Broadcast(this, oPtc);

		if (IsRole() && m_ai && !m_ai->IsEnabled())
		{
			XAIEventArgs args;
			args.MsgTargetTemplateId = 0;
			args.EventArg = "PlayerDead";
			args.Templateid = GetTemplateID();
			args.Pos = GetXObject()->GetPosition_p();
			args.FloatArg = GetXObject()->GetFaceDegree();
			args.SkillId = 0;

			if (m_killer && m_killer->GetKillerSkill() != 0)
				args.SkillId = (UINT32)m_killer->GetKillerSkill();

			m_currScene->MsgChannel().SendMsgTo(AISceneMsgChannel::GlobalAI, AISceneMsgChannel::Cmd, args);
		}


	}
}

void Unit::TriggerDeath()
{
	if (m_IsDead)
	{
		SSError<<"id:"<<GetID()<<" has died"<<END;
		return;
	}
	m_IsDead = true;

	m_bBlockAddHP = false;

	if (NULL != m_currScene && NULL != m_currScene->GetSceneGroup())
	{
		m_currScene->GetSceneGroup()->RemoveUnit(this);
	}

	XRealDeadActionArgs args;
	args.RemainTime = 2.0f;
	m_pObject->DispatchAction(&args);

	OnDied();
	SendDeathNotfiy();
}

bool Unit::OnAttributeRegenerate(IArgs *pargs, void *param)
{
	if (m_currScene == NULL || IsDead())
	{
		return true;
	}

	if (IsRole() && !m_currScene->IsSyncScene())
	{
		return true;
	}

	AttributeWatcher wather(this);

	double hpRegen = m_combatAttr->GetAttr(TOTAL_HPRecovery);
	wather.AddAttr(BASIC_CurrentHP, hpRegen);

	double mpRegen = m_combatAttr->GetAttr(TOTAL_MPRecovery);
	wather.AddAttr(BASIC_CurrentMP, mpRegen);

	double saRegen = m_combatAttr->GetAttr(TOTAL_SuperArmorReg);
	wather.AddAttr(BASIC_CurrentSuperArmor, saRegen);

	CheckAttrState();

	wather.CheckChangedAttributeAndNotify(true);
	return true;
}

static void AppearanceBuffCB(void *arg, const XBuff *pBuff)
{
	if (!pBuff->IsMustNotify())
		return;
	KKSG::UnitAppearance *pAppearance = (KKSG::UnitAppearance *)arg;
	KKSG::BuffInfo* info = pAppearance->add_buffs();
	info->set_buffid(pBuff->GetID());
	info->set_bufflevel(pBuff->GetLevel());
	info->set_lefttime(pBuff->GetLeftTime());
	if(pBuff->GetEffectData().MobID > 0)
		info->set_mobuid(pBuff->GetEffectData().MobID);
	if(pBuff->GetEffectData().bReduceCD)
		info->set_breducecd(pBuff->GetEffectData().bReduceCD);
	if(pBuff->GetEffectData().TransformID != 0)
		info->set_transformid(pBuff->GetEffectData().TransformID);
	info->set_curhp(pBuff->GetHP());
	info->set_maxhp(pBuff->GetMaxHP());
	info->set_stackcount(pBuff->GetStackCount());
}

void Unit::GetAppearance(KKSG::UnitAppearance &Appearance, bool isbrief)
{
	Appearance.set_level(m_level);
	Appearance.set_uid(GetID());
	Appearance.set_fightgroup(m_fightgroup);
	Appearance.set_direction(m_pObject->GetFaceDegree());
	Appearance.set_isdead(IsDead());
	Appearance.set_specialstate(m_specialstate);
	if(m_pObject->IsMobShieldable()) Appearance.set_mobshieldable(true);

	if (0 != m_hostid)
	{
		Appearance.set_hostid(m_hostid);
	}
	m_pObject->GetPosition_p().assignTo(*Appearance.mutable_position());

	if(isbrief)
	{
		m_combatAttr->SaveToBriefAttributes(*Appearance.mutable_attributes());
	}
	else
	{
		m_combatAttr->SaveToAttributes(*Appearance.mutable_attributes());
	}

	XBuffComponent *pBuffs = m_pObject->BuffComponent();
	if(pBuffs)
	{
		pBuffs->IterateBuff(AppearanceBuffCB, &Appearance);
		pBuffs->SetAllBuffsInfo(Appearance.mutable_allbuffsinfo());
	}
}

double Unit::Distance(Unit *pUnit)
{
	return Vector3::Magnitude(pUnit->GetXObject()->GetPosition_p() - m_pObject->GetPosition_p());
}

std::ostream & operator<<(std::ostream &stream, Unit *pUnit)
{
	pUnit->StreamOutput(stream);
	return stream;
}

Unit * Unit::FindUnit(UINT64 uID)
{
	switch(Unit::GetUnitType(uID))
	{
	case KKSG::Category_Role:
		return RoleManager::Instance()->FindByRoleID(uID);
	case KKSG::Category_Enemy:
		return EnemyManager::Instance()->FindEnemy(uID);
	case KKSG::Category_DummyRole:
		return (Unit*)DummyRoleManager::Instance()->Find(uID);
	default:
		return NULL;
	}
}

double Unit::GetParalyzeAttribute()
{
	double x = GetAttr(TOTAL_Paralyze);
	auto Row = CombatParamMgr::Instance()->GetCombatParamConf(this);
	double dProb = x / (x + Row->ParalyzeBase);
	if (dProb >  GetGlobalConfig().ParalyzeLimit)
	{
		dProb =  GetGlobalConfig().ParalyzeLimit;
	}

	return dProb;
}

double Unit::GetParalyzeDefense()
{
	double x = GetAttr(TOTAL_ParaResist);
	auto Row = CombatParamMgr::Instance()->GetCombatParamConf(this);
	double dProb = x / (x + Row->ParaResistBase);
	if (dProb >  GetGlobalConfig().ParaResistLimit)
	{
		dProb =  GetGlobalConfig().ParaResistLimit;
	}

	return dProb;
}

int Unit::GetHpPercent()
{
	double max = GetAttr(TOTAL_MaxHP);
	double current = GetAttr(TOTAL_CurrentHP);
	if (0.0l == max)
	{
		return 0;
	}
	return (int)((current/max)*100);
}

double Unit::GetPreciseHpPercent()
{
	double max = GetAttr(TOTAL_MaxHP);
	double current = GetAttr(TOTAL_CurrentHP);
	if (0.0l >= max)
	{
		return 0;
	}
	return (current/max)*100;
};

bool Unit::IsOppose(const Unit *beattacked, const Unit *attacker)
{
	if (attacker == beattacked)
	{
		return false;
	}
	if (beattacked->IsFightGroupNeutral() || attacker->IsFightGroupNeutral())
	{
		return false;
	}
	return FightGroupMgr::Instance()->CanAttack(beattacked->GetFightGroup(), attacker->GetFightGroup());
}

bool Unit::IsSameGroup(const Unit *other, const Unit *me)
{
	return other->GetFightGroup() == me->GetFightGroup() 
		&& !FightGroupMgr::Instance()->CanAttack(other->GetFightGroup(), me->GetFightGroup());
}

void Unit::Revive(float hprecoverper) 
{
	ReviveOnServer();

	PtcG2C_ReviveNotify oRevivePtc;
	oRevivePtc.m_Data.set_roleid(GetID());
	oRevivePtc.m_Data.set_type(KKSG::ReviveType(m_reviveinfo.type));
	m_currScene->Broadcast(this, oRevivePtc);

	m_killer->Reset();

	if (m_currScene->IsSyncScene())
	{
		ResetHpMp(hprecoverper);
	}

	OnRevive();

	m_reviveinfo.Reset();

	if (NULL != m_currScene && NULL != m_currScene->GetSceneGroup())
	{
		m_currScene->GetSceneGroup()->AddUnit(this);
	}
}

void Unit::ReviveOnServer()
{
	if(!m_IsDead)
	{
		SSWarn << "Revive " << GetID() << " but it's alive!" << END;
	}
	m_IsDead = false;
	m_IsDeprecated = false;

	XReviveEventArgs args;
	m_pObject->DispatchAction(&args);
}

void Unit::SubHp(Unit* attacker, double hurt)
{
	AttributeWatcher watcher(this);
	watcher.AddWatchedAttribute(BASIC_CurrentHP);
	watcher.AddWatchedAttribute(BASIC_CurrentMP);
	watcher.AddWatchedAttribute(BASIC_CurrentSuperArmor);

	if (hurt < 1)
	{
		hurt = 1;
	}
	AddAttr(BASIC_CurrentHP, -hurt);
	CheckAttrState();
	
	watcher.CheckChangedAttributeAndNotify(true);
	return;
}

std::string Unit::GetAIString()
{
	return "default";
}

void Unit::GetAIArgs(AIDataArgs &args)
{
	args.mSpeed = 5.0f;
	args.mNormalAttackProb = 1.0f;
	args.mEnterFightRange = 20.0f;
	args.mIsWander = false;
	args.mActionGap = 1.5f;
	args.mAIStartTime = 1.5f;
	args.mIsFixedInCd = false;
	args.mFightTogetherDis = 30.0f;
	args.mType = 1;

	args.mRatioLeft = 0.2f;
	args.mRatioRight = 0.2f;
	args.mRatioDistance = 0.2f;
	args.mRatioIdle = 0.2f;
	args.mRatioExp = 0.2f;
	args.mRatioSkill = 0.2f;
}

void Unit::Update(float delta)
{
	if (m_currScene && m_currScene->SyncMode() == SERVERMODE)
	{
		if (!m_aiTime && m_currScene->GetStopAITime())
		{
			if(GetXObject()) GetXObject()->Idled();
			m_aiTime = m_currScene->GetStopAITime();
			EnableAI(false);

			if (m_currScene->GetSceneState() == SCENE_RUNNING)
			{
				MakePuppet(true);
			}
		}

		timeval now;
		gettimeofday(&now, NULL);
		UINT64 time = now.tv_sec;
		time *= 1000;
		time += now.tv_usec / 1000;
		if (m_aiTime && time > m_aiTime)
		{
			MakePuppet(false);
			EnableAI(true);
			m_aiTime = 0;
		}
	}
	if (m_IsDead == false && GetAttr(BASIC_CurrentHP) <= 0)
	{
		TriggerDeath();
	}
}

UINT64 Unit::GetKillerID()
{
	return m_killer->GetKillerID();
}

UINT64 Unit::GetExecutionerID()
{
	return m_killer->GetExecutionerID();
}

void Unit::EnableAI(bool isenable)
{
	if (isenable)
	{
		XAIEnableEventArgs oArgs;
		GetXObject()->DispatchAction(&oArgs);
	}
	else
	{
		XAIDisableEventArgs oArgs;
		GetXObject()->DispatchAction(&oArgs);
	}
}

const void Unit::ChangeFightGroup(UINT32 fightgroup)
{
	if (fightgroup == GetFightGroup())
	{
		return;
	}
	SetFightGroup(fightgroup);
	// notify client 
	if (NULL != m_currScene)
	{
		if (NULL != m_currScene->GetSceneGroup())
		{
			m_currScene->GetSceneGroup()->ChangeUnit(this);
		}
		PtcG2C_FightGroupChangeNtf ntf;
		ntf.m_Data.set_uid(m_uID);
		ntf.m_Data.set_fightgroup(m_fightgroup);
		m_currScene->Broadcast(this, ntf);
	}
}

Unit* Unit::GetHostUnit()
{
	return 0 == m_hostid ? NULL : Unit::FindUnit(m_hostid);
}

Unit* Unit::GetFinalHostUnit()
{
	return 0 == m_finalHostid ? NULL : Unit::FindUnit(m_finalHostid);
}

void Unit::SetHost(Unit* pUnit)
{
	m_hostid = pUnit->GetID(); 
	m_finalHostid = pUnit->GetFinalHostId();
	if (m_finalHostid == 0)
		m_finalHostid = m_hostid;
}

void Unit::ResetHpMp(float hprecoverper)
{
	///> 满血
	AttributeWatcher watcher(this);
	double hpMax = m_combatAttr->GetAttr(TOTAL_MaxHP);
	watcher.SetAttr(PERCENT_CurrentHP, 0);
	watcher.SetAttr(BASIC_CurrentHP, hpMax * hprecoverper);
	double mpMax = m_combatAttr->GetAttr(TOTAL_MaxMP);
	watcher.SetAttr(PERCENT_CurrentMP, 0);
	watcher.SetAttr(BASIC_CurrentMP, mpMax);
	watcher.CheckChangedAttributeAndNotify(true);
}

void Unit::ResetBaseAttr(double lifescale, double atkscale /*= 1.0*/, double defscale /*= 1.0*/, bool notify /*= false*/)
{
	AttributeWatcher watcher(this);

	double fvalue = GetAttr(BASIC_MaxHP);
	fvalue = fvalue * lifescale;
	watcher.SetAttr(BASIC_MaxHP, fvalue);
	watcher.SetAttr(BASIC_CurrentHP, fvalue);

	fvalue = GetAttr(BASIC_PhysicalAtk);
	fvalue = fvalue * atkscale;
	watcher.SetAttr(BASIC_PhysicalAtk, fvalue);

	fvalue = GetAttr(BASIC_PhysicalAtkMod);
	fvalue = fvalue * atkscale;
	watcher.SetAttr(BASIC_PhysicalAtkMod, fvalue);

	fvalue = GetAttr(BASIC_MagicAtk);
	fvalue = fvalue * atkscale;
	watcher.SetAttr(BASIC_MagicAtk, fvalue);

	fvalue = GetAttr(BASIC_MagicAtkMod);
	fvalue = fvalue * atkscale;
	watcher.SetAttr(BASIC_MagicAtkMod, fvalue);

	fvalue = GetAttr(BASIC_PhysicalDef);
	fvalue = fvalue * defscale;
	watcher.SetAttr(BASIC_PhysicalDef, fvalue);

	fvalue = GetAttr(BASIC_PhysicalDefMod);
	fvalue = fvalue * defscale;
	watcher.SetAttr(BASIC_PhysicalDefMod, fvalue);

	fvalue = GetAttr(BASIC_MagicDef);
	fvalue = fvalue * defscale;
	watcher.SetAttr(BASIC_MagicDef, fvalue);

	fvalue = GetAttr(BASIC_MagicDefMod);
	fvalue = fvalue * defscale;
	watcher.SetAttr(BASIC_MagicDefMod, fvalue);

	if(notify)
	{
		watcher.CheckChangedAttributeAndNotify(true);
	}
}

bool Unit::MakePuppet(bool ismake, bool isnotifyclient)
{
	if(!IsRole()) return false;

	if (ismake == IsInSpecailState(KKSG::Unit_Puppet))
	{
		return false;
	}

	XEntity* entity = dynamic_cast<XEntity*>(GetXObject());
	if(entity)
	{
		entity->Puppet(ismake);
	}

	if (ismake)
	{
		AddSpecialState(KKSG::Unit_Puppet);
	}
	else
	{
		DelSpecialState(KKSG::Unit_Puppet);
	}

	if (isnotifyclient)
	{
		NotifyState(1 << KKSG::Unit_Puppet);
	}

	SSInfo<<"id:"<<GetID()<<" puppet state:"<<m_specialstate<<" ismake:"<<ismake<<END;

	return true;
}

bool Unit::MakeInvisible(bool ismake, bool isnotifyclient)
{
	if(!IsRole()) return false;

	if (ismake == IsInSpecailState(KKSG::Unit_Invisible))
	{
		return false;
	}
	if (ismake)
	{
		AddSpecialState(KKSG::Unit_Invisible);
	}
	else
	{
		DelSpecialState(KKSG::Unit_Invisible);
	}

	if (isnotifyclient)
	{
		NotifyState(1 << KKSG::Unit_Invisible);
	}

	SSInfo<<"id:"<<GetID()<<" invisible state:"<<m_specialstate<<" ismake:"<<ismake<<END;

	return true;
}

void Unit::NotifyState(UINT32 effectmask)
{
	if (NULL != m_currScene)
	{
		PtcG2C_SpecialStateNtf ntf;
		ntf.m_Data.set_uid(GetID());
		ntf.m_Data.set_state(m_specialstate);
		ntf.m_Data.set_effectmask(effectmask);

		///> 防止在addunit之前调用,先发给自己
		Send(ntf);
		///> 广播给其他玩家
		m_currScene->Broadcast(this, ntf, this);
	}
	else
	{
		SSWarn<<"scene is NULL, state:"<<m_specialstate<<" mask:"<<effectmask<<END;
	}
}

void Unit::OnSceneReady()
{
	if(m_pObject)
	{
		m_pObject->OnSceneReady();
	}
}

UINT32 Unit::GetCombatUseLevel()
{
	if (m_combatUseLevel)//特殊
	{
		return m_combatUseLevel;
	}
	return GetLevel();
}

void Unit::ClearMobs()
{
	if (NULL == GetXObject())
	{
		return;
	}
	XSkillComponent* xskill = GetXObject()->SkillComponent(); 
	if (NULL == xskill)
	{
		return;
	}
	const std::vector<UINT64>& ids = xskill->GetSkillMobs();
	for (auto i = ids.begin(); i != ids.end(); ++i)
	{
		Enemy* enemy = EnemyManager::Instance()->FindEnemy(*i);
		if (NULL == enemy)
		{
			SSWarn<<"enemy is NULL, id:"<<*i<<" unit:"<<GetID()<<END;
		}
		else
		{
			SSInfo<<"clear enemy is id:"<<*i<<" unit:"<<GetID()<<END;
			enemy->TriggerDeath();
		}
	}
}


void Unit::AddSyncAttr(int id, double value)
{
	m_AttrWatch.insert(std::unordered_map<int, double>::value_type(id, value));
}
