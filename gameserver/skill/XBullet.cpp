#include "pch.h"

#include "XBullet.h"
#include "XBulletCore.h"
#include "XBulletMgr.h"

#include "skill/XSkillCore.h"
#include "skill/XSkill.h"
#include "entity/XEntity.h"
#include "scene/collid.h"
#include "unit/unit.h"
#include "scene/scene.h"
#include "scene/grid.h"
#include "table/XEntityPresentation.h"
#include "component/XPresentComponent.h"
#include "utility/UtilMath.h"
#include "component/XNetComponent.h"

XBullet::XBullet(bool client, UINT64 id, XBulletCore* data)
	:_magic_num(0xCBA98765),
	 _id(id), 
	 _extra_id(0), 
	 _client(client), 
	 _active(true), 
	 _pingpong(false), 
	 _hit_triggered(false), 
	 _layer_mask(-1), 
	 _elapsed(0), 
	 _data(data), 
	 _origin(Vector3::zero),
	 _position(Vector3::zero),
	 _tail_results(0),
	 _tail_results_token(INVALID_HTIMER)
{
	_origin = data->Begin();
	_position = _origin;
}

XBullet::~XBullet()
{
	_magic_num = 0;

	CTimerMgr::Instance()->KillTimer(_tail_results_token);

	delete _data;
	_data = NULL;

	auto it = _hurt_target.begin();
	while(it != _hurt_target.end())
	{
		CTimerMgr::Instance()->KillTimer((*it).second.TimerToken);
		++it;
	}

	_hurt_target.clear();
}

bool XBullet::IsExpired()
{
	if(!XEntity::IsValidEntity(_data->GetFirer())) return true;

	if (_tail_results != 0)
	{
		if (_tail_results >= _data->GetResultData()->LongAttackData.TriggerAtEnd_Count)
			//finally expired
			return true;
		else
			//Feature #45343
			return false;
	}

	if(_data->GetResultData()->LongAttackData.IsPingPong && !_pingpong)
	{
		if(_elapsed > _data->Life()) OnPingPong();
	}

	bool expired = (!_active || (!_pingpong && _elapsed > _data->Life()));

	if (_data->GetResultData()->LongAttackData.TriggerAtEnd_Count <= 0)
		return expired;
	else
	{
		if (expired)
		{
			//Feature #45343
			_active = false;
			OnTailResult(NULL, NULL);

			return false;
		}
		else
			return expired;
	}
}

bool XBullet::OnTailResult(IArgs*, void* param)
{
	assert(_magic_num == 0xCBA98765);
	//BUG #47076
	if(!XEntity::IsValidEntity(_data->GetFirer())) return true;

	if (param == NULL)
	{
		_tail_results = 0;
	}

	if (_tail_results < _data->GetResultData()->LongAttackData.TriggerAtEnd_Count)
	{
		_tail_results++;

		TailResult();

		CTimerMgr::Instance()->KillTimer(_tail_results_token);

		_tail_results_token = XCommon::SetTimer(
			_data->GetResultData()->LongAttackData.TriggerAtEnd_Cycle, 
			DelegateCombination(XBullet, OnTailResult, this),
			(void*)this,
			__FILE__, 
			__LINE__);
	}

	return true;
}

void XBullet::TailResult()
{
	if (_data->GetResultData()->LongAttackData.TriggerAtEnd)
	{
		if (_data->IsWarning()) _position = _data->WarningPos();
		Result(NULL, _data->GetResultData()->LongAttackData.TriggerAtEnd_Count == 0);
	}
}

void XBullet::Destroy()
{
	CTimerMgr::Instance()->KillTimer(_tail_results_token);

	if(!_client)
	{
		XEntity* firer = _data->GetFirer();

		if(_data->GetResultData()->LongAttackData.TriggerAtEnd_Count == 0 && firer && !firer->IsDead())
		{
			TailResult();
		}
	}
}

bool XBullet::IsHurtEntity(UINT64 id)
{
	auto it = _hurt_target.find(id);

	if(it != _hurt_target.end())
	{
		return !((*it).second.Hurtable);
	}
	else
		return false;
}

bool XBullet::OnRefined(IArgs*, void* param)
{
	assert(_magic_num == 0xCBA98765);

	XBulletTarget* pbt = reinterpret_cast<XBulletTarget*>(param);

	auto it = _hurt_target.find(pbt->ID);

	if(it != _hurt_target.end())
	{
		XBulletTarget& bt = (*it).second;

		if(bt.HurtCount < _data->GetResultData()->LongAttackData.Refine_Count)
		{
			bt.Hurtable = true;
		}
	}

	return true;
}

void XBullet::OnRefined()
{
	auto it = _hurt_target.begin();

	while (it != _hurt_target.end())
	{
		XBulletTarget& bt = (*it).second;
		//reset
		bt.Hurtable = true;
		bt.HurtCount = 0;
		CTimerMgr::Instance()->KillTimer(bt.TimerToken);

		++it;
	}
}

void XBullet::OnPingPong()
{
	_pingpong = true;

	if(_data->GetResultData()->LongAttackData.AutoRefine_at_Half) OnRefined();
}

void XBullet::OnResult(UINT64 id)
{
	auto it = _hurt_target.find(id);

	if(it == _hurt_target.end())
	{
		XBulletTarget bt;

		// _hurt_target.insert(make_pair<UINT64, XBulletTarget>(id, bt));lgh
		_hurt_target[id]=bt;//.insert(make_pair<UINT64, XBulletTarget>(id, bt));

		_hurt_target[id].ID = id;
		_hurt_target[id].Hurtable = false;
		_hurt_target[id].HurtCount++;
		_hurt_target[id].TimerToken = _data->GetResultData()->LongAttackData.Refine_Cycle > 0 ? 
			XCommon::SetTimer(
			_data->GetResultData()->LongAttackData.Refine_Cycle, 
			DelegateCombination(XBullet, OnRefined, this), 
			&_hurt_target[id],
			__FILE__, 
			__LINE__) : 
			INVALID_HTIMER;
	}
	else
	{
		XBulletTarget& bt = (*it).second;

		CTimerMgr::Instance()->KillTimer(bt.TimerToken);

		bt.Hurtable = false;
		bt.HurtCount++;
		bt.TimerToken = _data->GetResultData()->LongAttackData.Refine_Cycle > 0 ? 
			XCommon::SetTimer(
			_data->GetResultData()->LongAttackData.Refine_Cycle, 
			DelegateCombination(XBullet, OnRefined, this), 
			&((*it).second),
			__FILE__, 
			__LINE__) : 
			INVALID_HTIMER;
	}

	OnOnceTriggered();
}

void XBullet::Result(XEntity* target, bool cycle)
{
	if(target == NULL)
	{
		XSkill::SkillResult(
			this,
			_data->FlyTo(),
			_position,
			cycle);
	}
	else
	{
		if(!_data->GetResultData()->Attack_Only_Target || target == _data->GetTarget())
		{
			Vector3 dir = target->GetPosition_p() - _origin;
			//maybe zero
			dir.y = 0; dir.Normalize();

			XSkill::SkillResult(
				_data->Token(),
				_data->GetFirer(),
				_data->GetSkillCore(),
				this,
				_data->GetSequnce(),
				_data->ResultID(),
				_data->ResultTime(),
				dir,
				target,
				NULL);

			if (_data->GetResultData()->LongAttackData.TriggerOnce) _hit_triggered = true;
		}
	}
}

void XBullet::OnOnceTriggered()
{
	if (_data->GetResultData()->LongAttackData.TriggerOnce)
	{
		if (_data->GetResultData()->LongAttackData.IsPingPong)
		{
			OnPingPong();
		}
		else
			_active = false;
	}
}

void XBullet::Update(float fDeltaT)
{
	if(!_active) return;

	XEntity* firer = _data->GetFirer();
	if(!firer) return;

	_elapsed += fDeltaT;

	float dis = 0; Vector3 dir = Vector3::forward;

	switch(_data->GetResultData()->LongAttackData.Type)
	{
	case Ring:
		{
			_position.x = firer->GetPosition_p().x;
			_position.z = firer->GetPosition_p().z;
		}break;
	case Sphere:
	case Plane:
		{
			dis = ((_elapsed > _data->Running() && _elapsed < _data->Life()) ? 0 : _data->GetVelocity()) * fDeltaT;
			dir = _data->FlyTo();
		}break;
	case Satellite:
		{
			if(_elapsed - fDeltaT <= 0.001f)
			{
				Vector3 v = firer->GetPosition_p() + _data->FlyTo() * _data->GetResultData()->LongAttackData.RingRadius;

				_position.x = v.x;
				_position.z = v.z;

				dis = 0;
				dir = Vector3::HorizontalRotate(firer->GetFace(), _data->GetResultData()->LongAttackData.Palstance < 0 ? -90.0f : 90.0f);
			}
			else
			{
				Vector3 next = Vector3::HorizontalRotate(_data->FlyTo(), _data->GetResultData()->LongAttackData.Palstance * _elapsed) * _data->GetResultData()->LongAttackData.RingRadius;

				next += firer->GetPosition_p();

				Vector3 d = next - _position; d.y = 0;
				dis = Vector3::Magnitude(d);
				dir = Vector3::Normalized(d);
			}
		}break;
	default:
		break;
	}

	if(_data->GetResultData()->LongAttackData.IsPingPong && _pingpong)
	{
		Vector3 v = firer->GetPosition_p() - _position; v.y = 0;
		if(dis * dis >= Vector3::sqrtMagnitude(v))
		{
			_active = false;
			return;
		}

		dir = Vector3::Normalized(v);
	}
	else if(_data->GetResultData()->LongAttackData.Follow)
	{
		XEntity* pEnt = _data->GetTarget();
		if(pEnt)
		{
			dir = Vector3::Horizontaled(pEnt->GetPosition_p() - _position);
		}
	}

	Vector3 move = dir * dis;

	_origin = _position;
	_position += move;

	float y = 0;
	//can not out of the bounds
	if(!firer->GetUnit()->GetCurrScene()->GetGrid()->TryGetHeight(_position.x, _position.z, y))
	{
		if (_data->GetResultData()->LongAttackData.IsPingPong)
		{
			OnPingPong();
		}
		else
			_active = false; 

		return;
	}
	//check static collision
	if(_data->GetResultData()->LongAttackData.StaticCollider)
	{
		if(y < 0)
		{
			if (_data->GetResultData()->LongAttackData.IsPingPong)
			{
				OnPingPong();
			}
			else
				_active = false; 

			return;
		}
	}
	//check dynamic collision
	if(_data->GetResultData()->LongAttackData.DynamicCollider)
	{
		if (!firer->GetUnit()->GetCurrScene()->CheckDynamicBlock(_origin, _position))
		{
			if (_data->GetResultData()->LongAttackData.IsPingPong)
			{
				OnPingPong();
			}
			else
				_active = false; 

			return;
		}
	}

	if(!_data->HasTarget())
	{
		if(_data->FlyWithTerrain())
		{
			_position.y = y + _data->InitH();
		}
		else if(fabs(_position.y - y) < 0.1f)
		{
			_active = false; return;
		}
		
		move = _position - _origin;
	}

	if(!_client)
	{
		if(_data->GetResultData()->LongAttackData.Manipulation)
		{
			Scene* pScene = firer->GetUnit()->GetCurrScene();

			if(pScene != NULL)
			{
				const std::vector<Unit*>* pSet = pScene->GetOpponents(firer->GetUnit()->GetFightGroup());

				if(pSet)
				{
					auto itor = (*pSet).begin();
					while(itor != (*pSet).end())
					{
						XEntity* x = XEntity::ValidEntity((*itor)->GetID());
						if(x)
						{
							Vector3 gap = _position - x->GetPosition_p(); gap.y = 0;
							float gap_dis = Vector3::Magnitude(gap);

							if (gap_dis < _data->GetResultData()->LongAttackData.ManipulationRadius)
							{
								float len = _data->GetResultData()->LongAttackData.ManipulationForce * fDeltaT;

								x->ApplyMove(Vector3::Normalized(gap) * std::min(gap_dis, len));
							}
						}

						++itor;
					}
				}
			}
		}

		if(!_hit_triggered)
		{
			if(_data->GetResultData()->LongAttackData.WithCollision)
			{
				switch(_data->GetResultData()->LongAttackData.Type)
				{
					case Ring:
						{
							float t = (_elapsed > _data->Life()) ? 0 : (_data->GetResultData()->LongAttackData.RingFull ? ((_elapsed > _data->Life() * 0.5f) ? (_data->Life() - _elapsed) : _elapsed) : _elapsed);
							float _ir = t * _data->GetResultData()->LongAttackData.RingVelocity;
							float _or = _ir + _data->GetResultData()->LongAttackData.RingRadius;

							RingBulletCollideUnit(
								firer->GetUnit()->GetCurrScene(), 
								firer, 
								_ir,
								_or,
								this);
						}break;
					case Sphere:
					case Satellite:
						{
							//hlen
							Vector3 project(move.x, 0, move.z);
							float hlen = Vector3::Magnitude(project) * 0.5f;

							dir.y = 0;
							//rotation
							float rotation = dir.IsZero() ? 0 : Vector3::Angle(Vector3::right, dir);
							rotation = (rotation == 0) ? 0 : (Vector3::Clockwise(Vector3::right, dir) ? -rotation : rotation);

							BulletCollideUnit(
								firer->GetUnit()->GetCurrScene(), 
								firer, 
								Vector3(_position.x + dir.x * hlen, 0, _position.z + dir.z * hlen),
								hlen,
								rotation,
								_data->Radius(), 
								this);
						}break;
					case Plane:
						{
							PlaneBulletCollideUnit(
								firer->GetUnit()->GetCurrScene(), 
								firer, 
								_position, 
								move, 
								_data->Radius(),
								this);
						}break;
				}
			}
		}
	}
}

void XBullet::RingBulletCollideUnit(Scene* pScene, XEntity* firer, float _ir, float _or, XBullet* bullet)
{
	if(pScene)
	{
		const Vector3& center = firer->GetPosition();

		const vector<Unit*>* enemies = pScene->GetOpponents(firer->GetUnit()->GetFightGroup());
		if (NULL == enemies) return;

		auto it = enemies->begin();
		while (it != enemies->end())
		{
			XEntity* oppo = dynamic_cast<XEntity*>((*it)->GetXObject());

			if(oppo)
			{
				bool collided = false;

				const XEntityPresentation::RowData* present = oppo->PresentComponent() ? oppo->PresentComponent()->PresentLib() : 0;

				//calculating...
				if(present && present->Huge)
				{
					int i = 0;
					auto is = present->HugeMonsterColliders.begin();

					while (is != present->HugeMonsterColliders.end())
					{
						float radius = (*is)[2] * oppo->GetUnit()->GetScale();
						//float height = (*is)[3] * oppo->GetUnit()->GetScale();

						Vector3 v = oppo->HugeMonsterColliderCenter(i) - center; v.y = 0;
						float dis = Vector3::sqrtMagnitude(v);

						collided = dis > (_ir * _ir) && dis < (_or * _or);

						if(collided) break;

						++is; ++i;
					}
				}
				else
				{
					Vector3 v = oppo->RadiusCenter() - center; v.y = 0;
					float dis = Vector3::sqrtMagnitude(v);

					collided = dis > (_ir * _ir) && dis < (_or * _or);
				}

				if(collided) bullet->Result(oppo);
			}

			if(bullet->IsExpired()) break;
			++it;
		}
	}
}

void XBullet::PlaneBulletCollideUnit(Scene* pScene, XEntity* firer, const Vector3& origin, const Vector3& move, float r, XBullet* bullet)
{
	if(pScene)
	{
		const Vector3& side = Vector3::HorizontalRotate(move, 90);
		const Vector3& left = origin + side * r;
		const Vector3& right = origin - side * r;

		const vector<Unit*>* enemies = pScene->GetOpponents(firer->GetUnit()->GetFightGroup());
		if (NULL == enemies) return;

		auto it = enemies->begin();
		while (it != enemies->end())
		{
			XEntity* oppo = dynamic_cast<XEntity*>((*it)->GetXObject());

			if(oppo)
			{
				bool collided = false;

				const XEntityPresentation::RowData* present = oppo->PresentComponent() ? oppo->PresentComponent()->PresentLib() : 0;

				//calculating...
				if(present && present->Huge)
				{
					int i = 0;
					auto is = present->HugeMonsterColliders.begin();

					while (is != present->HugeMonsterColliders.end())
					{
						float radius = (*is)[2] * oppo->GetUnit()->GetScale();
						//float height = (*is)[3] * oppo->GetUnit()->GetScale();

						const Vector3& center = oppo->HugeMonsterColliderCenter(i);

						collided = UtilMath::IsLineSegmentCross(center, center - move, left, right);

						if(collided) break;

						++is; ++i;
					}
				}
				else
				{
					const Vector3& pos = oppo->RadiusCenter();

					collided = UtilMath::IsLineSegmentCross(pos, pos - move, left, right);
				}

				if(collided) bullet->Result(oppo);
			}

			if(bullet->IsExpired()) break;
			++it;
		}
	}
}

void XBullet::BulletCollideUnit(
	Scene* pScene, 
	XEntity* firer, 
	const Vector3& rectcenter,
	float hlen,
	float rotation,
	float r, 
	XBullet* bullet)
{
	if(pScene)
	{
		const vector<Unit*>* enemies = pScene->GetOpponents(firer->GetUnit()->GetFightGroup());
		if (NULL == enemies) return;

		auto it = enemies->begin();
		while (it != enemies->end())
		{
			XEntity* oppo = dynamic_cast<XEntity*>((*it)->GetXObject());

			if(oppo)
			{
				bool collided = false;

				const XEntityPresentation::RowData* present = oppo->PresentComponent() ? oppo->PresentComponent()->PresentLib() : 0;
				//calculating...
				if(present && present->Huge)
				{
					int i = 0;
					auto is = present->HugeMonsterColliders.begin();

					while (is != present->HugeMonsterColliders.end())
					{
						float radius = (*is)[2] * oppo->GetUnit()->GetScale();
						//float height = (*is)[3] * oppo->GetUnit()->GetScale();

						const Vector3& center = oppo->HugeMonsterColliderCenter(i);
						Vector3 cycle(center.x - rectcenter.x, 0, center.z - rectcenter.z);

						collided = UtilMath::IsRectCycleCross(hlen, r, cycle, radius);

						if(!collided)
						{
							float oppo_r = radius;
							float distance = Vector3::Magnitude(cycle);
							collided = (distance - oppo_r) < r;
						}

						if(collided) break;

						++is; ++i;
					}
				}
				else
				{
					const Vector3& rc = oppo->RadiusCenter();
					Vector3 cycle(rc.x - rectcenter.x, 0, rc.z - rectcenter.z);
					cycle.HorizontalRotate(rotation, false);

					collided = UtilMath::IsRectCycleCross(hlen, r, cycle, (*it)->GetBoundRaidus());

					if(!collided)
					{
						float oppo_r = (*it)->GetBoundRaidus();
						float distance = Vector3::Magnitude(cycle);
						collided = (distance - oppo_r) < r;
					}
				}

				if(collided) bullet->Result(oppo);
			}

			if(bullet->IsExpired()) break;
			++it;
		}
	}
}
