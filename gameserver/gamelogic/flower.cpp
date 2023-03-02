#include "pch.h"
#include "flower.h"
#include "time.h"
#include "util/XCommon.h"
#include "pb/project.pb.h"
#include "foreach.h"
#include "mail/mailsender.h"
#include "table/globalconfig.h"

INSTANCE_SINGLETON(FlowerConfig);

FlowerConfig::FlowerConfig()
{

}

FlowerConfig::~FlowerConfig()
{

}

bool FlowerConfig::Init()
{
	if (!m_flowertable.LoadFile("table/Flower.txt"))
	{
		SSWarn<<"load file table/Flower.txt failed"<<END;
		return false;
	}
	if (!m_flowerReturnTable.LoadFile("table/FlowerReturn.txt"))
	{
		SSWarn<<"load file table/FlowerReturn.txt failed"<<END;
		return false;
	}
	if (!m_flowerRankTable.LoadFile("table/FlowerRankReward.txt"))
	{
		SSWarn<<"load file table/FlowerRankReward.txt failed"<<END;
		return false;
	}
	if (!m_flowerRainTable.LoadFile("table/FlowerRain.txt"))
	{
		SSWarn<<"load file table/FlowerRain.txt failed"<<END;
		return false;
	}
	if (!m_flowerNotice.LoadFile("table/FlowerNotice.txt"))
	{
		SSWarn<<"load file table/FlowerNotice.txt failed"<<END;
		return false;
	}
	return true;
}

void FlowerConfig::Uninit()
{

}	

ItemDesc FlowerConfig::GetFlowerCost(UINT32 count)
{
	ItemDesc item;
	FlowerTable::RowData* rowdata = m_flowertable.GetBycount(count);
	if (NULL != rowdata)
	{
		item.itemID = rowdata->cost[0];
		item.itemCount = rowdata->cost[1];
	}
	return item;
}

void FlowerConfig::GetFlowerReward(UINT32 count, std::vector<ItemDesc>& items)
{
	FlowerTable::RowData* rowdata = m_flowertable.GetBycount(count);
	if (NULL == rowdata)
	{
		return;
	}
	for (auto i = rowdata->reward.begin(); i != rowdata->reward.end(); ++i)
	{
		ItemDesc item;
		item.itemID = i->seq[0];
		item.itemCount = i->seq[1];
		items.push_back(item);
	}
	return;
}

const FlowerRankRewardTable::RowData* FlowerConfig::GetFlowerRewardData(UINT32 rank)
{
	foreach(i in m_flowerRankTable.Table)
	{
		if (rank >= (*i)->rank.seq[0] && rank <= (*i)->rank.seq[1])
		{
			return *i;
		}
	}

	return NULL;
}

const FlowerReturnTable::RowData* FlowerConfig::GetFlowerReturnData(UINT32 itemID, UINT32 itemNum)
{
	foreach(i in m_flowerReturnTable.Table)
	{
		if (itemID == (*i)->receiveFlower.seq[0] && itemNum == (*i)->receiveFlower.seq[1])
		{
			return *i;
		}
	}

	return NULL;
}

bool FlowerConfig::IsFlowerRain(int itemID, int itemCount)
{
	foreach(i in m_flowerRainTable.Table)
	{
		if ((*i)->FlowerID == itemID && itemCount >= (*i)->Count)
		{
			return true;
		}
	}
		
	return false;
}

const FlowerSendNoticeTable::RowData* FlowerConfig::GetNoticeData(UINT32 itemID, UINT32 count)
{
	foreach(i in m_flowerNotice.Table)
	{
		if ((*i)->ItemID == itemID && (*i)->Num == count)
		{
			return (*i);
		}
	}

	return NULL;
}
