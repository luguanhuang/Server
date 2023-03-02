#include "pch.h"
#include "XActionReceiver.h"
#include "unit/role.h"
#include "entity/XActionDefine.h"
#include "unit/unit.h"
#include "entity/XObject.h"
#include "skill/XSkillCore.h"
#include "XSkillComponent.h"
#include "skill/XSkillMgr.h"
#include "gamelogic/skilllevel.h"
#include "entity/XRole.h"
#include "scene/scene.h"
#include "scene/grid.h"
#include "XNetComponent.h"
#include "unit/pet.h"

bool XActionReceiver::IsValid(Role* pRole)
{
	if (pRole->GetCurrScene() == NULL)
	{
		return false;
	}

	//validate check
	if(pRole->IsDead()) return false;

	PetSys* pPetSys = pRole->Get<PetSys>();
	if(pPetSys && pPetSys->IsCopilot()) return false;

	return true;
}

void XActionReceiver::OnMoveReceived(Role* pRole, float velocity, float face, float x, float z, bool interia)
{
	//validate check
	if(!IsValid(pRole)) return;

	const Vector3& forward = Vector3::FloatToAngle(face);

	XEntity* entity = dynamic_cast<XEntity*>(pRole->GetXObject());
	entity->SetClientManualForward(velocity == 0 ? Vector3::zero : forward);

	Vector3 des = pRole->GetXObject()->GetPosition_p();

	if(velocity > 0)
	{
		Vector3 src(x, 0, z);

		//float delay = pRole->GetDelay() * 0.5f;

		des = (entity->NetComponent() && entity->NetComponent()->IsPassive()) ? (src + (forward * (velocity * 0.1f))) : src;

		if(!pRole->GetCurrScene()->GetGrid()->TryGetHeight(des.x, des.z, des.y) || des.y < 0)
		{
			des = pRole->GetXObject()->GetPosition_p();
		}
	}

	//process
	Vector3 c2s = des - pRole->GetXObject()->GetPosition_p(); c2s.y = 0;
	if(Vector3::sqrtMagnitude(c2s) > 100.0f)
	{
		if(pRole->GetXObject()->NetComponent()->DuringCorrectProtected())
		{
			LogInfo("Role [%llu] is running out of meaningful to x: %.3lf z: %.3lf from x: %.3lf z: %.3lf during correct-protected", pRole->GetID(), des.x, des.z, pRole->GetXObject()->GetPosition_p().x, pRole->GetXObject()->GetPosition_p().z);
		}
		else
		{
			LogError("Role [%llu] is running out of meaningful to x: %.3lf z: %.3lf from x: %.3lf z: %.3lf in scene %d", pRole->GetID(), des.x, des.z, pRole->GetXObject()->GetPosition_p().x, pRole->GetXObject()->GetPosition_p().z, pRole->GetCurrScene()->GetSceneTemplateID());
			pRole->GetXObject()->CorrectLocation();
		}
	}
	else
	{
		XMoveActionArgs move_arg;

		move_arg.Inertia = interia;
		move_arg.Destination = des;
		move_arg.Speed = pRole->GetXObject()->GetUnit()->GetRunSpeed();
		move_arg.Stoppage = (velocity == 0);

		pRole->GetXObject()->DispatchAction(&move_arg);
	}
}

/*
 * 'face' in case client delay
 */
void XActionReceiver::OnMoveReceived(Role* pRole, float velocity, float face, float x, float z, float desx, float desz, bool interia)
{
	//validate check
	if(!IsValid(pRole)) return;

	XEntity* entity = dynamic_cast<XEntity*>(pRole->GetXObject());

	Vector3 des(desx, 0, desz);
	if(!pRole->GetCurrScene()->GetGrid()->TryGetHeight(des.x, des.z, des.y) || des.y < 0)
	{
		LogError("Role [%llu] report invalid des position x: %.3lf z: %.3lf", pRole->GetID(), des.x, des.z);
		return;
	}
	else
		des.y = entity->GetPosition_p().y;

	//process
	Vector3 src(x, 0, z);
	Vector3 c2s = src - pRole->GetXObject()->GetPosition_p(); c2s.y = 0;
	if(Vector3::sqrtMagnitude(c2s) > 100.0f)
	{
		if(pRole->GetXObject()->NetComponent()->DuringCorrectProtected())
		{
			LogInfo("Role [%llu] report position running out of meaningful x: %.3lf z: %.3lf server at x: %.3lf z: %.3lf during correct-protected", pRole->GetID(), src.x, src.z, pRole->GetXObject()->GetPosition_p().x, pRole->GetXObject()->GetPosition_p().z);
		}
		else
		{
			LogError("Role [%llu] report position running out of meaningful x: %.3lf z: %.3lf server at x: %.3lf z: %.3lf in scene %d", pRole->GetID(), src.x, src.z, pRole->GetXObject()->GetPosition_p().x, pRole->GetXObject()->GetPosition_p().z, pRole->GetCurrScene()->GetSceneTemplateID());
			pRole->GetXObject()->CorrectLocation();
		}
	}
	else
	{
		entity->SetClientManualForward(Vector3::zero);

		XMoveActionArgs move_arg;
		move_arg.Inertia = interia;
	
		if(velocity == 0)
		{
			move_arg.StopTowards = Vector3::FloatToAngle(face);
			float overlapped = Vector3::sqrtMagnitude(des - entity->GetPosition_p());

			if(overlapped > 0.15f)
				move_arg.Destination = des;
			else
				move_arg.Destination = entity->GetPosition_p();
		}
		else
			move_arg.Destination = des;

		move_arg.Speed = pRole->GetXObject()->GetUnit()->GetRunSpeed();
		move_arg.Stoppage = (velocity == 0);

		pRole->GetXObject()->DispatchAction(&move_arg);
	}
}

void XActionReceiver::OnSkillReceived(Role* pRole, UINT64 target, UINT32 skill, INT32 slot, bool hasmanualface, float manualface)
{
	//validate check
	if(!IsValid(pRole)) return;

	XRole* role = dynamic_cast<XRole*>(pRole->GetXObject());

	if(!role) return;

	XSkillCore* soul = role->SkillComponent()->SkillManager()->GetSkill(skill);

	if(soul == NULL) 
	{
		LogError("Role [%llu] is casting an invalid skill %u.", pRole->GetID(), skill);
		return;
	}

	//process
	XAttackActionArgs attck_arg;

	attck_arg.Slot = slot;
	attck_arg.Target = target;
	attck_arg.Identify = skill;
	attck_arg.HasManualFace = hasmanualface;
	attck_arg.ManualFace = manualface;

	pRole->GetXObject()->DispatchAction(&attck_arg);
}

void XActionReceiver::OnSkillSlotReceived(Role* pRole, UINT64 target, INT32 slot, bool hasmanualface, float manualface)
{
	//validate check
	if(!IsValid(pRole)) return;

	const std::vector<UINT32>& skills = pRole->GetRoleSkillLevelMgr()->GetBindSkillIDS();

	if(slot < 0 || (UINT32)slot >= skills.size()) 
	{
		LogError("Role [%llu] is casting a skill out of slot range.", pRole->GetID());
		return;
	}

	XRole* role = dynamic_cast<XRole*>(pRole->GetXObject());

	if(role)
	{
		if(!role->IsTransform() && skills[slot] == 0)
			LogError("Role [%llu] is casting a skill on empty slot %d.", pRole->GetID(), slot);
		else
			role->SetSlotLastAttackAt(slot, target);
	}
}