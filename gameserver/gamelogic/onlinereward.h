#ifndef __OFFLINEREWARD_H__
#define __OFFLINEREWARD_H__

#include "table/OnlineRewardTable.h"

#define NEXTDAY_REWARDID 38
#define ONLINE_REWARDID 39

class Role;
struct ItemDesc;
class OnlineRewardMgr
{
	OnlineRewardMgr();
	~OnlineRewardMgr();
	DECLARE_SINGLETON(OnlineRewardMgr);
public:
	bool Init();
	void Uninit();

	bool OnTimer();

	void OnLineNotify(Role* role);

	UINT32 GetTime(UINT32 index);

	std::vector<ItemDesc> GetOnlineReward(UINT32 index);

	UINT32 GetMaxIndex() { return m_table.Table.size() - 1; }

private:
	OnlineRewardTable m_table;
};

#endif