#include "pch.h"
#include "scenefinder.h"
#include "unit/unit.h"
#include "entity/XObject.h"
#include "util/XCommon.h"
#include "unit/enemy.h"
#include "unit/role.h"
#include "gamelogic/fightgroup.h"

bool FindEnemyCondition::operator()(Unit *pUnit)
{
	return pUnit->IsEnemy();
}

LogicAndCondition::LogicAndCondition(IFindUnitCondition *cond1, IFindUnitCondition *cond2)
{
	m_Cond[0] = cond1;
	m_Cond[1] = cond2;
}

bool LogicAndCondition::operator()(Unit *pUnit)
{
	return m_Cond[0]->operator()(pUnit) && m_Cond[1]->operator()(pUnit);
}

LogicOrCondition::LogicOrCondition(IFindUnitCondition *cond1, IFindUnitCondition *cond2)
{
	m_Cond[0] = cond1;
	m_Cond[1] = cond2;
}

bool LogicOrCondition::operator()(Unit *pUnit)
{
	return m_Cond[0]->operator()(pUnit) || m_Cond[1]->operator()(pUnit);
}

LogicNotCondition::LogicNotCondition(IFindUnitCondition *cond)
{
	m_Cond = cond;
}

bool LogicNotCondition::operator()(Unit *pUnit)
{
	return !m_Cond->operator()(pUnit);
}

static bool IsOppose(Unit *pUnit1, Unit *pUnit2)
{
	return Unit::IsOppose(pUnit1, pUnit2);
}

static bool IsSameGroup(Unit *pUnit1, Unit *pUnit2)
{
	return Unit::IsSameGroup(pUnit1, pUnit2);
}

bool FindAllyCondition::operator()(Unit *pUnit)
{
	return IsSameGroup(pUnit, m_pUnit);
}
//
//bool FindCollideUnitCondition::operator()(Unit *pUnit)
//{
//	return pUnit->IsCollider();
//}

bool FindEnemyIDCondition::operator()(Unit *pUnit)
{
	return pUnit->GetTemplateID() == m_enemyID;
}

bool FindEnemyWaveIDCondition::operator()(Unit *pUnit)
{
	if (pUnit->IsEnemy())
	{
		Enemy* enemy = dynamic_cast<Enemy*>(pUnit);
		return (m_waveID == enemy->GetWaveID());
	}
	return false;
}

bool FindEnemyTemplateIdCondition::operator()(Unit* pUnit)
{
	if (pUnit->IsEnemy() && !pUnit->IsDead())
	{
		return pUnit->GetTemplateID() == m_templateID; 
	}
	return false;
}

bool FindDummyRoleFightGroupCondition::operator()( Unit *pUnit )
{
	if(pUnit->IsDummyRole() && pUnit->GetFightGroup() == m_fightGroup)
	{
		return true;
	}
	return false;
}
//
//bool FindInGroupCondition::operator()( Unit *pUnit )
//{
//	if (m_fightgroups.empty())
//	{
//		return true;
//	}
//	if (FightGroupMgr::Instance()->InGroup(pUnit->GetFightGroup(), m_fightgroups))
//	{
//		return true;
//	}
//	return false;
//}
////
//bool FindInGroupInRangeCondition::operator()( Unit *pUnit )
//{
//	if (m_fightgroups.empty())
//	{
//		return true;
//	}
//	if (FightGroupMgr::Instance()->InGroup(pUnit->GetFightGroup(), m_fightgroups))
//	{
//		float d = Vector3::Distance(pUnit->GetXObject()->GetPosition_p(), m_position);
//		return (d <= m_raidus);
//	}
//	return false;
//}
//
bool FindBossCondition::operator()(Unit* pUnit)
{
	if(pUnit->IsEnemy())
	{
		if(((Enemy*)(pUnit))->IsBoss())
		{
			return true;
		}
	}
	return false;
}
