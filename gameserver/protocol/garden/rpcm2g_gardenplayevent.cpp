#include "pch.h"
#include "garden/rpcm2g_gardenplayevent.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "scene/scenemanager.h"
#include "scene/scene.h"
#include "gamelogic/bagtransition.h"
#include "garden/garden_event.h"
#include "unit/enemymanager.h"
#include "garden/ptcg2c_gardenplanteventnotice.h"
#include "scene/scenegarden.h"

// generate by ProtoGen at date: 2016/10/14 15:11:36

RPC_SERVER_IMPLEMETION(RpcM2G_GardenPlayEvent, GardenPlayEventArg, GardenPlayEventRes)

	void RpcM2G_GardenPlayEvent::OnCall(const GardenPlayEventArg &roArg, GardenPlayEventRes &roRes)
{
	Role* role = RoleManager::Instance()->FindByRoleID(roArg.role_id());
	if (role == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}

	switch (roArg.event_type())
	{
	case KKSG::PLANT:
		// 扣种子
		{
			ItemDesc item;
			item.itemID = roArg.seed_id();
			item.itemCount=1;
			std::vector<ItemDesc> item_list;
			item_list.push_back(item);
			INT32 result = GardenEvent::Instance()->ConsumeItems(
				*role,
				ItemFlow_Garden,
				ItemFlow_Garden_PlantSeed,
				item_list);			

			roRes.set_result((KKSG::ErrorCode)result);
			if (result == KKSG::ERR_SUCCESS)
			{
				// 放植物
				Scene* scene = SceneManager::Instance()->FindBySceneID(roArg.scene_uid());			
				if (scene == NULL)
				{
					SSWarn<<" scene is null ,and scene uid  is :"<<roArg.scene_uid()<<END;
					return ;
				}
				SceneGarden* garden = scene->GetSceneGarden();
				if (garden == NULL)
				{
					SSWarn<<" garden is null ,and scene uid  is :"<<roArg.scene_uid()<<END;
					return ;
				}

				UINT32 farmland_id = roArg.farmland_id()-1;

				UINT32 enemyID=roArg.sprite_id();  
				float rotate = 0;
				Vector3 pos;
				if (roArg.isguild())
				{
					Sequence<float, 3>& position = GetGlobalConfig().GuildPlantSeedPosition[farmland_id];
					pos.x = position.seq[0];
					pos.y = position.seq[1];
					pos.z = position.seq[2];
				}
				else
				{
					Sequence<float, 3>& position = GetGlobalConfig().PlantSeedPosition[farmland_id];
					pos.x = position.seq[0];
					pos.y = position.seq[1];
					pos.z = position.seq[2];
				}

				Enemy* unit = EnemyManager::Instance()->CreateEnemy(enemyID, pos, rotate, 1.0);
				if(NULL != unit)
				{
					scene->AddUnitAndNotify(unit);					
					// 保存到scene
					garden->AddFarmlandEnemy(roArg.farmland_id(),unit->GetID());
					SSDebug << "KKSG::PLANT :" <<roArg.farmland_id() <<":"<< enemyID << ":" << unit->GetID() << END;
				}
				if (roArg.isguild())
				{
					UINT64 guild_id = role->getGuildId();
					PtcG2C_GardenPlantEventNotice ntf;
					ntf.m_Data.set_event_type(KKSG::PLANT);			
					ntf.m_Data.set_garden_id(guild_id);
					ntf.m_Data.set_farmland_id(roArg.farmland_id());
					scene->Broadcast(ntf,role);

					LogDebug("PLANT EventNotice  gardenid %llu ，farmlandid : %d ",	guild_id,roArg.farmland_id());
				}
				roRes.set_role_id(roArg.role_id());
			}
		}
		break;	
	case KKSG::BANQUET:
		{
			// 检查自己是否在家园
			// 检查 家园是否有4个人
			Scene* scene = SceneManager::Instance()->FindBySceneID(roArg.scene_uid());
			if (scene == NULL)
			{
				SSWarn<<" scene is null ,and scene uid  is :"<<roArg.scene_uid()<<END;
				return ;
			}
			std::vector<UINT64> guests;
			INT32 result = GardenEvent::Instance()->CheckBanquetScenePlay(
				*scene,
				*role,
				guests);		
			if (result != KKSG::ERR_SUCCESS)
			{
				roRes.set_result((KKSG::ErrorCode)result);
				return;
			}

			auto it = guests.begin();
			auto it_end = guests.end();
			for (; it != it_end; ++it)
			{
				roRes.add_guests(*it);
			}
			// 扣烹饪食材
			{
				std::vector<ItemDesc> item_list;
				for (int i = 0; i < roArg.banquet_items_size(); ++i)
				{
					ItemDesc item;
					item.itemID = roArg.banquet_items(i).itemid();
					item.itemCount=roArg.banquet_items(i).itemcount();
					item_list.push_back(item);
				}

				INT32 result = GardenEvent::Instance()->ConsumeItems(
					*role,
					ItemFlow_Garden,
					ItemFlow_Garden_Banquet,
					item_list);

				roRes.set_result((KKSG::ErrorCode)result);
				roRes.set_role_id(roArg.role_id());
			}
		}
		break;

	default:
		break;
	}
}

void RpcM2G_GardenPlayEvent::OnDelayReplyRpc(const GardenPlayEventArg &roArg, GardenPlayEventRes &roRes, const CUserData &roUserData)
{
}
