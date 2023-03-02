#include "pch.h"

#include "component/XComponent.h"
#include "component/XIdleComponent.h"
#include "component/XMoveComponent.h"
#include "component/XRotationComponent.h"
#include "component/XSkillComponent.h"
#include "component/XChargeComponent.h"
#include "component/XBuffComponent.h"
#include "component/XNetComponent.h"
#include "component/XBeHitComponent.h"
#include "component/XDeathComponent.h"
#include "component/XPresentComponent.h"
#include "component/XGravityComponent.h"
#include "component/XFreezeComponent.h"
#include "component/XAIComponent.h"

#include "scene/scene.h"
#include "XEntity.h"
#include "skill/XSkillMgr.h"
#include "unit/role.h"
#include "gamelogic/skilllevel.h"

#include "unit/unit.h"
#include "scene/grid.h"
#include "unit/role.h"
#include "table/ProfessionMgr.h"
#include "XObject.h"
#include "state/XStateDefine.hpp"
#include "tablemgr/XEntityInfoLibrary.h"

XEntity::XEntity(Unit* pUnit)
	:XObject(pUnit),
	 _last_state(XState_Idle),
	 _last_casted(0),
	 _movement(Vector3::zero),
	 _client_manual_forward(Vector3::zero),
	 _gravity_disabled(false),
	 _stand_on(false),
	 _state_transfered(false),
	 _state_casted(false),
	 _state_synced(false),
	 _sync_sequence(1),
	 _transformer(0),
	 _mobbed_by(0),
	 _life_within_mobbed_skill(false),
	 _in_pvp_scene(false),
	 _in_pvpone_scene(false)
{

}

XEntity::~XEntity()
{
	Puppet(false);

	XEntity* pent = XEntity::ValidEntity(_mobbed_by);
	if(pent && pent->SkillComponent())
	{
		pent->SkillComponent()->RemoveSkillMob(GetID());
	}
}

bool XEntity::IsValidEntity(const XEntity* pEntity)
{
	return pEntity && ValidEntity(pEntity->GetID());
}

bool XEntity::IsValidEntity(UINT64 id)
{
	return ValidEntity(id) != NULL;
}

bool XEntity::IsValidEntity(const Unit* pUnit)
{
	return ValidEntity(pUnit) != NULL;
}

XEntity* XEntity::ValidEntity(UINT64 id)
{
	if(id == 0) return NULL;

	Unit* pUnit = Unit::FindUnit(id);

	return ValidEntity(pUnit);
}

XEntity* XEntity::ValidEntity(const Unit* pUnit)
{
	if(pUnit)
	{
		return (!pUnit->GetDeprecated() && !pUnit->IsDead()) ? dynamic_cast<XEntity*>(pUnit->GetXObject()) : 0;
	}

	return 0;
}

bool XEntity::Initialization()
{
	bool InHall = GetUnit()->IsEnterHall();
	//state machine
	_ppresent = static_cast<XPresentComponent*>(AttachComponent(new XPresentComponent()));
	_pmachine = static_cast<XStateMachine*>(AttachComponent(new XStateMachine()));
	_pmachine->SetDefaultState(dynamic_cast<IXStateTransform*>(AttachComponent(new XIdleComponent())));
	
	//skill
	_pskill = new XSkillComponent;
	AttachComponent(_pskill);

	_pnet	= static_cast<XNetComponent*>(AttachComponent(new XNetComponent)); //net
	_protate = static_cast<XRotationComponent*>(AttachComponent(new XRotationComponent()));

	//others
	AttachComponent(new XMoveComponent());

	if(!InHall) AttachComponent(new XChargeComponent());
	if(!InHall) _phit = static_cast<XBeHitComponent*>(AttachComponent(new XBeHitComponent()));
	if(!InHall) AttachComponent(new XFreezeComponent());
	if(!InHall) AttachComponent(new XDeathComponent());

	//buff
	_pbuff	= new XBuffComponent;
	AttachComponent(_pbuff);

	return true;
}

bool XEntity::Initilize()
{
	return XObject::Initilize() && Initialization();
}

void XEntity::OnCreated()
{
	XObject::OnCreated();
	//added last
	AttachComponent(new XGravityComponent());
}

void XEntity::OnEnterScene(Scene *scene)
{
	XObject::OnEnterScene(scene);

	_sync_sequence = 1;
	_in_pvp_scene = Scene::IsPVPCombat(scene);
	_in_pvpone_scene = (NULL == scene) ? false : scene->IsPvpOne();

	if(_ppresent) _ppresent->ShowUp();

	if (_ai)
	{
		_ai->OnEnterScene(scene);
	}
}

void XEntity::OnSceneReady()
{
	XObject::OnSceneReady();

	CoolDownAtLaunch();
}

void XEntity::CoolDownAtLaunch()
{
	if(SkillComponent())
	{
		const std::vector<XSkillCore*>& skills = SkillComponent()->SkillManager()->GetSkillOrder();

		std::vector<XSkillCore*>::const_iterator it = skills.begin();
		while(it != skills.end())
		{
			(*it)->MakeCoolDownAtLaunch();
			++it;
		}
	}
}

void XEntity::OnLeaveScene()
{
	Idled();

	XObject::OnLeaveScene();
}

XStateDefine XEntity::GetCurrentState()
{
	if(_pskill && _pskill->IsCasting()) return XState_Skill;

	return _pmachine->GetCurrentState();
}

void XEntity::Dying()
{

}

void XEntity::Died()
{
	GetUnit()->SetDeprecated(true);
}

void XEntity::Update(float fDeltaT)
{
	_gravity_disabled = false;

	XObject::Update(fDeltaT);

	Move(fDeltaT);

	LateUpdate(fDeltaT);
}

void XEntity::LateUpdate(float fDeltaT)
{
	if(_pnet)
	{
		{
			_state_transfered = (_last_state != GetCurrentState());

			if(_pskill) 
			{
				bool cast = _pskill->IsCasting();

				_state_casted = cast ? _pskill->CurrentSkill()->MainCore()->ID() != _last_casted : false;
				_last_casted = cast ? _pskill->CurrentSkill()->MainCore()->ID() : 0;
			}

			_last_state = GetCurrentState();
		}

		_state_synced = false;

		_pnet->ResetSyncData();
	}
}

void XEntity::Move(float fDelatT)
{
	if(!(_movement.x == 0 && _movement.z == 0 && _movement.y <=0 && _stand_on))
	{
		Vector3 pos = GetPosition_p() + _movement;

		float y = 0;

		Scene* pScene = GetUnit()->GetCurrScene();
		if(pScene)
		{
			if(pScene->GetGrid()->TryGetHeight(pos.x, pos.z, y))
			{
				if(y < 0)
				{
					if(pScene->GetGrid()->TryGetHeight(GetPosition_p().x, GetPosition_p().z, y))
					{
						_stand_on = false;
						SetPosition(GetPosition_p().x, GetPosition_p().y + _movement.y, GetPosition_p().z);

						if(GetPosition_p().y <= y)
						{
							SetPosition(GetPosition_p().x, y, GetPosition_p().z);
							_stand_on = true;
						}
					}
				}
				else
				{
					if(GetUnit()->IsIgnoreDynamicCollide() || pScene->CheckDynamicBlock(GetPosition_p(), pos))
					{
						_stand_on = false;
						SetPosition(pos);

						if(GetPosition_p().y <= y)
						{
							SetPosition(GetPosition_p().x, y, GetPosition_p().z);
							_stand_on = true;
						}
					}
				}
			}
			pScene->UpdateViewGrid(GetUnit());
		}
	}
	/*
	 * Clear movement for next frame
	 */
	_movement.Zero();
}

void XEntity::ApplyMove(const Vector3& movement)
{
	_movement += movement;
}

void XEntity::ApplyMove(float x, float y, float z)
{
	_movement.x += x;
	_movement.y += y;
	_movement.z += z;
}

XSkillMgr* XEntity::SkillManager()
{
	return _pskill != NULL ? _pskill->SkillManager() : NULL;
}

void XEntity::Idled()
{
	if (_pskill) _pskill->EndSkill();
	if (_pmachine) _pmachine->ForceToDefaultState(false);
}

void XEntity::Puppet(bool bPuppet)
{
	if(bPuppet) Idled();

	if(_pskill) _pskill->Enable(!bPuppet);
	if(_pmachine) _pmachine->Enable(!bPuppet);
}

int XEntity::GetProfession()
{
	Role* role = dynamic_cast<Role*>(GetUnit());

	if(role)
	{
		int prof = role->GetProfession();
		return ProfessionMgr::GetBasicProfession(prof);
	}

	return 0;
}

Vector3 XEntity::RadiusCenter()
{
	if(GetUnit()->GetBoundRaidusOffset().IsZero())
	{
		return GetPosition_p();
	}
	else
	{
		float yRot = Vector3::Angle(Vector3::forward, GetFace_p());
		if(!Vector3::Clockwise(Vector3::forward, GetFace_p())) yRot = -yRot;

		return GetPosition_p() + Vector3::HorizontalRotate(GetUnit()->GetBoundRaidusOffset(), yRot, false) * GetUnit()->GetScale();
	}
}

Vector3 XEntity::HugeMonsterColliderCenter(int idx)
{
	const vector<Sequence<float, 4>>& c = PresentComponent()->PresentLib()->HugeMonsterColliders;
	Vector3 offset(c[idx][0], 0, c[idx][1]);

	float yRot = Vector3::Angle(Vector3::forward, GetFace_p());
	if(!Vector3::Clockwise(Vector3::forward, GetFace_p())) yRot = -yRot;

	return GetPosition_p() + Vector3::HorizontalRotate(offset, yRot, false) * GetUnit()->GetScale();
}

KKSG::XQTEState XEntity::GetQTESpecificPhase()
{
	switch(_pmachine->GetCurrentState())
	{
	case XState_BeHit: return _phit->GetQTESpecificPhase();
	case XState_Freeze: return KKSG::QTE_HitFreeze;
	default: return KKSG::QTE_None;
	}
}

bool XEntity::OnTransform(UINT32 to)
{
	Idled();

	if(PresentComponent()) 
	{
		if(!PresentComponent()->OnTransform(to))
			return false;
	}

	if(IsTransform())
	{
		_transformer = 0;
	}
	else
	{
		if(to == 0) return false;
	}

	UINT32 log = _transformer;

	if(to != 0)
	{
		_transformer = to;
		if (_pSoul->IsRole())
		{
			((Role*)_pSoul)->Get<RoleSkillSet>()->OnTransform(to);
		}
	}

	if(SkillComponent()) 
	{
		if(IsTransform())
		{
			const XEntityStatistics::RowData* data = XEntityInfoLibrary::Instance()->TemplateID2Data_Enemy(_transformer);

			if(data)
				_pskill->ReAttachSkill(data->PresentID);
			else
			{
				_transformer = log;
				return false;
			}
		}
		else
			_pskill->ReAttachSkill(GetUnit()->GetPresentID());
	}

	if(HitComponent())
	{
		HitComponent()->ReUpdatePresentStatus();
	}

	return true;
}