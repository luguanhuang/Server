#include "pch.h"
#include "arenarewardmgr.h"
#include "item.h"
#include "unit/role.h"
#include "arenarecord.h"

INSTANCE_SINGLETON(ArenaRewardMgr);

ArenaRewardMgr::ArenaRewardMgr()
{

}

ArenaRewardMgr::~ArenaRewardMgr()
{
	 
}

bool ArenaRewardMgr::Init()
{
	if (!m_dailytable.LoadFile("table/ArenaDailyPoint.txt"))	
	{
		SSWarn<<"load file table/ArenaDailyPoint.txt failed"<<END;
		return false;
	}
	if (!m_ranktable.LoadFile("table/ArenaRankReward.txt"))
	{
		SSWarn<<"load file table/ArenaRankReward.txt failed"<<END;
		return false;
	}
	return true;
}

void ArenaRewardMgr::Uninit()
{

}

void ArenaRewardMgr::GetPointReward(UINT32 point, UINT32 index, std::vector<ItemDesc>& items)
{
	if (m_dailytable.Table.size() <= index)
	{
		return;
	}
	ArenaDailyPointTable::RowData* rowdata = m_dailytable.Table[index];
	if (NULL != rowdata && rowdata->Point <= point)
	{
		for (auto i = rowdata->Reward.begin(); i != rowdata->Reward.end(); ++i)
		{
			ItemDesc item;
			item.itemID = (*i)[0];
			item.itemCount = (*i)[1];
			items.push_back(item);
		}	
	}
}

void ArenaRewardMgr::GetRankReward(UINT32 rank, UINT32 index, std::vector<ItemDesc>& items)
{
	if (m_ranktable.Table.size() <= index)
	{
		return;
	}
	ArenaRankRewardTable::RowData* rowdata = m_ranktable.Table[index];
	if (NULL != rowdata && rowdata->Rank >= rank)
	{
		for (auto i = rowdata->Reward.begin(); i != rowdata->Reward.end(); ++i)
		{
			ItemDesc item;
			item.itemID = (*i)[0];
			item.itemCount = (*i)[1];
			items.push_back(item);
		}	
	}
}

void ArenaRewardMgr::NotifyPointReward(Role* role)
{
	//PtcG2C_ArenaRewardNtf ntf;
	//ntf.m_Data.set_pointorrank(1);

	//CArenaRecord* record = role->Get<CArenaRecord>();
	//record->Update();

	//const std::set<UINT32>& pointreward = record->GetPointReward();
	//UINT32 point = record->GetPoint();
	//ntf.m_Data.set_param(point);

	//int count = 0;
	//for (auto i = m_dailytable.Table.begin(); i != m_dailytable.Table.end(); ++i)
	//{
	//	ArenaDailyPointTable::RowData* rowdata = *i;
	//	UINT32 state = 0;
	//	if (rowdata->Point <= point)
	//	{
	//		auto i = pointreward.find(count);
	//		state = i == pointreward.end() ? 1 : 2;
	//	}
	//	ntf.m_Data.add_rewardstate(state);
	//	++count;
	//}	
	//role->Send(ntf);
}

void ArenaRewardMgr::NotifyRankReward(Role* role)
{
	//PtcG2C_ArenaRewardNtf ntf;
	//ntf.m_Data.set_pointorrank(2);

	//CArenaRecord* record = role->Get<CArenaRecord>();
	//const std::set<UINT32>& rankreward = record->GetRankReward();
	//UINT32 optimalrank = record->GetOptimalRank();
	//ntf.m_Data.set_param(optimalrank);
	//
	//int count = 0;
	//for (auto i = m_ranktable.Table.begin(); i != m_ranktable.Table.end(); ++i)
	//{
	//	ArenaRankRewardTable::RowData* rowdata = *i;
	//	UINT32 state = 0;
	//	if (rowdata->Rank >= optimalrank)
	//	{
	//		auto i = rankreward.find(count);
	//		state = i == rankreward.end() ? 1 : 2;
	//	}
	//	ntf.m_Data.add_rewardstate(state);
	//	++count;
	//}
	//role->Send(ntf);
}