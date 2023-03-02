#include "pch.h"
#include "XBuffMob.h"
#include "entity/XEntity.h"
#include "unit/unit.h"
#include "foreach.h"
#include "component/XBuffComponent.h"
#include "unit/enemymanager.h"
#include "entity/XObject.h"
#include "scene/scene.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "table/XSkillEffectMgr.h"
#include "scene/securitystatistics.h"
#include "scene/securityaistatistics.h"

bool XBuffMob::TryCreate(const BuffTable::RowData *pBuffInfo, XBuff *pBuff)
{
	if (pBuffInfo->MobID == 0)
		return false;

	pBuff->AddBuffEffect(new XBuffMob(pBuffInfo->MobID, pBuff));
	return true;
}

XBuffMob::XBuffMob(uint templateID, XBuff* pBuff)
	: m_TemplateID(templateID)
	, m_pBuff(pBuff)
	, m_MobID(0)
	, m_bUpdated(false)
{

}

void XBuffMob::OnAdd(XEntity* pEntity, CombatEffectHelper* pEffectHelper)
{
	m_pEntity = pEntity;

}

void XBuffMob::_Mob()
{
	Unit* caller = Unit::FindUnit(m_pBuff->GetCaster());
	if(caller && !caller->IsDead())
	{
		Enemy* pMonster = EnemyManager::Instance()->CreateEnemyByCaller(caller, m_TemplateID, m_pEntity->GetPosition(), m_pEntity->GetFaceDegree(), false);

		XSkillEffectMgr::Instance()->SetMobProperty(pMonster, caller, 0);

		m_MobID = pMonster->GetID();
		m_pBuff->GetEffectData()->MobID = m_MobID;

		if (caller->GetSecurityStatistics() && caller->GetSecurityStatistics()->_AIInfo)
			caller->GetSecurityStatistics()->_AIInfo->OnExternalCallMonster();

	}
}

void XBuffMob::OnRemove(XBuffRemoveReason reason)
{
	Unit* unit = Unit::FindUnit(m_MobID);
	if(unit && !unit->IsDead())
	{
		unit->TriggerDeath();
	}
}

void XBuffMob::OnUpdate()
{
	if (!m_bUpdated)
	{
		m_bUpdated = true;
		_Mob();
	}

	XEntity* pEntity = XEntity::ValidEntity(m_MobID);
	if(pEntity == NULL)
	{
		XRemoveBuffActionArgs args;
		args.nBuffId = m_pBuff->GetID();
		m_pEntity->DispatchAction(&args);
	}
}
