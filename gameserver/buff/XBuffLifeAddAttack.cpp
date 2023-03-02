#include "pch.h"
#include "XBuffLifeAddAttack.h"
#include "unit/unit.h"
#include "entity/XEntity.h"
#include "gamelogic/XCombat.h"

bool XBuffLifeAddAttack::TryCreate(const BuffTable::RowData *pBuffInfo, XBuff *pBuff)
{
	//if (pBuffInfo->LifeAddAttack[1] == 0)
	//	return false;

	if (pBuffInfo->SelfLifeAddAttack.empty())
		return false;
	pBuff->AddBuffEffect(new XBuffLifeAddAttack(pBuffInfo));
	return true;
}

XBuffLifeAddAttack::XBuffLifeAddAttack(const BuffTable::RowData *pBuffInfo)
{
	//m_Entity = NULL;
	//m_IsEffect = false;
	//m_HPTriggerPercent = pBuff->GetBuffInfo()->LifeAddAttack[0];
	//m_AttrID = (CombatAttrDef)(int)pBuff->GetBuffInfo()->LifeAddAttack[1];
	//m_AttrValue = pBuff->GetBuffInfo()->LifeAddAttack[2];

	//m_pBuff = pBuff;
	m_Data.SetRange(0, 0, 1, 0);
	m_Data.Init(pBuffInfo->SelfLifeAddAttack);
}

XBuffLifeAddAttack::~XBuffLifeAddAttack()
{

}

bool XBuffLifeAddAttack::OnCastDamage(const HurtInfo &hurtInfo, ProjectDamageResult &result)
{
	double hp = m_Entity->GetUnit()->GetPreciseHpPercent() / 100;
	result.ScaleValue(1.0 + m_Data.GetData(hp));
	//LogInfo("reduce damage from %d -> %d", oldvalue, (int)result.m_value);
	return true;
}

XBuffEffectPrioriy XBuffLifeAddAttack::GetPriority()
{
	return XBuffEffectPrioriy_TargetLifeAddAttack;
}
void XBuffLifeAddAttack::OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper)
{
	m_Entity = pEntity;

	//CheckHP();
}
//
//void XBuffLifeAddAttack::OnRemove(XBuffRemoveReason reason)
//{
//	RemoveEffect();
//}
//
//void XBuffLifeAddAttack::OnAttributeChanged(CombatAttrDef attr)
//{
//	if (attr == BASIC_CurrentHP)
//	{
//		CheckHP();
//	}
//}
//
//void XBuffLifeAddAttack::CheckHP()
//{
//	Unit *pUnit = m_Entity->GetUnit();
//	double maxhp = pUnit->GetAttr(TOTAL_MaxHP);
//	double hp = pUnit->GetAttr(BASIC_CurrentHP);
//	if (maxhp < 1) maxhp = 1;
//
//	bool shouldMakeEffect = (hp/maxhp <= m_HPTriggerPercent);
//	if (shouldMakeEffect != m_IsEffect)
//	{
//		if (shouldMakeEffect)
//		{
//			AddEffect();
//		}
//		else
//		{
//			RemoveEffect();
//		}
//	}
//}
//
//void XBuffLifeAddAttack::AddEffect()
//{
//	if (m_IsEffect == false)
//	{
//		m_IsEffect = true;
//
//		Unit *pUnit = m_Entity->GetUnit();
//		pUnit->AddAttr(m_AttrID, m_AttrValue);
//	}
//}
//
//void XBuffLifeAddAttack::RemoveEffect()
//{
//	if (m_IsEffect == true)
//	{
//		m_IsEffect = false;
//
//		Unit *pUnit = m_Entity->GetUnit();
//		pUnit->AddAttr(m_AttrID, -m_AttrValue);
//	}
//}


