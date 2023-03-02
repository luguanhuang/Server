#ifndef _MULTI_ACTIVITY_MGR_BASE_H_
#define _MULTI_ACTIVITY_MGR_BASE_H_

#include "table/MultiActivityList.h"
#include "timer.h"
#include "pb/enum.pb.h"

enum MultActivityTypeID
{
	MULT_ACTIVITY_1 = 1,			// 1 公会擂台战
	MULT_ACTIVITY_GUILD_BOSS = 2,	// 2 公会BOSS
	MULT_ACTIVITY_WORLD_BOSS = 3,	// 3 世界BOSS
	MULT_ACTIVITY_GUILD_QA = 4,		// 4 公会语音派对
	MULT_ACTIVITY_5 = 5,			// 5 公会挑战
	MULT_ACTIVITY_MULTI_QA = 6,		// 6 全民答题派对
	MULT_ACTIVITY_GUILD_BONUS = 7,	// 7 公会红包
	MULT_ACTIVITY_SKYCITY = 8,		// 8 天空之城
	MULT_ACTIVITY_GUILDRESWAR = 9,  // 9  工会资源争夺战
	MULT_ACTIVITY_CARDMATCH = 13,	// 13 扑克大赛 
	MULT_ACTIVITY_HORSE	= 14,		// 14 赛马活动
	MULT_ACTIVITY_POKERTOUNAMENT = 15,	// 15 
	MULT_ACTIVITY_16 = 16,			//领地争夺战
	MULT_ACTIVITY_GUILDCAMP_PARTY = 17,	// 工会营地排队
    MULT_ACTIVITY_WEEKEND4V4 = 18,	// 周末派对
	MULT_ACTIVITY_BIGMELEE = 22,	//大乱斗
	MULT_ACTIVITY_WEEKENDNEST = 23, //星挑战
	MULT_ACTIVITY_BATTLEFIELD = 24, //跨服战场
	MULT_ACTIVITY_ID_MAX
};

struct MultActivityTime
{
	MultActivityTime():startSec(0), endSec(0) {}

	UINT32 startSec;	// 开始时间(当日的秒)
	UINT32 endSec;		// 结束时间(当日的秒)
};

struct MultActivityConf
{
	int id;
	std::vector<INT32> openDays;
	std::vector<MultActivityTime> openTime;
	UINT32 playerLevel;
	UINT32 guildLevel;
	UINT32 maxCount;
	UINT32 icon;
	UINT32 openServerWeek;

	KKSG::MulActivityTimeState curState;
};

class MultActivityMgrBase : public ITimer
{
public:
	MultActivityMgrBase();
	virtual ~MultActivityMgrBase();

	virtual bool Init();
	virtual void Uninit();
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	bool CheckFile();
	void ClearFile();
	bool LoadFile();

	// 单个活动状态变化
	virtual void OnActivityStateChange(UINT32 id, KKSG::MulActivityTimeState lastState, KKSG::MulActivityTimeState curState);
	// 多个活动状态变化，一次check中变化的活动Id
	virtual void OnActivitysChange(UINT32 opencount, std::vector<UINT32>& changeIds);


	// 有多个开放时间， now在开放中，返回当前时间段，否则取 >= now 的最近的一个时间段
	// 当天不开放则返回开放那天的时间, 当天开放则返回的一定是今天的时间戳，无论是否结束
	UINT32 GetOpenTime(UINT32 id);
	UINT32 GetEndTime(UINT32 id);
	//UINT32 GetNextGuildBossGapDay();

	// 通用接口
	bool IsActivityTimeOpen(UINT32 id);
	bool IsGuildLevelOpen(UINT32 id, UINT32 guildLvl);
	bool IsActivityOnDay(UINT32 id, UINT32 weekNum);

	KKSG::MulActivityTimeState GetOpenTimeState(UINT32 id);
	KKSG::MulActivityTimeState GetOpenTimeState(UINT32 id, UINT32& leftTime, MultActivityTime& openTime);
	KKSG::MulActivityTimeState GetOpenTimeState(MultActivityConf* config, UINT32& leftTime, MultActivityTime& openTime);

	// GM加活动
	void GMAddOpenDayTime(UINT32 id, UINT32 startTime, UINT32 endTime);
	// 清掉GM加的活动时间
	void ResetConfig(UINT32 id);

	// 具体系统的接口封装
	bool IsGuildBossOpen(UINT32 guildLvl);
	time_t GetGuilBossOpenTime();
	time_t GetGuilBossEndTime();
	void SetGuildBossTime(UINT32 startTime, UINT32 endTime);
	UINT32 GetGuildBossOpenLvl();
	bool IsGuildBossOnTime();
	void SetOpenTime(UINT32 nTime)
	{
		m_nOpenTime = nTime;
	}

	MultActivityConf* GetActivityConfig(UINT32 id);

	void CheckActivityStateChange();

private:
	void AddConfig(MultiActivityList::RowData* conf);
	MultActivityTime GetNearTime(std::vector<MultActivityTime>& times, UINT32 todaySec);
	UINT32 GetOpenDayZeroSecond(MultActivityConf* config);


protected:
	UINT32 m_nOpenTime;
	HTIMER m_timer;
	MultiActivityList m_tableList;
	std::map<UINT32, MultActivityConf> m_activitys;
};

#endif