#ifndef __ARENAREWARDMGR_H__
#define __ARENAREWARDMGR_H__

#include "table/ArenaDailyPointTable.h"
#include "table/ArenaRankRewardTable.h"

struct ItemDesc;
class Role;
class ArenaRewardMgr
{
	ArenaRewardMgr();
	~ArenaRewardMgr();
	DECLARE_SINGLETON(ArenaRewardMgr);
public:
	bool Init();
	void Uninit();
	
	void GetPointReward(UINT32 point, UINT32 index, std::vector<ItemDesc>& items);
	void GetRankReward(UINT32 rank, UINT32 index, std::vector<ItemDesc>& items);

	void NotifyPointReward(Role* role);
	void NotifyRankReward(Role* role);

private:
	ArenaDailyPointTable m_dailytable;
	ArenaRankRewardTable m_ranktable;
};

#endif