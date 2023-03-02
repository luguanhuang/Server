#ifndef __PROFESSIONMGR_H__
#define __PROFESSIONMGR_H__

#include "table/ProfessionTable.h"
#include "table/ProfessionGrowthPercentTable.h"


class CProfessionMgr
{
	CProfessionMgr();
	~CProfessionMgr();
	DECLARE_SINGLETON(CProfessionMgr)

public:

	bool Init();
	void Uninit();
	bool Reload();
	void Clear();
	
	int GetBasicProfession(int Prof);
	UINT32 GetAttackType(UINT32 profId);

public:
	std::string GetProfessionName(UINT32 ID);
	
private:

	ProfessionTable m_oTable;
	ProfessionGrowthPercentTable m_oGrowthTable;
	
};

#endif // __PLAYERLEVELMANAGER_H__