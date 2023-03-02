#include "pch.h"
#include "scenegarden.h"
#include "scene.h"
#include "unit/enemymanager.h"

SceneGarden::SceneGarden(Scene* scene)
:m_scene(scene)
{
	
}

SceneGarden::~SceneGarden()
{

}

bool SceneGarden::Init(const KKSG::SceneGardenInfo& info)
{
	for (int i = 0; i < info.plants_size(); ++i)
	{
		///> 创建新的怪
		const KKSG::SceneGardenSlot& slot = info.plants(i);
		UINT32 farmland_id = slot.index() ;
		UINT32 enemyID = slot.plantid(); 
		UINT32 buff_id = slot.buffid();

		float rotate = 0;
		Vector3 pos;
		if (m_scene->GetSceneType() == KKSG::SCENE_GUILD_HALL)
		{
			Sequence<float, 3>& position = GetGlobalConfig().GuildPlantSeedPosition[farmland_id-1];
			pos.x = position.seq[0];
			pos.y = position.seq[1];
			pos.z = position.seq[2];
		}
		else
		{
			Sequence<float, 3>& position = GetGlobalConfig().PlantSeedPosition[farmland_id-1];
			pos.x = position.seq[0];
			pos.y = position.seq[1];
			pos.z = position.seq[2];
		}

		Enemy* new_unit = EnemyManager::Instance()->CreateEnemy(enemyID, pos, rotate, 1.0);
		if (new_unit == NULL)
		{
			SSWarn<<" new_unit is null ,and enemyID is :"<<enemyID<<END;
			continue;
		}
		m_scene->AddUnitAndNotify(new_unit);

		// 给怪添加buff 
		if (buff_id > 0 )
		{
			XAddBuffActionArgs args;
			args.singleBuff.buffID = buff_id;
			args.singleBuff.BuffLevel = 1;
			new_unit->GetXObject()->DispatchAction(&args);
		}	

		AddFarmlandEnemy(farmland_id,new_unit->GetID());
		AddPlant(farmland_id, new_unit->GetID());

		SSDebug << "m_garden->AddPlant :"<< "farmland_id <<:" <<farmland_id<<":"<< enemyID << ":" << new_unit->GetID() << END;
	}
	return true;
}
void SceneGarden::AddPlant(UINT32 slot, UINT64 plantid)
{
	auto i = m_slot2plant.find(slot);
	if (i != m_slot2plant.end())
	{
		SSWarn<<"exist slot:"<<i->first<<" plant:"<<i->second<<END;
	}
	m_slot2plant[slot] = plantid;
}

const std::map<UINT32, UINT64>& SceneGarden::GetPlants()
{
	return m_slot2plant;
}

// farm enemy
void SceneGarden::AddFarmlandEnemy(const UINT32 farmland_id,const UINT64 enemy_id)
{
	farmland_enemy_.insert(std::make_pair(farmland_id,enemy_id));
}

void SceneGarden::DeleteFarmlandEnemy(const UINT32 farmland_id)
{
	farmland_enemy_.erase(farmland_id);
}

UINT64 SceneGarden::GetEnemyID(const UINT32 farmland_id)
{
	auto it =  farmland_enemy_.find(farmland_id);
	auto it_end = farmland_enemy_.end();
	if (it != it_end)
	{
		return it->second;
	}

	return 0;
}
