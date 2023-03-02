#ifndef __FLOWER_H__
#define __FLOWER_H__

#include "table/FlowerTable.h"
//#include "item.h"
#include "table/FlowerReturnTable.h"
#include "table/FlowerRankRewardTable.h"
#include "table/FlowerRain.h"
#include "table/FlowerSendNoticeTable.h"
#include "table/FlowerWeekRankReward.h"

#define FLOWERRECORDLIMIT 20
#define FLOWERROLELIMIT 20
#define FLOWERROLETEMPLIMIT 100
#define FLOWER_RED_ITEMID 90

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

struct FlowerSendRecord 
{
	UINT64 roleid;
	UINT32 count;
	UINT32 timestamp;
	UINT32 updegreelevel;
	UINT32 itemID;

	FlowerSendRecord()
	{
		roleid = 0;
		count = 0;
		timestamp = 0;
		updegreelevel = 0;
		itemID = 0;
	}
};

struct RoleCountRecord
{
	UINT64 roleid;
	//UINT32 count;
	std::map<UINT32, UINT32> flowersCount;
	UINT32 GetConvertNum()const
	{
		return 0;
	}
};

struct ActivityFlowerData
{
	ActivityFlowerData()
	{
		gotReward = false;
		activityID = 0;
		activityEndTime = 0;
		updateTime = 0;
	}
	void Clear()
	{
		gotReward = false;
		activityID = 0;
		activityEndTime = 0;
		updateTime = 0;
		mapReceiveFlowersActivity.clear();
	}
	bool gotReward;
	UINT32 activityID;
	UINT32 activityEndTime;
	UINT32 updateTime;
	std::map<UINT32, UINT32> mapReceiveFlowersActivity;
};

struct Flower
{
	Flower(UINT64 roleid)
	{
		m_ischanged = false;
		m_allcount = 0;
		m_weekcount = 0;
		m_week = 0;
		m_roleID = roleid;
		m_updatetime = 0;
		m_updatetimeYes = 0;
	}

	void LoadFromDb(const KKSG::SocialInfo2Db* socialinfo2db);
	void SaveToDb(KKSG::SocialInfo2Db* info);
	void OnLoadFromDb();

	void Update();
	void OnDayPass();

	void Sort(bool isall = false);
	static bool IsLess(const RoleCountRecord& a, const RoleCountRecord& b);

	void AddFlower(UINT64 send, UINT64 besend, UINT32 itemID, UINT32 count);
	void AddSendFlower(UINT64 send, UINT64 besend, UINT32 itemID, UINT32 count);

	void PushRecord(UINT64 send, UINT32 itemID, UINT32 count, UINT32 uplevel, bool sendout);
	void RecordRoleCount(UINT64 send, UINT32 itemid, UINT32 count);

	static UINT32 GetReceiveCount(const std::map<UINT32, UINT32>& receiveCount);
	UINT32 GetTotalReceiveCount();
	UINT32 GetTodayReceiveCount();
	UINT32 GetYestodayReceiveCount();
	UINT32 GetThisWeekReceiveCount();
	UINT32 GetActivityReceiveCount();
	UINT32 GetTotalReceiveCount2Client(std::map<UINT32, UINT32>& mapReceiveFlowers2Client);
	void DoTxLog(UINT64 roleID);

	bool CheckGetFlowerActivityReward();
	void CheckUpdateFlowerActivity();
	void ClearActivityFlowerInfo();

	void DebugWeekPass();
	void DebugDayPass();

private:
	void _CheckAddActivityFlower(UINT64 send, UINT64 besend, UINT32 itemID, UINT32 count);


public:
	bool m_ischanged;
	UINT32 m_allcount;
	UINT32 m_weekcount;
	UINT32 m_week;

	UINT32 m_day;
	UINT32 m_updatetime;
	UINT32 m_updatetimeYes;
	UINT64 m_roleID;
	//别人送的
	//std::deque<FlowerSendRecord> m_records;
	std::vector<RoleCountRecord> m_rolecount;

	std::map<UINT32, UINT32> m_mapReceiveFlowersTotal;
	std::map<UINT32, UINT32> m_mapReceiveFlowersToday;
	std::map<UINT32, UINT32> m_mapReceiveFlowersYes;
	std::map<UINT32, UINT32> m_mapReceiveFlowersThisWeek;
	ActivityFlowerData m_activityFlowerData;
	//送别人的
	std::deque<FlowerSendRecord> m_sendrecords;
	std::map<UINT32, UINT32> m_mapSendFlowersTotal;

};

class FlowerConfig
{
	FlowerConfig();
	~FlowerConfig();
	DECLARE_SINGLETON(FlowerConfig);
public:
	bool Init();
	void Uninit();

	const FlowerRankRewardTable::RowData* GetFlowerRewardData(UINT32 rank);
	const FlowerReturnTable::RowData* GetFlowerReturnData(UINT32 itemID, UINT32 itemNum);
	bool IsFlowerRain(int itemID, int itemCount);
	const FlowerSendNoticeTable::RowData* GetNoticeData(UINT32 itemID, UINT32 count);
	const FlowerWeekRankReward::RowData* GetFlowerWeekRewardData(UINT32 rank);
private:
	FlowerTable m_flowertable;
	FlowerReturnTable m_flowerReturnTable;
	FlowerRankRewardTable m_flowerRankTable;
	FlowerRain m_flowerRainTable;
	FlowerSendNoticeTable m_flowerNotice;
	FlowerWeekRankReward m_flowerWeekRankTable;
};

#endif