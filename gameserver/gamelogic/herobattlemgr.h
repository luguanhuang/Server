#ifndef __HEROBATTLECONFIG_H__
#define __HEROBATTLECONFIG_H__

#include "util/utilsingleton.h"
#include "table/OverWatchTable.h"
#include "table/HeroBattleWeekReward.h"
#include "table/HeroBattleMapCenter.h"
#include "table/HeroBattleExperienceHero.h"
#include "table/HeroBattleAncientPower.h"

typedef OverWatchTable::RowData OverWatchConf;
typedef HeroBattleWeekReward::RowData HeroWeekRewardConf;
typedef HeroBattleMapCenter::RowData HeroBattleMapCenterConf;
typedef HeroBattleExperienceHero::RowData HeroBattleExperienceConf;
typedef HeroBattleAncientPower::RowData HeroBattleAncientConf;

class Role;

class HeroBattleMgr : public Singleton<HeroBattleMgr>
{	
public:
	HeroBattleMgr();
	~HeroBattleMgr();

	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	OverWatchConf* GetHeroConf(UINT32 heroid);
	HeroWeekRewardConf* GetHeroWeekRewardConf(UINT32 id);
	HeroBattleMapCenterConf* GetHeroMapCenterConf(UINT32 sceneid);
	HeroBattleExperienceConf* GetExperienceHeroConf(UINT32 itemid);
	HeroBattleAncientConf* GetAncientPowerConf(UINT32 heroid);

	bool IsInSpecialOpenTime();

	void GetNewRandomFreeHero(std::vector<UINT32>& freehero, Role* pRole);
	void AddSomeRandomFreeHero(std::vector<UINT32>& freehero, UINT32 num);

private:
	OverWatchTable m_oTable;
	HeroBattleWeekReward m_oWeekReward;
	HeroBattleMapCenter m_oMapCenter;
	HeroBattleExperienceHero m_oExperienceHero;
	HeroBattleAncientPower m_oAncientPower;
};


#endif