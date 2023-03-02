#ifndef __PVPRECORD_H__
#define __PVPRECORD_H__

#include "unit/roleserilizeinterface.h"
#include "pvpbasedata.h"
#include "unit/role.h"

namespace KKSG
{
	class PvpOneRec; 
	class PvpOneClientRec;
	class PvpBaseData;
	class PvpHistory;
}

class MyPvpRecord : public RoleSerializeInterface
{
public:
	MyPvpRecord(Role* role);
	~MyPvpRecord();
	virtual bool Load( const KKSG::RoleAllInfo *poRoleAllInfo );
	virtual void Save( KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged );
	void Update();
	void DayZero();
	void WeekZero();

	void AddOneRec(KKSG::PvpOneRec& rec);
	void FillAllCountClient(KKSG::PvpBaseData* data);
	void FillMyRecsClient(KKSG::PvpHistory* data);

	void SetWeekRewardHaveGet();
	bool GetWeekRewardHaveGet();

	int LeftJoinCountTodayInT();
	int GetJoinCountTodayInT();
	int GetWinCountThisWeek();
	int GetWinCountAll();
	int GetLoseCountAll();
	int GetDrawCountAll();
	int GetCountAll();
	int GetMatchScore();
	void AddPlayTime(UINT32 iTime);
	UINT32 GetTodayPlayTimes() { return m_todayplaytimes;}

	void TestPrint();
	void TestClear();
	time_t TestGetLastDayUpT(){return m_lastDayUpdateT;}
	time_t TestGetLastWeekUpT(){return m_lastWeekUpdateT;}
	void TestSetLastDayUpT(time_t t){m_lastDayUpdateT = t;}
	void TestSetLastWeekUpT(time_t t){m_lastWeekUpdateT = t;}

private:
	Role* m_poRole;
	std::list<KKSG::PvpOneRec> m_pvpRecs;

	int m_joinCountTodayInT;//某时间段参加次数
	int m_winCountThisWeek;
	int m_winCountAll;
	int m_loseCountAll;
	int m_drawCountAll;

	bool m_weekRewardHaveGet;

	time_t m_lastDayUpdateT;
	time_t m_lastWeekUpdateT;
	UINT32 m_todayplaytime;//当天参与保护队长累计时长;
	UINT32 m_todayplaytimes;//当天保护队长参与场数;

	Switch m_isModified;
};

#endif