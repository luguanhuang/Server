#include "pch.h"
#include "XRole.h"
#include "component/XQuickTimeEventComponent.h"
#include "unit/role.h"
#include "gamelogic/skilllevel.h"
#include "skill/XSkillCore.h"
#include "skill/XSkillMgr.h"
#include "component/XNavigationComponent.h"
#include "component/XAIComponent.h"
#include "scene/scene.h"
#include "scene/grid.h"
#include "component/XSkillComponent.h"
#include "table/XEntityPresentation.h"
#include "component/XPresentComponent.h"
#include "component/XManipulationComponent.h"
#include "component/XNetComponent.h"
#include "unit/enemy.h"
#include "unit/pet.h"

XRole::XRole(Unit* pUnit)
	:XEntity(pUnit)
{
	for(int i = 0; i < 7; i++)
	{
		_last_slot_attack_time[i] = 0;
	}

	_last_slot_attack_target = 0;
}

XRole::~XRole()
{
	_last_slot_attack_target = 0;
}

bool XRole::Initilize()
{
	XEntity::Initilize();

	bool InHall = GetUnit()->IsEnterHall();
	//role initialization goes here
	//...
	if(!InHall)
	{
		_pqte = static_cast<XQuickTimeEventComponent*>(AttachComponent(new XQuickTimeEventComponent()));
		_ai = static_cast<XAIComponent*>(AttachComponent(new XAIComponent()));

		AttachComponent(new XManipulationComponent());
	}

	return true;
}

void XRole::CoolDownAtLaunch()
{
	Role *pRole = dynamic_cast<Role*>(GetUnit());

	if(pRole && SkillComponent())
	{
		const std::vector<UINT32>& skills = pRole->GetRoleSkillLevelMgr()->GetBindSkillIDS();

		std::vector<UINT32>::const_iterator it = skills.begin();
		while(it != skills.end())
		{
			XSkillCore* pCore = SkillManager()->GetSkill(*it);
			if(pCore) pCore->MakeCoolDownAtLaunch();

			++it;
		}
	}
}

void XRole::CoolDownAllSkills()
{
	Role *pRole = dynamic_cast<Role*>(GetUnit());

	if(pRole && SkillComponent())
	{
		const std::vector<UINT32>& skills = pRole->GetRoleSkillLevelMgr()->GetBindSkillIDS();

		std::vector<UINT32>::const_iterator it = skills.begin();
		while(it != skills.end())
		{
			XSkillCore* pCore = SkillManager()->GetSkill(*it);
			if(pCore) pCore->CoolDown();

			++it;
		}
	}
}

void XRole::InstallNavComponent()
{
	if (!_pnavigation)
		_pnavigation = static_cast<XNavigationComponent*>(AttachComponent(new XNavigationComponent()));
}

void XRole::SetSlotLastAttackAt(int slot, UINT64 target)
{
	if(slot < 7 && slot >=0)
	{
		_last_slot_attack_time[slot] = TimeUtil::GetMilliSecond();
	}

	_last_slot_attack_target = target;
}

void XRole::ZeroSlotLastAttack(int slot)
{
	if(slot < 7 && slot >= 0)
	{
		_last_slot_attack_time[slot] = 0;
	}
}

void XRole::Update(float fDeltaT)
{
	Role* role = static_cast<Role*>(GetUnit());
	PetSys* pPetSys = role->Get<PetSys>();

	if(pPetSys)
	{
		if(pPetSys->IsCopilot())
		{
			if(StateMachine()->GetCurrentState() != XState_Idle)
				StateMachine()->ForceToDefaultState(false);

			XObject::Update(fDeltaT);
		}
		else
		{
			XEntity::Update(fDeltaT);

			XEntity* copilot = XEntity::ValidEntity(pPetSys->GetCopilotRoleId());
			if(copilot)
			{
				copilot->SetPosition(GetPosition_p());
				copilot->SetFace(GetFace_p());
			}
		}
	}
	else
		XEntity::Update(fDeltaT);
}

void XRole::LateUpdate(float fDeltaT)
{
	XEntity::LateUpdate(fDeltaT);

	Scene* pScene = GetUnit()->GetCurrScene();

	if(pScene != NULL && _pmachine->GetCurrentState() != XState_Move)
	{
		UINT32 type = pScene->GetSceneType();
		if (SceneConfig::Instance()->IsBattleSceneExcept(type))
			return;
		if(SceneConfig::Instance()->IsCollideExcept(type))
			return;

		const vector<Unit*>* enemies = pScene->GetOpponentUnitWithCollision(GetUnit()->GetFightGroup());
		if (NULL == enemies)
		{
			return;
		}

		vector<Unit*>::const_iterator it = enemies->begin();
		while (it != enemies->end())
		{
			const Vector3& pos = GetPosition_p();

			do 
			{
				if((*it)->IsRole() || (*it)->IsDummyRole()) break;
				if((*it)->IsEnemy())
				{
					Enemy* enemy = dynamic_cast<Enemy*>(*it);
					if(enemy->IsPuppet() && !XEntity::IsValidEntity(enemy)) break;
				}

				XEntity* oppo = dynamic_cast<XEntity*>((*it)->GetXObject());

				if(oppo)
				{
					if(oppo->SkillComponent() != NULL && oppo->SkillComponent()->IsCasting())
					{
						if(oppo->SkillComponent()->CurrentSkill()->MainCore()->GetSoul()->IgnoreCollision) break;
					}
					//Feature #45632
					if(oppo->GetPosition_p().y > pos.y + GetUnit()->GetBoundHeight()) break;

					const XEntityPresentation::RowData* present = oppo->PresentComponent() ? oppo->PresentComponent()->PresentLib() : 0;
					//calculating...
					if(present && present->Huge)
					{
						int i = 0;
						auto it = present->HugeMonsterColliders.begin();

						while (it != present->HugeMonsterColliders.end())
						{
							float radius = (*it)[2] * oppo->GetUnit()->GetScale();
							A(pScene, pos, oppo->HugeMonsterColliderCenter(i), GetUnit()->GetBoundRaidus() + radius);

							++it; ++i;
						}
					}
					else
						A(pScene, pos, oppo->RadiusCenter(), GetUnit()->GetBoundRaidus() + (*it)->GetBoundRaidus());
				}
			} while (false);

			++it;
		}
	}
}

void XRole::A(Scene* pScene, const Vector3& me, const Vector3& it, float r)
{
	Vector3 dis = me - it; dis.y = 0;

	float d = Vector3::sqrtMagnitude(dis);

	if(d - r * r < -0.001f)
	{
		const Vector3& dir = Vector3::Horizontaled(dis);
		const Vector3& position = it + dir * r;

		float y = position.y;

		if(pScene->GetGrid()->TryGetHeight(position.x, position.z, y) && y >= 0)
		{
			if(pScene->CheckDynamicBlock(GetPosition_p(), position))
			{
				SetPosition(position.x, _stand_on ? y : me.y, position.z);
				_pnet->Passive(true);
			}
		}
	}
}

bool XRole::OnTransform(UINT32 to)
{
	return XEntity::OnTransform(to);
}
