#include "pch.h"
#include "XCombinedSkill.h"
#include "entity/XEntity.h"
#include "skill/XSkillMgr.h"
#include "component/XSkillComponent.h"
#include "XSkillCore.h"
#include "timeutil.h"
#include "util/XCommon.h"
#include "XSkillMgr.h"
#include "LogInit.h"

XCombinedSkill::XCombinedSkill(XEntity* firer)
	:XArtsSkill(firer), _result_time(0), _combined_id(0), _combined_token(0), _shut_down(false)
{

}

XCombinedSkill::~XCombinedSkill()
{

}

void XCombinedSkill::FireEvents()
{
	float offset = MainCore()->GetSoul()->Combined[_combined_id].At;

	for (size_t i = 0; i < _data->Result.size(); i++)
	{
		_data->Result[i].Token = _result_time++;

		if (_data->Result[i].At >= offset)
		{
			float delay = (_data->Result[i].At - offset);

			AddedTimerToken(
				XCommon::SetTimer(
				delay * _time_scale,
				DelegateCombination(XSkill, OnResult, this),
				reinterpret_cast<XResultData*>(&_data->Result[i]),
				__FILE__, __LINE__));
		}
	}

	for (size_t i = 0; i < _data->Charge.size(); i++)
	{
		float charge_at = _data->Charge[i].Using_Curve ? 0 : _data->Charge[i].At;

		if(charge_at >= offset)
		{
			AddedTimerToken(
				XCommon::SetTimer(
				(charge_at - offset) * _time_scale,
				DelegateCombination(XArtsSkill, ChargeTo, this),
				reinterpret_cast<void*>(i),
				__FILE__, __LINE__));
		}
		else
		{
			int n = ((int)((offset - charge_at) * 1000) << 16) | i;
			ChargeTo(NULL, (void*)n);
		}
	}

	for(size_t i = 0; i < _data->Manipulation.size(); i++)
	{
		AddedTimerToken(
			XCommon::SetTimer(
			(_data->Manipulation[i].At - offset) * _time_scale,
			DelegateCombination(XArtsSkill, Manipulate, this),
			reinterpret_cast<void*>(&_data->Manipulation[i]),
			__FILE__, __LINE__));
	}

	for(size_t i = 0; i < _data->Warning.size(); i++)
	{
		AddedTimerToken(
			XCommon::SetTimer(
			(_data->Warning[i].At - offset) * _time_scale,
			DelegateCombination(XArtsSkill, Warning, this),
			reinterpret_cast<XWarningData*>(&_data->Warning[i]),
			__FILE__, __LINE__));
	}

	for(size_t i = 0; i < _data->Mobs.size(); i++)
	{
		if(_data->Mobs[i].TemplateID > 0)
		{
			AddedTimerToken(
				XCommon::SetTimer(
				(_data->Mobs[i].At - offset) * _time_scale,
				DelegateCombination(XArtsSkill, Mob, this),
				reinterpret_cast<XMobUnitData*>(&_data->Mobs[i]),
				__FILE__, __LINE__));
		}
	}
}

void XCombinedSkill::ShutDown()
{
	_shut_down = true;
}

bool XCombinedSkill::Launch(XSkillCore* next)
{
	if(next->GetSoul()->Combined.size() > 0)
	{
		_main_core = next;

		if(Combined(NULL, 0))
		{
			MainCore()->Execute(this);
			MainCoreStart();

			return true;
		}
	}
	
	return false;
}

void XCombinedSkill::MainCoreStart()
{
	_result_time = 0;
	_shut_down = false;

	for(size_t i = 0; i < MainCore()->GetSoul()->Logical.QTEData.size(); i++)
	{
		if(_firer->QuickTimeEventComponent() && MainCore()->GetSoul()->Logical.QTEData[i].QTE != 0)
		{
			AddedCombinedTimerToken(
				XCommon::SetTimer(
				MainCore()->GetSoul()->Logical.QTEData[i].At * _time_scale,
				DelegateCombination(XArtsSkill, QTEOn, this),
				reinterpret_cast<void*>(MainCore()->GetSoul()->Logical.QTEData[i].QTE),
				__FILE__, __LINE__));

			AddedCombinedTimerToken(
				XCommon::SetTimer(
				MainCore()->GetSoul()->Logical.QTEData[i].End * _time_scale,
				DelegateCombination(XArtsSkill, QTEOff, this),
				reinterpret_cast<void*>(MainCore()->GetSoul()->Logical.QTEData[i].QTE),
				__FILE__, __LINE__));
		}
	}

	int PreservedStrength = MainCore()->PreservedStrength();
	if(PreservedStrength > 0)
	{
		AddedCombinedTimerToken(
			XCommon::SetTimer(
			MainCore()->GetSoul()->Logical.PreservedAt * _time_scale,
			DelegateCombination(XArtsSkill, PreservedS, this),
			reinterpret_cast<void*>(PreservedStrength),
			__FILE__, __LINE__));
	}

	if(!MainCore()->GetSoul()->Logical.Exstring.empty())
	{
		AddedCombinedTimerToken(
			XCommon::SetTimer(
			MainCore()->GetSoul()->Logical.Exstring_At * _time_scale,
			DelegateCombination(XArtsSkill, ExString, this),
			reinterpret_cast<void*>(&(MainCore()->GetSoul()->Logical.Exstring)),
			__FILE__, __LINE__));
	}
}

bool XCombinedSkill::Combined(IArgs* args, void* param)
{
	_combined_id = (int)(UINT64)param;

	XSkillCore* pCore = _firer->SkillComponent()->SkillManager()->GetSkill(XCommon::XHash(MainCore()->GetSoul()->Combined[_combined_id].Name.c_str()));

	if(pCore)
	{
		if(_combined_id > 0)
		{
			XEntity* target = Target();

			Puppetize();

			_target = target ? target->GetID() : 0;
		}

		//update core & data first for Execute()
		_core = pCore;
		_data = _core->GetSoul();

		if(_combined_id > 0)
		{
			if(_stop_method) _stop_method(this, 0);

			if(_shut_down)
			{
				Cease();
				return true;
			}
			else
				Execute();
		}

		if(_combined_id + 1 < MainCore()->GetSoul()->Combined.size())
		{
			_combined_token = XCommon::SetTimer(
				(MainCore()->GetSoul()->Combined[_combined_id].End - MainCore()->GetSoul()->Combined[_combined_id].At) * _time_scale,
				DelegateCombination(XCombinedSkill, Combined, this),
				reinterpret_cast<void*>(_combined_id + 1),
				__FILE__, __LINE__);
		}

		return true;
	}
	else
		LogError("Missing combined skill %s\n", MainCore()->GetSoul()->Combined[_combined_id].Name.c_str());

	return false;
}

void XCombinedSkill::CombinedStop()
{
	CTimerMgr::Instance()->KillTimer(_combined_token);
	if (MainCore()->GetSoul()->Logical.QTEData.size() != 0) QTEOff(0, 0);
}

void XCombinedSkill::CombinedKillTimerAll()
{
	vector<HTIMER>::const_iterator it = _combined_actionToken.begin();
	while (it != _combined_actionToken.end())
	{
		CTimerMgr::Instance()->KillTimer(*it);
		++it;
	}

	_combined_actionToken.clear();
}
