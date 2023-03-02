#include "pch.h"
#include "XSpawnInfoGenerator.h"
#include "pb/project.pb.h"
#include "XLevelSpawnMgr.h"
#include "SpawnSourceHandler.h"
#include "scene/scene.h"


INSTANCE_SINGLETON(XSpawnInfoGenerator)

XSpawnInfoGenerator::XSpawnInfoGenerator()
{

}

XSpawnInfoGenerator::~XSpawnInfoGenerator()
{

}

bool XSpawnInfoGenerator::Init()
{
	RegistHandler(Spawn_Source_Monster, new SpawnSourceMonsterHandler);
	RegistHandler(Spawn_Source_Player, new SpawnSourcePlayerHandler);
	RegistHandler(Spawn_Source_Random, new SpawnSourceRandomHandler);
	return true;
}

void XSpawnInfoGenerator::Uninit()
{
	for (auto i = m_handler.begin(); i != m_handler.end(); ++i)
	{
		delete i->second;
	}

	m_handler.clear();
}

void XSpawnInfoGenerator::RegistHandler(int type, IWaveGenerateHander *handler)
{
	m_handler[type] = handler;
}

void XSpawnInfoGenerator::GenerateWaveData(Role *pRole, Scene *pScene, KKSG::SceneCfg *pData)
{
	std::vector<XLevelWave *> allWave;
	if (XLevelSpawnMgr::Instance()->GetWaveInfo(pScene->GetSceneTemplateID(), allWave))
	{
		for (unsigned int i = 0; i < allWave.size(); ++i)
		{
			XLevelWave *pWave = allWave[i];

			if (!pWave->m_Levelscript.empty())
				continue;

			auto itHandler = m_handler.find(pWave->m_SpawnType);
			if (itHandler != m_handler.end())
			{
				KKSG::UnitAppearance *pEnemyData = pData->add_enemywaves();
				pEnemyData->set_waveid(pWave->m_Id);
				itHandler->second->FillWave(pRole, pScene, pWave, pEnemyData);
			}
			else
			{
				LogWarn("spwan wave %d type %d failed!", i, pWave->m_SpawnType);
			}
		}
	}
}

