#include "pch.h"

#include "XBullet.h"
#include "XBulletCore.h"
#include "XBulletMgr.h"

#include "skill/XSkill.h"

#include "unit/unit.h"
#include "scene/scene.h"
#include "scene/grid.h"

XBulletCore::XBulletCore(
	UINT64 token, 
	UINT64 firer, 
	XEntity* target, 
	XSkillCore* core, 
	int idx, 
	UINT32 resultID,
	int diviation,
	int wid)
	:_token(token),
	 _firer(firer), 
	 _target(target ? target->GetID() : 0),
	 _core(core),
	 _sequnce(idx), 
	 _result_id(resultID), 
	 _result_time(core->GetSoul()->Result[idx].Token), 
	 _life(0), 
	 _running(0),
	 _radius(0), 
	 _velocity(0), 
	 _init_h(0),
	 _warning(false), 
	 _with_terrain(true),
	 _warning_pos(Vector3::zero)
{
	XEntity* pFire = GetFirer();

	_result = &(_core->GetSoul()->Result[_sequnce]);

	_life = GetResultData()->LongAttackData.Runningtime + GetResultData()->LongAttackData.Stickytime;
	_running = GetResultData()->LongAttackData.Runningtime;
	_radius = GetResultData()->LongAttackData.Radius;

	if(GetResultData()->Attack_All)
	{
		_warning_pos = target ? target->GetPosition_p() : Vector3::zero;
	}
	else if(GetResultData()->Warning)
	{
		_warning_pos = core->WarningPosAt[GetResultData()->Warning_Idx][wid].WarningAt;
	}

	_warning = !_warning_pos.IsZero();

	_begin = pFire->GetPosition(); _begin.y += (pFire->GetUnit()->GetBoundHeight() * 0.5f);
	Vector3 dir = _warning ? (_warning_pos - pFire->GetPosition_p()) : (((GetResultData()->LongAttackData.Reinforce && target) ? (target->GetPosition_p() - pFire->GetPosition_p()) : pFire->GetFace()));

	Vector3 result_bullet_at(
		GetResultData()->LongAttackData.At_X, 
		GetResultData()->LongAttackData.At_Y, 
		GetResultData()->LongAttackData.At_Z);

	_begin += Vector3::HorizontalRotate(result_bullet_at, pFire->GetFaceDegree(), false);

	dir.y = 0;

	_fly_to = Vector3::HorizontalRotate(dir, (float)diviation);
	_velocity = _warning ? Vector3::Magnitude(_warning_pos - _begin) / _running : GetResultData()->LongAttackData.Velocity;

	float h = 0;

	if (target != NULL && GetResultData()->LongAttackData.AimTargetCenter)
	{
		h = (_begin.y - target->GetPosition_p().y - target->GetUnit()->GetBoundHeight() * 0.5f);
		if(fabs(h) > 2) h = 2 * (h / fabs(h));
	}

	_fly_to = (h == 0 || _velocity == 0) ? _fly_to : Vector3::Normalized(h * Vector3::down + _velocity * _running * _fly_to);

	if(_fly_to.IsZero()) _fly_to = pFire->GetFace();

	_init_h = _begin.y - pFire->GetUnit()->GetCurrScene()->GetGrid()->GetHeight(_begin.x, _begin.z);
	if (_init_h < 0) _init_h = 0;

	_with_terrain = GetResultData()->LongAttackData.FlyWithTerrain;
}

const XResultData* XBulletCore::GetResultData() const
{
	return _result;
}
