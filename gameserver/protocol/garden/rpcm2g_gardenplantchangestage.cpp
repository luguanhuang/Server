#include "pch.h"
#include "garden/rpcm2g_gardenplantchangestage.h"

// generate by ProtoGen at date: 2016/10/18 11:34:36

#include "scene/scenemanager.h"
#include "scene/scene.h"
#include "gamelogic/bagtransition.h"
#include "garden/garden_event.h"
#include "unit/enemymanager.h"
#include "scene/scenegarden.h"


RPC_SERVER_IMPLEMETION(RpcM2G_GardenPlantChangeStage, GardenPlantChangeStageArg, GardenPlantChangeStageRes)

	void RpcM2G_GardenPlantChangeStage::OnCall(const GardenPlantChangeStageArg &roArg, GardenPlantChangeStageRes &roRes)
{
	Scene* scene = SceneManager::Instance()->FindBySceneID(roArg.scene_uid());			
	if (scene == NULL)
	{
		SSWarn<<" scene is null ,and scene_uid is :"<<roArg.scene_uid()<<END;
		return;
	}
	SceneGarden* garden = scene->GetSceneGarden();
	if (NULL == garden)
	{
		SSWarn<<" garden is null ,and scene_uid is :"<<roArg.scene_uid()<<END;
		return;
	}

	UINT64 enemy_id = garden->GetEnemyID(roArg.farmland_id());

	// 销毁之前的怪
	Enemy* unit = EnemyManager::Instance()->FindEnemy(enemy_id);
	if (unit == NULL)
	{
		SSWarn<<" unit is null ,and unit is :"<<enemy_id<<END;
		return;
	}
	LogDebug("TriggerDeath unit,id is : %llu ", unit->GetID());
	unit->TriggerDeath();
	// 先删除
	garden->DeleteFarmlandEnemy(roArg.farmland_id());

	// 创建新的怪
	UINT32 farmland_id = roArg.farmland_id()-1;		
	UINT32 enemyID=roArg.sprite_id();  
	float rotate = 0;
	Vector3 pos;	
	if (roArg.isguild())
	{
		// 工会地块坐标
		auto& postion  = GetGlobalConfig().GuildPlantSeedPosition[farmland_id];	
		pos.x = postion.seq[0];
		pos.y = postion.seq[1];
		pos.z = postion.seq[2];
	}
	else
	{
		auto& postion  = GetGlobalConfig().PlantSeedPosition[farmland_id];	
		pos.x = postion.seq[0];
		pos.y = postion.seq[1];
		pos.z = postion.seq[2];
	}

	Enemy* new_unit = EnemyManager::Instance()->CreateEnemy(enemyID, pos, rotate, 1.0);
	if (new_unit == NULL)
	{
		SSWarn<<" new_unit is null ,and enemyID is :"<<enemyID<<END;
		return;
	}
	scene->AddUnitAndNotify(new_unit);

	// 建立映射关系
	garden->AddFarmlandEnemy(roArg.farmland_id(),new_unit->GetID());

	LogDebug("AddUnitAndNotify new_unit,id is : %llu ", new_unit->GetID());

	// 给怪添加buff 
	if (roArg.buff_id() > 0 )
	{
		XAddBuffActionArgs args;
		args.singleBuff.buffID = roArg.buff_id();
		args.singleBuff.BuffLevel = 1;
		new_unit->GetXObject()->DispatchAction(&args);

		LogDebug("RpcM2G_GardenPlantChangeStage roArg.buff_id() is %d ", roArg.buff_id());
	}		

	roRes.set_result(KKSG::ERR_SUCCESS);
	roRes.set_garden_id(roArg.garden_id());
	roRes.set_farmland_id(roArg.farmland_id());
}

void RpcM2G_GardenPlantChangeStage::OnDelayReplyRpc(const GardenPlantChangeStageArg &roArg, GardenPlantChangeStageRes &roRes, const CUserData &roUserData)
{
}
