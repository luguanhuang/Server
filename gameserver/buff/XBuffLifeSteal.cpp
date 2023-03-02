#include "pch.h"
#include "XBuffLifeSteal.h"
#include "entity/XEntity.h"
#include "unit/unit.h"
#include "foreach.h"
#include "component/XBuffComponent.h"
#include "gamelogic/XCombat.h"
#include "unit/attributewatcher.h"
#include "XBuff.h"

bool XBuffLifeSteal::TryCreate(const BuffTable::RowData *pBuffInfo, XBuff *pBuff)
{
	if (pBuffInfo->LifeSteal[0] == 0)
		return false;

	pBuff->AddBuffEffect(new XBuffLifeSteal(pBuff, pBuffInfo->LifeSteal));
	return true;
}

XBuffLifeSteal::XBuffLifeSteal(XBuff* pBuff, const Sequence<float, 2>& data)
	: m_Ratio(data[0])
	, m_UpperBoundRatio(data[1])
	, m_StealUpperBound(0)
	, m_pBuff(pBuff)
{
}

void XBuffLifeSteal::OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper)
{
	if (m_UpperBoundRatio > 0 && pEntity != NULL)
	{
		m_StealUpperBound = pEntity->GetUnit()->GetAttr(TOTAL_MaxHP) * m_UpperBoundRatio;
	}
}

bool XBuffLifeSteal::OnCastDamage(const HurtInfo &hurtInfo, ProjectDamageResult &result)
{
	///> 先屏蔽非技能攻击
	if (hurtInfo.m_skillId == 0)
		return true;

	if (m_pBuff->GetRelevantSkills() != NULL && !m_pBuff->GetRelevantSkills()->empty())
	{
		///> 指定技能才吸血
		if (m_pBuff->GetRelevantSkills()->count(hurtInfo.m_skillId) == 0)
			return true;
	}

	if(result.m_Accept)
	{
		double hp = result.GetValue() * m_Ratio;
		if (m_UpperBoundRatio > 0)
		{
			hp = std::min(hp, m_StealUpperBound);
			m_StealUpperBound -= hp;
		}
		if(hp > 0)
		{
			AttributeWatcher watcher(m_pBuff->GetEntity()->GetUnit());
			watcher.AddAttr(BASIC_CurrentHP, hp);
			m_pBuff->GetEntity()->GetUnit()->CheckAttrState();
			watcher.CheckChangedAttributeAndNotify(true, !m_pBuff->GetBuffInfo()->DontShowText);
		}
	}

	return true;
}
