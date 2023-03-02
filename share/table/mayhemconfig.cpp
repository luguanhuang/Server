#include "pch.h"
#include "mayhemconfig.h"
#include "commondef.h"

INSTANCE_SINGLETON(MayhemConfigMgr)

MayhemConfigMgr::MayhemConfigMgr()
{

}


MayhemConfigMgr::~MayhemConfigMgr()
{

}

bool MayhemConfigMgr::Init()
{
	return LoadFile();
}

void MayhemConfigMgr::Uninit()
{
	ClearFile();
}

void MayhemConfigMgr::ClearFile()
{
	m_oPointTable.Clear();
	m_oRankTable.Clear();
}

bool MayhemConfigMgr::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}
	m_oPointTable.LoadFile("table/BigMeleePointReward.txt");
	m_oRankTable.LoadFile("table/BigMeleeRankReward.txt");
	return true;
}

bool MayhemConfigMgr::CheckFile()
{
	BigMeleePointReward tmptable;
	if (!tmptable.LoadFile("table/BigMeleePointReward.txt"))
	{
		SSWarn << "load table/BigMeleePointReward.txt failed" << END;
		return false;
	}
	BigMeleeRankReward tmptable1;
	if (!tmptable1.LoadFile("table/BigMeleeRankReward.txt"))
	{
		SSWarn << "load table/BigMeleeRankReward.txt failed" << END;
		return false;
	}
	return true;
}

void  MayhemConfigMgr::GetPointAward(INT32 point, INT32 level, std::vector<ItemDesc>& list)
{
	BigMeleePointReward::RowData* pRowData = NULL;

	for (UINT32 i = 0; i < m_oPointTable.Table.size(); i ++)
	{
		if (m_oPointTable.Table[i]->levelrange[0] <= level && level <= m_oPointTable.Table[i]->levelrange[1])
		{
			if (m_oPointTable.Table[i]->point <= point)
			{
				if (pRowData == NULL || (pRowData && pRowData->point < m_oPointTable.Table[i]->point))
				{
					pRowData = m_oPointTable.Table[i];
				}

			}
		}
	}
	if (pRowData)
	{
		for (auto it = pRowData->reward.begin(); it != pRowData->reward.end(); it ++)
		{
			list.push_back(ItemDesc(it->seq[0], it->seq[1]));
		}
	}
}

void MayhemConfigMgr::GetRankAward(INT32 rank, INT32 level, std::vector<ItemDesc>& list)
{
	if (INVALID_RANK == rank)
	{
		return;
	}

	vector<Sequence<int, 2>> item;
	for (UINT32 i = 0; i < m_oRankTable.Table.size(); i ++)
	{
		if (m_oRankTable.Table[i]->levelrange[0]<= level && level <= m_oRankTable.Table[i]->levelrange[1])
		{
			if (m_oRankTable.Table[i]->rank[0] <= rank && rank <= m_oRankTable.Table[i]->rank[1])
			{
				for (auto it = m_oRankTable.Table[i]->reward.begin(); it != m_oRankTable.Table[i]->reward.end(); it ++)
				{
					list.push_back(ItemDesc(it->seq[0], it->seq[1]));
				}
				break;
			}
		}
	}
}
