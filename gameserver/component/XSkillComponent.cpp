#include "pch.h"

#include "XSkillComponent.h"
#include "xdelegate.hpp"
#include "entity/XEntity.h"
#include "skill/XSkillCore.h"
#include "skill/XSkillMgr.h"
#include "skill/XJAComboSkill.h"
#include "skill/XUltraSkill.h"
#include "XNetComponent.h"
#include "XPresentComponent.h"

#include "tablemgr/XEntityInfoLibrary.h"

#include "unit/unit.h"
#include "scene/scene.h"
#include "skill/XBulletMgr.h"
#include "LogInit.h"
#include "entity/XObject.h"
#include "XQuickTimeEventComponent.h"
#include "util/XCommon.h"
#include "XBuffComponent.h"
#include "unit/role.h"
#include "gamelogic/skilllevel.h"
#include "unit/dummyrole.h"

XSkillComponent::XSkillComponent(void)
	:_skill(NULL),
	 _skillMgr(NULL),
	 _machine(NULL),
	 _last_target(0),
	 _skill_timer(INVALID_HTIMER)
{

}

XSkillComponent::~XSkillComponent(void)
{
	_machine = NULL;
}

bool XSkillComponent::OnAttack(IArgs* pargs, void *)
{
	if(GetHost()->GetUnit()->IsDead()) return false;

	XAttackActionArgs* args = static_cast<XAttackActionArgs*>(pargs);

	XSkillCore* soul = _skillMgr->GetSkill(args->Identify);

	if(!soul)
	{
		LogError("Entity [%llu] does not have skill %d", GetEntity()->GetID(), args->Identify);
		return false;
	}

	soul = PhysicalAttackAdaptor(soul);
	if(soul == NULL)
	{
		LogError("Entity [%llu] skill %d Physical Adapter error.", GetEntity()->GetID(), args->Identify);
		return false;
	}

	soul = TryGetSkillReplace(soul->ID(), soul);

	//server we get time scale here...
	args->TimeScale = soul->GetTimeScale();

	UINT64 target = args->Target;

	if(soul->GetSoul()->NeedTarget && target == 0 && !GetEntity()->GetUnit()->IsRole())
	{
		XEntity* plast = XEntity::ValidEntity(_last_target);

		if(plast && soul->IsInAttackField(GetEntity()->GetPosition_p(), GetEntity()->GetFace_p(), plast))
		{
			target = _last_target;
		}
		else
		{
			XEntity* pTar = soul->FindTargetAt(GetEntity()->GetPosition_p(), GetEntity()->GetFace_p());
			target = (pTar == NULL ? 0 : pTar->GetID());
		}
	}

	_last_target = target;

	bool QuickTimeEvent = GetEntity()->QuickTimeEventComponent() ? GetEntity()->QuickTimeEventComponent()->HasSkill(soul->ID()) : false;
	
	bool fired = false;
	bool precheck = true;

	//check skill silence
	if( _skillMgr->GetPhysicalIdentity() != args->Identify && 
		GetEntity()->BuffComponent() && 
		GetEntity()->BuffComponent()->IsBuffStateOn(XBuffType_Silencing))
	{
		precheck = false;
	}

	if(  precheck &&
		//cd check
		(soul->CooledDown() /*&&* mp value*/) &&
		//QTE event skill
		(QuickTimeEvent ||/*for simple logic*/
		 //soul condition checks
		 (soul->CanCast(args->GetToken()) && (_skill == NULL || !_skill->Casting() || _skill->MainCore()->CanReplacedBy(soul)))))
	{
		if(_skill != NULL)
		{
			GetEntity()->SyncIncreament();
			EndSkill();
		}

		XSkill* skill = _skillMgr->GetCarrier(soul->CarrierID());
		//in case check skill casting in Fire call stack...
		_skill = skill;
		/*
		 * enlarge skill start at time due to net latency
		 */
		if(_skill && _skill->Fire(target, soul, args))
		{
			CTimerMgr::Instance()->KillTimer(_skill_timer);
			//set up skill timer
			_skill_timer = XCommon::SetTimer(
				_skill->MainCore()->GetSoul()->Time * _skill->TimeScale(), 
				DelegateCombination(XSkillComponent, AutoEnd, this), 
				NULL, 
				NULL, 
				__FILE__, 
				__LINE__);

			//in case ending during timer setting
			if(IsCasting())
			{
				_skill->Execute();
				fired = true;
			}
		}
		else
		{
			if(_skill && _skill->Casting())
			{
				//another skill is fired in last skill-fire
			}
			else
				_skill = NULL;
		}

		//BUG #25754
		if(!soul->CooledDown() && GetEntity()->NetComponent())
		{
			GetEntity()->NetComponent()->BroadcastCDCall(soul->ID(), soul->CurrentRunTime(), false);
		}
	}

	if(QuickTimeEvent)
	{
		XSkillFiredEventArgs skillfired;
		skillfired.Id = soul->ID();
		skillfired.Succeed = fired;
		GetEntity()->DispatchAction(&skillfired);
	}

	return fired;
}

bool XSkillComponent::AddSkillMob(XEntity* mob)
{
	bool res = false;

	if (std::find(_skill_mobs.begin(), _skill_mobs.end(), mob->GetID()) == _skill_mobs.end()) 
	{
		_skill_mobs.push_back(mob->GetID());
		
		//check limit
		if(mob->GetUnit()->GetSummonGroup() > 0 && (uint)(mob->GetUnit()->GetSummonGroup()) < GetGlobalConfig().EntitySummonGroupLimit.size())
		{
			int limit = GetGlobalConfig().EntitySummonGroupLimit[mob->GetUnit()->GetSummonGroup()];
			int count = 0; Unit* deleted = NULL;

			auto it = _skill_mobs.begin();
			while (it != _skill_mobs.end())
			{
				XEntity* pEnt = XEntity::ValidEntity(*it);

				if(pEnt)
				{
					if(pEnt->GetUnit()->GetSummonGroup() == mob->GetUnit()->GetSummonGroup())
					{
						if(count == 0) deleted = pEnt->GetUnit();
						count++;
					}
				}

				++it;
			}

			if(count > limit)
			{
				res = (deleted != mob->GetUnit());
				deleted->TriggerDeath();
			}
			else
				res = true;
		}
		else
			res = true;
	}

	return res;
}

bool XSkillComponent::AutoEnd(IArgs* pargs, void *)
{
	_skill_timer = INVALID_HTIMER;

	EndSkill();
	return true;
}

bool XSkillComponent::OnRealDead(IArgs* pargs, void *)
{
	XRealDeadActionArgs* args = static_cast<XRealDeadActionArgs*>(pargs);

	EndSkill();
	//BUG #47076
	XBulletMgr::Instance()->ClearBullets(GetEntity()->GetID());

	return true;
}

bool XSkillComponent::OnLeaveScene(IArgs* pargs, void *)
{
	EndSkill();
	XBulletMgr::Instance()->ClearBullets(GetEntity()->GetID());

	return true;
}

bool XSkillComponent::OnPresevedStrengthStop(IArgs* pargs, void *)
{
	if( IsCasting() &&
		CurrentSkill()->MainCore()->PreservedStrength() > 0 && 
		CurrentSkill()->TimeElapsed() < CurrentSkill()->MainCore()->GetSoul()->Logical.PreservedEndAt)
	{
		EndSkill();
	}

	return true;
}

bool XSkillComponent::OnAddSkill(IArgs* pargs, void *)
{
	XAddSkillEventArgs* args = static_cast<XAddSkillEventArgs*>(pargs);
	_skillMgr->AttachSkillByID(args->SkillHash);

	return true;
}

bool XSkillComponent::OnRemoveSkill(IArgs* pargs, void *)
{
	XRemoveSkillEventArgs* args = static_cast<XRemoveSkillEventArgs*>(pargs);
	_skillMgr->DetachSkill(args->SkillHash);

	return true;
}

bool XSkillComponent::OnDynamicCDRatioChanged(IArgs* pargs, void *)
{
	XAttributeChangedEventArgs* args = static_cast<XAttributeChangedEventArgs*>(pargs);

	if(args->AttrID == BASIC_SKILL_CD || args->AttrID == PERCENT_SKILL_CD)
	{
		const vector<XSkillCore*>& skills = _skillMgr->GetSkillOrder();

		auto it = skills.begin();
		while(it != skills.end())
		{
			(*it)->GetCoolDown();

			++it;
		}
	}

	return true;
}

void XSkillComponent::OnAttachToHost(XObject* pHost)
{
	XComponent::OnAttachToHost(pHost);

	_skillMgr = new XSkillMgr(GetEntity());

	if(XEntity::IsValidEntity(GetEntity()->GetUnit()->GetHostUnit()))
		GetEntity()->GetUnit()->GetSkillLevelMgr()->RefreshMobLinkedLevels(GetEntity()->GetUnit(), GetEntity()->GetUnit()->GetHostUnit());

	if(!GetEntity()->GetUnit()->IsEnterHall()) AttachSkill();
}

void XSkillComponent::OnDetachFromHost()
{
	EndSkill();

	if(_skillMgr != NULL)
	{
		delete _skillMgr;
		_skillMgr = NULL;
	}

	XComponent::OnDetachFromHost();
}

void XSkillComponent::ActionSubscribe()
{
	RegisterAction(XAction_Attack, DelegateCombination(XSkillComponent, OnAttack, this));
	RegisterAction(XAction_RealDead, DelegateCombination(XSkillComponent, OnRealDead, this));
	RegisterAction(XAction_StrengthPresevationStop, DelegateCombination(XSkillComponent, OnPresevedStrengthStop, this));
	RegisterAction(XAction_LeaveScene, DelegateCombination(XSkillComponent, OnLeaveScene, this));

	RegisterAction(XAction_AddSkill, DelegateCombination(XSkillComponent, OnAddSkill, this));
	RegisterAction(XAction_RemoveSkill, DelegateCombination(XSkillComponent, OnRemoveSkill, this));

	RegisterAction(XAction_AttributeChange, DelegateCombination(XSkillComponent, OnDynamicCDRatioChanged, this));
}

void XSkillComponent::Update(float fDeltaT)
{
	if(_skill != NULL && !_skill->Update(fDeltaT))
	{
		EndSkill();
	}
}

void XSkillComponent::EndSkill()
{
	if(_skill != NULL)
	{
		if(_skill_timer != INVALID_HTIMER)
		{
			CTimerMgr::Instance()->KillTimer(_skill_timer);
			_skill_timer = INVALID_HTIMER;
		}

		_skill->Cease();
		_skill = NULL;
	}
}

bool XSkillComponent::IsOverResults()
{
	if(IsCasting())
	{
		vector<XResultData>& data = _skill->Core()->GetSoul()->Result;
		return data.size() > 0 && data[data.size() - 1].At < _skill->TimeElapsed();
	}

	return true;
}

bool XSkillComponent::CanPerformAction(XStateDefine state, XActionArgs* pargs)
{
	return (_skill == NULL) ?  true : _skill->CanPerformAction(state, pargs->GetToken());
}

void XSkillComponent::ReAttachSkill(UINT32 presentid)
{
	XBulletMgr::Instance()->ClearBullets(GetEntity()->GetID());

	if(_skillMgr != NULL)
	{
		delete _skillMgr;
		_skillMgr = NULL;
	}

	_skillMgr = new XSkillMgr(GetEntity());

	const XEntityPresentation::RowData* pData = XEntityInfoLibrary::Instance()->PresentID2Data(presentid);

	if(pData && GetEntity()->IsTransform())
	{
		const string& skill_prefix = "SkillPackage/" + pData->SkillLocation;
		const XPresentComponent* present = GetEntity()->PresentComponent();
		const XEntityPresentation::RowData* origin = present->PresentLib();

		XSkillData *pSkillData = NULL;

		pSkillData = _skillMgr->LoadSkillData(pData->A, skill_prefix); if(pSkillData) _skillMgr->AttachPhysicalSkill(pSkillData);
		pSkillData = _skillMgr->LoadSkillData(pData->AA, skill_prefix); if(pSkillData) _skillMgr->AttachSkill(pSkillData, false);
		pSkillData = _skillMgr->LoadSkillData(pData->AAA, skill_prefix); if(pSkillData) _skillMgr->AttachSkill(pSkillData, false);
		pSkillData = _skillMgr->LoadSkillData(pData->AAAA, skill_prefix); if(pSkillData) _skillMgr->AttachSkill(pSkillData, false);
		pSkillData = _skillMgr->LoadSkillData(pData->AAAAA, skill_prefix); if(pSkillData) _skillMgr->AttachSkill(pSkillData, false);
		pSkillData = _skillMgr->LoadSkillData(pData->Appear, skill_prefix); if(pSkillData) _skillMgr->AttachAppearSkill(pSkillData);
		pSkillData = _skillMgr->LoadSkillData(pData->Disappear, skill_prefix); if(pSkillData) _skillMgr->AttachDisappearSkill(pSkillData);
		pSkillData = _skillMgr->LoadSkillData(pData->Dash, skill_prefix); if(pSkillData) _skillMgr->AttachDashSkill(pSkillData);
		pSkillData = _skillMgr->LoadSkillData(pData->ArmorBroken, skill_prefix); if(pSkillData) _skillMgr->AttachBrokenSkill(pSkillData);
		pSkillData = _skillMgr->LoadSkillData(pData->SuperArmorRecoverySkill, skill_prefix); if(pSkillData) _skillMgr->AttachRecoverySkill(pSkillData);


		if(pData->OtherSkills.size() > 0)
		{
			vector<string>::const_iterator it = pData->OtherSkills.begin();
			while (it != pData->OtherSkills.end())
			{
				pSkillData = _skillMgr->LoadSkillData(*it, skill_prefix); if(pSkillData) _skillMgr->AttachSkill(pSkillData);
				++it;
			}
		}
		else if(GetEntity()->GetUnit()->IsRole())
		{
			Role *pRole = dynamic_cast<Role*>(GetEntity()->GetUnit());
			const std::vector<UINT32>& skills = pRole->GetRoleSkillLevelMgr()->GetBindSkillIDS();

			std::vector<UINT32>::const_iterator it = skills.begin();
			while(it != skills.end())
			{
				if(*it)
				{
					SkillList::RowData* data = XSkillEffectMgr::Instance()->GetSkillConfig(*it, 0);
					if(data) OtherAttachFilter(*it, data, pData, present->SkillPrefix());
				}

				++it;
			}

			//QTE skill and basic skill
			for (auto i = pRole->Get<RoleSkillSet>()->begin(); i != pRole->Get<RoleSkillSet>()->end(); i++)
			{
				SkillList::RowData* data = XSkillEffectMgr::Instance()->GetSkillConfig(i->skillhash, 0);

				if (data && (data->IsBasicSkill || data->SkillType == KKSG::Skill_Help))
				{
					OtherAttachFilter(i->skillhash, data, pData, present->SkillPrefix());
				}
			}
		}

		_skillMgr->StatisticsPhysicalSkill();
	}
	else
	{
		InnerAttachSkill(pData);
	}
}

void XSkillComponent::AttachSkill()
{
	UINT32 present_id = GetEntity()->GetUnit()->GetPresentID();

	InnerAttachSkill(GetEntity()->PresentComponent()->PresentLib());
}

void XSkillComponent::InnerAttachSkill(const XEntityPresentation::RowData* pData)
{
	if(!pData) return;

	const string& skill_prefix = "SkillPackage/" + pData->SkillLocation;

	if(GetEntity()->GetUnit()->IsRole() && !GetEntity()->IsTransform())
	{
		Role *pRole = dynamic_cast<Role*>(GetEntity()->GetUnit());
		const std::vector<UINT32>& skills = pRole->GetRoleSkillLevelMgr()->GetBindSkillIDS();

		std::vector<UINT32>::const_iterator it = skills.begin();
		while(it != skills.end())
		{
			if(*it)
			{
				SkillList::RowData* data = XSkillEffectMgr::Instance()->GetSkillConfig(*it, 0);
				if(data) AttachFilter(*it, data, pData, skill_prefix);
			}
			
			++it;
		}

		//QTE skill and basic skill
		for (auto i = pRole->Get<RoleSkillSet>()->begin(); i != pRole->Get<RoleSkillSet>()->end(); i++)
		{
			SkillList::RowData* data = XSkillEffectMgr::Instance()->GetSkillConfig(i->skillhash, 0);

			if (data && (data->IsBasicSkill || data->SkillType == KKSG::Skill_Help))
			{
				AttachFilter(i->skillhash, data, pData, skill_prefix);
			}
		}
	}
	else if(GetEntity()->GetUnit()->IsDummyRole())
	{
		DummyRole *dummyrole = dynamic_cast<DummyRole*>(GetEntity()->GetUnit());
		const std::vector<UINT32>& bindskills = dummyrole->GetBindSkills();

		std::vector<UINT32>::const_iterator it = bindskills.begin();
		while(it != bindskills.end())
		{
			if(*it)
			{
				SkillList::RowData* data = XSkillEffectMgr::Instance()->GetSkillConfig(*it, 0);
				if(data) AttachFilter(*it, data, pData, skill_prefix);
			}

			++it;
		}

		//QTE skill and basic skill
		const std::vector<UINT32>& skills = dummyrole->GetSkills();
		for (auto i = skills.begin(); i != skills.end(); i++)
		{
			SkillList::RowData* data = XSkillEffectMgr::Instance()->GetSkillConfig(*i, 0);

			if (data && (data->IsBasicSkill || data->SkillType == KKSG::Skill_Help))
			{
				AttachFilter(*i, data, pData, skill_prefix);
			}
		}
	}
	else
	{
		XSkillData *pSkillData = NULL;

		pSkillData = _skillMgr->LoadSkillData(pData->A, skill_prefix); if(pSkillData) _skillMgr->AttachPhysicalSkill(pSkillData);
		pSkillData = _skillMgr->LoadSkillData(pData->AA, skill_prefix); if(pSkillData) _skillMgr->AttachSkill(pSkillData, false);
		pSkillData = _skillMgr->LoadSkillData(pData->AAA, skill_prefix); if(pSkillData) _skillMgr->AttachSkill(pSkillData, false);
		pSkillData = _skillMgr->LoadSkillData(pData->AAAA, skill_prefix); if(pSkillData) _skillMgr->AttachSkill(pSkillData, false);
		pSkillData = _skillMgr->LoadSkillData(pData->AAAAA, skill_prefix); if(pSkillData) _skillMgr->AttachSkill(pSkillData, false);
		pSkillData = _skillMgr->LoadSkillData(pData->Appear, skill_prefix); if(pSkillData) _skillMgr->AttachAppearSkill(pSkillData);
		pSkillData = _skillMgr->LoadSkillData(pData->Disappear, skill_prefix); if(pSkillData) _skillMgr->AttachDisappearSkill(pSkillData);
		pSkillData = _skillMgr->LoadSkillData(pData->Dash, skill_prefix); if(pSkillData) _skillMgr->AttachDashSkill(pSkillData);
		pSkillData = _skillMgr->LoadSkillData(pData->ArmorBroken, skill_prefix); if(pSkillData) _skillMgr->AttachBrokenSkill(pSkillData);
		pSkillData = _skillMgr->LoadSkillData(pData->SuperArmorRecoverySkill, skill_prefix); if(pSkillData) _skillMgr->AttachRecoverySkill(pSkillData);

		vector<string>::const_iterator it = pData->OtherSkills.begin();

		while (it != pData->OtherSkills.end())
		{
			pSkillData = _skillMgr->LoadSkillData(*it, skill_prefix); if(pSkillData) _skillMgr->AttachSkill(pSkillData);
			++it;
		}
	}

	_skillMgr->StatisticsPhysicalSkill();
}

void XSkillComponent::AttachFilter(uint id, const SkillList::RowData* data, const XEntityPresentation::RowData* pData, const string& skill_prefix)
{
	XSkillData *pSkillData = _skillMgr->LoadSkillData(data->SkillScript, skill_prefix);

	if(pSkillData)
	{
		if (_skillMgr->GetPhysicalIdentity() == 0 && XCommon::XHash(pData->A.c_str()) == id) _skillMgr->AttachPhysicalSkill(pSkillData);
		else if (_skillMgr->GetUltraIdentity() == 0 && XCommon::XHash(pData->Ultra.c_str()) == id)_skillMgr->AttachUltraSkill(pSkillData);
		else if (_skillMgr->GetAppearIdentity() == 0 && XCommon::XHash(pData->Appear.c_str()) == id)_skillMgr->AttachAppearSkill(pSkillData);
		else if (_skillMgr->GetDisappearIdentity() == 0 && XCommon::XHash(pData->Disappear.c_str()) == id)_skillMgr->AttachDisappearSkill(pSkillData);
		else if (_skillMgr->GetDashIdentity() == 0 && XCommon::XHash(pData->Dash.c_str()) == id)_skillMgr->AttachDashSkill(pSkillData);
		else if (_skillMgr->GetRecoveryIdentity() == 0 && XCommon::XHash(pData->SuperArmorRecoverySkill.c_str()) == id)_skillMgr->AttachRecoverySkill(pSkillData);
		else if (_skillMgr->GetBrokenIdentity() == 0 && XCommon::XHash(pData->ArmorBroken.c_str()) == id)_skillMgr->AttachRecoverySkill(pSkillData);
		else if (XCommon::XHash(pData->AA.c_str()) == id)_skillMgr->AttachSkill(pSkillData, false);
		else if (XCommon::XHash(pData->AAA.c_str()) == id) _skillMgr->AttachSkill(pSkillData, false);
		else if (XCommon::XHash(pData->AAAA.c_str()) == id) _skillMgr->AttachSkill(pSkillData, false);
		else if (XCommon::XHash(pData->AAAAA.c_str()) == id) _skillMgr->AttachSkill(pSkillData, false);
		else _skillMgr->AttachSkill(pSkillData);
	}
}

void XSkillComponent::OtherAttachFilter(uint id, const SkillList::RowData* data, const XEntityPresentation::RowData* pData, const string& skill_prefix)
{
	if (XCommon::XHash(pData->A.c_str()) == id) return;
	else if (XCommon::XHash(pData->Ultra.c_str()) == id) return;
	else if (XCommon::XHash(pData->Appear.c_str()) == id) return;
	else if (XCommon::XHash(pData->Disappear.c_str()) == id) return;
	else if (XCommon::XHash(pData->Dash.c_str()) == id) return;
	else if (XCommon::XHash(pData->SuperArmorRecoverySkill.c_str()) == id) return;
	else if (XCommon::XHash(pData->ArmorBroken.c_str()) == id) return;
	else if (XCommon::XHash(pData->AA.c_str()) == id) return;
	else if (XCommon::XHash(pData->AAA.c_str()) == id) return;
	else if (XCommon::XHash(pData->AAAA.c_str()) == id) return;
	else if (XCommon::XHash(pData->AAAAA.c_str()) == id) return;

	XSkillData *pSkillData = _skillMgr->LoadSkillData(data->SkillScript, skill_prefix);
	if(pSkillData) _skillMgr->AttachSkill(pSkillData);
}

XSkillCore* XSkillComponent::PhysicalAttackAdaptor(XSkillCore* soul)
{
	XRole* role = dynamic_cast<XRole*>(GetEntity());

	if(role)
	{
		if(soul->GetSoul()->Logical.Association)
		{
			if(role->IsClientManualOn())
			{
				if(!soul->GetSoul()->Logical.MoveType)
				{
					soul = GetEntity()->SkillManager()->GetSkill(XCommon::XHash(soul->GetSoul()->Logical.Association_Skill.c_str()));
				}
			}
			else
			{
				if(soul->GetSoul()->Logical.MoveType)
				{
					soul = GetEntity()->SkillManager()->GetSkill(XCommon::XHash(soul->GetSoul()->Logical.Association_Skill.c_str()));
				}
			}
		}
	}

	return soul;
}

void XSkillComponent::SetSkillReplace(UINT32 from, UINT32 to)
{
	if (to == 0)
	{
		auto it = _skills_replace.find(from);
		if (it != _skills_replace.end())
			_skills_replace.erase(it);
	}
	else
	{
		_skills_replace[from] = to;
	}
}

XSkillCore* XSkillComponent::TryGetSkillReplace(UINT32 skillID, XSkillCore* soul)
{
	auto it = _skills_replace.find(skillID);
	if (it == _skills_replace.end())
		return soul;

	XSkillCore* c = _skillMgr->GetSkill(it->second);
	return c ? c : soul;
}
