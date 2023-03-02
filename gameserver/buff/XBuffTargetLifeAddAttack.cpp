#include "pch.h"
#include "XBuffTargetLifeAddAttack.h"
#include "gamelogic/XCombat.h"
#include "entity/XEntity.h"
#include "unit/unit.h"
#include "foreach.h"

bool XBuffTargetLifeAddAttack::TryCreate(const BuffTable::RowData *pBuffInfo, XBuff *pBuff)
{
	if (pBuffInfo->TargetLifeAddAttack.empty())
		return false;

	pBuff->AddBuffEffect(new XBuffTargetLifeAddAttack(pBuffInfo->TargetLifeAddAttack));
	return true;
}

bool XBuffTargetLifeAddAttack::OnCastDamage(const HurtInfo &hurtInfo, ProjectDamageResult &result)
{
	double hp = hurtInfo.m_target->GetUnit()->GetPreciseHpPercent() / 100;
	result.ScaleValue(1.0 + m_Data.GetData(hp));
	//LogInfo("reduce damage from %d -> %d", oldvalue, (int)result.m_value);
	return true;
}

XBuffTargetLifeAddAttack::XBuffTargetLifeAddAttack(const vector<Sequence<float, 2>>& datas)
{
	m_Data.SetRange(0, 0, 1, 0);
	m_Data.Init(datas);

}

XBuffTargetLifeAddAttack::~XBuffTargetLifeAddAttack()
{
}

XBuffEffectPrioriy XBuffTargetLifeAddAttack::GetPriority()
{
	return XBuffEffectPrioriy_TargetLifeAddAttack;
}