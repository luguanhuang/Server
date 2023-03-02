#include "pch.h"

#include "XBeHitComponent.h"
#include "XPresentComponent.h"
#include "skill/XSkillCore.h"
#include "utility/XActionCurve.h"
#include "util/XCommon.h"
#include "entity/XEnemy.h"
#include "unit/unit.h"
#include "scene/scene.h"
#include "scene/grid.h"
#include "table/globalconfig.h"
#include "entity/XObject.h"
#include "XFlyComponent.h"
#include "XEntityStatistics.h"
#include "tablemgr/XEntityInfoLibrary.h"

using namespace KKSG;

XBeHitComponent::XBeHitComponent(void)
	:_pos(Vector3::zero),
	 _des(Vector3::zero),
	 _phase(Hit_Present),
	 _last_set_qte(QTE_None),
	 _last_offset(0),
	 _last_height(0),
	 _delta_x(0),
	 _delta_y(0),
	 _delta_z(0),
	 _deltaH(0),
	 _land_time(0),
	 _bounce_time(0),
	 _getup_time(0),
	 _gravity(0),
	 _rticalV(0),
	 _factor(0),
	 _elapsed(0),
	 _change_to_fly(false),
	 _hit_down(false),
	 _hit_bounce(false),
	 _total_time(0),
	 _present_straight(1),
	 _hard_straight(1),
	 _offset(0),
	 _height(0),
	 _curve_h(NULL),
	 _curve_v(NULL),
	 _curve_height_scale(1),
	 _curve_offset_scale(1),
	 _curve_height_time_scale(1),
	 _curve_offset_time_scale(1),
	 _hit_by(0),
	 _bHasFlyPresent(false),
	 _bHasRollPresent(false),
	 _bSelfHasFlyPresent(false),
	 _bSelfHasRollPresent(false)
{
	_selfState = XState_BeHit;
}

XBeHitComponent::~XBeHitComponent(void)
{

}

void XBeHitComponent::ActionSubscribe()
{
	RegisterAction(XAction_BeHit, DelegateCombination(XActionStateComponent<XBeHitActionArgs>, OnAction, this));
}

void XBeHitComponent::OnAttachToHost(XObject* pHost)
{
	XActionStateComponent<XBeHitActionArgs>::OnAttachToHost(pHost);

	UINT32 present_id = GetHost()->GetUnit()->GetPresentID();
}

void XBeHitComponent::Attached()
{
	_bSelfHasFlyPresent = GetEntity()->PresentComponent()->PresentLib()->HitFly.size() > 0;
	_bSelfHasRollPresent = GetEntity()->PresentComponent()->PresentLib()->Hit_Roll.size() > 0;

	_bHasFlyPresent = _bSelfHasFlyPresent;
	_bHasRollPresent = _bSelfHasRollPresent;
}

void XBeHitComponent::OnDetachFromHost()
{
	XActionStateComponent<XBeHitActionArgs>::OnDetachFromHost();
}

void XBeHitComponent::OnGetAction(XBeHitActionArgs* pAction)
{
	XActionStateComponent<XBeHitActionArgs>::OnGetAction(pAction);

	_hit_down = false;
	_hit_bounce = false;
}

bool XBeHitComponent::Begin()
{
	GetEntity()->SyncIncreament();

	const XEntityPresentation::RowData* row_data = GetEntity()->PresentComponent()->PresentLib();

	_hit_by = _args.HitFrom;
	_change_to_fly = _args.ForceToFlyHit;

	_present_straight = (_change_to_fly ? (_args.Data()->Additional_Using_Default ?  GetGlobalConfig().Hit_PresentStraight : _args.Data()->Additional_Hit_Time_Present_Straight) : _args.Data()->Time_Present_Straight);
	_hard_straight = (_change_to_fly ? (_args.Data()->Additional_Using_Default ?  GetGlobalConfig().Hit_HardStraight : _args.Data()->Additional_Hit_Time_Hard_Straight) : _args.Data()->Time_Hard_Straight);
	_offset = (_change_to_fly ? (_args.Data()->Additional_Using_Default ?  GetGlobalConfig().Hit_Offset : _args.Data()->Additional_Hit_Offset) : _args.Data()->Offset);
	_height = (_change_to_fly ? (_args.Data()->Additional_Using_Default ?  GetGlobalConfig().Hit_Height : _args.Data()->Additional_Hit_Height) : _args.Data()->Height);

	float OffsetTimeScale_Offset = 1;
	float OffsetTimeScale_Height = 1;
	float OffsetTimeScale_Present = 1;
	float OffsetTimeScale_Hard = 1;

	switch (_args.Data()->State)
	{
	case Hit_Back:
		{
			if (_change_to_fly)
			{
				uint size = row_data->HitFlyOffsetTimeScale.size();

				OffsetTimeScale_Offset = (size < 1) ? 1 : (row_data->HitFlyOffsetTimeScale[0] == 0 ? 1 : row_data->HitFlyOffsetTimeScale[0]);
				OffsetTimeScale_Height = (size < 2) ? 1 : (row_data->HitFlyOffsetTimeScale[1] == 0 ? 1 : row_data->HitFlyOffsetTimeScale[1]);
				OffsetTimeScale_Present = (size < 3) ? 1 : (row_data->HitFlyOffsetTimeScale[2] == 0 ? 1 : row_data->HitFlyOffsetTimeScale[2]);
				OffsetTimeScale_Hard = (size < 4) ? 1 : (row_data->HitFlyOffsetTimeScale[3] == 0 ? 1 : row_data->HitFlyOffsetTimeScale[3]);
			}
			else
			{
				uint size = row_data->HitBackOffsetTimeScale.size();

				OffsetTimeScale_Offset = (size < 1) ? 1 : (row_data->HitBackOffsetTimeScale[0] == 0 ? 1 : row_data->HitBackOffsetTimeScale[0]);
				OffsetTimeScale_Present = (size < 2) ? 1 : (row_data->HitBackOffsetTimeScale[1] == 0 ? 1 : row_data->HitBackOffsetTimeScale[1]);
				OffsetTimeScale_Hard = (size < 3) ? 1 : (row_data->HitBackOffsetTimeScale[2] == 0 ? 1 : row_data->HitBackOffsetTimeScale[2]);
			}
		}break;
	case Hit_Fly:
		{
			uint size = row_data->HitFlyOffsetTimeScale.size();

			OffsetTimeScale_Offset = (size < 1) ? 1 : (row_data->HitFlyOffsetTimeScale[0] == 0 ? 1 : row_data->HitFlyOffsetTimeScale[0]);
			OffsetTimeScale_Height = (size < 2) ? 1 : (row_data->HitFlyOffsetTimeScale[1] == 0 ? 1 : row_data->HitFlyOffsetTimeScale[1]);
			OffsetTimeScale_Present = (size < 3) ? 1 : (row_data->HitFlyOffsetTimeScale[2] == 0 ? 1 : row_data->HitFlyOffsetTimeScale[2]);
			OffsetTimeScale_Hard = (size < 4) ? 1 : (row_data->HitFlyOffsetTimeScale[3] == 0 ? 1 : row_data->HitFlyOffsetTimeScale[3]);
		}break;
	case Hit_Roll:
		{
			if (_change_to_fly)
			{
				uint size = row_data->HitFlyOffsetTimeScale.size();

				OffsetTimeScale_Offset = (size < 1) ? 1 : (row_data->HitFlyOffsetTimeScale[0] == 0 ? 1 : row_data->HitFlyOffsetTimeScale[0]);
				OffsetTimeScale_Height = (size < 2) ? 1 : (row_data->HitFlyOffsetTimeScale[1] == 0 ? 1 : row_data->HitFlyOffsetTimeScale[1]);
				OffsetTimeScale_Present = (size < 3) ? 1 : (row_data->HitFlyOffsetTimeScale[2] == 0 ? 1 : row_data->HitFlyOffsetTimeScale[2]);
				OffsetTimeScale_Hard = (size < 4) ? 1 : (row_data->HitFlyOffsetTimeScale[3] == 0 ? 1 : row_data->HitFlyOffsetTimeScale[3]);
			}
			else
			{
				uint size = row_data->HitRollOffsetTimeScale.size();

				OffsetTimeScale_Offset = (size < 1) ? 1 : (row_data->HitRollOffsetTimeScale[0] == 0 ? 1 : row_data->HitRollOffsetTimeScale[0]);
				OffsetTimeScale_Present = (size < 2) ? 1 : (row_data->HitRollOffsetTimeScale[1] == 0 ? 1 : row_data->HitRollOffsetTimeScale[1]);
				OffsetTimeScale_Hard = (size < 3) ? 1 : (row_data->HitRollOffsetTimeScale[2] == 0 ? 1 : row_data->HitRollOffsetTimeScale[2]);
			}
		}break;
	default:
		break;
	}

	_present_straight = _present_straight * OffsetTimeScale_Present;
	_hard_straight = _hard_straight * OffsetTimeScale_Hard * _args.Paralyze;

	if(_change_to_fly || _args.Data()->State == Hit_Fly) _height = _height * OffsetTimeScale_Height;
	_offset = _offset * OffsetTimeScale_Offset;

	//Prepare
	uint bounce_getup_size = row_data->HitFly_Bounce_GetUp.size();

	_land_time = ((_change_to_fly || _args.Data()->State == Hit_Fly) && _bHasFlyPresent) ? (bounce_getup_size < 1 ? 0 : row_data->HitFly_Bounce_GetUp[0]) : 0;
	_bounce_time = ((_change_to_fly || _args.Data()->State == Hit_Fly) && _bHasFlyPresent) ? (bounce_getup_size < 2 ? 0 :row_data->HitFly_Bounce_GetUp[1]) : 0;
	
	_getup_time = ((_change_to_fly || _args.Data()->State == Hit_Fly) && _bHasFlyPresent) ? 
				   (bounce_getup_size < 3 ? 0 : row_data->HitFly_Bounce_GetUp[2]) : 
				   ((_args.Data()->State == Hit_Roll && _bHasRollPresent) ? 
					 row_data->HitRoll_Recover :
					(row_data->HitBack_Recover.size() < 1 ? 0 : row_data->HitBack_Recover[0]));

	_total_time = _present_straight + _land_time + _bounce_time + _hard_straight + _getup_time;

	_pos = _entity->GetPosition();
	_des = _pos + _args.HitDirection * _offset;

	float y = 0;

	if(GetEntity()->GetUnit()->GetCurrScene()->GetGrid()->TryGetHeight(_des.x, _des.z, y))
	{
		_deltaH = _pos.y - (_entity->Fly() ? y + _entity->Fly()->CurrentHeight() : y);
	}
	else
		_deltaH = 0;
	
	if(_deltaH < 0) _deltaH = 0;

	if(_args.Data()->CurveUsing)
	{
		const string& curve_prefix = GetEntity()->PresentComponent()->CurvePrefix();

		uint hit_curve_size = row_data->HitCurves.size();

		_curve_h = ((_change_to_fly || _args.Data()->State == Hit_Fly) && _bHasFlyPresent) ? (hit_curve_size < 5 ? NULL : XActionCurveMgr::Instance()->GetCurve(curve_prefix + row_data->HitCurves[4])) : NULL;
		_curve_v = ((_change_to_fly || _args.Data()->State == Hit_Fly) && _bHasFlyPresent) ? 
					(hit_curve_size < 4 ? NULL : XActionCurveMgr::Instance()->GetCurve(curve_prefix + row_data->HitCurves[3])) :
					((_args.Data()->State == Hit_Roll && _bHasRollPresent) ?
					  (hit_curve_size < 6 ? NULL : XActionCurveMgr::Instance()->GetCurve(curve_prefix + row_data->HitCurves[5])) : 
					  (_args.Data()->State == Hit_Back ? (hit_curve_size < (_args.Data()->State_Animation + 1) ? NULL : XActionCurveMgr::Instance()->GetCurve(curve_prefix + row_data->HitCurves[_args.Data()->State_Animation])) : 
					   (hit_curve_size < 1 ? NULL : XActionCurveMgr::Instance()->GetCurve(curve_prefix + row_data->HitCurves[0]))));

		_curve_height_scale = (_curve_h == NULL || _curve_h->GetMaxValue() == 0) ? 1 : _height / _curve_h->GetMaxValue();
		_curve_offset_scale = (_curve_v == NULL) ? 1 : (_curve_v->GetMaxValue() == 0 ? 1 : _offset / _curve_v->GetMaxValue());

		_curve_height_time_scale = (_curve_h == NULL) ? 1 : _present_straight / _curve_h->GetTimeSpan();
		_curve_offset_time_scale = (_curve_v == NULL) ? 1 : _present_straight / _curve_v->GetTimeSpan();

		_last_offset = 0;
		_last_height = 0;
	}
	else
	{
		_pos.y = 0; _des.y = 0;

		_factor = XCommon::GetSmoothFactor(Vector3::Magnitude(_pos - _des), _present_straight, 0.01f);

		_rticalV = (_change_to_fly || _args.Data()->State == Hit_Fly) ? (_height * 4.0f) / _present_straight : 0;
		_gravity = _rticalV / 1 * 2.0f;
	}

	_elapsed = 0;
	_phase = Hit_Present;

	TrytoTirggerQTE();

	return true;
}

void XBeHitComponent::Cancel(XStateDefine next) 
{
	_elapsed = 0;

	_rticalV = 0;
	_gravity = 0;

	_deltaH = 0;

	_curve_h = NULL;
	_curve_v = NULL;

	_hit_by = 0;

	_change_to_fly = false;

	TrytoTirggerQTE(true);
};

void XBeHitComponent::ActionUpdate(float deltaTime)
{
	float last_elapsed = _elapsed;
	_elapsed += deltaTime;

	switch(_phase)
	{
	case Hit_Present:
		{
			bool fly = (_change_to_fly || _args.Data()->State == Hit_Fly) && _bHasFlyPresent;

			if(_elapsed > _present_straight)
			{
				_elapsed = _present_straight;
				if(fly)
				{
					_phase = Hit_Landing;

					_hit_down = true;
					_hit_bounce = false;
				}
				else
					_phase = Hit_Hard;

				TrytoTirggerQTE();
			}

			CalcDeltaPos(deltaTime, last_elapsed);
			float deltaH = -(_deltaH / _present_straight) * deltaTime;

			if (_offset < 0)
			{
				XEntity* pBy = XEntity::ValidEntity(_hit_by);

				if(pBy)
				{
					float translate = sqrt(_delta_x * _delta_x + _delta_z * _delta_z);
					float dis = Vector3::Magnitude(pBy->GetPosition_p() - GetEntity()->GetPosition_p());

					if (translate > dis - 0.5)
					{
						_delta_x = 0;
						_delta_z = 0;
					}
				}
			}

			_entity->ApplyMove(_delta_x, _delta_y + deltaH, _delta_z);
			if(fly) _entity->DisableGravity();

		}break;
	case Hit_Landing:
		{
			if(!_hit_bounce)
			{
				if (_elapsed > _present_straight + _land_time) 
				{
					_hit_bounce = true;
					if(_elapsed <= _present_straight + _land_time + _bounce_time) TrytoTirggerQTE();
				}
			}
			//save one frame
			if(_hit_bounce)
			{
				if (_elapsed > _present_straight + _land_time + _bounce_time) 
				{
					_phase = Hit_Hard;
					_hit_bounce = false;

					TrytoTirggerQTE();
				}
			}
		}break;
	case Hit_Hard:
		{
			if (_elapsed > _present_straight + _land_time + _bounce_time + _hard_straight)
			{
				_phase = Hit_GetUp;
				TrytoTirggerQTE();
			}
		}break;
	case Hit_GetUp:
		{
			if (_elapsed > _total_time - _getup_time * 0.7f)
			{
				_hit_down = false;
			}

			if(_elapsed > _total_time)
			{
				Finish();
			}
		}break;
	default: break;
	}
}

void XBeHitComponent::ReUpdatePresentStatus()
{
	_bHasFlyPresent = _bSelfHasFlyPresent;
	_bHasRollPresent = _bSelfHasRollPresent;

	if(GetEntity()->IsTransform())
	{
		const XEntityStatistics::RowData* data = XEntityInfoLibrary::Instance()->TemplateID2Data_Enemy(GetEntity()->GetTransformer());
		if(data)
		{
			const XEntityPresentation::RowData* pData = XEntityInfoLibrary::Instance()->PresentID2Data(data->PresentID);
			if(pData)
			{
				_bHasFlyPresent = pData->HitFly.size() > 0;
				_bHasRollPresent = pData->Hit_Roll.size() > 0;
			}
		}
	}
}

void XBeHitComponent::CalcDeltaPos(float deltaTime, float last_elapsed)
{
	Vector3 delta;
	float h = 0;

	if (_args.Data()->CurveUsing)
	{
		float ev = (_elapsed) / _curve_offset_time_scale;
		float eh = (_elapsed) / _curve_height_time_scale;

		float c_v = _curve_v == NULL ? 0 : _curve_v->Evaluate(ev) * _curve_offset_scale;
		float c_h = _curve_h == NULL ? 0 : _curve_h->Evaluate(eh) * _curve_height_scale;

		Vector3 v = _args.HitDirection * (c_v - _last_offset);
		delta.x = v.x; delta.z = v.z;

		h = c_h - _last_height;

		_last_height = c_h;
		_last_offset = c_v;
	}
	else
	{
		float v1 = _rticalV - _gravity * (last_elapsed);
		float v2 = _rticalV - _gravity * (_elapsed);

		h = (v1 + v2) / 2.0f * deltaTime;

		_pos = _entity->GetPosition();

		delta = (_des - _pos) * min(1.0f, _factor * deltaTime);
	}

	_delta_x = delta.x;
	_delta_y = h;
	_delta_z = delta.z;
}

XQTEState XBeHitComponent::GetQTESpecificPhase()
{
	XQTEState status = QTE_None;

	switch(_phase)
	{
	case Hit_Present:
		{
			switch(_args.Data()->State)
			{
			case Hit_Back: status = (_change_to_fly && _bHasFlyPresent) ? QTE_HitFlyPresent : QTE_HitBackPresent; break;
			case Hit_Fly: status = _bHasFlyPresent ? QTE_HitFlyPresent : QTE_HitBackPresent; break;
			case Hit_Roll: status = (_change_to_fly && _bHasFlyPresent) ? QTE_HitFlyPresent : (_bHasRollPresent ? QTE_HitRollPresent : QTE_HitBackPresent); break;
			default: break;
			}
		}break;
	case Hit_Landing:
		{
			status = _hit_bounce ? QTE_HitFlyBounce : QTE_HitFlyLand;
		}break;
	case Hit_Hard:
		{
			switch(_args.Data()->State)
			{
			case Hit_Back: status = (_change_to_fly && _bHasFlyPresent) ? QTE_HitFlyStraight : QTE_HitBackStraight; break;
			case Hit_Fly: status = _bHasFlyPresent ? QTE_HitFlyStraight : QTE_HitBackStraight; break;
			case Hit_Roll: status = (_change_to_fly && _bHasFlyPresent) ? QTE_HitFlyStraight : (_bHasRollPresent ? QTE_HitRollStraight : QTE_HitBackStraight); break;
			default: break;
			}
		}break;
	case Hit_GetUp:
		{
			switch(_args.Data()->State)
			{
			case Hit_Back: status = (_change_to_fly && _bHasFlyPresent) ? QTE_HitFlyGetUp : QTE_HitBackGetUp; break;
			case Hit_Fly: status = _bHasFlyPresent ? QTE_HitFlyGetUp : QTE_HitBackGetUp; break;
			case Hit_Roll: status = (_change_to_fly && _bHasFlyPresent) ? QTE_HitFlyGetUp : (_bHasRollPresent ? QTE_HitRollGetUp : QTE_HitBackGetUp); break;
			default: break;
			}
		}break;
	default:
		break;
	}

	return status;
}

void XBeHitComponent::TrytoTirggerQTE(bool bEnd)
{
	if(GetEntity()->QuickTimeEventComponent())
	{
		XQTEState qte = QTE_None;

		if(!bEnd)
		{
			qte = GetQTESpecificPhase();

			if(_last_set_qte != QTE_None)
			{
				XSkillQTEEventArgs args;
				args.State = _last_set_qte;
				args.On = false;
				GetEntity()->DispatchAction(&args);
			}

			_last_set_qte = qte;
		}
		else
			_last_set_qte = QTE_None;

		XSkillQTEEventArgs args;
		args.State = qte;
		args.On = !bEnd;
		GetEntity()->DispatchAction(&args);
	}
}
