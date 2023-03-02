#include "pch.h"
#include "onlinereward.h"
#include "rewardrecord.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "item.h"
#include "time.h"
#include "globalconfig.h"
#include "util/timespecificmgr.h"
#include "reward/ptcg2c_onlinerewardntf.h"
#include "reward/ptcg2c_nextdayrewardntf.h"

INSTANCE_SINGLETON(OnlineRewardMgr);

OnlineRewardMgr::OnlineRewardMgr()
{

}

OnlineRewardMgr::~OnlineRewardMgr()
{

}

bool OnlineRewardMgr::Init()
{
	if (!m_table.LoadFile("table/OnlineReward.txt"))
	{
		SSWarn<<"load file table/OnlineReward.txt"<<END;
		return false;
	}
	return true;
}

void OnlineRewardMgr::Uninit()
{
}

void OnlineRewardMgr::OnLineNotify(Role* role)
{
	CRewardRecord* record = role->Get<CRewardRecord>();
	role->UpdateOnlineTime();
	UINT32 onlinetime = role->GetOnlineTime();

	PtcG2C_OnlineRewardNtf ntf;
	for (auto i = m_table.Table.begin(); i != m_table.Table.end(); ++i)
	{
		OnlineRewardTable::RowData* rowdata = *i;
		if (record->IsTakenOnline(rowdata->time))
		{
			ntf.m_Data.add_state(2);
			ntf.m_Data.add_timeleft(0);
		}
		else
		{
			if (onlinetime >= rowdata->time)
			{
				ntf.m_Data.add_state(1);
				ntf.m_Data.add_timeleft(0);
			}
			else
			{
				ntf.m_Data.add_state(0);
				ntf.m_Data.add_timeleft(rowdata->time - onlinetime);
			}
		}
	}
	role->Send(ntf);
}

std::vector<ItemDesc> OnlineRewardMgr::GetOnlineReward(UINT32 index)
{
	std::vector<ItemDesc> items;
	OnlineRewardTable::RowData* rowdata = m_table.GetBytime(index);
	if (NULL == rowdata)
	{
		return items;
	}
	for (auto i = rowdata->reward.begin(); i != rowdata->reward.end(); ++i)
	{
		ItemDesc item(i->seq[0], i->seq[1]);
		items.push_back(item);
	}
	return items;
}

UINT32 OnlineRewardMgr::GetTime(UINT32 index)
{
	if (m_table.Table.size() <= index)
	{
		return (UINT32)(-1);
	}
	return m_table.Table[index]->time;
}
