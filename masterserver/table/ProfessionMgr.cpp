#include "pch.h"
#include "ProfessionMgr.h"
#include "foreach.h"

INSTANCE_SINGLETON(CProfessionMgr)

CProfessionMgr::CProfessionMgr()
{

}

CProfessionMgr::~CProfessionMgr()
{

}

bool CProfessionMgr::Init()
{
	if (!m_oTable.LoadFile("table/Profession.txt"))
	{
		LogWarn("Load file %s failed!", "table/Profession.txt");
		return false;
	}
	if (!m_oGrowthTable.LoadFile("table/ProfessionGrowthPercent.txt"))
	{
		LogWarn("load file table/ProfessionGrowthPercent.txt failed!");
		return false;
	}
	return true;
}

void CProfessionMgr::Uninit()
{

}

void CProfessionMgr::Clear()
{
	m_oTable.Clear();
}

bool CProfessionMgr::Reload()
{
	Clear();
	return Init();
}

int CProfessionMgr::GetBasicProfession(int Prof)
{
	return Prof%10;
}

std::string CProfessionMgr::GetProfessionName(UINT32 ID)
{
	std::string name;

	ProfessionGrowthPercentTable::RowData* row = NULL;
	for (UINT32 i = 0; i < m_oGrowthTable.Table.size(); ++ i)
	{
		row = m_oGrowthTable.Table[i];
		if (row->ProfessionID == ID)
		{
			name = row->ProfessionName;
			break;
		}
	}
	return name;
}

UINT32 CProfessionMgr::GetAttackType(UINT32 profId)
{

	foreach (i in m_oTable.Table)
	{
		if ((*i)->ID == profId)
		{
			return (*i)->AttackType;
		}
	}

	return 0;
}
