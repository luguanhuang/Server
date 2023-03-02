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

	void WeekZero();//���Ƹı�ؿ�
	void CheckOpen();//���ƿ���

	void RankClear();//���а�
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
	int m_lastSceneIndex;//-1��ʾû�п���
	UINT32 m_lastUseSealType;

	std::unordered_set<UINT64> m_hintRecord;//�ƺ���¼

};

#endif