#ifndef __BOSSRUSHMGR_H__
#define __BOSSRUSHMGR_H__

#include "table/BossRushTable.h"
#include "table/BossRushBuffTable.h"
#include "unit/enemy.h"

namespace KKSG
{
	class BossRushData;
	class UnitAppearance;
}

class Role;
struct BRRoleState;

class BossRushMgr
{
	BossRushMgr();
	~BossRushMgr();
	DECLARE_SINGLETON(BossRushMgr);
public:
	bool Init();
	void Uninit();

	bool LoadFile();

	int RefreshData(Role* pRole);
	bool MakeRefreshCost(Role* pRole);

	void FinishOneCount(Role* pRole, bool win);
	void OnBattleFinish(Role* pRole, std::vector<Sequence<UINT32,2>>& reward, int& succeedcount);

	void FillBossAppearance(Role* pRole, KKSG::UnitAppearance* app);
	void FillRoleStatus(Role* pRole, KKSG::BossRushData* data);

	int LeftRefreshCount(Role* pRole);
	void AddRefreshCount(Role* pRole);
	void ResetRefreshCount(Role* pRole);

	int LeftBossRushCount(Role* pRole);
	void AddBossRushCount(Role* pRole);

	int GetCurRefreshCount(Role* pRole);
	int GetCurRank(Role* pRole);
	inline int GetMaxRank() { return m_maxRank; }

	void OnRoleEnterScene(Role* pRole);
	void OnRoleCountinue(Role* pRole);

	void FillBossData(Enemy* pBoss, Scene* pScene);

	BossRushTable::RowData* GetConf(int confid);	

	BossRushBuffTable::RowData* GetBuffConf(int confid);

	void TestClear(Role* pRole);
	void TestPrint(Role* pRole);

private:

	int GetMaxRefreshCount(Role* pRole);

	int GetCurBossRushCount(Role* pRole);
	int GetMaxBossRushCount();

	void GiveReward(Role* pRole);
	void AddStateRank(Role* pRole);

	int RandomConf(Role* pRole, int oldconfid);
	void RandomBuff(int& brbid1, int& brbid2);

	void ClearRoleState(Role* pRole);
	BRRoleState* GetRoleState(Role* pRole);

	int m_maxRank;//×î´ó²ãÊý

	BossRushTable m_brTable;
	BossRushBuffTable m_bbuffTable;
};

#endif