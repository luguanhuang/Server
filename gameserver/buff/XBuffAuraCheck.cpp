#include "pch.h"
#include "XBuffAuraCheck.h"
#include "entity/XEntity.h"
#include "scene/scenefinder.h"
#include "unit/enemymanager.h"
#include "unit/rolemanager.h"
#include "scene/scene.h"
#include "unit/unit.h"
#include "xdelegate.hpp"
#include "util/XCommon.h"
#include "gamelogic/combateffect.h"
#include <cmath>

bool XBuffAuraCheck::TryCreate(const BuffTable::RowData *pBuffInfo, XBuff *pBuff)
{
	if (pBuffInfo->AuraAddBuffID.empty())
		return false;

	pBuff->AddBuffEffect(new XBuffAuraCheck(pBuff));
	return true;
}

XBuffAuraCheck::XBuffAuraCheck(XBuff *pBuff)
{
	m_pBuff = pBuff;
	m_UnitID = 0;
	m_handler = INVALID_HTIMER;
	m_effectexcept = false;
	m_effectexceptself = false;

	m_shapeType = (ShapeType)(int)_GetParam(0);
	m_startDelay = _GetParam(1);
	m_interval = std::max(_GetParam(2), GetGlobalConfig().BuffMinAuraInterval);

	for (auto i = pBuff->GetBuffInfo()->EffectGroup.begin(); i != pBuff->GetBuffInfo()->EffectGroup.end(); ++i)
	{
		if ((*i) == -1)
		{
			m_effectexcept = true;
		}
		else if ((*i) == -2)
		{
			m_effectexcept = true;
			m_effectexceptself = true;
		}
		else 
		{
			m_fightgroups.insert((*i));
		}
	}
}

XBuffAuraCheck::~XBuffAuraCheck()
{
	_KillTimer();
}

void XBuffAuraCheck::OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper)
{
	_KillTimer();

	switch(m_shapeType)
	{
	case BACT_CIRCLE:
		{
			float radius = _GetParam(3);
			radius += pEffectHelper->GetBuffAuraRadius();

			m_sqrRadius = radius * radius;

			float exclusiveRadius = _GetParam(4);
			m_sqrexclusiveRadius = exclusiveRadius * exclusiveRadius;
		}
		break;
	case BACT_RECTANGLE:
		{
			m_halfWidth = _GetParam(3) / 2;
			m_halfHeight = _GetParam(4) / 2;
			m_halfExclusiveWidth = _GetParam(5) / 2;
			m_halfExclusiveHeight = _GetParam(6) / 2;
		}
		break;
	default:
		break;
	}

	m_UnitID = pEntity->GetID();
	if(SecondsToMillionSec(m_startDelay) == 0)
		OnTimer(NULL, NULL);
	else
		m_handler = XCommon::SetTimer(m_startDelay, DelegateCombination(XBuffAuraCheck, OnTimer, this), __FILE__, __LINE__);
}

void XBuffAuraCheck::OnRemove(XBuffRemoveReason reason)
{
	_KillTimer();
}

bool XBuffAuraCheck::OnTimer(IArgs*, void*)
{
	Unit *pUnit = Unit::FindUnit(m_UnitID);

	if (pUnit && pUnit->GetCurrScene())
	{
		XAddBuffActionArgs args;
		for(auto it = m_pBuff->GetBuffInfo()->AuraAddBuffID.begin(); it != m_pBuff->GetBuffInfo()->AuraAddBuffID.end(); ++it)
		{
			BuffDesc desc;
			desc.buffID = (*it)[0];
			desc.BuffLevel = (*it)[1];
			desc.skillID = m_pBuff->GetSkillID();
			desc.type = m_pBuff->GetType();
			args.vecBuffs.push_back(desc);
		}
		//args.Caster = m_UnitID;
		args.Caster = m_pBuff->GetCaster();

		if (m_effectexcept)
		{
			if (m_effectexceptself)
			{
				m_fightgroups.insert(pUnit->GetFightGroup());
			}
		}
		else
		{
			if (m_fightgroups.empty())
			{
				m_fightgroups.insert(pUnit->GetFightGroup());
			}
		}
		std::vector<Unit *> units;
		pUnit->GetCurrScene()->GetSameLineUnits(pUnit, units);

		const Vector3& pPos = pUnit->GetXObject()->GetPosition_p();

		const Vector3& unitface = pUnit->GetXObject()->GetFace_p();
		const Vector3& rightdir = Vector3::HorizontalRotate(unitface, 90);

		for(auto i = units.begin(); i != units.end(); ++i)
		{
			Unit* pOther = *i;
			if (pOther->IsDead())
			{
				continue;
			}

			if (!_CanAttack(pOther->GetFightGroup()))
			{
				continue;
			}

			switch(m_shapeType)
			{
			case BACT_CIRCLE:
				{
					if(m_sqrRadius > 0 || m_sqrexclusiveRadius > 0)
					{
						float dis = Vector3::sqrtMagnitude(pPos - pOther->GetXObject()->GetPosition_p());
						if (m_sqrRadius > 0 && dis > m_sqrRadius)
							continue;
						if (m_sqrexclusiveRadius > 0 && dis < m_sqrexclusiveRadius)
							continue;
					}
				}
				break;
			case BACT_RECTANGLE:
				{
					Vector3 oppo2this = pOther->GetXObject()->GetPosition_p() - pPos;
					if (m_halfWidth > 0.0f || m_halfExclusiveWidth > 0.0f)
					{
						float dotres = std::abs(Vector3::Dot(oppo2this, unitface));

						if (m_halfWidth > 0.0f && dotres > m_halfWidth)
							continue;
						if (m_halfExclusiveWidth > 0.0f && dotres < m_halfExclusiveWidth)
							continue;
					}

					if (m_halfHeight > 0.0f || m_halfExclusiveHeight > 0.0f)
					{
						float res = std::abs(Vector3::Dot(oppo2this, rightdir));

						if (m_halfHeight > 0.0f && res > m_halfHeight)
							continue;
						if (m_halfExclusiveHeight > 0.0f && res < m_halfExclusiveHeight)
							continue;
					}

				}
				break;
			}

			pOther->GetXObject()->DispatchAction(&args);
		}

		//if (m_radius <= 0)
		//{
		//	FindInGroupCondition cond(m_fightgroups);
		//	pUnit->GetCurrScene()->GetUnitByCondition(cond, units);
		//}
		//else
		//{
		//	FindInGroupInRangeCondition cond(pUnit->GetXObject()->GetPosition_p(), m_radius, m_fightgroups);
		//	pUnit->GetCurrScene()->GetUnitByCondition(cond, units);
		//}
		//
		//for (size_t i = 0; i < units.size(); ++i)
		//{
		//	if (units[i]->IsDead())
		//	{
		//		continue;
		//	}

		//	units[i]->GetXObject()->DispatchAction(&args);
		//}
		if(GetValid())
			m_handler = XCommon::SetTimer(m_interval, DelegateCombination(XBuffAuraCheck, OnTimer, this), __FILE__, __LINE__);
	}
	
	return true;
}

float XBuffAuraCheck::_GetParam(int index)
{
	if(m_pBuff->GetBuffInfo()->AuraParams.size() <= (size_t)index)
		return 0.0f;
	return m_pBuff->GetBuffInfo()->AuraParams[index];
}

void XBuffAuraCheck::_KillTimer()
{
	if (m_handler != INVALID_HTIMER)
	{
		CTimerMgr::Instance()->KillTimer(m_handler);
		m_handler = INVALID_HTIMER;
	}
}

bool XBuffAuraCheck::_CanAttack(UINT32 fightgroup)
{
	if (m_effectexcept)
	{
		return m_fightgroups.count(fightgroup) == 0;
	}
	else
	{
		return m_fightgroups.count(fightgroup) != 0;
	}
}