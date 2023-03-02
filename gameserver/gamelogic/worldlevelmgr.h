#ifndef __WORLDLEVELMGR_H__
#define __WORLDLEVELMGR_H__

#include "table/WorldLevelExpBuff.h"


typedef WorldLevelExpBuff::RowData WorldLevelConf;

/*struct WorldLevelConf
{
	WorldLevelConf()
		:worldLevel(_worldLevel), level(_level), expBuff(_expBuff), backExpBuff(_backExpBuff){}

	bool operator < (const WorldLevelConf& conf) const
	{
		return level[0] < conf.level[0];
	}
	UINT32 worldLevel;
	Sequence<UINT32,2> level;
	double expBuff;
	double backExpBuff;
};*/

class WorldLevelMgr
{
	WorldLevelMgr();
	~WorldLevelMgr();
	DECLARE_SINGLETON(WorldLevelMgr);
public:
	bool Init();
	void Uninit();
		
	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	void OnGetMsWorldLevel(UINT32 level);
	double GetExpBuff(UINT32 level, UINT64 roleid);
	double GetBackExpBuff(UINT32 level, UINT64 roleid);
	UINT32 GetLevel() { return m_worldLevelNow; }

	const WorldLevelConf* GetWorldLevelConf(UINT32 level, UINT64 roleid);

private:
	WorldLevelExpBuff m_oTable;
	std::unordered_map<UINT32, std::vector<WorldLevelConf*>> m_worldLevelMap;
	UINT32 m_worldLevelNow;
};


#endif