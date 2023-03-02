#include "pch.h"
#include "garden/rpcc2m_plantharvest.h"
#include "garden/garden_manager.h"
#include "role/rolemanager.h"
#include "network/gslink.h"
#include "define/tlogenum.h"
#include "garden/garden_utility.h"
#include "event/eventmgr.h"
#include "common/rpcm2g_msgiveitem.h"
#include "scene/scenemanager.h"
#include "task/roletaskmgr.h"

// generate by ProtoGen at date: 2016/10/3 19:04:40

RPC_SERVER_IMPLEMETION(RpcC2M_PlantHarvest, PlantHarvestArg, PlantHarvestRes)

	void RpcC2M_PlantHarvest::OnCall(const PlantHarvestArg &roArg, PlantHarvestRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"role is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	if (roArg.quest_type() == KKSG::FRIEND)
	{
		LogWarn("err type ,can't harvest in friend garden");
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return ;
	}

	Garden* garden = NULL;
	GuildGardenPlant* guild_plant = NULL;
	UINT64 garden_id = role->GetID();
	std::string garden_name = role->GetName();
	if (roArg.quest_type() == KKSG::MYSELF)
	{
		garden = GardenUtility::Instance()->GetMyGarden(garden_id,garden_name);
		if (NULL == garden)
		{
			LogWarn("not find the garden,role id is : %llu",role->GetID());
			roRes.set_result(KKSG::ERR_INVALID_REQUEST);
			return;
		}		
	}
	else if (roArg.quest_type() == KKSG::GUILD)
	{
		GuildGarden* guild_garden = GardenUtility::Instance()->GetGuildGarden(roArg.garden_id());
		if (NULL == guild_garden)
		{
			LogWarn("not find the guild garden,role id is : %llu",role->GetID());
			roRes.set_result(KKSG::ERR_INVALID_REQUEST);
			return ;
		}
		guild_plant =  guild_garden->GetGuildPlant();
		if (NULL == guild_plant)
		{
			LogWarn("not find the guild garden plant info ,role id is : %llu",role->GetID());
			roRes.set_result(KKSG::ERR_INVALID_REQUEST);
			return ;
		}
	}

	if (roArg.quest_type() == KKSG::GUILD)
	{
		// 确保是自己种的
		bool owner = false;
		const PlantInfoStore& store_info = guild_plant->GetPlantInfoStore();
		auto it = store_info.plantinfo_record.find(roArg.farmland_id());
		auto it_end = store_info.plantinfo_record.end();
		if (it != it_end)
		{
			if (role->GetID() == it->second->owner )
			{
				owner = true;
			}
		}

		if (!owner)
		{
			SSError<<" harvest err  farmland id  :"<<roArg.farmland_id()<<END;
			roRes.set_result(KKSG::ERR_INVALID_REQUEST);
			return ;
		}
	}

	std::vector<ItemDesc> gain_items;
	bool havest = false;
	bool extra = false;

	UINT32 result = 0;
	bool pre_havest = false;
	bool pre_extra = false;
	bool pre_richcorp = false;
	bool richcorp = false;
	 
	if (roArg.quest_type() == KKSG::MYSELF)
	{
		const PlantInfoStore&  store = garden->GetPlantInfoStore();
		pre_havest = store.harvest;
		pre_extra = store.extra;
		pre_richcorp = store.rich_crop;

		OneOwnerScene* owner_scene = CSceneManager::Instance()->FindOwnerSceneBySceneId(FAMILYGARDEN_MAP_ID,role->GetSceneID());
		OneOwnerScene* garden_scene = CSceneManager::Instance()->FindOwnerScene(FAMILYGARDEN_MAP_ID,role->GetID());
		if (owner_scene == NULL  || garden_scene == NULL)
		{
			roRes.set_result(KKSG::ERR_INVFIGHT_ME_SCENE);
			return; 
		}		
		if (owner_scene->sceneid  == garden_scene->sceneid)
		{
			result = garden->PlantHarvest(roArg.farmland_id(),gain_items,havest,extra);
		}
		else
		{
			result = KKSG::ERR_INVFIGHT_ME_SCENE;
		}
	}
	else
	{
		if (role->GetMapID() == GUILD_MAP_ID)
		{
			result = guild_plant->PlantHarvest(roArg.farmland_id(),gain_items,havest,extra);
		}
		else
		{
			result = KKSG::ERR_INVFIGHT_ME_SCENE;
		}
	}
	if (result != KKSG::ERR_SUCCESS)
	{
		roRes.set_result((KKSG::ErrorCode)result);
		return ;
	}
	if (havest)
	{
		roRes.set_harvest(true);
	}
	if (extra)
	{
		roRes.set_extra(true);
	}

	if (!pre_havest)
	{
		// 第一次收获
		EventMgr::Instance()->AddEvent(role->GetID(), ACH_EVENT_OFFSET+ACHIEVE_COM_GARDEN,firstHavest);
	}

	if (!pre_extra && extra)
	{
		// 第一次额外收获
		EventMgr::Instance()->AddEvent(role->GetID(), ACH_EVENT_OFFSET+ACHIEVE_COM_GARDEN,firstExtra);
	}

	if (!pre_richcorp && havest)
	{
		// 第一次丰收
		EventMgr::Instance()->AddEvent(role->GetID(), DESIGNATION_COM_TYPE_GARDEN,firstRichCorp);
	}

	RpcM2G_MsGiveItem* rpc = RpcM2G_MsGiveItem::CreateRpc();
	rpc->m_oArg.set_roleid(role->GetID());
	rpc->m_oArg.set_reason(ItemFlow_Garden);
	rpc->m_oArg.set_subreason(ItemFlow_Garden_PlantSeed);

	auto it = gain_items.begin();
	auto it_end = gain_items.end();
	for (; it != it_end; ++it)
	{
		KKSG::ItemBrief* item = rpc->m_oArg.add_items();
		item->set_itemid(it->itemID);
		item->set_itemcount((UINT32)it->itemCount);

		KKSG::ItemBrief* itembf = roRes.add_items();
		itembf->set_itemid(it->itemID);
		itembf->set_itemcount((UINT32)it->itemCount);
	}

	GSLink::Instance()->SendToLine(role->GetGsLine(), *rpc);

	RoleTaskMgr::Instance()->UpdateTask(role->GetID(),KKSG::TaskOper_Add,KKSG::TaskConn_Activity,KKSG::TaskActType_GardenHarvest);

	roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcC2M_PlantHarvest::OnDelayReplyRpc(const PlantHarvestArg &roArg, PlantHarvestRes &roRes, const CUserData &roUserData)
{
}
