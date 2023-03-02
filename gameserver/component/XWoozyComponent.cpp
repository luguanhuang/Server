#include "pch.h"
#include "XWoozyComponent.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "unit/unit.h"
#include "entity/XObject.h"
#include "XSkillComponent.h"
#include "skill/XSkillCore.h"
#include "skill/XSkillMgr.h"
#include "util/XCommon.h"
#include "state/XStateMachine.h"
#include "state/XStateDefine.hpp"
#include "XNetComponent.h"
#include "scene/scene.h"
#include "gamelogic/XLevelWave.h"
#include "unit/role.h"
#include "XAIComponent.h"

XWoozyComponent::XWoozyComponent()
	:_OnBroken(0),
	 _OnRecover(0),
	 _OnWoozy(0),
	 _transfer(0),
	 _woozy_enabled(0),
	 _token_on(0),
	 _token_off(0)
{

}

XWoozyComponent::~XWoozyComponent()
{
	Clear();
}

void XWoozyComponent::OnDetachFromHost()
{
	Clear();
	XComponent::OnDetachFromHost();
}

void XWoozyComponent::Attached()
{
	_woozy_enabled = XEntityInfoLibrary::Instance()->TemplateID2Data_Enemy(GetHost()->GetUnit()->GetTemplateID())->WeakStatus;
}

void XWoozyComponent::ActionSubscribe()
{
	RegisterAction(XAction_RealDead, DelegateCombination(XWoozyComponent, OnDead, this));
	RegisterAction(XAction_ArmorBroken, DelegateCombination(XWoozyComponent, OnArmorBroken, this));
	RegisterAction(XAction_ArmorRecover, DelegateCombination(XWoozyComponent, OnArmorRecover, this));
}

void XWoozyComponent::Clear()
{
	_OnBroken = false;
	_OnRecover = false;

	_transfer = false;

	if (_token_on != INVALID_HTIMER)
	{
		CTimerMgr::Instance()->KillTimer(_token_on);
	}
	if (_token_off != INVALID_HTIMER)
	{
		CTimerMgr::Instance()->KillTimer(_token_off);
	}
}

void XWoozyComponent::Update(float fDeltaT)
{
	if(_OnRecover) RecoverArmor();
	if(_OnBroken) BrokenArmor();
	if(_OnWoozy) Woozy();
}

bool XWoozyComponent::OnArmorBroken(IArgs* pargs, void *)
{
	_OnBroken = true;
	_transfer = true;

	return true;
}

bool XWoozyComponent::OnArmorRecover(IArgs* pargs, void *)
{
	_OnRecover = true;
	_transfer = true;

	return true;
}

bool XWoozyComponent::OnDead(IArgs* pargs, void *)
{
	Clear();
	return true;
}

void XWoozyComponent::BrokenArmor()
{
	if(_woozy_enabled)
	{
		GetEntity()->Idled();

		//skill
		XAttackActionArgs attck_arg;
		attck_arg.Identify = GetHost()->SkillComponent()->SkillManager()->GetBrokenIdentity();
		attck_arg.Target = NULL;
		GetHost()->SkillComponent()->SkillManager()->CoolDown(attck_arg.Identify);
		GetHost()->DispatchAction(&attck_arg);

		_OnBroken = false;

		float time = GetHost()->SkillComponent()->SkillManager()->GetSkill(GetHost()->SkillComponent()->SkillManager()->GetBrokenIdentity())->GetSoul()->Time;

		CTimerMgr::Instance()->KillTimer(_token_on);
		_token_on = XCommon::SetTimer(time, DelegateCombination(XWoozyComponent, WoozyOn, this), __FILE__, __LINE__);

		//clear cd for all players
		Scene* pScene = GetEntity()->GetUnit()->GetCurrScene();
		if(pScene)
		{
			const std::list<Role*>& roles = pScene->GetAllRoles();

			std::list<Role*>::const_iterator it = roles.begin();
			while(it != roles.end())
			{
				XRole* pRole = dynamic_cast<XRole*>((*it)->GetXObject());
				if(pRole) pRole->CoolDownAllSkills();

				++it;
			}
		}
	}
	else
	{
		_OnBroken = false;
		WoozyOn(0, 0);
	}
}

void XWoozyComponent::RecoverArmor()
{
	if(_woozy_enabled)
	{
		//skill
		XAttackActionArgs attck_arg;
		attck_arg.Identify = GetHost()->SkillComponent()->SkillManager()->GetRecoveryIdentity();
		const std::vector<Unit*>& hateUnit = GetHost()->AIComponent()->EnmityList().GetHateEntity(true);

		if (hateUnit.size() != 0)
			attck_arg.Target = hateUnit[0]->GetID();
		else
			attck_arg.Target = NULL;
		GetHost()->SkillComponent()->SkillManager()->CoolDown(attck_arg.Identify);
		if(GetHost()->DispatchAction(&attck_arg))
		{
			XSkillCore* pCore = GetHost()->SkillComponent()->SkillManager()->GetSkill(GetHost()->SkillComponent()->SkillManager()->GetRecoveryIdentity());

			float time = pCore->GetSoul()->Time;

			CTimerMgr::Instance()->KillTimer(_token_off);
			_token_off = XCommon::SetTimer(time, DelegateCombination(XWoozyComponent, WoozyOff, this), __FILE__, __LINE__);

			_OnRecover = false;
		}
	}
	else
	{
		_OnRecover = false;
		WoozyOff(NULL, NULL);
	}
}

void XWoozyComponent::Woozy()
{
	GetEntity()->Idled();

	_OnWoozy = false;
}

bool XWoozyComponent::WoozyOn(IArgs* pargs, void *)
{
	if(_woozy_enabled) _OnWoozy = true;

	_transfer = false;

	XWoozyOnArgs args;
	GetHost()->DispatchAction(&args);

	return true;
}

bool XWoozyComponent::WoozyOff(IArgs* pargs, void *)
{
	_transfer = false;

	XWoozyOffArgs args;
	GetHost()->DispatchAction(&args);

	return true;
}