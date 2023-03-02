#ifndef __FLOWER_H__
#define __FLOWER_H__

#include "table/FlowerTable.h"
#include "item.h"
#include "table/FlowerReturnTable.h"
#include "table/FlowerRankRewardTable.h"
#include "table/FlowerRain.h"
#include "table/FlowerSendNoticeTable.h"

#define FLOWERRECORDLIMIT 20
#define FLOWERROLELIMIT 100

enum FlowerCount
{
	FlowerOne = 1,
	FlowerNine = 9,
	FlowerDoubleNine = 99,
};

namespace KKSG
{
	class SocialInfo2Db;
}

class FlowerConfig
{
	FlowerConfig();
	~FlowerConfig();
	DECLARE_SINGLETON(FlowerConfig);
public:
	bool Init();
	void Uninit();

	ItemDesc GetFlowerCost(UINT32 count);
	void GetFlowerReward(UINT32 count, std::vector<ItemDesc>& items);
	const FlowerRankRewardTable::RowData* GetFlowerRewardData(UINT32 rank);
	const FlowerReturnTable::RowData* GetFlowerReturnData(UINT32 itemID, UINT32 itemNum);
	bool IsFlowerRain(int itemID, int itemCount);
	const FlowerSendNoticeTable::RowData* GetNoticeData(UINT32 itemID, UINT32 count);
private:
	FlowerTable m_flowertable;
	FlowerReturnTable m_flowerReturnTable;
	FlowerRankRewardTable m_flowerRankTable;
	FlowerRain m_flowerRainTable;
	FlowerSendNoticeTable m_flowerNotice;
	
};

#endif