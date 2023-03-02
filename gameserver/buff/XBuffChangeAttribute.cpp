#include "pch.h"
#include <cmath>
#include "XBuffChangeAttribute.h"
#include "component/XComponent.h"
#include "unit/unit.h"
#include "entity/XEntity.h"
#include "unit/combatattribute.h"
#include "unit/attributewatcher.h"
#include "foreach.h"
#include "XBuffSpecialState.h"
#include "gamelogic/killerwatcher.h"
#include "gamelogic/XCombat.h"
#include "scene/securitybuffstatistics.h"
#include "unit/role.h"
#include "table/ProfessionMgr.h"
#include "gamelogic/combateffect.h"


bool XBuffChangeAttribute::TryCreate(CombatEffectHelper* pEffectHeler, XBuff *pBuff)
{
	if (pEffectHeler->GetBuffData()->BuffChangeAttribute.empty() && !pEffectHeler->HasEffect(CET_Buff_ChangeAttribute))
		return false;

	pBuff->AddBuffEffect(new XBuffChangeAttribute(pBuff));
	return true;
}

XBuffChangeAttribute::XBuffChangeAttribute(XBuff *pBuff)
{
	m_pBuff = pBuff;
}

XBuffChangeAttribute::~XBuffChangeAttribute()
{
}

void XBuffChangeAttribute::_Convert(Unit* pUnit, CombatAttrDef attrDef, double deltaValue)
{
	if (pUnit == NULL)
		return;

	///> 玩家才有职业
	if (!pUnit->IsRole())
		return;

	Role* pRole = static_cast<Role*>(pUnit);
	int prof = pRole->GetProfession();

	{
		if (!AttributeDefInfo::IsFirstLevelAttr(attrDef))
			return;

		CombatAttrDef basicAttrKey = AttributeDefInfo::GetBasicAttr(attrDef);

		///> 二级属性列表
		auto pCoeffs = ProfessionMgr::Instance()->GetProfessionAttrConvertCoefficients(prof, basicAttrKey);
		if (pCoeffs == NULL)
			return;

		///> 如果是变化百分比，则需要拿Basic属性
		if (AttributeDefInfo::IsPercentAttr(attrDef))
			deltaValue *= (0.01 * pUnit->GetAttr(basicAttrKey));

		if (std::abs(deltaValue) < 0.000001)
			return;

		for (auto it = pCoeffs->cbegin(); it != pCoeffs->cend(); ++it)
		{
			CombatAttrDef level2ndAttr = (CombatAttrDef)it->first;
			double level2ndValue = it->second * deltaValue;

			CombatAttrDef level2ndAttr2 = AttributeDefInfo::GetAttrCurAttr(level2ndAttr);

			///> 改血蓝上限 也要改当前血蓝。并且要根据是增加还是减少，决定改变当前血蓝在前在后
			if (level2ndAttr2 != Invalid)
			{
				if (deltaValue > 0)
				{
					m_AdditionalAttrs.push_back(AttrPair(level2ndAttr, level2ndValue));
					m_AdditionalAttrs.push_back(AttrPair(level2ndAttr2, level2ndValue));
				}
				else
				{
					m_AdditionalAttrs.push_back(AttrPair(level2ndAttr2, level2ndValue));
					m_AdditionalAttrs.push_back(AttrPair(level2ndAttr, level2ndValue));
				}
			}
			else
			{
				m_AdditionalAttrs.push_back(AttrPair(level2ndAttr, level2ndValue));
			}
		}
	}
}

void XBuffChangeAttribute::OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper)
{
	Unit *pUnit = pEntity->GetUnit();
	auto *Attrs = &m_pBuff->GetBuffInfo()->BuffChangeAttribute;
	if (pEffectHelper->HasEffect(CET_Buff_ChangeAttribute))
	{
		m_bOnlyFromTable = false;
		foreach(it in m_pBuff->GetBuffInfo()->BuffChangeAttribute)
		{
			m_OriginalAttrs.push_back(*it);
		}
		pEffectHelper->GetBuffChangeAttribute(m_OriginalAttrs);
		Attrs = &m_OriginalAttrs;
	}
	else
	{
		m_bOnlyFromTable = true;
	}
	AttributeWatcher watcher(pUnit);
	int index = 0;
	m_bChanging = true;
	foreach(i in (*Attrs))
	{
		CombatAttrDef attrKey = (CombatAttrDef)(int)(*i)[0];
		double attrValue = (*i)[1];
		if ((*i)[2] != 0)
		{
			CombatAttrDef convertFromAttr = (CombatAttrDef)(int)(*i)[2];
			attrValue = attrValue * pUnit->GetAttr(convertFromAttr);
			m_ConvertorDeltaValue[index] = attrValue;
			m_SetConvertor.insert(convertFromAttr);
		}

		ChangeAttr(pUnit, attrKey, attrValue, watcher);

		_Convert(pUnit, attrKey, attrValue);
		++index;
	}

	///> 原始数据处理后 冒出来的新属性，再添加一轮
	for(auto it = m_AdditionalAttrs.begin(); it != m_AdditionalAttrs.end(); ++it)
	{
		ChangeAttr(pUnit, it->first, it->second, watcher);
	}

	pUnit->CheckAttrState();
	watcher.CheckChangedAttributeAndNotify(true);
	m_bChanging = false;
}

void XBuffChangeAttribute::OnRemove(XBuffRemoveReason reason)
{
	Unit *pUnit = m_pBuff->GetEntity()->GetUnit();
	auto *Attrs = m_bOnlyFromTable ? &m_pBuff->GetBuffInfo()->BuffChangeAttribute : &m_OriginalAttrs;
	AttributeWatcher watcher(pUnit);

	m_bChanging = true;
	int index = Attrs->size() - 1;

	XSecurityBuffStatistics *pBuffStatistics = XSecurityBuffStatistics::TryGetStatistics(pUnit);

	for(auto it = m_AdditionalAttrs.rbegin(); it != m_AdditionalAttrs.rend(); ++it)
	{
		if (pBuffStatistics != NULL)
			pBuffStatistics->OnAttributeChanged(pUnit, m_pBuff, it->first, it->second);
		ChangeAttr(pUnit, it->first, -it->second, watcher);
	}

	for(auto i = Attrs->rbegin(); i != Attrs->rend(); ++i)
	{
		CombatAttrDef attrKey = (CombatAttrDef)(int)(*i)[0];
		double attrValue = -(*i)[1];
		if ((*i)[2] != 0 && m_ConvertorDeltaValue.count(index) > 0)
		{
			attrValue = -m_ConvertorDeltaValue[index];
		}

		if (pBuffStatistics != NULL)
			pBuffStatistics->OnAttributeChanged(pUnit, m_pBuff, attrKey, -attrValue);
		ChangeAttr(pUnit, attrKey, attrValue, watcher);
		--index;

	}

	pUnit->CheckAttrState();
	watcher.CheckChangedAttributeAndNotify(true);
	m_bChanging = false;
}

void XBuffChangeAttribute::ChangeAttr(Unit* pUnit, CombatAttrDef attrKey, double attrValue, AttributeWatcher& attrWatcher)
{
	PercentWatcher percentWatcher(pUnit, attrKey, attrValue / 100, attrWatcher);
	//if(percentWatcher.IsValid())
	//{
	//	attrWatcher.AddWatchedAttribute(attrKey);

	//	attrKey = percentWatcher.GetBasicAttr();
	//	attrValue = percentWatcher.GetTargetDelta();
	//}

	double actualValue = XCombat::CheckChangeHPLimit(attrKey, attrValue, m_pBuff->GetEntity(), true, true);
	if (actualValue == 0.0)
		return;

	if(attrKey == BASIC_CurrentHP)
	{
		KillerWatchParam killerParam(pUnit);
		killerParam.KillerID = m_pBuff->GetCaster();
		killerParam.KillerBuff = m_pBuff->GetID();
		KillerWatcher::CheckKiller(killerParam, actualValue);
	}

	attrWatcher.AddAttr(attrKey, actualValue);
	percentWatcher.Check();
}

void XBuffChangeAttribute::OnAttributeChanged(CombatAttrDef attr)
{
	if (m_bChanging)
		return;

	if (m_SetConvertor.count(attr) == 0 || m_ConvertorDeltaValue.size() == 0)
		return;

	m_bChanging = true;

	Unit *pUnit = m_pBuff->GetEntity()->GetUnit();
	auto *Attrs = m_bOnlyFromTable ? &m_pBuff->GetBuffInfo()->BuffChangeAttribute : &m_OriginalAttrs;
	AttributeWatcher watcher(pUnit);
	int index = 0;
	for(auto it = Attrs->begin(); it != Attrs->end(); ++it)
	{
		if ((*it)[2] == 0)
			continue;
		CombatAttrDef convertFromAttr = (CombatAttrDef)(int)(*it)[2];
		if (convertFromAttr != attr)
			continue;
		if (m_ConvertorDeltaValue.count(index) == 0)
			continue;

		CombatAttrDef attrKey = (CombatAttrDef)(int)(*it)[0];
		double attrValue = (*it)[1];
		attrValue = attrValue * pUnit->GetAttr(convertFromAttr);
		double deltaValue = attrValue - m_ConvertorDeltaValue[index];
		m_ConvertorDeltaValue[index] += deltaValue;

		ChangeAttr(pUnit, attrKey, deltaValue, watcher);
		++index;
	}

	pUnit->CheckAttrState();
	watcher.CheckChangedAttributeAndNotify(true);
	m_bChanging = false;
}

void XBuffChangeAttribute::OnBattleEnd()
{
	Unit *pUnit = m_pBuff->GetEntity()->GetUnit();
	auto *Attrs = m_bOnlyFromTable ? &m_pBuff->GetBuffInfo()->BuffChangeAttribute : &m_OriginalAttrs;

	int index = Attrs->size() - 1;

	XSecurityBuffStatistics *pBuffStatistics = XSecurityBuffStatistics::TryGetStatistics(pUnit);

	for(auto it = m_AdditionalAttrs.rbegin(); it != m_AdditionalAttrs.rend(); ++it)
	{
		if (pBuffStatistics != NULL)
			pBuffStatistics->OnAttributeChanged(pUnit, m_pBuff, it->first, it->second);
	}

	for(auto i = Attrs->rbegin(); i != Attrs->rend(); ++i)
	{
		CombatAttrDef attrKey = (CombatAttrDef)(int)(*i)[0];
		double attrValue = -(*i)[1];
		if ((*i)[2] != 0 && m_ConvertorDeltaValue.count(index) > 0)
		{
			attrValue = -m_ConvertorDeltaValue[index];
		}

		if (pBuffStatistics != NULL)
			pBuffStatistics->OnAttributeChanged(pUnit, m_pBuff, attrKey, -attrValue);
		--index;
	}
}

PercentWatcher::PercentWatcher(Unit* pUnit, CombatAttrDef attr, double targetDeltaPercent, AttributeWatcher& watcher)
{
	m_bValid = false;
	if (attr == PERCENT_MaxHP || attr == PERCENT_MaxMP || attr == PERCENT_MaxSuperArmor)
	{
		m_BasicAttr = AttributeDefInfo::GetAttrCurAttr(attr);
		if (m_BasicAttr == Invalid)
			return;

		m_bValid = true;
		m_pUnit = pUnit;
		m_Watcher = &watcher;
		//m_PercentAttr = attr;

		double curPercent = m_pUnit->GetAttr(attr);
		if(curPercent + targetDeltaPercent <= -1.0)
			targetDeltaPercent = (-1.0 - curPercent + 0.001);

		m_TargetDeltaPercent = targetDeltaPercent;

		double curRatio = std::max(0.001, 1.0 + curPercent);

		double oldBasic = m_pUnit->GetAttr(m_BasicAttr);
		m_TargetDeltaBasic = oldBasic / curRatio * m_TargetDeltaPercent;
		if (m_BasicAttr == BASIC_CurrentHP)
			m_TargetDeltaBasic = XCombat::CheckChangeHPLimit(m_BasicAttr, m_TargetDeltaBasic, pUnit->GetXObject(), true, true);

		if (m_TargetDeltaPercent < 0)
			_ChangeBasic();
	}
}

void PercentWatcher::Check()
{
	if (!m_bValid)
		return;

	if (m_TargetDeltaPercent > 0)
		_ChangeBasic();
	//double curBasic = m_pUnit->GetAttr(m_BasicAttr);
	//double curDeltaBasic = curBasic - m_OldBasic;
	//double curDeltaPercent = m_TargetDeltaBasic == 0 ? m_TargetDeltaPercent : m_TargetDeltaPercent * curDeltaBasic / m_TargetDeltaBasic;

	//m_pUnit->AddAttr(m_PercentAttr, curDeltaPercent * 100);
}

void PercentWatcher::_ChangeBasic()
{
	m_Watcher->AddAttr(m_BasicAttr, m_TargetDeltaBasic);
}