#include "pch.h"
#include "CombatParamMgr.h"
#include "table/globalconfig.h"
#include "unit/unit.h"
#include "scene/scene.h"
#include "scene/attributepk.h"


INSTANCE_SINGLETON(CombatParamMgr)

CombatParamMgr::CombatParamMgr()
{

}

CombatParamMgr::~CombatParamMgr()
{

}

bool CombatParamMgr::Init()
{
	if (!m_oTable.LoadFile("table/CombatParamList.txt"))
	{
		LogWarn("Load file table/CombatParamList.txt failed!");
	}

	if (!m_oSATable.LoadFile("table/SuperArmorRecoveryCoffTable.txt"))
	{
		LogWarn("Load file table/SuperArmorRecoveryCoffTable.txt failed!");
	}

	return true;
}

void CombatParamMgr::Uninit()
{

}

CombatParamTable::RowData * CombatParamMgr::GetCombatParamConf(int Level)
{
	if (Level <= 0) 
	{
		return m_oTable.Table[0];
	}

	if (Level > (int)m_oTable.Table.size())
	{
		return m_oTable.Table.back();
	}

	ASSERT(Level == m_oTable.Table[Level-1]->PlayerLevel);
	return m_oTable.Table[Level-1];
}

CombatParamTable::RowData * CombatParamMgr::GetCombatParamConf( Unit* unit )
{
	if(NULL == unit)
	{
		return NULL;
	}
	int Level = unit->GetCombatUseLevel();

	return GetCombatParamConf(Level);
}

SuperArmorRecoveryCoffTable::RowData * CombatParamMgr::GetSuperArmorCoff(int monsterType, int behitState)
{
	for (unsigned i = 0; i < m_oSATable.Table.size(); ++i)
	{
		if (m_oSATable.Table[i]->monster_type == monsterType &&
			m_oSATable.Table[i]->Value == behitState)
		{
			return m_oSATable.Table[i];
		}
	}

	return NULL;
}

