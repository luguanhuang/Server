#include "pch.h"
#include "XBuffChangeFightGroup.h"
#include "component/XComponent.h"
#include "unit/unit.h"
#include "entity/XEntity.h"
#include "unit/combatattribute.h"
#include "unit/attributewatcher.h"
#include "foreach.h"

bool XBuffChangeFightGroup::TryCreate(const BuffTable::RowData *pBuffInfo, XBuff *pBuff)
{
	if (pBuffInfo->ChangeFightGroup == -1)
		return false;

	pBuff->AddBuffEffect(new XBuffChangeFightGroup(pBuff, (UINT32)pBuffInfo->ChangeFightGroup));
	return true;
}

XBuffChangeFightGroup::XBuffChangeFightGroup(XBuff *pBuff, UINT32 fightgroup)
{
	m_pBuff = pBuff;
	m_FightGroup = fightgroup;
}

XBuffChangeFightGroup::~XBuffChangeFightGroup()
{

}

void XBuffChangeFightGroup::OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper)
{
	m_pEntity = pEntity;
	m_PreservedFightGroup = pEntity->GetUnit()->GetFightGroup();
	pEntity->GetUnit()->ChangeFightGroup(m_FightGroup);
}

void XBuffChangeFightGroup::OnRemove(XBuffRemoveReason reason)
{
	m_pEntity->GetUnit()->ChangeFightGroup(m_PreservedFightGroup);
}
