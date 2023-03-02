#include "pch.h"
#include "garden/ptcm2g_gardeneventntf.h"
#include "unit/enemymanager.h"
#include "garden/ptcg2c_gardenplanteventnotice.h"
#include "scene/scene.h"
#include "scene/scenemanager.h"
#include "garden/ptcg2c_gardenbanquetnotice.h"
#include "component/XBuffComponent.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "entity/XRole.h"
#include "garden/garden_event.h"
#include "unit/roleoutlook.h"
#include "rolesummary/rolesummarybase.h"
#include "scene/scenegarden.h"

// generate by ProtoGen at date: 2016/10/17 21:21:57

void PtcM2G_GardenEventNtf::Process(UINT32 dwConnID)
{	
	switch (m_Data.event_type())
	{
	case KKSG::PLANT_STATE_CHANGE:
		{	
			Scene* scene = SceneManager::Instance()->FindBySceneID(m_Data.scene_uid());
			if (scene == NULL)
			{
				SSWarn<<" scene is null ,and scene uid  is :"<<m_Data.scene_uid()<<END;
				return ;
			}
			SceneGarden* garden = scene->GetSceneGarden();
			if (garden == NULL)
			{
				SSWarn<<" garden is null ,and scene uid  is :"<<m_Data.scene_uid()<<END;
				return ;
			}

			UINT64 enemy_id = garden->GetEnemyID(m_Data.farmland_id());
			// 目前规划是通过buf来改变			
			Enemy* unit = EnemyManager::Instance()->FindEnemy(enemy_id);
			if (unit == NULL)
			{
				SSWarn<<" unit is null ,and unit is :"<<enemy_id<<END;
				return;
			}
			// 先清理掉已有的buff
			if (NULL != unit->GetXObject()->BuffComponent())
			{
				unit->GetXObject()->BuffComponent()->ClearBuff();
			}

			// 给怪添加buff 
			if (m_Data.buff_id() > 0 )
			{
				XAddBuffActionArgs args;
				args.singleBuff.buffID = m_Data.buff_id();
				args.singleBuff.BuffLevel = 1;
				unit->GetXObject()->DispatchAction(&args);
			}		

			LogDebug("PLANT_STATE_CHANGE m_Data.buff_id() is %d ", m_Data.buff_id());
		}
		break;
	case KKSG::BANQUET_STAGE:
		// 通知场景 改变宴会阶段
		// 场景广播
		{
			Scene* scene = SceneManager::Instance()->FindBySceneID(m_Data.scene_uid());
			if (scene == NULL)
			{
				SSWarn<<" scene is null ,and scene uid  is :"<<m_Data.scene_uid()<<END;
				return ;
			}

			// 第一阶段，拉人到桌子边
			if (1 == m_Data.banquet_stage())
			{
				std::vector<Role*> roles;
				int guests_size = m_Data.guests_size();
				for ( int i = 0 ; i < guests_size; ++i)
				{					
					Role* role = GardenEvent::Instance()->SetRolePose(
						m_Data.banquet_stage(),
						*scene,
						m_Data.guests(i).key(),
						i);			
					if (NULL != role)
					{
						roles.push_back(role);
					}
				}
				if (!roles.empty())
				{
					scene->ForceOneLine(roles);
				}
			}

			// 最后一个阶段，拉到椅子上
			if (0 == m_Data.banquet_stage())
			{
				std::vector<Role*> roles;
				int guests_size = m_Data.guests_size();
				for ( int i = 0 ; i < guests_size; ++i)
				{
					Role* role = GardenEvent::Instance()->SetRolePose(
						m_Data.banquet_stage(),
						*scene,
						m_Data.guests(i).key(),
						i);							
					if (NULL != role)
					{
						roles.push_back(role);
					}
				}
				if (!roles.empty())
				{
					scene->ForceOneLine(roles);
				}
			}

			PtcG2C_GardenBanquetNotice ntf;			
			ntf.m_Data.set_garden_id(m_Data.garden_id());
			ntf.m_Data.set_banquet_id(m_Data.banquet_id());
			ntf.m_Data.set_banquet_stage(m_Data.banquet_stage());
			
			int guests_size = m_Data.guests_size();
			for ( int i = 0 ; i < guests_size; ++i)
			{
				ntf.m_Data.set_timestswk(m_Data.guests(i).value());
				LogDebug(" role %llu and  this week banquettimes: %d ", m_Data.guests(i).key(),m_Data.guests(i).value());
				Role* role = scene->FindSceneRole(m_Data.guests(i).key());
				if (NULL != role)
				{
					role->Send(ntf);
				}
			}

			LogDebug(" BANQUET_STAGE garden_id %llu and banquet_id is %d and stage is %d,time is %d ", m_Data.garden_id(),m_Data.banquet_id(),m_Data.banquet_stage(),time(NULL));
		}
		break;

	case KKSG::PLANT_DELETE:
		{
			Scene* scene = SceneManager::Instance()->FindBySceneID(m_Data.scene_uid());
			if (scene == NULL)
			{
				SSWarn<<" scene is null ,and scene uid  is :"<<m_Data.scene_uid()<<END;
				return ;
			}
			SceneGarden* garden = scene->GetSceneGarden();
			if (garden == NULL)
			{
				SSWarn<<" garden is null ,and scene uid  is :"<<m_Data.scene_uid()<<END;
				return ;
			}

			UINT64 enemy_id = garden->GetEnemyID(m_Data.farmland_id());

			// 销毁之前的怪
			Enemy* unit = EnemyManager::Instance()->FindEnemy(enemy_id);
			if (unit == NULL)
			{
				SSWarn<<" unit is null ,and unit is :"<<enemy_id<<END;
				return;
			}
			unit->TriggerDeath();
			//	清理映射关系
			garden->DeleteFarmlandEnemy(m_Data.farmland_id());

			PtcG2C_GardenPlantEventNotice ntf;
			ntf.m_Data.set_event_type(KKSG::PLANT_DELETE);			
			ntf.m_Data.set_garden_id(m_Data.garden_id());
			ntf.m_Data.set_farmland_id(m_Data.farmland_id());
			LogDebug("PtcG2C_GardenPlantEventNotice  gardenid %llu ，farmlandid : %d ",	m_Data.garden_id(),m_Data.farmland_id());

			scene->Broadcast(ntf);

		}
		break;
	case KKSG::PLANT_SPRITE:
		// 场景广播
		{
			Scene* scene = SceneManager::Instance()->FindBySceneID(m_Data.scene_uid());
			if (scene == NULL)
			{
				SSWarn<<" scene is null ,and scene uid  is :"<<m_Data.scene_uid()<<END;
				return ;
			}

			PtcG2C_GardenPlantEventNotice ntf;
			ntf.m_Data.set_event_type(KKSG::PLANT_SPRITE);
			ntf.m_Data.set_sprite_id(m_Data.sprite_id());
			ntf.m_Data.set_garden_id(m_Data.garden_id());
			ntf.m_Data.set_exist(m_Data.exist());
			LogDebug("PtcG2C_GardenPlantEventNotice  sprite_id %d ，exist : %d ",	m_Data.sprite_id(),m_Data.exist());

			scene->Broadcast(ntf);
		}
		break;
	case KKSG::PLANT_MATURE:
		{
			Scene* scene = SceneManager::Instance()->FindBySceneID(m_Data.scene_uid());
			if (scene == NULL)
			{
				SSWarn<<" scene is null ,and scene uid  is :"<<m_Data.scene_uid()<<END;
				return ;
			}
			SceneGarden* garden = scene->GetSceneGarden();
			if (garden == NULL)
			{
				SSWarn<<" garden is null ,and scene uid  is :"<<m_Data.scene_uid()<<END;
				return ;
			}

			UINT64 enemy_id = garden->GetEnemyID(m_Data.farmland_id());

			// 目前规划是通过buf来改变			
			Enemy* unit = EnemyManager::Instance()->FindEnemy(enemy_id);
			if (unit == NULL)
			{
				SSWarn<<" unit is null ,and unit is :"<<enemy_id<<END;
				return;
			}
			// 先清理掉已有的buff
			if (NULL != unit->GetXObject()->BuffComponent())
			{
				unit->GetXObject()->BuffComponent()->ClearBuff();
			}

			XAddBuffActionArgs args;
			args.singleBuff.buffID = GetGlobalConfig().PlantGrowMatureBuff;
			args.singleBuff.BuffLevel = 1;
			unit->GetXObject()->DispatchAction(&args);

			PtcG2C_GardenPlantEventNotice ntf;
			ntf.m_Data.set_event_type(KKSG::PLANT_MATURE);
			ntf.m_Data.set_farmland_id(m_Data.farmland_id());
			ntf.m_Data.set_garden_id(m_Data.garden_id());				
			scene->Broadcast(ntf);

			LogDebug("PLANT_MATURE garden_id:%dllu farmland:%d,buffid:%d, unit:%llu,isdead:%d ,time:%d ",
				m_Data.garden_id(),
				m_Data.farmland_id(),
				args.singleBuff.buffID,
				unit->GetID(),
				unit->IsDead(),
				time(NULL));
		}
		break;
	case KKSG::FISH_FACE:
		{
			// gardenid 即为role id
			Role* role = RoleManager::Instance()->FindByRoleID(m_Data.garden_id());
			if (NULL == role)
			{
				SSWarn<<" can't find role ,the role id  is :"<<m_Data.garden_id()<<END;
				return;
			}
			role->GetXObject()->CorrectLocation(role->GetXRole()->GetPosition_p(), GetGlobalConfig().FishingCameraDesdir, true);
			SSDebug<<"FISH START roleid:"<<role->GetID()<<" dir:"<<role->GetXRole()->GetFaceDegree()<<END;
		}
		break;
	case KKSG::FISH_RESULT:
		{
			Scene* scene = SceneManager::Instance()->FindBySceneID(m_Data.scene_uid());
			if (scene == NULL)
			{
				SSWarn<<" scene is null ,and scene uid  is :"<<m_Data.scene_uid()<<END;
				return ;
			}
			
			// gardenid 即为role id
			Role* role = RoleManager::Instance()->FindByRoleID(m_Data.garden_id());
			if (NULL == role)
			{
				SSWarn<<" can't find role ,the role id  is :"<<m_Data.garden_id()<<END;
				return;
			}

			/*PtcG2C_GardenPlantEventNotice ntf;
			ntf.m_Data.set_event_type(KKSG::FISH_RESULT);
			ntf.m_Data.set_role_id(m_Data.garden_id());
			ntf.m_Data.set_fish_result(m_Data.exist());
			scene->Broadcast(ntf,role);*/
			
			role->SetState(KKSG::OutLook_Fish, m_Data.exist());
			RoleOutLook outlook(role);
			outlook.Fill(SUMMARY_STATE, role);
			outlook.BroadCast();

			LogDebug("FISH_RESULT role_id:%llu,result:%d ", role->GetID(), m_Data.exist()); 
		}
		break;
	case KKSG::FISH_STOP:
		{
			Scene* scene = SceneManager::Instance()->FindBySceneID(m_Data.scene_uid());
			if (scene == NULL)
			{
				SSWarn<<" scene is null ,and scene uid  is :"<<m_Data.scene_uid()<<END;
				return ;
			}
				
			// gardenid 即为role id
			Role* role = RoleManager::Instance()->FindByRoleID(m_Data.garden_id());
			if (NULL == role)
			{
				SSWarn<<" can't find role ,the role id  is :"<<m_Data.garden_id()<<END;
				return;
			}

			/*PtcG2C_GardenPlantEventNotice ntf;
			ntf.m_Data.set_event_type(KKSG::FISH_STOP);
			ntf.m_Data.set_role_id(m_Data.garden_id());
			ntf.m_Data.set_fish_stop(true);
			scene->Broadcast(ntf,role);*/

			if (KKSG::OutLook_Fish == role->GetState().statetype())
			{
				role->SetState(KKSG::OutLook_Normal);
				RoleOutLook outlook(role);
				outlook.Fill(SUMMARY_STATE, role);
				outlook.BroadCast();
			}

			LogDebug("FISH_STOP role_id:%llu,fish_stop ", role->GetID()); 
		}
		break;
	default:
		break;
	}
}
