#ifndef __XLEVEL_SPAWN_MGR__
#define __XLEVEL_SPAWN_MGR__

#include "XLevelWave.h"

///> 安全日志关卡静态数据
struct XLevelStatistic
{
	XLevelStatistic()
	{
		SvrMonsterCount = 0;       	
		SvrBossType = 0;						
		SvrMonsterAtkMAX = 0;		
		SvrMonsterSkillMAX = 0;    	 		
		SvrMonsterHpMax = 0;			
		SvrMonsterHpMin = (UINT32)(-1);			
		SvrBossAtkMAX = 0;        	
		SvrBossSkillMAX = 0;			 
		SvrBossHpMax = 0;         	
		SvrBossHpMin = (UINT32)(-1);         	
	}

	void Check()
	{
		if (SvrBossHpMin == (UINT32)(-1))
		{
			SvrBossHpMin = 0;
		}
		if (SvrMonsterHpMin == (UINT32)(-1))
		{
			SvrMonsterHpMin = 0;
		}
	}

	int SvrMonsterCount;       	///> Svr端数据,本关卡配置的怪物数量（包括小怪和boss）" 
	int SvrBossType;			///> Svr端数据，副本内BOSS编号,多个BOSS,仅记录最终BOSS"		
	int SvrMonsterAtkMAX;		///> Svr端数据,本关卡配置的小怪攻击力最大值" 
	int SvrMonsterSkillMAX;    	///> Svr端数据,本关卡配置的小怪技能伤害最大值" 		
	int SvrMonsterHpMax;		///> Svr端数据,本关卡配置的小怪生命值最大值" 
	int SvrMonsterHpMin;		///> Svr端数据,本关卡配置的小怪生命值最小值" 
	int SvrBossAtkMAX;        	///> Svr端数据,本关卡配置的BOSS攻击力最大值" 
	int SvrBossSkillMAX;		///> Svr端数据,本关卡配置的boss技能伤害最大值" 
	int SvrBossHpMax;         	///> Svr端数据,本关卡配置的Boss生命值最大值" 
	int SvrBossHpMin;         	///> Svr端数据,本关卡配置的Boss生命值最小值"
};

struct XLevelDoodad
{
	int m_WaveID;
	int m_DoodadID;
	int index;
};

class XLevelSpawnMgr
{
	XLevelSpawnMgr();
	~XLevelSpawnMgr();
	DECLARE_SINGLETON(XLevelSpawnMgr)

public:
	bool Init();
	void Uninit();

	bool LoadFile();
	void Release();

	void AddSceneStaticWaveInfo(UINT32 dwSceneID, const std::string& strLevelConfigFile);

	bool GetWaveInfo(UINT32 dwSceneID, std::vector<XLevelWave*>& vec);
	XLevelWave* GetWaveInfo(UINT32 dwSceneID, int waveID);

	bool GetLevelDoodad(UINT32 dwSceneID, std::vector<XLevelDoodad>& vec);
	bool GetLevelWaveMapDoodad(UINT32 dwSceneID, std::map<int, std::vector<XLevelDoodad>>& vec);

	XLevelStatistic* GetStatistic(UINT32 sceneid);

private:
	void ParseWaves();
	void ParseWave(XLevelStatistic& info, XLevelWave* wave, UINT32 sceneid);

private:
	std::map<UINT32, XLevelStatistic> m_sceneid2info;
	std::map<UINT32, std::map<int, XLevelWave*>> m_StaticWaves;
	
};
#endif // __XLEVEL_SPAWN_MGR__