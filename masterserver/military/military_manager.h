#ifndef __MILITARY_H__
#define __MILITARY_H__

#include "timer.h"
#include <time.h>
#include "mail/maildata.h"
#include "table/MilitaryRankReward.h"
class CRole;
class MilitaryManager : public ITimer
{
	MilitaryManager();
	~MilitaryManager();
	DECLARE_SINGLETON(MilitaryManager);
public:
	bool Init();
	void Uninit();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	void OnDesignation(UINT64 nRoleID,UINT32 nRank);
	void CheckMonthRankRewrad();

	UINT32 GetRank(UINT64 roleid);
	UINT32 GetAllRank();
	void GetMilitaryRankReward(UINT32 rank,std::vector<ItemDesc>& items);
	
	void TestPrint();
	void TestClear();

	void ClearFile();
	bool LoadFile();
	bool CheckFile();

private:

	MilitaryRankReward militaryrank_reward_;
	HTIMER handler_;

	time_t last_month_reward_;
};

#endif
