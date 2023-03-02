#ifndef __MAYHEM_CONFIG_H__
#define __MAYHEM_CONFIG_H__

#include "table/BigMeleePointReward.h"
#include "table/BigMeleeRankReward.h"
#include "define/itemdef.h"

class MayhemConfigMgr
{
	MayhemConfigMgr();
	~MayhemConfigMgr();
	DECLARE_SINGLETON(MayhemConfigMgr)

public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	void GetPointAward(INT32 point, INT32 level, std::vector<ItemDesc>& list);
	void GetRankAward(INT32 rank, INT32 level, std::vector<ItemDesc>& list);

private:
	BigMeleePointReward m_oPointTable;
	BigMeleeRankReward m_oRankTable;
};
#endif

