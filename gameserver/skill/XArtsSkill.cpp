#include "pch.h"
#include "XArtsSkill.h"
#include "entity/XEntity.h"
#include "XSkillCore.h"
#include "xdelegate.hpp"
#include "timermgr.h"
#include "scene/scene.h"

#include "XBullet.h"
#include "XBulletCore.h"
#include "XBulletMgr.h"
#include "unit/unit.h"
#include "scene/grid.h"
#include "utility/uidhelper.h"
#include "table/XSkillEffectMgr.h"
#include "unit/enemymanager.h"
#include "gamelogic/XLevelWave.h"
#include "unit/role.h"
#include "component/XSkillComponent.h"
#include "scene/securitystatistics.h"
#include "scene/securityaistatistics.h"
#include "unit/attributewatcher.h"
#include "component/XBuffComponent.h"

XArtsSkill::XArtsSkill(XEntity* firer)
	:XSkill(firer)
{

}

XArtsSkill::~XArtsSkill()
{

}

//skill do self starting here
void XArtsSkill::Start()
{
	XSkill::Start();

	if(_data->NeedTarget) 
	{
		FocusTarget(XEntity::ValidEntity(_target));
		if(_firer->GetUnit()->IsRole())
		{
			Role* role = dynamic_cast<Role*>(_firer->GetUnit());
			Manual(_firer->ClientManualForward());
		}
	}

	FireEvents();
}

void XArtsSkill::FireEvents()
{
	for (size_t i = 0; i < _data->Result.size(); i++)
	{
		_data->Result[i].Token = i;
		float delay = _data->Result[i].At;

		AddedTimerToken(
			XCommon::SetTimer(
			delay * _time_scale,
			DelegateCombination(XSkill, OnResult, this),
			reinterpret_cast<XResultData*>(&_data->Result[i]),
			__FILE__, 
			__LINE__));
	}

	for (size_t i = 0; i < _data->Charge.size(); i++)
	{
		float charge_at = _data->Charge[i].Using_Curve ? 0 : _data->Charge[i].At;

		AddedTimerToken(
			XCommon::SetTimer(
			charge_at * _time_scale,
			DelegateCombination(XArtsSkill, ChargeTo, this),
			reinterpret_cast<void*>(i),
			__FILE__, 
			__LINE__));

		/*{
			int n = ((int)((_time_elapsed - charge_at) * 1000) << 16) | i;
			ChargeTo(NULL, (void*)n);
		}*/
	}

	for(size_t i = 0; i < _data->Manipulation.size(); i++)
	{
		AddedTimerToken(
			XCommon::SetTimer(
			_data->Manipulation[i].At * _time_scale,
			DelegateCombination(XArtsSkill, Manipulate, this),
			reinterpret_cast<void*>(&_data->Manipulation[i]),
			__FILE__, 
			__LINE__));
	}

	for(size_t i = 0; i < _data->Warning.size(); i++)
	{
		AddedTimerToken(
			XCommon::SetTimer(
			_data->Warning[i].At * _time_scale,
			DelegateCombination(XArtsSkill, Warning, this),
			reinterpret_cast<XWarningData*>(&_data->Warning[i]),
			__FILE__, 
			__LINE__));
	}

	for(size_t i = 0; i < _data->Logical.QTEData.size(); i++)
	{
		if(_firer->QuickTimeEventComponent() && _data->Logical.QTEData[i].QTE != 0)
		{
			AddedTimerToken(
				XCommon::SetTimer(
				_data->Logical.QTEData[i].At * _time_scale,
				DelegateCombination(XArtsSkill, QTEOn, this),
				reinterpret_cast<void*>(_data->Logical.QTEData[i].QTE),
				__FILE__, __LINE__));

			AddedTimerToken(
				XCommon::SetTimer(
				_data->Logical.QTEData[i].End * _time_scale,
				DelegateCombination(XArtsSkill, QTEOff, this),
				reinterpret_cast<void*>(_data->Logical.QTEData[i].QTE),
				__FILE__, __LINE__));
		}
	}

	int PreservedStrength = _core->PreservedStrength();
	if(PreservedStrength > 0)
	{
		AddedTimerToken(
			XCommon::SetTimer(
			_data->Logical.PreservedAt * _time_scale,
			DelegateCombination(XArtsSkill, PreservedS, this),
			reinterpret_cast<void*>(PreservedStrength),
			__FILE__, __LINE__));
	}

	if(!_data->Logical.Exstring.empty())
	{
		AddedTimerToken(
			XCommon::SetTimer(
			_data->Logical.Exstring_At * _time_scale,
			DelegateCombination(XArtsSkill, ExString, this),
			reinterpret_cast<void*>(&(_data->Logical.Exstring)),
			__FILE__, __LINE__));
	}

	for(size_t i = 0; i < _data->Mobs.size(); i++)
	{
		if(_data->Mobs[i].TemplateID > 0)
		{
			AddedTimerToken(
				XCommon::SetTimer(
				_data->Mobs[i].At * _time_scale,
				DelegateCombination(XArtsSkill, Mob, this),
				reinterpret_cast<XMobUnitData*>(&_data->Mobs[i]),
				__FILE__, __LINE__));
		}
	}

	if(_data->Chain.TemplateID > 0 && !_data->Chain.Name.empty())
	{
		AddedTimerToken(
			XCommon::SetTimer(
			_data->Chain.At * _time_scale,
			DelegateCombination(XArtsSkill, CastChain, this),
			reinterpret_cast<XCastChain*>(&_data->Chain),
			__FILE__, __LINE__));
	}
}

//skill do self stoppage here
void XArtsSkill::Stop()
{
	if (_core == MainCore() && _data->Logical.QTEData.size() != 0) QTEOff(0, 0);
	
	if (_data->Manipulation.size() > 0)
	{
		XManipulationOffArgs args;
		args.DenyToken = 0;
		_firer->DispatchAction(&args);
	}

	for(size_t i = 0; i < _mob_unit.size(); i++)
	{
		Unit* unit = Unit::FindUnit(_mob_unit[i]);
		if(unit && !unit->IsDead()) 
		{
			XEntity* e = dynamic_cast<XEntity*>(unit->GetXObject());
			if(e && e->IsLifewithinMobbedSkill())
				unit->TriggerDeath();
		}
	}

	_mob_unit.clear();
}

//the point of skill to calculate hurt and other sequence.
void XArtsSkill::Result(XResultData* data)
{
	if(data->Loop)
	{
		UINT32 i = (data->Index << 16);
		LoopResults(NULL, reinterpret_cast<void*>(i));
	}
	else
	{
		if(data->Group)
		{
			UINT32 i = (data->Index << 16);
			GroupResults(NULL, reinterpret_cast<void*>(i));
		}
		else
		{
			if(data->LongAttackEffect)
			{
				Project(data);
			}
			else
			{
				XSkill::SkillResult(
					_token, 
					_firer, 
					_core, 
					data->Index, 
					MainCore()->ID(), 
					data->Token, 
					_firer->GetFace_p(), 
					_firer->GetPosition_p());
			}
		}
	}
}

//update effects per frame
bool XArtsSkill::Present(float fDeltaT)
{
	/*
	 * Do we need to enlarge skill_time due to net latency ?
	 */
	//return _time_elapsed < MainCore()->GetSoul()->Skill_Time  * _time_scale;

	return true;
}

bool XArtsSkill::Project(XResultData* data, int additionalAngle, int loop, int group)
{
	Scene* scene = _firer->GetUnit()->GetCurrScene();
	if(scene == NULL || !data->LongAttackData.Valid) return true;

	if(data->Attack_All)
	{
		const vector<Unit*>* enemies = scene->GetOpponents(_firer->GetUnit()->GetFightGroup());
		if (NULL == enemies)
		{
			return true;
		}

		for(size_t i = 0; i < (*enemies).size(); i++)
		{
			if(XEntity::ValidEntity((*enemies)[i]->GetID()))
			{
				XEntity* pEnt = dynamic_cast<XEntity*>((*enemies)[i]->GetXObject());
				if(pEnt) 
				{
					if(pEnt->MobbedBy() != 0 && !data->Mobs_Inclusived) continue;
					XBulletMgr::Instance()->ShootBullet(GenerateBullet(data, pEnt, additionalAngle, loop, group, -1, true), false);
				}
			}
		}
	}
	else if(data->Warning)
	{
		for(size_t i = 0; i < _core->WarningPosAt[data->Warning_Idx].size(); i++)
		{
			XBulletMgr::Instance()->ShootBullet(GenerateBullet(data, NULL, additionalAngle, loop, group, i, true), false);
		}
	}
	else
	{
		if(data->LongAttackData.Reinforce)
		{
			if(loop == 0 && group == 0)
			{
				const vector<Unit*>* enemies = scene->GetOpponents(_firer->GetUnit()->GetFightGroup());
				if (enemies)
				{
					for(size_t i = 0; i < (*enemies).size(); i++)
					{
						if(XEntity::ValidEntity((*enemies)[i]->GetID()))
						{
							XEntity* pEnt = dynamic_cast<XEntity*>((*enemies)[i]->GetXObject());
							if(pEnt && pEnt->BuffComponent() && pEnt->BuffComponent()->IsBuffStateOn(XBuffType_Mark))
								XBulletMgr::Instance()->ShootBullet(GenerateBullet(data, pEnt, additionalAngle, 0, 0, -1, true), false);
						}
					}
				}
			}
		}
		else
			XBulletMgr::Instance()->ShootBullet(GenerateBullet(data, Target(), additionalAngle, loop, group), false);
	}

	return true;
}

bool XArtsSkill::LoopResults(IArgs* args, void* param)
{
	UINT32 i = reinterpret_cast<intptr_t>(param);

	UINT32 count = i >> 16;
	int execute_cout = i & 0xFFFF;

	if (count >= _data->Result.size() || !_data->Result[count].Loop || execute_cout >= _data->Result[count].Loop_Count) return false;

	if(_data->Result[count].Group)
	{
		UINT32 i = (count << 16) | (execute_cout << 8) | 0;
		GroupResults(NULL, reinterpret_cast<void*>(i));
	}
	else if(_data->Result[count].LongAttackEffect)
		Project(&_data->Result[count], 0, execute_cout, 0);
	else
	{
		XSkill::SkillResult(
			_token,
			_firer,
			_core,
			count,
			MainCore()->ID(),
			_data->Result[count].Token,
			_firer->GetFace_p(),
			_firer->GetPosition_p());
	}

	/*
	 * _firer may dead in upper SkillResult call
	 * so check _firer validation here
	 */
	if (!XEntity::IsValidEntity(_firer)) return false;
	/*
	 * skill may cease in upper SkillResult call
	 */
	if (!Casting()) return false;

	execute_cout++;
	if (execute_cout < _data->Result[count].Loop_Count)
	{
		i = (count << 16) | execute_cout;
		AddedTimerToken(
			XCommon::SetTimer(
			_data->Result[count].Cycle * _time_scale,
			DelegateCombination(XArtsSkill, LoopResults, this),
			reinterpret_cast<void*>(i),
			__FILE__, __LINE__));
	}

	return true;
}

bool XArtsSkill::GroupResults(IArgs* args, void* param)
{
	UINT32 i = reinterpret_cast<intptr_t>(param);

	UINT32 count = i >> 16;

	int group_cout = i & 0x00FF;
	int loop_cout = (i & 0xFF00) >> 8;

	if(count >= _data->Result.size() || !_data->Result[count].Group || group_cout >= _data->Result[count].Group_Count) return false;

	const Vector3& face = _firer->GetFace_p();
	int angle = _data->Result[count].Deviation_Angle + _data->Result[count].Angle_Step * group_cout;
	angle = _data->Result[count].Clockwise ? angle : -angle;

	if(_data->Result[count].LongAttackEffect)
		Project(&_data->Result[count], angle, loop_cout, group_cout);
	else
		XSkill::SkillResult(
		_token,
		_firer,
		_core,
		count,
		MainCore()->ID(),
		_data->Result[count].Token,
		Vector3::HorizontalRotate(face, (float)angle),
		_firer->GetPosition_p());

	/*
	 * _firer may dead in upper SkillResult call
	 * so check _firer validation here
	 */
	if (!XEntity::IsValidEntity(_firer)) return false;
	/*
	 * skill may cease in upper SkillResult call
	 */
	if (!Casting()) return false;

	++group_cout;
	if(group_cout < _data->Result[count].Group_Count)
	{
		i = (count << 16) | (loop_cout << 8) | (group_cout);
		AddedTimerToken(
			XCommon::SetTimer(
				_data->Result[count].Time_Step * _time_scale,
				DelegateCombination(XArtsSkill, GroupResults, this),
				reinterpret_cast<void*>(i),
				__FILE__, __LINE__));
	}

	return true;
}

bool XArtsSkill::ChargeTo(IArgs*, void* param)
{
	int n = reinterpret_cast<intptr_t>(param);

	int i = n & 0xFF;
	float offset = (n >> 16) / 1000.0f;

	XChargeActionArgs args(&_data->Charge[i]);
	args.SetToken(_token);
	args.TimeGone = offset;
	args.TimeScale = _time_scale;
	args.Target = _data->Charge[i].AimTarget ? _target : 0;
	args.TimeSpan = _data->Charge[i].End - (_data->Charge[i].Using_Curve ? 0 : _data->Charge[i].At);

	if(!_data->Charge[i].Using_Curve && args.TimeSpan <= 0)
	{
		LogError("Zero Charge TimeSpan of skill %s", _data->Name.c_str());
		return false;
	}
	else
		return _firer->DispatchAction(&args);
}

bool XArtsSkill::Manipulate(IArgs*, void* param)
{
	XManipulationData* data = reinterpret_cast<XManipulationData*>(param);

	XManipulationOnArgs args(data);
	int token = (int)(args.GetToken());
	_firer->DispatchAction(&args);

	AddedTimerToken(
		XCommon::SetTimer(
		(data->End - data->At) * _time_scale,
		DelegateCombination(XArtsSkill, KillManipulate, this),
		reinterpret_cast<void*>(token),
		__FILE__, __LINE__));

	return true;
}

bool XArtsSkill::KillManipulate(IArgs*, void* param)
{
	XManipulationOffArgs args;
	args.DenyToken = reinterpret_cast<intptr_t>(param);
	_firer->DispatchAction(&args);

	return true;
}

bool XArtsSkill::QTEOn(IArgs*, void* param)
{
	if (_firer->QuickTimeEventComponent())
	{
		XSkillQTEEventArgs args;
		args.State = reinterpret_cast<intptr_t>(param);
		args.On = true;
		_firer->DispatchAction(&args);
	}

	return true;
}

bool XArtsSkill::QTEOff(IArgs*, void* param)
{
	if (_firer->QuickTimeEventComponent())
	{
		XSkillQTEEventArgs args;
		args.State = reinterpret_cast<intptr_t>(param);
		args.On = false;
		_firer->DispatchAction(&args);
	}

	return true;
}

bool XArtsSkill::Warning(IArgs*, void* param)
{
	XWarningData* data = reinterpret_cast<XWarningData*>(param);

	Scene* pScene = _firer->GetUnit()->GetCurrScene();

	std::vector<XSkillCore::XSkillWarningPackage>& list = _core->WarningPosAt[data->Index];
	list.clear();

	if(data->RandomWarningPos || data->Type == Warning_Multiple)
	{
		UINT64 extra_id = 0; 

		auto it = _core->WarningRandomItems[data->Index].begin();
		while (it != _core->WarningRandomItems[data->Index].end())
		{
			XEntity* e = (it->ID == 0) ? Target() : XEntity::ValidEntity(it->ID);

			auto is = it->Pos.begin();
			while(is != it->Pos.end())
			{
				++extra_id;
				
				if(e)
				{
					UINT32 m = *is;
					float r = (m & 0xFFFF) / 10.0f;
					UINT32 d = (m >> 16);

					//LogInfo("%lld : %d", extra_id, m);
					XSkillCore::XSkillWarningPackage pakcage;

					pakcage.WarningAt = e->GetPosition_p() + (r * Vector3::HorizontalRotate(Vector3::forward, d));
					pakcage.WarningTo = extra_id;
					if(pScene) pScene->GetGrid()->TryGetHeight(
						pakcage.WarningAt.x, 
						pakcage.WarningAt.z, 
						pakcage.WarningAt.y);

					list.push_back(pakcage);
				}

				++is;
			}

			++it;
		}
	}
	else
	{
		switch(data->Type)
		{
		case Warning_None:
			{
				XSkillCore::XSkillWarningPackage pakcage;

				const Vector3 v(data->OffsetX, data->OffsetY, data->OffsetZ);
				float magniatude = v.IsZero() ? 0 : Vector3::Magnitude(v);
				const Vector3& offset = (magniatude == 0) ? Vector3::zero : Vector3::HorizontalRotate(v, _firer->GetFaceDegree()) * magniatude;

				pakcage.WarningAt = Firer()->GetPosition_p() + offset;
				pakcage.WarningTo = 0;
				if(pScene) pScene->GetGrid()->TryGetHeight(
					pakcage.WarningAt.x, 
					pakcage.WarningAt.z, 
					pakcage.WarningAt.y);

				list.push_back(pakcage);
			}break;
		case Warning_Target:
			{
				XSkillCore::XSkillWarningPackage pakcage;

				if(HasValidTarget())
				{

					pakcage.WarningAt = Target()->GetPosition_p();
					pakcage.WarningTo = Target()->GetID();
					if(pScene) pScene->GetGrid()->TryGetHeight(
						pakcage.WarningAt.x, 
						pakcage.WarningAt.z, 
						pakcage.WarningAt.y);
				}
				else
				{
					const Vector3 v(data->OffsetX, data->OffsetY, data->OffsetZ);
					float magniatude = v.IsZero() ? 0 : Vector3::Magnitude(v);
					const Vector3& offset = (magniatude == 0) ? Vector3::zero : Vector3::HorizontalRotate(v, _firer->GetFaceDegree()) * magniatude;

					pakcage.WarningAt = Firer()->GetPosition_p() + offset;
					pakcage.WarningTo = 0;
					if(pScene) pScene->GetGrid()->TryGetHeight(
						pakcage.WarningAt.x, 
						pakcage.WarningAt.z, 
						pakcage.WarningAt.y);
				}

				list.push_back(pakcage);
			}break;
		case Warning_All:
			{
				const vector<Unit*>* enemies = pScene->GetOpponents(_firer->GetUnit()->GetFightGroup());
				if (NULL == enemies)
				{
					return true;
				}

				for(size_t i = 0; i < (*enemies).size(); i++)
				{
					XEntity* obj = dynamic_cast<XEntity*>((*enemies)[i]->GetXObject());

					if(obj)
					{
						if(obj->MobbedBy() != 0 && !data->Mobs_Inclusived) continue;

						XSkillCore::XSkillWarningPackage pakcage;
						pakcage.WarningAt = obj->GetPosition_p();
						pakcage.WarningTo = obj->GetID();
						if(pScene) pScene->GetGrid()->TryGetHeight(
							pakcage.WarningAt.x, 
							pakcage.WarningAt.z, 
							pakcage.WarningAt.y);

						list.push_back(pakcage);
					}
				}
			}break;
		case Warning_Multiple:
			break;
		}
	}

	return true;
} 

bool XArtsSkill::PreservedS(IArgs*, void* param)
{
	int strength = reinterpret_cast<intptr_t>(param);

	AttributeWatcher watcher(_firer->GetUnit());
	watcher.SetAttr(BASIC_CurrentXULI, strength);

	return true;
}

bool XArtsSkill::ExString(IArgs*, void* param)
{
	string* data = reinterpret_cast<string*>(param);

	if ((*data).find("aicommand") == string::npos)
	{
		_firer->GetUnit()->GetCurrScene()->GetSpawner()->m_oLevelState.SetExternalString(*data);
	}

	return true;
}

bool XArtsSkill::Mob(IArgs*, void* param)
{
	XMobUnitData* data = reinterpret_cast<XMobUnitData*>(param);

	Vector3 offset(data->Offset_At_X, data->Offset_At_Y, data->Offset_At_Z);

	float yRot = Vector3::Angle(Vector3::forward, _firer->GetFace_p());
	if(!Vector3::Clockwise(Vector3::forward, _firer->GetFace_p())) yRot = -yRot;

	offset = Vector3::HorizontalRotate(offset, yRot, false);

	Vector3 mobat = _firer->GetPosition_p() + offset;
	Scene* pScene = _firer->GetUnit()->GetCurrScene();
	if(pScene)
	{
		if(!pScene->GetGrid()->TryGetHeight(mobat.x, mobat.z, mobat.y) || mobat.y < 0)
		{
			mobat = _firer->GetPosition_p();
		}

		Enemy* enemy = EnemyManager::Instance()->CreateEnemyByCaller(
			_firer->GetUnit(), data->TemplateID, mobat, _firer->GetFaceDegree(), data->Shield);
	
		if(enemy)
		{
			XEntity* pent = dynamic_cast<XEntity*>(enemy->GetXObject());
			if(pent)
			{
				pent->SetMobber(_firer->GetID());
				pent->SetLifewithinMobbedSkill(data->LifewithinSkill);

				if(_firer->SkillComponent()->AddSkillMob(pent))
				{
					XSkillEffectMgr::Instance()->SetMobProperty(enemy, _firer->GetUnit(), Core()->ID());

					//log security
					if (_firer->GetUnit()->GetSecurityStatistics() && _firer->GetUnit()->GetSecurityStatistics()->_AIInfo)
						_firer->GetUnit()->GetSecurityStatistics()->_AIInfo->OnExternalCallMonster();

					_mob_unit.push_back(enemy->GetID());
				}
				else
				{
					/*
					 * enemy already be triggered death during AddSkillMob or
					 * already in skill mobs set
					 */
				}
			}
		}
	}

	return true;
}

bool XArtsSkill::CastChain(IArgs*, void* param)
{
	XCastChain* data = reinterpret_cast<XCastChain*>(param);

	std::vector<UINT64>::const_iterator it = _firer->SkillComponent()->GetSkillMobs().begin();

	while (it != _firer->SkillComponent()->GetSkillMobs().end())
	{
		XEntity* pEnt = XEntity::ValidEntity(*it);
		if(pEnt && pEnt->GetUnit()->GetTemplateID() == data->TemplateID)
		{
			XAttackActionArgs attck_arg;
			attck_arg.Target = NULL;
			attck_arg.Identify = XCommon::XHash(data->Name.c_str());
			pEnt->DispatchAction(&attck_arg);
		}

		++it;
	}

	return true;
}

XBullet* XArtsSkill::GenerateBullet(XResultData* data, XEntity* target, int additional, int loop, int group, int wid, bool extrainfo)
{
	int diviation = data->LongAttackData.FireAngle + additional;

	XBullet* pb = new XBullet(
		false,
		(((UINT64)(loop)) << 56) | (((UINT64)(group)) << 48) | (((UINT64)(GetCombinedId())) << 40) | (((UINT64)(data->Index)) << 32) | (UINT32)_token,
		new XBulletCore(
		_token,
		_firer->GetID(), 
		target, 
		_core, 
		data->Index,
		MainCore()->ID(),
		diviation,
		wid));

	if(extrainfo) pb->SetExtraID(target ? target->GetID() : _core->WarningPosAt[data->Warning_Idx][wid].WarningTo);

	return pb;
}
