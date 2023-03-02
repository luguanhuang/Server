#include "pch.h"
#include "XAIComponent.h"
#include "ai/aibehaviortree.h"
#include "unit/unit.h"
#include "entity/XObject.h"
#include "unit/enemy.h"
#include "entity/XEntity.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "scene/scenefinder.h"
#include "scene/scene.h"
#include "component/XNavigationComponent.h"
#include "unit/globalaiunit.h"
#include "gamelogic/skilllevel.h"
#include "table/XSkillEffectMgr.h"
#include "ai/ainodefactory.h"
#include "battle/ptcg2c_notifycliententerfight.h"
#include "xdelegate.hpp"
#include "gamelogic/killerwatcher.h"
#include "ai/aionlinedebugmgr.h"
#include "unit/dummyrole.h"
#include "entity/XRole.h"
#include "unit/role.h"
#include "scene/securityaistatistics.h"
#include "scene/securitystatistics.h"
#include "../ai/aiscenehandler.h"
#include "gamelogic/XPatrol.h"

UINT32 XAIComponent::_target_index;
UINT32 XAIComponent::_master_index;
UINT32 XAIComponent::_navtarget_index;
UINT32 XAIComponent::_is_oppo_casting_skill_index;
UINT32 XAIComponent::_is_hurt_oppo_index;
UINT32 XAIComponent::_target_distance_index;
UINT32 XAIComponent::_is_fixed_in_cd_index;
UINT32 XAIComponent::_normal_attack_prob_index;
UINT32 XAIComponent::_enter_fight_range_index;
UINT32 XAIComponent::_fight_together_dis_index;
UINT32 XAIComponent::_is_wander_index;
UINT32 XAIComponent::_max_hp_index;
UINT32 XAIComponent::_current_hp_index;
UINT32 XAIComponent::_max_super_armor_index;
UINT32 XAIComponent::_current_super_armor_index;
UINT32 XAIComponent::_type_index;
UINT32 XAIComponent::_target_rotation_index;
UINT32 XAIComponent::_attack_range_index;
UINT32 XAIComponent::_min_keep_range_index;
UINT32 XAIComponent::_is_casting_skill_index;
UINT32 XAIComponent::_is_fighting_index;
UINT32 XAIComponent::_is_qte_state_index;
UINT32 XAIComponent::_movedir_index;
UINT32 XAIComponent::_movedest_index;
UINT32 XAIComponent::_movespeed_index;
UINT32 XAIComponent::_bornpos_index;
UINT32 XAIComponent::_skill_id_index;
UINT32 XAIComponent::_heart_rate_index;

bool XAIComponent::_global_debug = false;

class XSecurityStatistics;

XAIComponent::XAIComponent()
	:_magic_num(0xDCBA9876)
{
	_behaviorTree = NULL;
	_TimerToken = INVALID_HTIMER;
	_bIsEnable = true;
	_action_gap_factor = 1.0f;
	_target_id = 0;
	_old_target_id = 0;
	_master_id = 0;
	_navtarget = Vector3::zero;
	_is_fighting = false;
	_is_qte_state = false;
	_is_hurt_oppo = false;
	_target_distance = 0.0f;
	_casting_skill = "";
	_host = NULL;
	_blocked = false;
	_prepare_combo_skill = false;
	_is_global = false;
	_debug_level = 0;
	_ignore_skill_cd = false;
	_is_casting_skill = false;
	_type = 0;
	_hurt_enemy_id = 0;
	_hurt_skill_id = 0;
	_max_hp =0;
	_current_hp = 0;
	_max_super_armor = 0;
	_current_super_armor = 0;
	_navtarget = Vector3::zero;
	_gm_autoplay = false;
	_inited = false;
	_spawn_time = 0;
	_scene_handler = NULL;
}


void XAIComponent::InitConfig()
{

}

XAIComponent::~XAIComponent()
{
	_magic_num = 0;

	if (_behaviorTree)
	{
		delete _behaviorTree;
		_behaviorTree = NULL;
	}

	_host = NULL;

	if (_scene_handler)
	{
		delete _scene_handler;
		_scene_handler = NULL;
	}
};


void XAIComponent::OnAttachToHost(XObject* pHost)
{
	XComponent::OnAttachToHost(pHost);
	_host = pHost->GetUnit();
	//LogInfo("AiComponent Attached[%x], _hostid:[%d]", this, _host->GetTemplateID());
}

void XAIComponent::OnDetachFromHost()
{
	//LogInfo("AIComponent Dettached[%x], _hostid:[%d]", this, _host->GetTemplateID());
	_host->SetAIComponent(NULL);
	_host->SetBehavior(NULL);
	_host->SetAIData(NULL);

	XComponent::OnDetachFromHost();
	SafeKillTimer();

	for (UINT32 i=0; i<_timer_token.size(); i++)
	{
		CTimerMgr::Instance()->KillTimer(_timer_token[i]);
	}

	_timer_token.clear();

	UINT32 lifetime = (UINT32)time(NULL) - _spawn_time;

	if (_host->GetSecurityStatistics() && _host->GetSecurityStatistics()->_AIInfo)
		_host->GetSecurityStatistics()->_AIInfo->SetLifeTime(1.0f * lifetime);

#ifdef AIDEBUG
	AIOnlineDebugMgr::Instance()->RemoveWatch(_host->GetID());
	AIOnlineDebugMgr::Instance()->RemoveObserver(_host->GetID());
	AIOnlineDebugMgr::Instance()->SetDebugAll(_host->GetID(), false);
#endif
}

void XAIComponent::Attached()
{
	ManualAttached();
	SetBehaviorTree(_host->GetAIString());
}

void XAIComponent::SetBehaviorTree(const std::string& tree)
{
	if (_behaviorTree != NULL)
	{
		delete _behaviorTree;
	}
	_behaviorTree = new AIBehaviorTree();
	_inited = _behaviorTree->Init(tree);

	InitFixVariables();

	_host->SetBehavior(_behaviorTree);
	_host->SetAIData(&_behaviorTree->Data());

}
void XAIComponent::ManualAttached()
{
	InitVariables();
	_enmity_list.Init(_host);
	InitDummyRoleSkills();
	_host_templateid = _host->GetTemplateID();
	_host_presentid = _host->GetPresentID();
	_host_typeid = Unit::GetUnitType(_host->GetID());
	_host->SetAIComponent(this);
	_spawn_time = (UINT32)time(NULL);

	GlobalAIUnit* gAI = dynamic_cast<GlobalAIUnit*>(_host);

	if (gAI != NULL)
	{
		_is_global = true;
		_host_typeid = 100;
		//AIFire(0.1f);   start global ai after players entered
	}
	else
	{
		if (_host->IsRole())
			Enable(false);
		_patrol.Init(this);
	}
}

void XAIComponent::ActionSubscribe()
{
	RegisterAction(XAction_AITick, DelegateCombination(XAIComponent, OnTickAI, this));
	RegisterAction(XAction_AIEnterFight, DelegateCombination(XAIComponent, OnAIEnterFight, this));
	RegisterAction(XAction_AIStartSkill, DelegateCombination(XAIComponent, OnStartSkill, this));
	RegisterAction(XAction_AIEndSkill, DelegateCombination(XAIComponent, OnEndSkill, this));
	RegisterAction(XAction_AISkillHurt, DelegateCombination(XAIComponent, OnSkillHurt, this));
	RegisterAction(XAction_Enmity, DelegateCombination(XAIComponent, OnEnmity, this));
	RegisterAction(XAction_AIEnable, DelegateCombination(XAIComponent, OnAIEnable, this));
	RegisterAction(XAction_AIDisable, DelegateCombination(XAIComponent, OnAIStop, this));
	RegisterAction(XAction_OnPlayerTransfer, DelegateCombination(XAIComponent, OnPlayerTranser, this));
	RegisterAction(XAction_ArmorBroken, DelegateCombination(XAIComponent, OnArmorBroken, this));
	RegisterAction(XAction_ArmorRecover, DelegateCombination(XAIComponent, OnWoozyArmorRecover, this));
	RegisterAction(XAction_AIEvent, DelegateCombination(XAIComponent, OnReceiveAIEvent, this));
	RegisterAction(XAction_RealDead, DelegateCombination(XAIComponent, OnDeathEvent, this));
}

void XAIComponent::InitIndex()
{
	AINodeFactory::SetActiveTree("");

	_target_index = AINodeFactory::CreateIndexByName("target");
	_master_index = AINodeFactory::CreateIndexByName("master");
	_navtarget_index = AINodeFactory::CreateIndexByName("navtarget");
	_is_oppo_casting_skill_index = AINodeFactory::CreateIndexByName("is_oppo_casting_skill");
	_is_hurt_oppo_index = AINodeFactory::CreateIndexByName("is_hurt_oppo");
	_target_distance_index = AINodeFactory::CreateIndexByName("target_distance");
	_is_fixed_in_cd_index = AINodeFactory::CreateIndexByName("is_fixed_in_cd");
	_normal_attack_prob_index = AINodeFactory::CreateIndexByName("normal_attack_prob");
	_enter_fight_range_index = AINodeFactory::CreateIndexByName("enter_fight_range");
	_fight_together_dis_index = AINodeFactory::CreateIndexByName("fight_together_dis");
	_is_wander_index = AINodeFactory::CreateIndexByName("is_wander");
	_max_hp_index = AINodeFactory::CreateIndexByName("max_hp");
	_current_hp_index = AINodeFactory::CreateIndexByName("current_hp");
	_max_super_armor_index = AINodeFactory::CreateIndexByName("max_super_armor");
	_current_super_armor_index = AINodeFactory::CreateIndexByName("current_super_armor");
	_type_index = AINodeFactory::CreateIndexByName("type");
	_target_rotation_index = AINodeFactory::CreateIndexByName("target_rotation");
	_attack_range_index = AINodeFactory::CreateIndexByName("attack_range");
	_min_keep_range_index = AINodeFactory::CreateIndexByName("min_keep_range");
	_is_casting_skill_index = AINodeFactory::CreateIndexByName("is_casting_skill");
	_is_fighting_index = AINodeFactory::CreateIndexByName("is_fighting");
	_is_qte_state_index = AINodeFactory::CreateIndexByName("is_qte_state");
	_movedir_index = AINodeFactory::CreateIndexByName("movedir");
	_movedest_index = AINodeFactory::CreateIndexByName("movedest");
	_movespeed_index = AINodeFactory::CreateIndexByName("movespeed");
	_bornpos_index = AINodeFactory::CreateIndexByName("bornpos");
	_skill_id_index = AINodeFactory::CreateIndexByName("skillid");
	_heart_rate_index = AINodeFactory::CreateIndexByName("heartrate");

	///> 以下属于不存index的值 但需先放到Common里的变量，因为每个Entity都有创建
	AINodeFactory::CreateIndexByName("ratioleft");
	AINodeFactory::CreateIndexByName("ratiodistance");
	AINodeFactory::CreateIndexByName("ratioexp");
	AINodeFactory::CreateIndexByName("ratioidle");
	AINodeFactory::CreateIndexByName("ratioright");
	AINodeFactory::CreateIndexByName("ratioskill");
}

void XAIComponent::InitBehaviorTree()
{
	
	if (_behaviorTree == NULL)
	{
		_behaviorTree = new AIBehaviorTree();
		_inited = _behaviorTree->Init(_host->GetAIString());
	}
}

void XAIComponent::InitVariables()
{
	_host->GetAIArgs(_ai_args);

	if (_ai_args.mActionGap <= 0)
	{
		LogError("AI heart rate error, templateid[%d]", _host->GetTemplateID());
		_ai_args.mActionGap = 1;
	}

	XSkillCore* pysicalSkill = NULL;
	
	if (GetEntity() && GetEntity()->SkillManager())
		pysicalSkill = GetEntity()->SkillManager()->GetPhysicalSkill();

	if (pysicalSkill != NULL)
	{
		_attack_range = pysicalSkill->CastRangeUpper();
		_min_keep_range = pysicalSkill->CastRangeLower();
	}
	else
	{
		_attack_range = 0;
		_min_keep_range = 0;
	}

	_born_pos = GetEntity()->GetPosition();

	if (GetEntity()->SkillManager() != NULL)
	{
		for (auto it = GetEntity()->SkillManager()->GetSkillOrder().begin(); it != GetEntity()->SkillManager()->GetSkillOrder().end(); it ++)
		{
			UINT32 skillid = (*it)->ID();
			int skillLevel = (*it)->Level();
			double costMP = XSkillEffectMgr::Instance()->GetSkillCostMP(skillid, skillLevel, _host->GetSkillCasterTypeID());
			int nSkillHpMaxLimit = XSkillEffectMgr::Instance()->GetSkillHpMaxLimit((*it)->GetSoul()->Name, 1, _host->GetSkillCasterTypeID());
			int nSkillHpMinLimit = XSkillEffectMgr::Instance()->GetSkillHpMinLimit((*it)->GetSoul()->Name, 1, _host->GetSkillCasterTypeID());

			_skillid2costmp.insert(std::make_pair(skillid, costMP));
			_skillid2hpmax.insert(std::make_pair(skillid, nSkillHpMaxLimit));
			_skillid2hpmin.insert(std::make_pair(skillid, nSkillHpMinLimit));
		}
	}

}

void XAIComponent::InitLinkedSkills()
{
	_link_cannot_skills.clear();

	if (GetEntity()->MobbedBy() != 0)
	{
		for (auto it = GetEntity()->SkillManager()->GetSkillOrder().begin(); it != GetEntity()->SkillManager()->GetSkillOrder().end(); it ++)
		{
			UINT32 skillid = (*it)->ID();

			if (XSkillEffectMgr::Instance()->AICantCast(skillid, (*it)->Level(), _host->GetTemplateID()))
				_link_cannot_skills.insert(skillid);

		}
	}
}

bool XAIComponent::IsLinkedSkillCannotCast(UINT32 skillid)
{
	if (GetEntity()->MobbedBy() == 0 || _link_cannot_skills.size() == 0)
		return false;

	return _link_cannot_skills.find(skillid) != _link_cannot_skills.end();
}

void XAIComponent::InitFixVariables()
{
	if (!_behaviorTree) return;

	AINodeFactory::SetActiveTree(_host->GetAIString());

	_behaviorTree->Data().SetFloatByIndex(AINodeFactory::CreateIndexByName("ratioleft"), _ai_args.mRatioLeft);
	_behaviorTree->Data().SetFloatByIndex(AINodeFactory::CreateIndexByName("ratiodistance"), _ai_args.mRatioDistance);
	_behaviorTree->Data().SetFloatByIndex(AINodeFactory::CreateIndexByName("ratioexp"), _ai_args.mRatioExp);
	_behaviorTree->Data().SetFloatByIndex(AINodeFactory::CreateIndexByName("ratioidle"), _ai_args.mRatioIdle);
	_behaviorTree->Data().SetFloatByIndex(AINodeFactory::CreateIndexByName("ratioright"), _ai_args.mRatioRight);
	_behaviorTree->Data().SetFloatByIndex(AINodeFactory::CreateIndexByName("ratioskill"), _ai_args.mRatioSkill);
	_behaviorTree->Data().SetTransformByIndex(_master_index, NULL);  //服务器此处和客户端有所不同
	_behaviorTree->Data().SetFloatByIndex(_normal_attack_prob_index, _ai_args.mNormalAttackProb);
	_behaviorTree->Data().SetFloatByIndex(_enter_fight_range_index, _ai_args.mEnterFightRange);
	_behaviorTree->Data().SetFloatByIndex(_fight_together_dis_index, _ai_args.mFightTogetherDis);
	_behaviorTree->Data().SetBoolByIndex(_is_wander_index, _ai_args.mIsWander);
	_behaviorTree->Data().SetIntByIndex(_type_index, _ai_args.mType);
	_behaviorTree->Data().SetFloatByIndex(_attack_range_index, _attack_range);
	_behaviorTree->Data().SetFloatByIndex(_min_keep_range_index, _min_keep_range);
	_behaviorTree->Data().SetBoolByIndex(_is_casting_skill_index, _is_casting_skill);
	_behaviorTree->Data().SetBoolByIndex(_is_qte_state_index, _is_qte_state);
	_behaviorTree->Data().SetVector3ByIndex(_bornpos_index, _born_pos);

	//_behaviorTree->Data().SetFloatByIndex(_max_hp_index, _max_hp);
	//_behaviorTree->Data().SetFloatByIndex(_current_hp_index, _current_hp);
	//_behaviorTree->Data().SetFloatByIndex(_max_super_armor_index, _max_super_armor);
	//_behaviorTree->Data().SetFloatByIndex(_current_super_armor_index, _current_super_armor);
	//_behaviorTree->Data().SetFloatByIndex(_target_rotation_index, _target_rotation);
	_behaviorTree->SetHost(_host);
}


void XAIComponent::InitDummyRoleSkills()
{
	if (!_host->IsDummyRole() && !_host->IsRole())
		return;

	_dummyrole_skills.clear();

	DummyRole* dummy = dynamic_cast<DummyRole*>(_host);
	if(dummy)
	{
		const vector<UINT32>& skills = dummy->GetBindSkills();
		for(auto i = skills.begin(); i != skills.end(); ++i)
		{
			XSkillCore* core = GetSkillMgr()->GetSkill(*i);
			if(core && !GetSkillMgr()->IsPhysicalAttack(core->ID()) && core->ID() != GetSkillMgr()->GetDashIdentity()
				&& core->ID() != GetSkillMgr()->GetDisappearIdentity() && core->ID() != GetSkillMgr()->GetAppearIdentity()
				&& core->GetSoul()->Logical.CanCastAt_QTE == 0)
				_dummyrole_skills.push_back(core);
		}
	}

	Role* role = dynamic_cast<Role*>(_host);

	if (role)
	{
		const vector<XSkillCore*>& skills = GetSkillMgr()->GetSkillOrder();
		for(auto i = skills.begin(); i != skills.end(); ++i)
		{
			XSkillCore* core = (*i);
			if(core && !GetSkillMgr()->IsPhysicalAttack(core->ID()) && core->ID() != GetSkillMgr()->GetDashIdentity()
				&& core->ID() != GetSkillMgr()->GetDisappearIdentity() && core->ID() != GetSkillMgr()->GetAppearIdentity()
				&& core->GetSoul()->Logical.CanCastAt_QTE == 0)
				_dummyrole_skills.push_back(core);
		}
	}
}

//消息响应
bool XAIComponent::OnTickAI(IArgs* pargs, void *)
{
	assert(_magic_num == 0xDCBA9876);

	if (!_bIsEnable || !_inited)
		return false;

	//if ((_host->IsRole() && !_gm_autoplay) && (dynamic_cast<GlobalAIUnit*>(_host)) == NULL) // 玩家AI不做tick
	//	return false;

	if (!XEntity::ValidEntity(_host->GetID()) && (dynamic_cast<GlobalAIUnit*>(_host)) == NULL)
		return false;

	if (_behaviorTree != NULL && (XEntity::ValidEntity(_host->GetID()) || (dynamic_cast<GlobalAIUnit*>(_host)) != NULL))
	{
		UpdateVariable();
		SetTreeVariable();
		_enmity_list.UpdateProvoke();
		_behaviorTree->Tick();
		_action_gap_factor = _behaviorTree->Data().GetFloatByIndex(_heart_rate_index);
		if (_action_gap_factor == 0)_action_gap_factor = 1.0f;
	}

	_ai_event.EventArg = "";

	AIFire(_ai_args.mActionGap*_action_gap_factor);
	return true;
}

bool XAIComponent::OnAIEnterFight(IArgs* pargs, void *)
{
	assert(_magic_num == 0xDCBA9876);

	if (_is_fighting)
		return false;

	XAIEnterFightEventArgs* enterFight = (XAIEnterFightEventArgs*)pargs;
	Unit* target =  Unit::FindUnit(enterFight->TargetID);

	InitLinkedSkills();

	if (target)
	{
		SetFighting(true);
		SendEnterFightEvent();
		SendEnterFightToClient();
		_target_id = enterFight->TargetID;

		if(_scene_handler)
			_scene_handler->OnEnterFight(target);

		if (_host->GetCurrScene())
			_host->GetCurrScene()->AddEnterFight(target->GetID());

		_behaviorTree->Data().SetTransformByIndex(_target_index, target);
		_behaviorTree->Data().SetFloatByIndex(_target_distance_index, (float)_host->Distance(target));
		return true;
	}

	return false;
}

bool XAIComponent::OnAIEnable(IArgs* pargs, void*)
{
	assert(_magic_num == 0xDCBA9876);

	_bIsEnable = true;
	AIFire(0.0f);
	return true;
}

bool XAIComponent::OnAIStop(IArgs* pargs, void *)
{
	assert(_magic_num == 0xDCBA9876);

	_bIsEnable = false;

	if (_TimerToken != INVALID_HTIMER)
	{
		SafeKillTimer();

		if(_host && _host->GetXObject() && _host->GetXObject()->GetUnit())
		{
			XNavigationComponent* xna = _host->GetXObject()->NavigationComponent();
			if (xna)
			{
				if(_host->GetXObject()->GetUnit()->IsDummyRole())
				{
					xna->StopMove();
				}
				if(xna->IsOnUse())
				{
					xna->SetUseState(false);
				}
			}

			GetEntity()->Idled();

			XMoveActionArgs args;
			args.Speed = 1;
			args.Destination = _host->GetXObject()->GetPosition_p();
			_host->GetXObject()->DispatchAction(&args);
		}

		if(_TimerToken != INVALID_HTIMER)
			LogError("Set AIComponent token Invalid");
		
	}
	return true;
}

void XAIComponent::SetGmAutoPlay(bool enable)
{
	_gm_autoplay = enable; 
	Enable(true);

	if (_host->IsRole())
	{
		XRole* xrole = dynamic_cast<XRole*>(_host->GetXObject());
		if (xrole)xrole->InstallNavComponent();
	}
	

	if (enable) AIFire(0.1f);
}

void XAIComponent::UpdateVariable()
{
	if (_target_id)
	{
		Unit* target = Unit::FindUnit(_target_id);
		if (!target)
			return;

		_target_distance = (float)_host->Distance(target);
		_target_distance -= target->GetBoundRaidus();

		if (_target_distance <= 0) _target_distance = 0;
	}
	else
	{
		_target_distance = 9999999;
		_targets.clear();

		_max_hp = (float)_host->GetAttr(TOTAL_MaxHP);
		_current_hp = (float)_host->GetAttr(TOTAL_CurrentHP);
		_max_super_armor = (float)_host->GetAttr(TOTAL_MaxSuperArmor);
		_current_super_armor = (float)_host->GetAttr(TOTAL_CurrentSuperArmor);
	}
}

void XAIComponent::SetTreeVariable()
{
	_behaviorTree->Data().SetTransformByIndex(_target_index, Unit::FindUnit(_target_id));
	_behaviorTree->Data().SetVector3ByIndex(_navtarget_index, _navtarget);
	_behaviorTree->Data().SetFloatByIndex(_target_distance_index, _target_distance);
	//_behaviorTree->Data().SetVector3ByIndex(_movedir_index, Vector3::zero);
	//_behaviorTree->Data().SetVector3ByIndex(_movedest_index, Vector3::zero);
	_behaviorTree->Data().SetFloatByIndex(_movespeed_index, 1.0f);

	if (_master_id != 0)
	{
		_behaviorTree->Data().SetTransformByIndex(_master_index, Unit::FindUnit(_master_id));
	}
	else if (GetEntity()->MobbedBy() != 0)
	{
		_master_id = GetEntity()->MobbedBy();
	}
}

void XAIComponent::EnemyDetected(Unit* pTarget)
{
	if (GetEntity()->IsDead())
		return;

	if (_scene_handler)
		_scene_handler->OnEnemyDetected(pTarget);
}

void XAIComponent::NotifyAllyIntoFight(Unit* target)
{
	std::vector<Unit*> allys;
	FindAllyCondition AllyCond(_host);
	_host->GetCurrScene()->GetUnitByCondition(AllyCond, allys);
	for (auto it = allys.begin(); it != allys.end(); ++it)
	{
		Unit* pUnit = *it;
		if (pUnit->IsDead() || !pUnit->AIComponent() || pUnit->AIComponent()->IsFighting())
			continue;

		if (_host->Distance(pUnit) > FightTogetherDis())
			continue;

		XAIEnterFightEventArgs oArgs;
		oArgs.TargetID = target->GetID();
		(*it)->GetXObject()->DispatchAction(&oArgs);
	}
}

void XAIComponent::AIFire(float fTime)
{
	SafeKillTimer();

	if(fTime <= 0.00001f)
	{
		XAIAutoFightEventArgs oArgs;
		oArgs.PresentId = _host_presentid;
		oArgs.TemplateId = _host_templateid;
		oArgs.HeartRateTime = _ai_args.mActionGap;
		oArgs.AlphaTime = _action_gap_factor;
		oArgs.EventTime = fTime;
		oArgs.TypeId = _host_typeid;
		if (_host->GetCurrScene())
			oArgs.SceneId = _host->GetCurrScene()->GetSceneTemplateID();
		GetHost()->DispatchAction(&oArgs);
	}
	else
	{
		XAIAutoFightEventArgs *pArgs = new XAIAutoFightEventArgs;
		pArgs->PresentId = _host_presentid;
		pArgs->TemplateId = _host_templateid;
		pArgs->HeartRateTime = _ai_args.mActionGap;
		pArgs->AlphaTime = _action_gap_factor;
		pArgs->EventTime = fTime;
		pArgs->TypeId = _host_typeid;
		if (_host->GetCurrScene())
			pArgs->SceneId = _host->GetCurrScene()->GetSceneTemplateID();
		_TimerToken = GetHost()->DelayAction(pArgs, fTime);
	}
}


bool XAIComponent::OnStartSkill(IArgs* pargs, void *)
{
	assert(_magic_num == 0xDCBA9876);

	XAIStartSkillEventArgs* startSkill = (XAIStartSkillEventArgs*) pargs;

	if(startSkill->IsCaster)
	{
		_is_hurt_oppo = false;
		_is_casting_skill = true;
		_cast_skillid = startSkill->SkillId;
		_behaviorTree->Data().SetIntByIndex(_skill_id_index, _cast_skillid);
		_behaviorTree->Data().SetBoolByIndex(_is_hurt_oppo_index, _is_hurt_oppo);
		_behaviorTree->Data().SetBoolByIndex(_is_casting_skill_index, _is_casting_skill);
	}

	return true;
}

bool XAIComponent::OnEndSkill(IArgs* pargs, void *)
{
	assert(_magic_num == 0xDCBA9876);

	XAIEndSkillEventArgs* endSkill = (XAIEndSkillEventArgs*)pargs;

	if (endSkill->IsCaster)
	{
		if ((int)GetEntity()->SkillManager()->GetDashIdentity() != endSkill->SkillId)
		{
			_is_casting_skill = false;
			_cast_skillid = 0;
			//_is_hurt_oppo = false;
			_behaviorTree->Data().SetBoolByIndex(_is_casting_skill_index, _is_casting_skill);
		}
	}

	return true;
}

bool XAIComponent::OnSkillHurt(IArgs* pargs, void *)
{
	assert(_magic_num == 0xDCBA9876);

	XAISkillHurtEventArgs* skillHurt = (XAISkillHurtEventArgs*)pargs;
	_hurt_skill_id = skillHurt->SkillId;

	if (skillHurt->IsCaster)
	{
		_is_hurt_oppo = true;
		_behaviorTree->Data().SetBoolByIndex(_is_hurt_oppo_index, _is_hurt_oppo);

		Unit* hurt = Unit::FindUnit(skillHurt->TargetUid);

		if (!hurt) return false;

		_hurt_enemy_id = hurt->GetTemplateID();

		for (UINT32 i=0; i<GetGlobalConfig()._send_msg_skill_ids.size(); i++)
		{
			if (_hurt_skill_id == GetGlobalConfig()._send_msg_skill_ids[i])
			{
				XAIEventArgs args;
				args.MsgTargetTemplateId = 0;
				args.EventArg = "skillhurt";
				args.Templateid = _host->GetTemplateID();
				args.Pos = hurt->GetXObject()->GetPosition_p();
				args.FloatArg = hurt->GetXObject()->GetFaceDegree();
				args.SkillId = _hurt_skill_id;
				args.EnemyTemplateId = hurt->GetTemplateID();

				if (hurt->GetCurrScene())
					hurt->GetCurrScene()->MsgChannel().SendMsgTo(AISceneMsgChannel::GlobalAI, AISceneMsgChannel::Cmd, args);
			}
		}
	}
	else
	{
		Unit* caster = Unit::FindUnit(skillHurt->CasterUid);
		
		if (caster)
		{
			if (!_host->IsRole())
			{
				auto it = GetGlobalConfig()._provoke_skill_ids.begin();

				while (it != GetGlobalConfig()._provoke_skill_ids.end())
				{
					if (it->first == _hurt_skill_id)
					{
						_enmity_list.SetProvokeUnit(caster, it->second);
						break;
					}
					it ++;
				}
			}
		}
	}

	return true;
}

bool XAIComponent::OnEnmity(IArgs* pargs, void *)
{
	assert(_magic_num == 0xDCBA9876);

	XEnmityEventArgs* enmityEvent = (XEnmityEventArgs*)pargs;
	
	if (!enmityEvent)
		return false;

	Unit* starter =  Unit::FindUnit(enmityEvent->m_caster);

	if (starter == _host)
		return false;

	if (starter)
	{
		UINT32 skilllevel = starter->GetSkillLevelMgr()->GetSkillLevel(enmityEvent->m_skillid);
		SkillList::RowData* skill = XSkillEffectMgr::Instance()->GetSkillConfig(enmityEvent->m_skillid, skilllevel, starter->GetSkillCasterTypeID());

		if (skill)
			_enmity_list.AddHateValue(starter, enmityEvent->m_value*skill->EnmityRatio+skill->EnmityExtValue);
		else
			_enmity_list.AddHateValue(starter, enmityEvent->m_value);
		NotifyAllyIntoFight(starter);
	}

	return true;
}


bool XAIComponent::OnPlayerTranser(IArgs* pargs, void *)
{
	assert(_magic_num == 0xDCBA9876);

	return true;
}

bool XAIComponent::OnArmorBroken(IArgs* pargs, void *)
{
	assert(_magic_num == 0xDCBA9876);

	const XEntityStatistics::RowData* enemy = XEntityInfoLibrary::Instance()->TemplateID2Data_Enemy(_host->GetTemplateID());
	if (_TimerToken != INVALID_HTIMER && enemy && enemy->WeakStatus)
	{
		SafeKillTimer();
		Scene* curScene = _host->GetCurrScene();

		if(!curScene)
			return false;

		XAIEventArgs args;
		args.MsgTargetTemplateId = 0;
		args.EventArg = "ArmorBroken";
		args.Templateid = _host->GetTemplateID();
		args.Pos = _host->GetXObject()->GetPosition_p();
		args.FloatArg = _host->GetXObject()->GetFaceDegree();

		curScene->MsgChannel().SendMsgTo(AISceneMsgChannel::GlobalAI, AISceneMsgChannel::Cmd, args);
	}

	return true;
}

bool XAIComponent::OnReceiveAIEvent(IArgs* pargs, void *)
{
	assert(_magic_num == 0xDCBA9876);

	XAIEventArgs* eventArgs = dynamic_cast<XAIEventArgs*>(pargs);

	if (!eventArgs)
		return false;

	_ai_event.EventType = eventArgs->EventType;
	_ai_event.EventArg = eventArgs->EventArg;
	_ai_event.Templateid = eventArgs->Templateid;
	_ai_event.Pos = eventArgs->Pos;
	_ai_event.WaveId = eventArgs->WaveId;
	_ai_event.UnitId = eventArgs->UnitId;
	_ai_event.SkillId = eventArgs->SkillId;
	_ai_event.FloatArg = eventArgs->FloatArg;
	_ai_event.EnemyTemplateId = eventArgs->EnemyTemplateId;
	_ai_event.SenderUID = eventArgs->SenderUID;

	if (_TimerToken != INVALID_HTIMER)
	{
		AIFire(0.0f);
	}

	_ai_event.EventType = 0;
	_ai_event.EventArg = "";

	return true;
}

bool XAIComponent::OnDeathEvent(IArgs* pargs, void *)
{
	assert(_magic_num == 0xDCBA9876);

	SafeKillTimer();

	for (UINT32 i=0; i<_timer_token.size(); i++)
	{
		CTimerMgr::Instance()->KillTimer(_timer_token[i]);
	}

	_timer_token.clear();

	Scene* curScene = _host->GetCurrScene();

	if(!curScene)
		return false;

	XAIEventArgs args;
	args.MsgTargetTemplateId = 0;
	args.EventArg = "Dead";
	args.Templateid = _host->GetTemplateID();
	args.Pos = _host->GetXObject()->GetPosition_p();
	args.FloatArg = _host->GetXObject()->GetFaceDegree();
	args.SkillId = 0;
	args.EnemyTemplateId = _hurt_enemy_id;
	args.SenderUID = _host->GetKillerID();
	if (_host->GetKiller()->GetKillerSkill() != 0)
		args.SkillId = (UINT32)_host->GetKiller()->GetKillerSkill();

	if (_debug_level > 0)
	{
		char infobuff[256]; sprintf(infobuff, "Kill skill id: %u", _hurt_skill_id);
		AIOnlineDebugMgr::Instance()->SendDebugMsg(curScene->GetSceneID(), infobuff);
	}

	if (_host->IsEnemy())
	{
		Enemy* enemy = dynamic_cast<Enemy*>(_host);
		args.UnitId = _host->GetID();
		args.WaveId = enemy->GetWaveID();
		curScene->MsgChannel().SendMsgTo(AISceneMsgChannel::GlobalAI, AISceneMsgChannel::Cmd, args);
	}
	else
	{
		//LogInfo("Role dead, roleid[%d]", _host->GetTemplateID());

		if (_host->IsRole())
			args.EventArg = "PlayerDead";
		curScene->MsgChannel().SendMsgTo(AISceneMsgChannel::GlobalAI, AISceneMsgChannel::Cmd, args);
	}

	

	return true;
}

bool XAIComponent::OnWoozyArmorRecover(IArgs* pargs, void *)
{
	assert(_magic_num == 0xDCBA9876);

	AIFire(_action_gap_factor* _ai_args.mActionGap);
	return true;
}

XSkillMgr* XAIComponent::GetSkillMgr()
{
	return GetEntity()->SkillManager();
}

bool XAIComponent::InitOpponetEnmity(IArgs*, void*)
{
	if (!_host)
		return false;

	Scene* curScene = _host->GetCurrScene();
	if (!curScene) 
		return false;
	const std::vector<Unit*>* opponent = curScene->GetOpponents(_host->GetFightGroup());
	if (NULL != opponent)
	{
		bool initmsgsend = false;
		for (UINT32 i = 0; i < opponent->size(); i++)
		{
			if (!XEntity::ValidEntity((*opponent)[i]->GetID())) continue;

			const XEntityStatistics::RowData* enemy = XEntityInfoLibrary::Instance()->TemplateID2Data_Enemy((*opponent)[i]->GetTemplateID());

			if (enemy && enemy->InitEnmity > 0)
			{
				_enmity_list.AddHateValue((*opponent)[i], enemy->InitEnmity * 1.0f);
				SetFighting(true);
				_behaviorTree->Data().SetTransformByIndex(_target_index, (*opponent)[i]);
				_behaviorTree->Data().SetFloatByIndex(_target_distance_index, (float)_host->Distance((*opponent)[i]));

				if (!initmsgsend)
				{
					SendEnterFightEvent();
					SendEnterFightToClient();
					NotifyAllyIntoFight((*opponent)[i]);
					initmsgsend = true;
				}
			}
		}
	}


	SafeKillTimer();

	//AIFire(_action_gap_factor* _ai_args.mActionGap);
	
	if (_host->IsDummyRole() && (UINT32)(KKSG::FightRole) == _host->GetFightGroup())
		AIFire(4.0f);
	else
		AIFire(0.1f); // 快点开始AI


	return true;
}

void XAIComponent::AddTimerToken(HTIMER token)
{
	_timer_token.push_back(token);
}

void XAIComponent::SendEnterFightToClient()
{
	XDelegate d = XDelegate::registerMethod<XAIComponent, &XAIComponent::DoSendEnterFightEvent>(this);
	HTIMER timer = XCommon::SetTimer(0.1f, d, __FILE__, __LINE__);	 //CTimerMgr::Instance()->SetTimer(d, NULL, NULL, 100, 1, __FILE__, __LINE__);
	AddTimerToken(timer);
}

bool XAIComponent::DoSendEnterFightEvent(IArgs *args, void *parg)
{
	assert(_magic_num == 0xDCBA9876);

	if (_host->IsEnemy() && _host->GetCurrScene())
	{
		PtcG2C_NotifyClientEnterFight oPtc;
		oPtc.m_Data.set_enterfight(true);
		oPtc.m_Data.set_enemyid(_host->GetID());
		_host->GetCurrScene()->Broadcast(oPtc);
	}
	return true;
}

void XAIComponent::SendEnterFightEvent()
{
	XAIEventArgs* aievent = new XAIEventArgs();
	aievent->Templateid = _host->GetTemplateID();
	aievent->Pos = _host->GetXObject()->GetPosition();
	aievent->EventArg = "enterfight";

	const XEntityStatistics::RowData* enemy = XEntityInfoLibrary::Instance()->TemplateID2Data_Enemy(_host->GetTemplateID());

	float delaytime = 1;
	if (enemy) delaytime = enemy->AIStartTime; 
	HTIMER token = _host->GetXObject()->DelayAction(aievent, delaytime);
	AddTimerToken(token);
}

void XAIComponent::SafeKillTimer()
{
	if(_TimerToken != INVALID_HTIMER)
	{ 
		CTimerMgr::Instance()->KillTimer(_TimerToken);
		_TimerToken = INVALID_HTIMER;
	}
}

void XAIComponent::ResetAILogic()
{
	for (UINT32 i=0; i<_timer_token.size(); i++)
	{
		CTimerMgr::Instance()->KillTimer(_timer_token[i]);
	}

	_timer_token.clear();

	XAIEventArgs* aievent = new XAIEventArgs();
	aievent->Templateid = _host->GetTemplateID();
	aievent->Pos = _host->GetXObject()->GetPosition();
	aievent->EventArg = "reset";
	float delaytime = 1;
	HTIMER token = _host->GetXObject()->DelayAction(aievent, delaytime);
	AddTimerToken(token);
}

void XAIComponent::SetTargets(const std::vector<Unit*>& targets)
{
	_targets.assign(targets.begin(), targets.end());
}

void XAIComponent::OnEnterScene(Scene *scene)
{
	InitOpponetEnmity(NULL, NULL);
	_scene_handler = AISceneHandler::CreateSceneHandler(this, scene);
}
