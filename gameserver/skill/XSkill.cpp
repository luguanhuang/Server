#include "pch.h"
#include "XSkill.h"
#include "XSkillCore.h"
#include "entity/XEntity.h"
#include "state/XStateMachine.h"
#include "xdelegate.hpp"
#include "XSkillMgr.h"
#include "component/XNetComponent.h"
#include "scene/scene.h"

#include "component/XSkillComponent.h"
#include "entity/XActionDefine.h"
#include "gamelogic/XCombat.h"
#include "unit/unit.h"
#include "XBullet.h"
#include "table/globalconfig.h"
#include "component/XRotationComponent.h"
#include "component/XBeHitComponent.h"
#include "XCombinedSkill.h"
#include "XBulletCore.h"
#include "unit/role.h"
#include "unit/combatstatistics.h"
#include "scene/securityskillstatistics.h"

const int XSkill::XJAComboSkillHash = 0;
const int XSkill::XArtsSkillHash = 1;
const int XSkill::XUltraSkillHash = 2;
const int XSkill::XCombinedSkillHash = 3;

PtcG2C_SkillResult XSkill::sPtc;

using namespace KKSG;

XSkill::XSkill(XEntity* firer)
	:_firer(firer),
	 _token(0),
	 _slot_pos(-1),
	 _core(NULL),
	 _data(NULL),
	 _target(0),
	 _time_scale(1.0f),
	 _fire_at(0),
	 _start_method(0),
	 _update_method(0),
	 _stop_method(0),
	 _result_method(0),
	 _casting(false),
	 _execute(false),
	 _skill_forward(Vector3::forward)
{

}

XSkill::~XSkill()
{
	_firer = 0;
	_target = 0;
}

bool XSkill::Update(float fDeltaT)
{
	bool res = _casting;

	if (_casting && _execute)
	{
		CalcForward();

		res = Present(fDeltaT);

		if(res && _update_method) _update_method(this, fDeltaT);
	}

	return res;
}

void XSkill::Execute()
{
	if(_casting && !_execute)
	{
		_execute = true;
		//for charge component get right direction
		CalcForward();

		_core->Execute(this);
		/*
		 * start a skill
		 */
		Start();

		if(_start_method) _start_method(this, 0);
	}
}

bool XSkill::OnResult(IArgs* args, void* param)
{
	bool result = true;

	if(_result_method) result = _result_method(this, 0);

	if(result)
	{
		if(_firer->GetUnit()->IsRole())
		{
			Role* role = dynamic_cast<Role*>(_firer->GetUnit());
			role->Statistics().AppendTime();
		}

		Result(reinterpret_cast<XResultData*>(param));
	}

	return true;
}

//fire the skill
bool XSkill::Fire(UINT64 target, XSkillCore* pCore , XAttackActionArgs* pargs) 
{
	if (!_casting || !_execute)
	{
		if(!pCore || !(pCore->Fire(this))) return false;

		_firer->SyncIncreament();
		_token = _firer->SyncSequence();
		//BUG #32773
		_time_scale = 1.0f / pargs->TimeScale;
		_target = XEntity::IsValidEntity(target) ? target : 0;

		if (pCore->GetSoul()->TypeToken == 3)
		{
			if(!Launch(pCore)) return false;
		}
		else
		{
			_core = pCore;
			_data = _core->GetSoul();
		}

		if(_firer->GetUnit()->IsRole())
		{
			Role* role = dynamic_cast<Role*>(_firer->GetUnit());
			role->Statistics().MarkTimeBaseLine();
		}
		
		_slot_pos = pargs->Slot;

		_actionToken.clear();

		_firer->StateMachine()->ForceToDefaultState(false);

		_casting = true;
		_execute = false;

		FocusTarget(Target());
		if(pargs->HasManualFace) Manual(Vector3::FloatToAngle(pargs->ManualFace));

		if(Casting())
		{
			XSecuritySkillStatistics* statistics = XSecuritySkillStatistics::TryGetStatistics(_firer->GetUnit());
			if (statistics != NULL) statistics->OnCast(MainCore()->ID());
		}

		_fire_at = TimeUtil::GetMilliSecond();

		HurtInfo phInfo;
		phInfo.m_caster = _firer;
		phInfo.m_target = Casting() ? Target() : NULL;
		phInfo.m_skillId = MainCore()->ID();
		phInfo.m_skilltoken = _token;
		phInfo.m_callback = DelegateCombination(XSkill, ExternalCallback, this);
		XCombat::ProjectStart(phInfo);

		//BUG #37955
		return Casting();
	}
	else
		return false;
}

bool XSkill::ExternalCallback(IArgs* args, void* param)
{
	UINT64 token = reinterpret_cast<UINT64>(param);
	if(token != _token || !Casting()) return false;

	XSkillExternalArgs* pargs = dynamic_cast<XSkillExternalArgs*>(args);
	if(!pargs) return false;

	XCombinedSkill* combined = dynamic_cast<XCombinedSkill*>(this);

	if(combined)
	{
		combined->AddedCombinedTimerToken(
			XCommon::SetTimer(
			pargs->delay,
			DelegateCombination(XSkill, ExternalExecute, this),
			args, __FILE__, __LINE__));
	}
	else
	{
		AddedTimerToken(
			XCommon::SetTimer(
			pargs->delay,
			DelegateCombination(XSkill, ExternalExecute, this),
			args, __FILE__, __LINE__));
	}

	return true;
}

bool XSkill::ExternalExecute(IArgs* args, void*)
{
	XSkillExternalArgs* pargs = reinterpret_cast<XSkillExternalArgs*>(args);
	pargs->callback(pargs);

	return true;
}

float XSkill::TimeElapsed() const
{
	UINT64 delta = TimeUtil::GetMilliSecond() - _fire_at;
	return _casting ? (delta / 1000.0f) / _time_scale : 0;
}

void XSkill::Puppetize()
{
	if (_execute)
	{
		_execute = false;

		//kill timer
		KillTimerAll();
		//derived class stop
		Stop();

		if (_update_method) _update_method(this, 0);
		if (_stop_method && MainCore()->GetSoul()->TypeToken != 3) _stop_method(this, 0);

		_core->Halt();

		_target = 0;
		//LogInfo("P: %.3f %.3f %.3f\n", _firer->GetPosition_p().x, _firer->GetPosition_p().y, _firer->GetPosition_p().z);
	}
}

//stop casting
void XSkill::Cease()
{
	if (_casting)
	{
		_casting = false;

		Puppetize();

		XCombinedSkill* pc = dynamic_cast<XCombinedSkill*>(this);
		if (pc)
		{
			pc->CombinedKillTimerAll();
			pc->CombinedStop();
		}

		if(MainCore()->GetSoul()->Result.size() > 0) 
		{
			if(_firer->GetUnit()->IsRole())
			{
				Role* role = dynamic_cast<Role*>(_firer->GetUnit());
				role->Statistics().MarkTimEndLine();
			}
		}

		MainCore()->Halt();

		//stop skill action
		if (_firer->StateMachine()->ActionToken() == _token)
		{
			_firer->StateMachine()->ForceToDefaultState(false);
		}

		_fire_at = 0;
		_time_scale = 1.0f;

		//BUG #37955
		HurtInfo phInfo;
		phInfo.m_caster = _firer;
		phInfo.m_target = Target();
		phInfo.m_skillId = MainCore()->ID();
		phInfo.m_skilltoken = _token;
		XCombat::ProjectEnd(phInfo);

		//LogInfo("P: %.3f %.3f %.3f\n", _firer->GetPosition_p().x, _firer->GetPosition_p().y, _firer->GetPosition_p().z);
	}

	if(_target != 0) _target = 0;

	_core = NULL;

	_start_method = NULL;
	_update_method = NULL;
	_stop_method = NULL;
	_result_method = NULL;
}

// when skill is casting, can this action be performed?
bool XSkill::CanPerformAction(XStateDefine state, UINT64 token)
{
	if (!_casting) return true;

	if (token > 0 && _token == token)
		return true;
	else if (XState_Death == state)
	{
		Cease();
		return true;
	}
	else
	{
		Vector3 face = _firer->ClientManualForward();
		return (MainCore() == NULL) ? true : MainCore()->CanAct(state, face);
	}
}

void XSkill::FocusTarget(XEntity* target)
{
	/*
	 * only two function to call FocusTarget
	 * target MUST be either 'null' or valid.
	 */
	if(!target) return;

	XRotationActionArgs rargs;
	rargs.SetToken(_token);
	rargs.TargetDir = Vector3::Horizontaled(target->RadiusCenter() - _firer->GetPosition_p());
	rargs.Palstance = _firer->GetUnit()->GetAutoRotateSpeed();
	_firer->DispatchAction(&rargs);
}

void XSkill::KillTimerAll()
{
	vector<HTIMER>::const_iterator it = _actionToken.begin();
	while (it != _actionToken.end())
	{
		CTimerMgr::Instance()->KillTimer(*it);
		++it;
	}

	_actionToken.clear();
}

XEntity* XSkill::Target() const 
{
	return HasValidTarget() ? XEntity::ValidEntity(_target) : NULL;
}

bool XSkill::HasValidTarget() const 
{ 
	XSkillCore* core = MainCore();
	return core != NULL && core->GetSoul()->NeedTarget && XEntity::ValidEntity(_target); 
}

void XSkill::CalcForward()
{
	if (!MainCore()->GetSoul()->MultipleAttackSupported)
		_skill_forward = _firer->RotateComponent() == NULL ? _firer->GetFace() : _firer->RotateComponent()->GetMeaningfulFaceVector3();
}

void XSkill::Reflection()
{
	_start_method = NULL;
	_update_method = NULL;
	_stop_method = NULL;
	_result_method = NULL;

	if (_data)
	{
		if(_data->Script.Start_Name.length() > 0) _start_method = XSkillScriptMgr::GetScriptFunc(_data->Name, _data->Script.Start_Name);
		if(_data->Script.Update_Name.length() > 0) _update_method = XSkillScriptMgr::GetScriptFunc(_data->Name, _data->Script.Update_Name);
		if(_data->Script.Stop_Name.length() > 0) _stop_method = XSkillScriptMgr::GetScriptFunc(_data->Name, _data->Script.Stop_Name);
		if(_data->Script.Result_Name.length() > 0) _result_method = XSkillScriptMgr::GetScriptFunc(_data->Name, _data->Script.Result_Name);
	}
}

void XSkill::Manual(const Vector3& dir)
{
	if (dir.IsZero() || MainCore()->GetSoul()->MultipleAttackSupported) return;

	XRotationActionArgs rargs;
	rargs.TargetDir = dir;
	rargs.Palstance = _firer->GetUnit()->GetAutoRotateSpeed();
	_firer->DispatchAction(&rargs);
}

void XSkill::SkillResult(
	UINT64 token,
	XEntity* firer, 
	XSkillCore* core, 
	XBullet* bullet,
	int triggerTime, 
	UINT32 resultID, 
	int resultTime, 
	const Vector3& hitdir, 
	XEntity* target, 
	PtcG2C_SkillResult* oPtc)
{
	if (bullet == NULL ? core->IsHurtEntity(target->GetID(), triggerTime) : (/*!bullet->IsExpired() && */bullet->IsHurtEntity(target->GetID()))) return;

	XStrickenResponse rps = target->SkillComponent()->IsCasting() ? target->SkillComponent()->CurrentSkill()->MainCore()->GetSoul()->Logical.StrickenMask : XSR_Cease;
	if (rps == XSR_Cease || core->CarrierID() == XSkill::XUltraSkillHash ||
		(rps == XSR_Half_Endure && target->SkillComponent()->IsOverResults()))
		rps = XSR_Cease;

	if (rps == XSR_Invincible) return;

	//should be maincore?
	if (!core->GetSoul()->Logical.AttackOnHitDown && target->StateMachine()->GetCurrentState() == XState_BeHit && target->HitComponent()->LaidOnGround()) return;

	HurtInfo phInfo;
	phInfo.m_caster = firer;
	phInfo.m_target = target;
	phInfo.m_skillId = resultID;
	phInfo.m_hitpoint = resultTime;
	phInfo.m_skilltoken = token;

	const ProjectDamageResult& dResult = XCombat::ProjectDamage(phInfo);
	//0 ~ 2
	//float paralyze = (float)(1 + (firer->GetUnit()->GetParalyzeAttribute() - target->GetUnit()->GetParalyzeDefense()));

	/*LogInfo("Skill : %s TriggerTime : %d\n", core->GetSoul()->Name.c_str(), triggerTime);*/
	//take effect
	SkillResult_TakeEffect(
		firer,
		target, 
		dResult, 
		core,
		triggerTime,
		hitdir,
		rps,
		target->StandOn(),
		1);

	TargetHurtInfo* pInfo = NULL;

	if(oPtc == NULL)
	{
		sPtc.m_Data.Clear();

		sPtc.m_Data.set_skillid(core->ID());
		sPtc.m_Data.set_pindex(triggerTime);
		sPtc.m_Data.set_firerid(firer->GetID());
		if(bullet) 
		{
			sPtc.m_Data.set_bulletid(bullet->ID());
			if(bullet->ExtraID() > 0) sPtc.m_Data.set_bulletextraid(bullet->ExtraID());
		}
		//build hurt info
		TargetHurtInfo* pInfo = sPtc.m_Data.add_targetlist();
		pInfo->set_unitid(target->GetID());

		DamageResult* res = pInfo->mutable_result();
		res->set_result(dResult.m_Result);
		res->set_value(dResult.GetValue());
		res->set_flag(dResult.m_flag);
		res->set_damagetype(dResult.m_type);
		res->set_elementtype(dResult.m_elementType);
		res->set_istargetdead(dResult.m_IsTargetDead);
		res->set_combocount(dResult.m_ComboCount);

		firer->NetComponent()->BroadcastSkillResult(sPtc);
	}
	else
	{
		//build hurt info
		TargetHurtInfo* pInfo = oPtc->m_Data.add_targetlist();
		pInfo->set_unitid(target->GetID());
		
		DamageResult* res = pInfo->mutable_result();
		res->set_result(dResult.m_Result);
		res->set_value(dResult.GetValue());
		res->set_flag(dResult.m_flag);
		res->set_damagetype(dResult.m_type);
		res->set_elementtype(dResult.m_elementType);
		res->set_istargetdead(dResult.m_IsTargetDead);
		res->set_combocount(dResult.m_ComboCount);
	}

	if(bullet)
	{
		bullet->OnResult(target->GetID());
	}
	else
		core->AddHurtTarget(target->GetID(), triggerTime);
}

void XSkill::SkillResult(
	XBullet* bullet, 
	const Vector3& forward, 
	const Vector3& position,
	bool bulletcycle)
{
	//only long range result goes here (actually only trigger end)

	XBulletCore* bulletcore = bullet->GetBulletCore();
	XSkillCore* skillcore = bulletcore->GetSkillCore();
	XEntity* firer = bulletcore->GetFirer();
	Scene* pScene = firer->GetUnit()->GetCurrScene();

	if(pScene != NULL)
	{
		const vector<Unit*>* enemies = pScene->GetOpponents(firer->GetUnit()->GetFightGroup());
		if (NULL == enemies)
		{
			return;
		}

		sPtc.m_Data.Clear();
		sPtc.m_Data.set_skillid(skillcore->ID());
		sPtc.m_Data.set_pindex(bulletcore->GetSequnce());
		sPtc.m_Data.set_firerid(firer->GetID());

		sPtc.m_Data.set_bulletid(bullet->ID());
		if(bullet->ExtraID() > 0) sPtc.m_Data.set_bulletextraid(bullet->ExtraID());

		vector<Unit*>::const_iterator it = (*enemies).begin();
		while (it != (*enemies).end())
		{
			XEntity* pEntity = dynamic_cast<XEntity*>((*it)->GetXObject());
			if(NULL == pEntity || firer == pEntity || 
				(skillcore->GetSoul()->Result[bulletcore->GetSequnce()].Attack_Only_Target && bulletcore->GetTarget() != pEntity)) 
			{
				++it;
				continue;
			}

			if(skillcore->IsInAttackField(bulletcore->GetSequnce(), position, forward, pEntity))
			{
				Vector3 dir = (skillcore->GetSoul()->Result[bulletcore->GetSequnce()].Affect_Direction == AttackDir) ?
					pEntity->GetPosition_p() - position : (firer->RotateComponent() ? firer->RotateComponent()->GetMeaningfulFaceVector3() : firer->GetFace_p());

				dir.y = 0; dir.Normalize();

				XSkill::SkillResult(
					bulletcore->Token(),
					firer,
					skillcore,
					bulletcycle ? bullet : NULL,
					bulletcore->GetSequnce(),
					bulletcore->ResultID(),
					bulletcore->ResultTime(),
					dir,
					pEntity,
					&sPtc);
			}

			++it;
		}

		if(sPtc.m_Data.targetlist_size() > 0) firer->NetComponent()->BroadcastSkillResult(sPtc);
	}
}

void XSkill::SkillResult(
	UINT64 token,
	XEntity* firer, 
	XSkillCore* core, 
	int triggerTime, 
	UINT32 resultID, 
	int resultTime, 
	const Vector3& forward, 
	const Vector3& position)
{
	Scene* pScene = firer->GetUnit()->GetCurrScene();

	if(pScene != NULL)
	{
		const vector<Unit*>* enemies = pScene->GetOpponents(firer->GetUnit()->GetFightGroup());
		if (NULL == enemies)
		{
			return;
		}

		sPtc.m_Data.Clear();
		sPtc.m_Data.set_skillid(core->ID());
		sPtc.m_Data.set_pindex(triggerTime);
		sPtc.m_Data.set_firerid(firer->GetID());

		vector<Unit*>::const_iterator it = (*enemies).begin();
		while (it != (*enemies).end())
		{
			XEntity* pEntity = dynamic_cast<XEntity*>((*it)->GetXObject());
			if(NULL == pEntity || firer == pEntity || 
				(core->GetSoul()->Result[triggerTime].Attack_Only_Target && core->Carrier()->Target() != pEntity)) 
			{
				++it;
				continue;
			}

			if(core->IsInAttackField(triggerTime, position, forward, pEntity))
			{
				Vector3 dir = (core->GetSoul()->Result[triggerTime].Affect_Direction == AttackDir) ?
					pEntity->GetPosition_p() - position : (firer->RotateComponent() ? firer->RotateComponent()->GetMeaningfulFaceVector3() : firer->GetFace_p());

				dir.y = 0; dir.Normalize();

				XSkill::SkillResult(
					token,
					firer,
					core,
					NULL,
					triggerTime,
					resultID,
					resultTime,
					dir,
					pEntity,
					&sPtc);
			}

			++it;
		}

		if(sPtc.m_Data.targetlist_size() > 0) firer->NetComponent()->BroadcastSkillResult(sPtc);
	}
}

void XSkill::SkillResult_TakeEffect(XEntity* firer, XEntity* target, const ProjectDamageResult& dResult, const XSkillCore* pCore, UINT32 triggerTime, const Vector3& hurtDirection, XStrickenResponse rps, bool standOn, float paralyze)
{
	if (dResult.m_Accept)
	{
		if(dResult.m_IsTargetDead) return;

		const XHitData& data = pCore->GetSoul()->Hit[triggerTime];

		if (rps == XSR_Half_Endure || rps == XSR_Full_Endure)
		{
			
		}
		else
		{
			switch(dResult.m_Result)
			{
			case PJRES_IMMORTAL:break;
			case PJRES_BATI:break;
			case PJRES_STUN:
				{
					XFreezeActionArgs args;
					args.FreezeFromHit = true;
					args.HitFrom = firer->GetID();
					args.ResultIdx = triggerTime;
					args.SkillFrom = pCore->ID();
					args.Duration =  GetGlobalConfig().StunTime;
					target->DispatchAction(&args);

				}break;
			case PJRES_BEHIT:
				{
					if (rps == XSR_Cease) target->SkillComponent()->EndSkill();

					if(data.State != Hit_Free)
					{
						if(data.State == Hit_Freezed)
						{
							XFreezeActionArgs args;
							args.FreezeFromHit = true;
							args.HitFrom = firer->GetID();
							args.ResultIdx = triggerTime;
							args.SkillFrom = pCore->ID();
							args.Duration = data.FreezeDuration;
							target->DispatchAction(&args);
						}
						else
						{
							XBeHitActionArgs args(&data);
							args.HitFrom = firer->GetID();
							args.HitDirection = hurtDirection;
							args.Paralyze = paralyze;
							args.HitFrom = firer->GetID();
							args.ResultIdx = triggerTime;
							args.SkillFrom = pCore->ID();
							args.ForceToFlyHit = (data.State == Hit_Back || data.State == Hit_Roll) && !standOn;
							target->DispatchAction(&args);
						}
					}
				}break;
			}
		}
	}
}
