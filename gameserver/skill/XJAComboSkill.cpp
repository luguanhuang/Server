#include "pch.h"
#include "XJAComboSkill.h"
#include "entity/XEntity.h"
#include "XSkillCore.h"
#include "timeutil.h"
#include "util/XCommon.h"
#include "XSkillMgr.h"
#include "entity/XRole.h"
#include "unit/role.h"

XJAComboSkill::XJAComboSkill(XEntity* firer)
	:XArtsSkill(firer)
{
	_during_ja = false;
}

XJAComboSkill::~XJAComboSkill()
{

}

//skill do self starting here
void XJAComboSkill::Start()
{
	_during_ja = false;

	JAAt();

	XArtsSkill::Start();
}

void XJAComboSkill::Stop()
{
	XArtsSkill::Stop();

	_during_ja = false;
}

UINT32 XJAComboSkill::GetNextJAIdentify()
{
	UINT32 id = 0;

	if(ValidJA())
	{
		XJAData jd = _data->Ja[0];

		bool trigger = false;
		XRole* role = dynamic_cast<XRole*>(_firer);

		if(role)
		{
			Role* unit = dynamic_cast<Role*>(role->GetUnit());

			if (unit && unit->IsClientAutoFight())
				trigger = true;
			else
			{
				UINT64 swype = role->SlotLastAttackAt(_slot_pos);
				float trigger_at = (swype - _fire_at) / 1000.0f;

				trigger = (trigger_at < jd.End * _time_scale && trigger_at > jd.At * _time_scale);

				if(!trigger)
				{
					SSDebug << "JA failed at: " << trigger_at << " " << jd.At << " " << jd.End << END;
				}
			}
		}
		else
			trigger = true;

		if(trigger)
		{
			if(role) role->ZeroSlotLastAttack(_slot_pos);
			id = XCommon::XHash(jd.Name.c_str());
		}
		else
			id = XCommon::XHash(jd.Next_Name.c_str());
	}

	return id;
}

bool XJAComboSkill::ReFire(IArgs* args, void*)
{
	//SSError << "JA Skill: " << _data->Name << END;

	UINT32 id = GetNextJAIdentify();

	if(id != 0)
	{
		XSkillCore* core = _firer->SkillManager()->GetSkill(id);

		if(core && core->CanCast(_token))
		{
			XRole* role = dynamic_cast<XRole*>(_firer);

			XAttackActionArgs attck_arg;
			attck_arg.Slot = _slot_pos;
			attck_arg.Identify = id;
			attck_arg.Target = role ? role->SlotLastAttackTarget() : _target;
			if(role) role->ZeroSlotLastTarget();
			_firer->DispatchAction(&attck_arg);
		}
		else
		{
			_during_ja = false;
			if(_data) SSDebug << "JA Failed : " << _data->Name << END;
		}
	}
	else
	{
		_during_ja = false;
		if(_data) SSDebug << "JA Failed : " << _data->Name << END;
	}

	return true;
}

void XJAComboSkill::JAAt()
{
	if(_data->Ja.size() == 1)
	{
		//SSError << "Set JA Skill: " << _data->Name << " At " << _data->Ja[i].JA_Point << END;

		AddedTimerToken(
			XCommon::SetTimer(
			_data->Ja[0].Point * _time_scale,
			DelegateCombination(XJAComboSkill, ReFire, this), NULL, NULL, __FILE__, __LINE__));

		_during_ja = true;
	}
}

bool XJAComboSkill::ValidJA()
{
	return true;
}