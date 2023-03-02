#include "pch.h"
#include "XSkillCore.h"
#include "xdelegate.hpp"
#include "state/XStateMachine.h"
#include "entity/XEntity.h"
#include "component/XBeHitComponent.h"
#include "component/XNetComponent.h"
#include "XSkill.h"
#include "scene/scene.h"
#include "skill/XSkillMgr.h"
#include "gamelogic/skilllevel.h"
#include "unit/unit.h"
#include "table/XSkillEffectMgr.h"
#include "unit/enemy.h"
#include "table/globalconfig.h"
#include "entity/XObject.h"
#include "component/XRotationComponent.h"
#include "component/XPresentComponent.h"
#include "component/XBuffComponent.h"
#include "utility/UtilMath.h"
#include <cmath>
#include "XJAComboSkill.h"
#include "scene/scene.h"
#include "util/XRandom.h"

using namespace std;

XSkillCore::XSkillCore(XEntity* firer, XSkillData* data)
	:_magic_num(0xFEDCBA98),
	 _id(0), 
	 _carrier_id(data->TypeToken),
	 _current_running_time(1),
	 _totally_running_time(1),
	 _preserved_strength(0),
	 _soul(data), 
	 _carrier(NULL), 
	 _firer(firer),
	 _is_init_cooling(false),
	 _ever_fired(false),
	 _is_on_syntonic(false),
	 _static_cd(1),
	 _init_cd(1),
	 _skill_level(0),
	 _last_lock_and_load_time(0),
	 _timer_token(0),
	 _offset(Vector3::zero),
	 _semi_dynamic_cd_ratio(0),
	 _last_dynamic_cd(0),
	 _dynamic_cd_ratio(1),
	 _dynamic_cd_delta(0),
	 _sync_at_sequence(0)
{
	_id = XCommon::XHash(_soul->Name.c_str());

	for (size_t i = 0; i < _soul->Result.size(); i++)
	{
		set<UINT64> sets;
		_hurt_target.push_back(sets);
	}

	for(size_t i = 0; i < _soul->Warning.size(); i++)
	{
		std::vector<XSkillWarningPackage> pos;
		WarningPosAt.push_back(pos);

		std::vector<XWarningRandomPackage> item;
		WarningRandomItems.push_back(item);
	}

	InitCoreData(true);
}

XSkillCore::~XSkillCore()
{
	if(_soul != NULL)
	{
		_soul->Release();
		_soul = NULL;
	}

	_firer = NULL;

	_hurt_target.clear();

	WarningPosAt.clear();
	WarningRandomItems.clear();

	CTimerMgr::Instance()->KillTimer(_timer_token);

	_is_init_cooling = false;
	_current_running_time = 0;

	_magic_num = 0;
}

//feature #46583 to public
void XSkillCore::InitCoreData(bool ctor)
{
	_skill_level = _firer->GetUnit()->GetSkillLevelMgr()->GetSkillLevel(ID());

	Scene* current = _firer->GetUnit()->GetCurrScene();
	bool IsPVP = current ? Scene::IsPVPCombat(current) : _firer->GetUnit()->IsEnterPVP();

	_init_cd = (float)(XSkillEffectMgr::Instance()->GetSkillInitCDRatio(
		ID(), 
		_skill_level, 
		_firer->GetUnit()->GetSkillCasterTypeID(), 
		IsPVP, 
		_firer->GetUnit()) * _soul->CoolDown);

	_static_cd = (float)(XSkillEffectMgr::Instance()->GetSkillCDStaticRatio(
		ID(), 
		_skill_level, 
		_firer->GetUnit()->GetSkillCasterTypeID(), 
		IsPVP) * _soul->CoolDown);

	_semi_dynamic_cd_ratio = XSkillEffectMgr::Instance()->GetSkillCDSemiDynamicRatio(_firer->GetUnit(), ID());
	_preserved_strength = XSkillEffectMgr::Instance()->GetStrengthValue(ID(), _skill_level, _firer->GetUnit()->GetSkillCasterTypeID());
	_totally_running_time = XSkillEffectMgr::Instance()->GetUsageCount(ID(), _skill_level, _firer->GetUnit()->GetSkillCasterTypeID());

	if(_totally_running_time == 0) _totally_running_time = 1;
	if(ctor) _current_running_time = _totally_running_time;
	//feature #48389
	if(ctor) _ever_fired = false;
	//feature #46583
	if (Reloading()) GetCoolDown();
}

bool XSkillCore::Fire(XSkill* carrier)
{
	if(_soul->OnceOnly && _ever_fired) return false;

	if(carrier != NULL && CooledDown())
	{
		_carrier = carrier;
		
		OnCdCall(_current_running_time - 1);

		return true;
	}
	else
		return false;
}

void XSkillCore::OnCdCall(int left_running_time, bool syntonic)
{
	_ever_fired = true;

	bool loaded_firer = !Reloading();

	_current_running_time = left_running_time;
	CheckRunningTime();

	if(loaded_firer || syntonic)
	{
		_last_lock_and_load_time = MillionSecToSeconds(TimeUtil::GetMilliSecond());

		float cd = GetCoolDown();

		CTimerMgr::Instance()->KillTimer(_timer_token);
		_timer_token = XCommon::SetTimer(cd, DelegateCombination(XSkillCore, OnReloaded, this), __FILE__, __LINE__);

		if(_soul->Logical.Syntonic_CoolDown_Skill.length() > 0)
		{
			_is_on_syntonic = true;

			XSkillCore* core = _firer->SkillManager()->GetSkill(XCommon::XHash(_soul->Logical.Syntonic_CoolDown_Skill.c_str()));

			if(core && core != this && !core->IsOnSyntonic())
			{
				core->OnCdCall(0, true);
			}

			_is_on_syntonic = false;
		}

		if(syntonic) _firer->NetComponent()->BroadcastCDCall(_id, _current_running_time, true);
	}

	_sync_at_sequence = _firer->SyncSequence();
}

void XSkillCore::Execute(XSkill* carrier)
{
	//BUG #33011
	_carrier = carrier;

	ClearHurtTarget();
	ClearWarningPos();

	if(TryCalcRandomWarningPos())
	{
		if(_firer->NetComponent()) _firer->NetComponent()->BroadcastRandomWarningPos(ID(), WarningRandomItems);
	}
}

bool XSkillCore::TryCalcRandomWarningPos()
{
	bool res = false;

	for(size_t i = 0; i < _soul->Warning.size(); i++)
	{
		const XWarningData& data = _soul->Warning[i];

		auto& item = WarningRandomItems[i];
		item.clear();

		if(data.RandomWarningPos) //random pos
		{
			res = true;

			switch(data.Type)
			{
			case Warning_All:
			case Warning_Multiple:
				{
					const vector<Unit*>* enemies = _firer->GetUnit()->GetCurrScene()->GetOpponents(_firer->GetUnit()->GetFightGroup());
					if (NULL == enemies) return false;

					int n = (data.Type == Warning_All) ? ((*enemies).size()) : data.MaxRandomTarget;

					for(size_t i = 0; i < (*enemies).size(); i++)
					{
						XEntity* obj = dynamic_cast<XEntity*>((*enemies)[i]->GetXObject());

						if(obj)
						{
							if(obj->MobbedBy() != 0) continue;

							bool counted = (data.Type == Warning_All) ? true : XCommon::IsPickedInRange(n, (*enemies).size() - i);

							if(counted)
							{
								n--;

								XWarningRandomPackage wrp(obj->GetID());
								item.push_back(wrp);
							}
						}
					}
				}break;
			case Warning_Target:
				{
					XWarningRandomPackage wrp(0);
					item.push_back(wrp);
				}break;
			case Warning_None:
				break;
			}
			//calculate random pos
			auto it = item.begin();
			while(it != item.end())
			{
				for(int i = 0; i < data.PosRandomCount; i++)
				{
					UINT32 d = XRandom::randInt(0, 360);
					UINT32 r = (UINT32)(XRandom::randDouble(0, data.PosRandomRange) * 10);
					
					UINT32 m = (d << 16) | r;
					(*it).Pos.push_back(m);
				}

				++it;
			}
		}
		else if(data.Type == Warning_Multiple)
		{
			res = true;

			const vector<Unit*>* enemies = _firer->GetUnit()->GetCurrScene()->GetOpponents(_firer->GetUnit()->GetFightGroup());
			if (NULL == enemies) return false;

			int n = data.MaxRandomTarget;

			for(size_t i = 0; i < (*enemies).size(); i++)
			{
				XEntity* obj = dynamic_cast<XEntity*>((*enemies)[i]->GetXObject());

				if(obj)
				{
					if(obj->MobbedBy() != 0) continue;

					if(XCommon::IsPickedInRange(n, (*enemies).size() - i))
					{
						n--;

						XWarningRandomPackage wrp(obj->GetID());
						wrp.Pos.push_back(0);
						item.push_back(wrp);
					}
				}
			}
		}
	}

	return res;
}

void XSkillCore::ClearHurtTarget()
{
	std::vector<set<UINT64> >::iterator it = _hurt_target.begin();
	while (it != _hurt_target.end())
	{
		(*it).clear();
		++it;
	}
}

void XSkillCore::ClearWarningPos()
{
	std::vector<std::vector<XSkillWarningPackage> >::iterator it = WarningPosAt.begin();
	while (it != WarningPosAt.end())
	{
		(*it).clear();
		++it;
	}
}

void XSkillCore::AddHurtTarget(UINT64 id, int triggerTime)
{
	if(!_soul->Result[triggerTime].Loop /*for multiple trigger end*/ && !_soul->Result[triggerTime].LongAttackEffect) _hurt_target[triggerTime].insert(id);
}

bool XSkillCore::IsHurtEntity(UINT64 id, int triggerTime)
{
	return _hurt_target[triggerTime].find(id) != _hurt_target[triggerTime].end();
}

float XSkillCore::GetElapsedCD()
{
	return Reloading() ? MillionSecToSeconds(TimeUtil::GetMilliSecond()) - _last_lock_and_load_time : GetCoolDown();
}

float XSkillCore::GetLeftCD()
{
	return _soul->OnceOnly ? (_ever_fired ? -1 : 0) : (GetCoolDown() - GetElapsedCD());
}

float XSkillCore::GetTimeScale() const
{
	return (float)(XSkillEffectMgr::Instance()->GetAttackSpeedRatio(_firer->GetUnit()->GetAttrPtr()));
}

bool XSkillCore::CanCast(UINT64 token)
{
	//if(fromclient) return true;

	if(token > 0 && token == _firer->StateMachine()->ActionToken())
	{
		return true;
	}

	XStateDefine current = _firer->StateMachine()->GetCurrent()->IsFinished() ? _firer->StateMachine()->GetDefault()->SelfState() : _firer->StateMachine()->GetCurrentState();

	return current == XState_Idle || current == XState_Move || current == XState_Charge;
}

bool XSkillCore::CanRotate()
{
	return  _carrier == NULL ||
		(_carrier->TimeElapsed() < _soul->Logical.Rotate_End &&
		_carrier->TimeElapsed() >= _soul->Logical.Rotate_At);
}

bool XSkillCore::CanAct(XStateDefine state, const Vector3& forward)
{
	float now = _carrier->TimeElapsed();

	bool can = true;

	switch(state)
	{
	case XState_Idle: can = false; break;
	case XState_Move:
		{
			if( now < _soul->Logical.Not_Move_End && 
				now >= _soul->Logical.Not_Move_At )
			{
				can = false;

				if(_soul->MultipleAttackSupported)
				{
					if(!forward.IsZero()) _carrier->SetSkillTowardsTo(forward);
				}
				else
				{
					XRotationActionArgs rargs;
					rargs.TargetDir = forward.IsZero() ? _firer->GetFace_p() : forward;
					rargs.ClearSkillTarget = true;
					rargs.Palstance = _soul->Logical.Rotate_Speed > 0 ? _soul->Logical.Rotate_Speed : _firer->GetUnit()->GetRotateSpeed();
					_firer->DispatchAction(&rargs);
				}
			}
		}break;
	default:
		break;
	}

	if(can) 
	{
		_carrier->Cease();
	}

	return can;
}

bool XSkillCore::CanReplacedBy(const XSkillCore* skill)
{
	//if(fromclient) return true;
	/*
	 * Main skill can always be act
	 */
	bool cancel = (_soul->Logical.CanReplacedby & (1 << skill->GetSoul()->TypeToken)) != 0;

	if (!cancel)
	{
		cancel = _carrier->TimeElapsed() > _soul->Logical.CanCancelAt;

		if (!cancel && _soul->TypeToken == 0)
		{
			//ja
			if(_soul->Ja.size() > 0)
			{
				XJAComboSkill* ja = dynamic_cast<XJAComboSkill*>(_carrier);

				if(ja && ja->DuringJA())
				{
					UINT32 id = XCommon::XHash(_soul->Ja[0].Name.c_str());
					cancel = (skill->ID() == id);

					if(!cancel)
					{
						//Adapter
						XSkillCore* core = _firer->SkillManager()->GetSkill(XCommon::XHash(_soul->Ja[0].Name.c_str()));
						if(core && core->GetSoul()->Logical.Association)
						{
							id = XCommon::XHash(core->GetSoul()->Logical.Association_Skill.c_str());
							cancel = (skill->ID() == id);
						}
					}
				}
			}
		}
	}

	return cancel;
}

Vector3 XSkillCore::ResultPos(const Vector3& result_pos, const Vector3& logical, float radius)
{
	Vector3 raw_dir = logical - result_pos; raw_dir.y = 0;
	float raw_distance = Vector3::Magnitude(raw_dir); raw_dir.Normalize();
	raw_distance = raw_distance > radius ? raw_distance - radius : 0;

	return result_pos + raw_dir * raw_distance;
}

bool XSkillCore::InnerIsInAttackField(int triggerTime, const Vector3& pos, const Vector3& forward, const Vector3& targetPos, float yRot)
{
	Vector3 dir = targetPos - pos; dir.y = 0;
	float distance = Vector3::Magnitude(dir);

	float d = GetRange(triggerTime);
	float w = GetScope(triggerTime);

	if(_soul->Result[triggerTime].Sector_Type)
	{
		float angle = (distance == 0) ? 0 : Vector3::Angle(forward, dir);
		return (distance < d && distance >= GetRangeLow(triggerTime) && angle <= w);
	}
	else
	{
		//#36415 rect with group effect
		float fowardRot = Vector3::Angle(Vector3::forward, forward);
		if(!Vector3::Clockwise(Vector3::forward, forward)) fowardRot = -fowardRot;

		yRot += fowardRot;

		if(distance > 0) dir.Normalize();

		float hw = w / 2.0f;
		float hd = d / 2.0f;

		_vecs[0].set(-hw, 0, _soul->Result[triggerTime].Rect_HalfEffect ? 0 : (-hd));
		_vecs[1].set(-hw, 0, hd);
		_vecs[2].set(hw, 0, hd);
		_vecs[3].set(hw, 0, _soul->Result[triggerTime].Rect_HalfEffect ? 0 : (-hd));

		return UtilMath::IsInRect(distance * dir, _vecs, Vector3::zero, yRot);
	}
}

bool XSkillCore::IsInAttackField(int triggerTime, const Vector3& pos, const Vector3& forward, XEntity* target)
{
	if(!XEntity::IsValidEntity(target)) return false;

	//consider offset
	_offset.set(_soul->Result[triggerTime].Offset_X, 0, _soul->Result[triggerTime].Offset_Z);

	float yRot = Vector3::Angle(Vector3::forward, _firer->GetFace_p());
	if(!Vector3::Clockwise(Vector3::forward, _firer->GetFace_p())) yRot = -yRot;

	const Vector3& position = pos + Vector3::HorizontalRotate(_offset, yRot, false);

	const XEntityPresentation::RowData* present = target->PresentComponent() ? target->PresentComponent()->PresentLib() : 0;
	//calculating...
	if(present && present->Huge)
	{
		int i = 0;
		auto it = present->HugeMonsterColliders.begin();

		while (it != present->HugeMonsterColliders.end())
		{
			float radius = (*it)[2] * target->GetUnit()->GetScale();

			if(InnerIsInAttackField(
				triggerTime, 
				position, 
				forward, 
				ResultPos(position, target->HugeMonsterColliderCenter(i), radius), 
				_soul->Result[triggerTime].None_Sector_Angle_Shift))
				return true;

			++it; ++i;
		}

		return false;
	}
	else
	{
		return InnerIsInAttackField(
			triggerTime, 
			position, 
			forward, 
			ResultPos(position, target->RadiusCenter(), target->GetUnit()->IsRole() ? 0 : target->GetUnit()->GetBoundRaidus()), 
			_soul->Result[triggerTime].None_Sector_Angle_Shift);
	}
}

bool XSkillCore::IsInAttackField(const Vector3& pos, const Vector3& forward, XEntity* target)
{
	return XEntity::IsValidEntity(target) && GetAttackWeights(
		Vector3(pos.x + _soul->Cast_Offset_X, pos.y, pos.z + _soul->Cast_Offset_Z), 
		forward, 
		target, 
		!_soul->Cast_Range_Rect) > -FLT_MAX;
}

XEntity* XSkillCore::FindTargetAt(const Vector3& pos, const Vector3& forward)
{
	XEntity* target = NULL;
	Scene* pScene = _firer->GetUnit()->GetCurrScene();

	const vector<Unit*>* enemies = pScene->GetOpponents(_firer->GetUnit()->GetFightGroup());
	if (NULL == enemies)
	{
		return target;
	}

	vector<Unit*>::const_iterator it = enemies->begin();
	float max = -FLT_MAX;

	while (it != enemies->end())
	{
		XEntity* pEnt = dynamic_cast<XEntity*>((*it)->GetXObject());

		if(XEntity::IsValidEntity(pEnt))
		{
			float weight = GetAttackWeights(
				Vector3(pos.x + _soul->Cast_Offset_X, pos.y, pos.z + _soul->Cast_Offset_Z), 
				forward,
				pEnt,
				!_soul->Cast_Range_Rect);

			if(weight > max)
			{
				max = weight;
				target = pEnt;
			}
		}

		++it;
	}

	return target;
}

float XSkillCore::GetAttackWeights(const Vector3& pos, const Vector3& forward, XEntity* target, bool sector)
{
	const Vector3& towards = Vector3::HorizontalRotate(forward, _soul->Cast_Scope_Shift);

	const XEntityPresentation::RowData* present = target->PresentComponent() ? target->PresentComponent()->PresentLib() : 0;
	//calculating...
	if(present && present->Huge)
	{
		int i = 0;
		auto it = present->HugeMonsterColliders.begin();

		float weight = -FLT_MAX;

		while (it != present->HugeMonsterColliders.end())
		{
			float radius = (*it)[2] * target->GetUnit()->GetScale();

			float res = InnerGetAttackWeights(pos, towards, dynamic_cast<Enemy*>(target->GetUnit()), ResultPos(pos, target->HugeMonsterColliderCenter(i), radius), sector);
			if(res > weight) weight = res;

			++it; ++i;
		}

		return weight;
	}
	else
	{
		return InnerGetAttackWeights(pos, towards, dynamic_cast<Enemy*>(target->GetUnit()), ResultPos(pos, target->RadiusCenter(), target->GetUnit()->IsRole() ? 0 : target->GetUnit()->GetBoundRaidus()), sector);
	}
}

float XSkillCore::InnerGetAttackWeights(const Vector3& pos, const Vector3& forward, Enemy* target, const Vector3& logical, bool sector)
{
	float weight = -FLT_MAX;

	Vector3 dir = logical - pos; dir.y = 0;
	float distance = Vector3::Magnitude(dir);

	//normalize
	dir.Normalize();
	float angle = (dir.IsZero() || distance == 0) ? 0 : Vector3::Angle(forward, dir);

	bool contains = false;

	if(sector)
	{
		contains = (distance < CastRangeUpper() &&
					distance >= CastRangeLower() &&
					angle <= CastScope());
	}
	else
	{
		if(distance > 0) dir.Normalize();

		float hw = CastScope() / 2.0f;
		float hd = CastRangeUpper() / 2.0f;

		_vecs[0].set(-hw, 0, -hd);
		_vecs[1].set(-hw, 0, hd);
		_vecs[2].set(hw, 0, hd);
		_vecs[3].set(hw, 0, -hd);

		contains = UtilMath::IsInRect(distance * dir, _vecs, Vector3::zero, 0);
	}

	if(contains)
	{
		weight = 0;

		/*float x = distance - CastRangeLower();
		float y = CastRangeUpper() - CastRangeLower();

		//distance per cell
		float t = y /  GetGlobalConfig().WithinRange;
		int cells = XIsInteger(t) ? std::floor(t + 0.05f) : std::ceil(t);

		float factor = ((float) GetGlobalConfig().RangeWeight) / cells;

		t = cells * (1 - (x / y));
		int occupied = XIsInteger(t) ? std::floor(t + 0.05f) : std::ceil(t);

		//added distance ratio
		weight += (occupied * factor);

		//added angle ratio
		weight += ((1 - (angle / CastScope())) * factor);

		//added type ratio
		if(target)
		{
			//immortal target
			if (!target->IsRole() && target->GetXObject() && target->GetXObject()->BuffComponent() && target->GetXObject()->BuffComponent()->IsBuffStateOn(XBuffType_Immortal))
				weight +=  GetGlobalConfig().ImmortalWeight;

			if(target->IsBoss())
				weight +=  GetGlobalConfig().BossWeight;
			else if(target->IsElite())
				weight +=  GetGlobalConfig().EliteWeight;
			else if(target->IsPuppet())
				weight +=  GetGlobalConfig().PupetWeight;
			else if(target->IsRole())
				weight +=  GetGlobalConfig().RoleWeight;
			else
				weight +=  GetGlobalConfig().EnemyWeight;
		}*/
	}

	return weight;
}

XSkillCore* XSkillCore::GetSpecificCore(XEntity* pEntity, XSkillCore* main, int triggerTime, int resultToken)
{
	XSkillCore* core = main;

	if(main->GetSoul()->TypeToken == 3)
	{
		const vector<XCombinedData>& v = core->GetSoul()->Combined;
		vector<XCombinedData>::const_iterator it = v.begin();

		int left = resultToken;
		core = NULL;

		while(it != v.end())
		{
			//combined skill
			XSkillCore* pCore = pEntity->SkillComponent()->SkillManager()->GetSkill(XCommon::XHash((*it).Name.c_str()));

			if(left == triggerTime && (int)pCore->GetSoul()->Result.size() > triggerTime)
			{
				core = pCore;
				break;
			}
			else
			{
				left -= pCore->GetSoul()->Result.size();
			}

			++it;
		}
	}

	return core;
}

void XSkillCore::AccelerateCD(float delta, bool ratio)
{
	if (!CooledDown())
	{
		if(ratio)
		{
			_dynamic_cd_delta += (GetCoolDown() - GetElapsedCD()) * delta;
		}
		else
			_dynamic_cd_delta += delta;

		//trigger effect
		GetCoolDown();
	}
	else
	{
		_dynamic_cd_delta = 0;
	}
}

void XSkillCore::ResetStaticCD()
{
	Scene* current = _firer->GetUnit()->GetCurrScene();
	bool IsPVP = current ? Scene::IsPVPCombat(current) : _firer->GetUnit()->IsEnterPVP();

	_static_cd = (float)(XSkillEffectMgr::Instance()->GetSkillCDStaticRatio(
		ID(), 
		_skill_level, 
		_firer->GetUnit()->GetSkillCasterTypeID(), 
		IsPVP) * _soul->CoolDown);

	if (Reloading()) GetCoolDown();
}

void XSkillCore::AccelerateStaticCD(float delta)
{
	if (delta < 0 || delta > 1) return;
	_static_cd = (1 - delta) * _static_cd;

	if (Reloading()) GetCoolDown();
}

float XSkillCore::CheckDynamicCD(float static_cd)
{
	float dynamic_cd = static_cd * XSkillEffectMgr::Instance()->CalcSkillCDDynamicRatio(1, _semi_dynamic_cd_ratio);

	if(Reloading())
	{
		float dynamic_cd_ratio = XSkillEffectMgr::Instance()->CalcSkillCDDynamicRatio(
			XSkillEffectMgr::Instance()->CanChangeCD(ID(), _skill_level, _firer->GetUnit()->GetSkillCasterTypeID()) ? XSkillEffectMgr::Instance()->GetSkillCDDynamicRatio(_firer->GetUnit()) : 1,
			_semi_dynamic_cd_ratio);

		float now = MillionSecToSeconds(TimeUtil::GetMilliSecond());

		if(_dynamic_cd_ratio != dynamic_cd_ratio)
		{
			float ratio = dynamic_cd_ratio / _dynamic_cd_ratio;
			_last_lock_and_load_time = now - (now - _last_lock_and_load_time) * ratio;
			
			_dynamic_cd_ratio = dynamic_cd_ratio;
		}

		dynamic_cd = static_cd * _dynamic_cd_ratio - _dynamic_cd_delta;

		if(_last_dynamic_cd != dynamic_cd)
		{
			float left = dynamic_cd - (now - _last_lock_and_load_time);

			CTimerMgr::Instance()->KillTimer(_timer_token);
			if(left > 0)
			{
				_timer_token = XCommon::SetTimer(left, DelegateCombination(XSkillCore, OnReloaded, this), __FILE__, __LINE__);
			}
			else
			{
				CoolDown();
			}
		}
	}

	_last_dynamic_cd = dynamic_cd;
	return dynamic_cd;
}

float XSkillCore::GetMultipleDirectionFactor()
{
	if (!_carrier || !_carrier->MainCore()->GetSoul()->MultipleAttackSupported) return 1;

	XEntity* target = _carrier->Target();

	Vector3 relative = target ? Vector3::Horizontaled(target->GetPosition_p() - _firer->GetPosition_p()) : _firer->GetFace_p();
	const Vector3& action = _carrier->SkillTowardsTo();    //action towards

	relative.y = 0;
	if(relative.IsZero()) relative = Vector3::forward;

	float angle = Vector3::Angle(relative, action);
	bool clockwise = Vector3::Clockwise(relative, action);

	angle = clockwise ? angle : 360.0f - angle;

	return angle / 360.0f;
}

void XSkillCore::CoolDown()
{
	CTimerMgr::Instance()->KillTimer(_timer_token);
	_current_running_time = _totally_running_time;

	OnReloaded(NULL, NULL);
}

bool XSkillCore::OnReloaded(IArgs* pargs, void *) 
{
	assert(_magic_num == 0xFEDCBA98);

	if(_is_init_cooling) _current_running_time = _totally_running_time;
	_is_init_cooling = false; 

	++_current_running_time;
	CheckRunningTime();

	if(Reloading())
	{
		_last_lock_and_load_time = MillionSecToSeconds(TimeUtil::GetMilliSecond());

		float cd = GetCoolDown();

		CTimerMgr::Instance()->KillTimer(_timer_token);
		if(cd > 0) _timer_token = XCommon::SetTimer(cd, DelegateCombination(XSkillCore, OnReloaded, this), __FILE__, __LINE__);
	}
	else
	{
		_dynamic_cd_delta = 0;
		_dynamic_cd_ratio = 1;
	}

	return true; 
}

void XSkillCore::MakeCoolDownAtLaunch()
{
	if(HasInitCD())
	{
		_current_running_time = 0;
		_is_init_cooling = true;

		_last_lock_and_load_time = MillionSecToSeconds(TimeUtil::GetMilliSecond());

		CTimerMgr::Instance()->KillTimer(_timer_token);
		_timer_token = XCommon::SetTimer(_init_cd, DelegateCombination(XSkillCore, OnReloaded, this), __FILE__, __LINE__);
	}
}

int XSkillCore::PreservedStrength()
{
	return _preserved_strength * _soul->Logical.PreservedStrength * XSkillEffectMgr::Instance()->GetXULIPower(_firer);
}
