#include "pch.h"
#include "randombossmgr.h"
#include "table/RandomBossTable.h"
#include "util/XRandom.h"
#include "foreach.h"

INSTANCE_SINGLETON(RandomBossMgr)

RandomBossMgr::RandomBossMgr()
{

}

RandomBossMgr::~RandomBossMgr()
{

}

bool RandomBossMgr::Init()
{
	RandomBossTable oTable;
	if (!oTable.LoadFile("table/RandomEntityList.txt"))
	{
		LogWarn("load table/RandomEntityList.txt failed!");
		return false;
	}

	UINT32 i;
	int randID = 0;
	RandomBossGroup group;
	RandomBossTable::RowData *pRow = oTable.Table[0];
	randID = pRow->RandomID;
	group.AddBossProb(pRow->EntityID, pRow->Prob);

	for (i = 1; i < oTable.Table.size(); ++i)
	{
		pRow = oTable.Table[i];
		if (pRow->RandomID != randID)
		{
			m_oBossGroups[randID] = group;
			group.Reset();
			randID = pRow->RandomID;
		}

		group.AddBossProb(pRow->EntityID, pRow->Prob);
	}

	m_oBossGroups[randID] = group;
	return true;
}

void RandomBossMgr::Uninit()
{

}

int RandomBossMgr::GetRandomBoss(int RandomID)
{
	auto it = m_oBossGroups.find(RandomID);
	if (it == m_oBossGroups.end())
	{
		return 0;
	}

	return it->second.ChooseRandomBossID();
}


int RandomBossGroup::ChooseRandomBossID()
{
	int rand = XRandom::randInt(0, totalProb);
	foreach (i in bosses)
	{
		rand -= i->prob;
		if (rand < 0)
		{
			return i->bossID;
		}
	}

	return 0;
}
