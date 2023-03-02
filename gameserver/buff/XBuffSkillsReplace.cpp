#include "pch.h"
#include "XBuffSkillsReplace.h"
#include "entity/XEntity.h"
#include "unit/unit.h"
#include "XBuff.h"
#include "component/XSkillComponent.h"

bool XBuffSkillsReplace::TryCreate(const BuffTable::RowData *pBuffInfo, XBuff *pBuff)
{
	if (pBuffInfo->SkillsReplace.size() == 0)
		return false;

	pBuff->AddBuffEffect(new XBuffSkillsReplace(pBuff));
	return true;
}

XBuffSkillsReplace::XBuffSkillsReplace(XBuff* pBuff)
	: m_pBuff(pBuff)
{
}

void XBuffSkillsReplace::OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper)
{
	m_pEntity = pEntity;
	if (!m_pEntity->SkillComponent())
		return;

	for (auto it = m_pBuff->GetBuffInfo()->SkillsReplace.begin(); it != m_pBuff->GetBuffInfo()->SkillsReplace.end(); ++it)
	{
		m_pEntity->SkillComponent()->SetSkillReplace(XCommon::XHash((*it)[0].c_str()), XCommon::XHash((*it)[1].c_str()));
	}
}

void XBuffSkillsReplace::OnRemove(XBuffRemoveReason reason)
{
	if (!m_pEntity->SkillComponent())
		return;

	for (auto it = m_pBuff->GetBuffInfo()->SkillsReplace.begin(); it != m_pBuff->GetBuffInfo()->SkillsReplace.end(); ++it)
	{
		m_pEntity->SkillComponent()->SetSkillReplace(XCommon::XHash((*it)[0].c_str()), 0);
	}
}
