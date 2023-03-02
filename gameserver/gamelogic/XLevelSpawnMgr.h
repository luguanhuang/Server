#ifndef __XLEVEL_SPAWN_MGR__
#define __XLEVEL_SPAWN_MGR__

#include "XLevelWave.h"

///> ��ȫ��־�ؿ���̬����
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

	int SvrMonsterCount;       	///> Svr������,���ؿ����õĹ�������������С�ֺ�boss��" 
	int SvrBossType;			///> Svr�����ݣ�������BOSS���,���BOSS,����¼����BOSS"		
	int SvrMonsterAtkMAX;		///> Svr������,���ؿ����õ�С�ֹ��������ֵ" 
	int SvrMonsterSkillMAX;    	///> Svr������,���ؿ����õ�С�ּ����˺����ֵ" 		
	int SvrMonsterHpMax;		///> Svr������,���ؿ����õ�С������ֵ���ֵ" 
	int SvrMonsterHpMin;		///> Svr������,���ؿ����õ�С������ֵ��Сֵ" 
	int SvrBossAtkMAX;        	///> Svr������,���ؿ����õ�BOSS���������ֵ" 
	int SvrBossSkillMAX;		///> Svr������,���ؿ����õ�boss�����˺����ֵ" 
	int SvrBossHpMax;         	///> Svr������,���ؿ����õ�Boss����ֵ���ֵ" 
	int SvrBossHpMin;         	///> Svr������,���ؿ����õ�Boss����ֵ��Сֵ"
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