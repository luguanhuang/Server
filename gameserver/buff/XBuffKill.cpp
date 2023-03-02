#include "pch.h"
#include "XBuffKill.h"
#include "entity/XEntity.h"
#include "unit/unit.h"
#include "foreach.h"
#include "component/XBuffComponent.h"
#include "gamelogic/killerwatcher.h"

bool XBuffKill::TryCreate(const BuffTable::RowData *pBuffInfo, XBuff *pBuff)
{
	if (pBuffInfo->Kill == 0)
		return false;

	pBuff->AddBuffEffect(new XBuffKill(pBuff));
	return true;
}

XBuffKill::XBuffKill(XBuff* pBuff)
{
	m_pBuff = pBuff;
}

void XBuffKill::OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper)
{
	m_pEntity = pEntity;
}

void XBuffKill::OnRemove(XBuffRemoveReason reason)
{
	if (reason != XBuffRemoveReason_Replace && m_pEntity != NULL && !m_pEntity->GetUnit()->IsDead() && !m_pEntity->BuffComponent()->IsDestroying() && !m_pEntity->BuffComponent()->IsLeavingScene())
	{
		Unit* pUnit = m_pEntity->GetUnit();
		double v = -pUnit->GetAttr(TOTAL_CurrentHP) - 99999999.0;
		KillerWatchParam killerParam(pUnit);
		killerParam.KillerID = m_pBuff->GetCaster();
		killerParam.KillerBuff = m_pBuff->GetID();
		KillerWatcher::CheckKiller(killerParam, v);

		pUnit->SetBlockAddHP(true);
		pUnit->AddAttr(BASIC_CurrentHP, v);
		pUnit->CheckAttrState();

	}
}

