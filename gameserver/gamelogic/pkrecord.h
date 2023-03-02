#ifndef __PKRECORD_H__
#define __PKRECORD_H__

#include "unit/roleserilizeinterface.h"
#include "pb/enum.pb.h"
#include "unit/role.h"

#define PROFESSION_MAX 7

namespace KKSG
{
	class PkRecord;
	class PkBaseHist;
}

struct ItemDesc;
struct SPkOneRecord
{
	UINT64 opposer;
	UINT32 profession;
	std::string name;
	int point;
	UINT32 honorpoint;
	KKSG::PkResultType result;

	SPkOneRecord()
	{
		opposer = 0;
		profession = 0;
		point = 0;
		honorpoint = 0;
		result = KKSG::PkResult_Win;
	}
};

struct PkResultPair
{
	UINT32 win;
	UINT32 lose;
	UINT32 draw;

	PkResultPair()
	{
		win = 0;
		lose = 0;
		draw = 0;
	}
};

struct PkBaseHistory
{
	PkBaseHistory();
	void Reset();

	void Save(KKSG::PkBaseHist& data);
	void Load(const KKSG::PkBaseHist& data);

	void AddWinNum();
	void AddLoseNum();
	void AddDrawNum();
	UINT32 GetAllNum();
	float GetWinRate();
	UINT32 GetContinueWin();

	UINT32 m_win;
	UINT32 m_lose;
	UINT32 m_draw;
	UINT32 m_lastwin;
	UINT32 m_lastlose;
	UINT32 m_continuewin;
	UINT32 m_continuelose;
};

class CPkRecord : public RoleSerializeInterface
{
public:
	CPkRecord(Role* role);

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

	void ToKKSG(KKSG::PkRecord* record);
	void FillPkRec(KKSG::PkRoleRec& data, KKSG::PkNVNType type);

	void DayReset();
	// n week
	void WeekReset();
	// one week
	void OneWeekReset();
	void Update();

	void PkWeekTimeSyn(UINT32 ctime);
	
	inline UINT32 GetPoint() { return m_point; }
	inline UINT32 GetWin() { return m_baseHistWeek.m_win; }
	inline UINT32 GetLose() { return m_baseHistWeek.m_lose; }
	inline UINT32 GetDraw() { return m_baseHistWeek.m_draw; }
	inline UINT32 GetContWin() { return m_baseHistWeek.m_continuewin; }
	inline UINT32 GetCont() { return m_baseHistWeek.m_win+m_baseHistWeek.m_lose+m_baseHistWeek.m_draw; }
	std::deque<SPkOneRecord>& GetRecords() { return m_records; }
	inline std::set<UINT32>& GetBoxTaken() { return m_boxtaken; }

	inline UINT32 GetWinAll() { return m_baseHistAll.m_win; }
	inline UINT32 GetLoseAll() { return m_baseHistAll.m_lose; }
	inline UINT32 GetDrawAll() { return m_baseHistAll.m_draw; }
	inline UINT32 GetCountAll() { return m_baseHistAll.m_win+m_baseHistAll.m_lose+m_baseHistAll.m_draw;}
	inline UINT32 GetContWinAll() { return m_baseHistAll.m_continuewin; }
	inline UINT32 GetContLoseAll() { return m_baseHistAll.m_continuelose; }

	UINT32 GetContWinDay();

	bool IsBoxTaken(UINT32 index);
	void SetBoxTaken(UINT32 index);
	void SetWin( UINT32 winTimes );

	UINT32 GetRewardCount();
	void AddRewardCount();
	UINT32 GetRewradLeftCount();	

	void AddHonorPoint(UINT32 hp);

	void AddPoint(UINT64 roleid, UINT32 point);
	void SubPoint(UINT64 roleid, UINT32 point);

	bool NeedAddRobot();

	void SetPoint(UINT32 point);

	void PushRecord(UINT64 roleid, UINT32 profession, std::string name, int point, UINT32 honorpoint, KKSG::PkResultType result);

	void AddWinNum();
	void AddLoseNum();
	void AddDrawNum();

	void AddPlayTime(UINT32 iTime);

	void FillRankData(KKSG::PkRankData& data, KKSG::PkNVNType type);
	UINT32 GetTodayPlayTimes() { return m_todayplaytimes; }
	UINT32 GetWeekPlayTimes() { return m_weekplaytimes; }

	UINT32 GetMaxPoint();

	//nvn(2v2) //下面函数从2v2开始支持 不支持1v1 1v1仍然使用原有接口
	void ResetDayInfo(KKSG::PkRecordSubInfo& info);
	void ResetWeekInfo(KKSG::PkRecordSubInfo& info);
	void AddPoint(KKSG::PkNVNType type, int point);	
	void AddResult(KKSG::PkNVNType type, KKSG::PkOneRec& rec);
	KKSG::PkRecordSubInfo* GetSubInfo(KKSG::PkNVNType type);
	//UINT32 GetRewardLeftCount(KKSG::PkNVNType type);
	//void AddRewardCount(KKSG::PkNVNType type);

	UINT32 GetPoint(KKSG::PkNVNType type);

	UINT32 GetLastWeekPoint();

	void NVNInfoPrint(KKSG::PkNVNType type);
	void TestReset();
	
private:
	Switch m_ismodify;
	UINT32 m_week;
	UINT32 m_day;
	UINT32 m_oneWeekResetTime;
	UINT32 m_dayRewardCount;
	UINT32 m_point;
	UINT32 m_honorpoint;
	UINT32 m_lastWeekPoint;
	UINT32 m_pointlastlose;
	UINT32 m_todayplaytime;//当天天梯赛累计游戏时长;
	UINT32 m_todayplaytimes;//当天天梯赛参与场数;
	UINT32 m_weekplaytimes; // 近七天天梯赛参与场数;
	UINT32 m_last7daytime; //最后更新最近7天的时间;

	PkBaseHistory m_baseHistWeek;
	PkBaseHistory m_baseHistAll;
	PkBaseHistory m_baseHistDay;

	KKSG::PkRecordSubInfo m_info2v2;

	PkResultPair m_results[PROFESSION_MAX];
	std::set<UINT32> m_boxtaken;
	std::deque<SPkOneRecord> m_records;
	Role* m_poRole;
};

#endif