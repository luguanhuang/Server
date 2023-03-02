#include "pch.h"
#include "fightgroup.h"
#include "pb/project.pb.h"

INSTANCE_SINGLETON(FightGroupMgr);

FightGroupMgr::FightGroupMgr()
{
	m_groupmax = 0;
}

FightGroupMgr::~FightGroupMgr()
{

}

bool FightGroupMgr::Init()
{
	if (!m_table.LoadFile("table/FightGroup.txt"))
	{
		SSWarn<<"load file table/FightGroup.txt failed"<<END;
		return false;
	}
	int count = m_table.Table.size() - 1;
	assert(count >= KKSG::FightHostility + 1);
	m_groupmax = (UINT32)count;
	m_fightgroups = new int*[count];
	for (int i = 0; i < count; ++i)
	{
		m_fightgroups[i] = new int[count];
	}
	
	for (int j = 0; j < count; ++j)
	{
		for (int k = 0; k < count; ++k)
		{
			if (j == k)
			{
				m_fightgroups[j][k] = 0;
				continue;
			}
			std::string mark = "";
			if (j == KKSG::FightNeutral || k == KKSG::FightNeutral)
			{
				mark = "x";	
			}
			else
			{
				mark = *((std::string*)m_table.Table[j+1] + (k+1));
			}
			assert(mark == "x" || mark == "o");
			m_fightgroups[j][k] = (mark == "x") ? 0 : 1;
		}
	}

	//Test();

	return true;
}

void FightGroupMgr::Uninit()
{
	for (int i = 0; i < (int)m_groupmax; ++i)
	{
		delete[] m_fightgroups[i];
	}
	delete[] m_fightgroups;
	m_fightgroups = NULL;
}

bool FightGroupMgr::CanAttack(UINT32 beattacked, UINT32 attacker)
{
	if (beattacked >= m_groupmax && attacker >= m_groupmax)
	{
		return beattacked != attacker;
	}
	else if (beattacked >= m_groupmax)
	{
		beattacked = m_groupmax - 1;
	}
	else if (attacker >= m_groupmax)
	{
		attacker = m_groupmax - 1;
	}
	return (1 == m_fightgroups[attacker][beattacked]);
}

bool FightGroupMgr::InGroup(UINT32 group, std::set<UINT32>& groups)
{
	return groups.find(group) != groups.end();
}

void FightGroupMgr::Test()
{
	for (int j = 0; j < (int)m_groupmax; ++j)
	{
		for (int k = 0; k < (int)m_groupmax; ++k)
		{
			SSWarn<<m_fightgroups[j][k]<<END;
		}
		SSWarn<<END;
	}
}