#ifndef __PLAYERLEVELMANAGER_H__
#define __PLAYERLEVELMANAGER_H__

#include "table/PlayerLevelTable.h"

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

private:
	bool LoadFile(const char *fileName);

	int m_MaxLevel;
	PlayerLevelTable m_oTable;
	PlayerLevelTable::RowData *m_LevelIndex[MAX_LEVEL_COUNT+1];
};

#endif // __PLAYERLEVELMANAGER_H__