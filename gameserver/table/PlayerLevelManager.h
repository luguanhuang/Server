#ifndef __PLAYERLEVELMANAGER_H__
#define __PLAYERLEVELMANAGER_H__

#include "PlayerLevelTable.h"
#include "OpenSystemTable.h"
#include "table/StageCatchUpExpCfg.h"
#define MAX_LEVEL_COUNT 200

class PlayerLevelManager
{
	PlayerLevelManager();
	~PlayerLevelManager();
	DECLARE_SINGLETON(PlayerLevelManager)

public:

	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();
	
	inline int MaxLevel() { return m_MaxLevel; }
	UINT64 GetLevelupExp(int lvl);
	double GetExpAddition(int level);
	PlayerLevelTable::RowData *GetLevelInfo(int level);
	PlayerLevelTable::RowData *GetNextLevelInfo(int level) { return GetLevelInfo(level+1); }

	int GetSkillPoint(int level, bool init);

	UINT32 GetCatchUpRate(const UINT32 days,const UINT32 level);

	UINT32 GetEnhanceMaxLvl(int level);

private:
	bool LoadFile(const char *fileName);

	int m_MaxLevel;
	PlayerLevelTable m_oTable;
	PlayerLevelTable::RowData *m_LevelIndex[MAX_LEVEL_COUNT+1];

private:
		StageCatchUpExpCfg  catchup_exp_;
};

#endif // __PLAYERLEVELMANAGER_H__