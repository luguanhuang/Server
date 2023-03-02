#include "pch.h"
#include "XBuffDamageDistanceScale.h"
#include "unit/unit.h"
#include "entity/XEntity.h"
#include "gamelogic/XCombat.h"

bool XBuffDamageDistanceScale::TryCreate(const BuffTable::RowData *pBuffInfo, XBuff *pBuff)
{
	//if (pBuffInfo->LifeAddAttack[1] == 0)
	//	return false;

	if (pBuffInfo->ChangeCastDamageByDistance.empty())
		return false;
	pBuff->AddBuffEffect(new XBuffDamageDistanceScale(pBuffInfo));
	return true;
}

XBuffDamageDistanceScale::XBuffDamageDistanceScale(const BuffTable::RowData *pBuffInfo)
{
	//m_Entity = NULL;
	//m_IsEffect = false;
	//m_HPTriggerPercent = pBuff->GetBuffInfo()->LifeAddAttack[0];
	//m_AttrID = (CombatAttrDef)(int)pBuff->GetBuffInfo()->LifeAddAttack[1];
	//m_AttrValue = pBuff->GetBuffInfo()->LifeAddAttack[2];

	//m_pBuff = pBuff;
	m_Data.SetRange(0, 0, 10000, 0);
	m_Data.Init(pBuffInfo->ChangeCastDamageByDistance);
}

XBuffDamageDistanceScale::~XBuffDamageDistanceScale()
{

}

bool XBuffDamageDistanceScale::OnCastDamage(const HurtInfo &hurtInfo, ProjectDamageResult &result)
{
	///> 先屏蔽非技能攻击
	if (hurtInfo.m_skillId == 0)
		return true;

	if (m_Entity == NULL || hurtInfo.m_target == NULL)
		return true;

	double dis = (double)Vector3::Distance(m_Entity->GetPosition_p(), hurtInfo.m_target->GetPosition_p());

	result.ScaleValue(1.0 + m_Data.GetData(dis));
	//LogInfo("reduce damage from %d -> %d", oldvalue, (int)result.m_value);
	return true;
}

XBuffEffectPrioriy XBuffDamageDistanceScale::GetPriority()
{
	return XBuffEffectPrioriy_TargetLifeAddAttack;
}
void XBuffDamageDistanceScale::OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper)
{
	m_Entity = pEntity;

	//CheckHP();
}
