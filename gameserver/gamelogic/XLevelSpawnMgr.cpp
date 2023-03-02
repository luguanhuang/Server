#include "pch.h"
#include "XLevelSpawnMgr.h"
#include "scene/sceneconfig.h"
#include "config.h"
#include "XLevelScriptMgr.h"
#include "tablemgr/XEntityInfoLibrary.h"

using namespace std;

INSTANCE_SINGLETON(XLevelSpawnMgr)

void GetSceneSpawnConfig(UINT32 sceneID, SceneInfo *pSceneInfo, void *)
{
	XLevelSpawnMgr::Instance()->AddSceneStaticWaveInfo(sceneID, pSceneInfo->m_strSpawnConfig);
}

bool XLevelSpawnMgr::Init()
{
	LoadFile();

	return true;
}

void XLevelSpawnMgr::Uninit()
{
	Release();
}

bool XLevelSpawnMgr::LoadFile()
{
	Release();

	XLevelScriptMgr::Instance()->Release();

	SceneConfig::Instance()->TravelAllSceneInfo(GetSceneSpawnConfig, NULL);

	ParseWaves();

	return true;
}

void XLevelSpawnMgr::Release()
{
	for(std::map< UINT32, std::map<int, XLevelWave*> >::iterator it = m_StaticWaves.begin(); it != m_StaticWaves.end(); it++)
	{
		for(auto i = it->second.begin(); i != it->second.end(); ++i)
		{
			delete i->second;
		}

		it->second.clear();
	}

	m_StaticWaves.clear();
	m_sceneid2info.clear();
}

bool XLevelSpawnMgr::GetWaveInfo(UINT32 dwSceneID, std::vector<XLevelWave*>& vec)
{
	std::map< UINT32, std::map<int, XLevelWave*> >::iterator it = m_StaticWaves.find(dwSceneID);
	if(it != m_StaticWaves.end())
	{
		for (auto i = it->second.begin(); i != it->second.end(); ++i)
		{
			vec.push_back(i->second);
		}
		return true;
	}

	return false;
}

XLevelWave* XLevelSpawnMgr::GetWaveInfo(UINT32 dwSceneID, int waveID)
{
	std::map< UINT32, std::map<int, XLevelWave*> >::iterator it = m_StaticWaves.find(dwSceneID);
	if(it != m_StaticWaves.end())
	{
		auto find = it->second.find(waveID);
		if (find != it->second.end())
		{
			return find->second;
		}
	}

	return NULL;
}

bool XLevelSpawnMgr::GetLevelDoodad(UINT32 dwSceneID, std::vector<XLevelDoodad>& vec)
{
	vec.clear();

	std::map< UINT32, std::map<int, XLevelWave*> >::iterator it = m_StaticWaves.find(dwSceneID);
	if(it != m_StaticWaves.end())
	{
		for(auto i = it->second.begin(); i != it->second.end(); ++i)
		{
			for (size_t j = 0; j < i->second->m_DoodadID.size(); ++j)
			{
				XLevelDoodad d;
				d.m_WaveID = i->second->m_Id;
				d.m_DoodadID = i->second->m_DoodadID[j];
				vec.push_back(d);
			}
		}
		return true;
	}

	return false;
}

bool XLevelSpawnMgr::GetLevelWaveMapDoodad(UINT32 dwSceneID, std::map<int, std::vector<XLevelDoodad>>& vec)
{
	vec.clear();

	std::map< UINT32, std::map<int, XLevelWave*> >::iterator it = m_StaticWaves.find(dwSceneID);
	if(it != m_StaticWaves.end())
	{
		for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
		{
			for (size_t j = 0; j < iter->second->m_DoodadID.size(); ++j)
			{
				XLevelDoodad d;
				d.m_WaveID = iter->second->m_Id;
				d.m_DoodadID = iter->second->m_DoodadID[j];
				d.index = j;
				vec[d.m_WaveID].push_back(d);
			}
		}

		return true;
	}

	return false;
}

void XLevelSpawnMgr::AddSceneStaticWaveInfo(UINT32 dwSceneID, const std::string& strLevelConfigFile)
{
	if(strLevelConfigFile.empty()) return;

	string strFile = string("table/") + strLevelConfigFile + string(".txt");
	std::string data = GSConfig::Instance()->ReadFileContent(strFile.c_str());
	std::stringstream ss(data);

	string line;
	std::getline(ss, line);

	int totalWave = atoi(line.c_str());

	std::getline(ss, line);

	int PreloadWave = atoi(line.c_str());

	for (int i = 0; i < PreloadWave; i++)
		 std::getline(ss, line);

	bool isnew = false;
	auto k = m_StaticWaves.find(dwSceneID);
	if (k != m_StaticWaves.end())
	{
		for (auto l = k->second.begin(); l != k->second.end(); ++l)
		{
			delete l->second;
			l->second = NULL;
		}
		k->second.clear();
	}
	std::map<int, XLevelWave*> oSceneStaticWaveInfo;
	for(int wv = 0; wv < totalWave; wv++)
	{
		XLevelWave *pNewWave = NULL;
		auto j = oSceneStaticWaveInfo.find(wv);
		if (j == oSceneStaticWaveInfo.end())
		{
			pNewWave = new XLevelWave();
		}
		else
		{
			pNewWave = j->second;
		}
		pNewWave->ReadFromeFile(ss);

		oSceneStaticWaveInfo[wv] = pNewWave;
	}

	m_StaticWaves[dwSceneID] = oSceneStaticWaveInfo;
	//}
	/*
	else
	{
		std::map<int, XLevelWave*>* oSceneStaticWaveInfo = &k->second;
		for(int wv = 0; wv < totalWave; wv++)
		{
			XLevelWave *pNewWave = NULL;
			auto j = oSceneStaticWaveInfo->find(wv);
			if (j == oSceneStaticWaveInfo->end())
			{
				pNewWave = new XLevelWave();
			}
			else
			{
				pNewWave = j->second;
			}
			pNewWave->ReadFromeFile(ss);

			(*oSceneStaticWaveInfo)[wv] = pNewWave;
		}
	}
	*/
	XLevelScriptMgr::Instance()->AddSceneSpawnScripts(dwSceneID, strLevelConfigFile);
}

void XLevelSpawnMgr::ParseWaves()
{
	for (auto i = m_StaticWaves.begin(); i != m_StaticWaves.end(); ++i)
	{
		// parse one scene
		XLevelStatistic info;
		for (auto j = i->second.begin(); j != i->second.end(); ++j)
		{
			ParseWave(info, j->second, i->first);
		}
		m_sceneid2info[i->first] = info;
	}
}

void XLevelSpawnMgr::ParseWave(XLevelStatistic& info, XLevelWave* wave, UINT32 sceneid)
{
	if (0 == wave->m_EnemyID)
	{
		return;
	}
	if (wave->m_SpawnType == Spawn_Source_Monster)
	{
		const XEntityStatistics::RowData* data = XEntityInfoLibrary::Instance()->TemplateID2Data_Enemy(wave->m_EnemyID);
		if (NULL == data)
		{
			SSWarn<<"enemy id NULL:"<<wave->m_EnemyID<<" sceneid:"<<sceneid<<END;
			return;
		}
		if (0 == data->Fightgroup)
		{
			if (data->Type == KKSG::Species_Boss)
			{
				info.SvrBossType = data->ID;						
				if ((int)data->AttackBase > info.SvrBossAtkMAX)
				{
					info.SvrBossAtkMAX = (int)data->AttackBase;		
					info.SvrBossSkillMAX = (int)data->AttackBase;    	 		
				}
				if ((int)data->MaxHP > info.SvrBossHpMax)
				{
					info.SvrBossHpMax = (int)data->MaxHP;			
				}
				if ((UINT32)data->MaxHP < (UINT32)info.SvrBossHpMin)
				{
					info.SvrBossHpMin = (int)data->MaxHP;			
				}
			}
			else
			{
				if ((int)data->AttackBase > info.SvrMonsterAtkMAX)
				{
					info.SvrMonsterAtkMAX = (int)data->AttackBase;		
					info.SvrMonsterSkillMAX = (int)data->AttackBase;    	 		
				}
				if ((int)data->MaxHP > info.SvrMonsterHpMax)
				{
					info.SvrMonsterHpMax = (int)data->MaxHP;			
				}
				if ((UINT32)data->MaxHP < (UINT32)info.SvrMonsterHpMin)
				{
					info.SvrMonsterHpMin = (int)data->MaxHP;			
				}
			}
			++info.SvrMonsterCount;       	
		}
	}
}

XLevelStatistic* XLevelSpawnMgr::GetStatistic(UINT32 sceneid)
{
	auto i = m_sceneid2info.find(sceneid);
	if (i == m_sceneid2info.end())
	{
		return NULL;
	}
	else
	{
		return &i->second;
	}
}

XLevelSpawnMgr::XLevelSpawnMgr()
{

}

XLevelSpawnMgr::~XLevelSpawnMgr()
{

}
