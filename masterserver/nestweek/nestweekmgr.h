#ifndef __NESTWEEKMGR_H__
#define __NESTWEEKMGR_H__

#include "timer.h"

class CRole;

class NestWeekMgr : public ITimer
{
	NestWeekMgr();
	~NestWeekMgr();
	DECLARE_SINGLETON(NestWeekMgr)

public:
	bool Init();
	void Uninit();

	void ReadDbData();
	void WriteDbData();

	void WeekZero();//控制改变关卡
	void CheckOpen();//控制开启

	void RankClear();//排行榜
	void GiveRankReward(const std::vector<UINT64>& roles, UINT32 rank);
	//void GiveFirstRankReward(std::vector<UINT64>& roles);

	void OnlineHint();
	void CheckHint(CRole* pRole);

	int GetCurExp();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	void TestSetNextWeek();


private:
	HTIMER m_handler;	

	UINT32 m_lastWeekUpTime;
	int m_lastSceneIndex;//-1表示没有开启
	UINT32 m_lastUseSealType;

	std::unordered_set<UINT64> m_hintRecord;//推红点记录

};

#endif