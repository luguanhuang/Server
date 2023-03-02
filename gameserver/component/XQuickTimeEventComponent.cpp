#include "pch.h"
#include "XQuickTimeEventComponent.h"
#include "skill/XSkill.h"
#include "skill/XSkillCore.h"
#include "timeutil.h"
#include "scene/scene.h"
#include "entity/XEntity.h"
#include "skill/XSkillMgr.h"
#include "unit/unit.h"
#include "gamelogic/skilldefine.h"
#include "SkillList.h"
#include "table/XSkillEffectMgr.h"

XQuickTimeEventComponent::XQuickTimeEventComponent(void)
	:_state(0)
{

}

XQuickTimeEventComponent::~XQuickTimeEventComponent(void)
{

}

void XQuickTimeEventComponent::ActionSubscribe()
{
	RegisterAction(XAction_QTE, DelegateCombination(XQuickTimeEventComponent, OnQuickTimeAction, this));
	RegisterAction(XAction_OnSkillFired, DelegateCombination(XQuickTimeEventComponent, OnSkillFired, this));
}

bool XQuickTimeEventComponent::OnQuickTimeAction(IArgs* pargs, void *)
{
	XSkillQTEEventArgs *args = dynamic_cast<XSkillQTEEventArgs*>(pargs);
	
	if(args->State == 0 || args->State > MAXQTE)
	{
		_qte_skills.clear();
		_state = 0;
	}
	else
	{
		if(args->On)
		{
			_state |= ((UINT64)1 << args->State);

			unordered_map<UINT32, vector<UINT32> >::const_iterator it = GetEntity()->SkillManager()->GetQTE().find(args->State);
			if(it != GetEntity()->SkillManager()->GetQTE().end())
			{
				for (size_t i = 0; i < it->second.size(); i++)
				{
					uint qte = it->second[i];
					//check ex-skill for qte
					uint level = GetEntity()->GetUnit()->GetSkillLevelMgr()->GetSkillLevel(qte);
					SkillList::RowData* data = XSkillEffectMgr::Instance()->GetSkillConfig(qte, level);
					if(data)
					{
						uint ex = XCommon::XHash(data->ExSkillScript.c_str());
						if(GetEntity()->SkillManager()->GetSkill(ex))
							qte = ex;
					}

					if(GetEntity()->SkillManager()->GetSkill(qte))
					{
						if (_qte_skills.find(qte) == _qte_skills.end())
						{
							_qte_skills.insert(qte);
						}
					}
				}

				XOnQTEOnEventArgs qteargs;
				qteargs.State = args->State;
				GetEntity()->DispatchAction(&qteargs);
			}
		}
		else
		{
			_state &= (~((UINT64)1 << args->State));

			unordered_map<UINT32, vector<UINT32> >::const_iterator it = GetEntity()->SkillManager()->GetQTE().find(args->State);
			if(it != GetEntity()->SkillManager()->GetQTE().end())
			{
				for (size_t i = 0; i < it->second.size(); i++)
				{
					uint qte = it->second[i];
					if(GetEntity()->SkillManager()->GetSkill(qte))
					{
						auto innerit = _qte_skills.find(qte);
						if (innerit != _qte_skills.end())
						{
							_qte_skills.erase(innerit);
						}

						//check ex-skill for qte
						uint level = GetEntity()->GetUnit()->GetSkillLevelMgr()->GetSkillLevel(qte);
						SkillList::RowData* data = XSkillEffectMgr::Instance()->GetSkillConfig(qte, level);
						if(data)
						{
							uint ex = XCommon::XHash(data->ExSkillScript.c_str());
							innerit = _qte_skills.find(ex);
							if (innerit != _qte_skills.end())
							{
								_qte_skills.erase(innerit);
							}
						}
					}
				}
			}
		}
	}

	return true;
}

bool XQuickTimeEventComponent::OnSkillFired(IArgs* pargs, void *)
{
	XSkillFiredEventArgs *args = dynamic_cast<XSkillFiredEventArgs*>(pargs);

	_qte_skills.erase(args->Id);

	return true;
}

bool XQuickTimeEventComponent::IsInState(UINT32 state)
{
	if (state == 0) return !IsInAnyState();
	if (state > MAXQTE) return false;

	return (_state & ((UINT64)1 << state)) != 0;
}

bool XQuickTimeEventComponent::IsInCanDashState()
{
	return IsInState(20);
}
