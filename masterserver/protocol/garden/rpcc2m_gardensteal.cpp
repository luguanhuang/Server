#include "pch.h"
#include "garden/rpcc2m_gardensteal.h"
#include "garden/garden_manager.h"
#include "role/rolemanager.h"
#include "garden/garden_utility.h"
#include "network/gslink.h"
#include "define/tlogenum.h"
#include "loghelper/tlogr.h"
#include "event/eventmgr.h"
#include "friend/friendop.h"
#include "common/rpcm2g_msgiveitem.h"
#include "scene/scenemanager.h"
#include "task/roletaskmgr.h"
// generate by ProtoGen at date: 2016/10/24 12:28:04

RPC_SERVER_IMPLEMETION(RpcC2M_GardenSteal, GardenStealArg, GardenStealRes)

	void RpcC2M_GardenSteal::OnCall(const GardenStealArg &roArg, GardenStealRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"role is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	Garden* mygarden = GardenUtility::Instance()->GetMyGarden(role->GetID(),role->GetName());
	if (NULL == mygarden)
	{
		LogWarn("not find the garden,role id is : %llu",role->GetID());
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	if (mygarden->GetPlantInfoStore().steal_count >=  GetGlobalConfig().GardenStealCount)
	{
		LogDebug("steal count is excel !");
		roRes.set_result(KKSG::ERR_GARDEN_STEAL_LIMIT);
		return;
	}

	if (roArg.quest_type() == KKSG::MYSELF
		|| role->GetID() == roArg.garden_id())
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<" can't steal owner garden ,the role id is  :"<<role->GetID()<<END;
		return;
	}

	Garden* garden = NULL;
	GuildGardenPlant* guild_plant = NULL;
	UINT32 result = 0;
	UINT64 garden_id = roArg.garden_id();
	std::string garden_name = role->GetName();
	if (roArg.quest_type() == KKSG::FRIEND)
	{
		// 是否好友
		FriendOp op(role->GetID());	
		if(!op.IsFriend(roArg.garden_id()))
		{
			LogWarn("not friend ,role id is : %llu,friend id is : %llu",role->GetID(),roArg.garden_id());
			roRes.set_result(KKSG::ERR_INVALID_REQUEST);
			return;
		}

		garden = GardenUtility::Instance()->GetFriendGarden(roArg.garden_id(),garden_name);
		if (NULL == garden)
		{
			LogWarn("not find the garden,role id is : %llu",roArg.garden_id());
			roRes.set_result(KKSG::ERR_INVALID_REQUEST);
			return;
		}
	}
	else if (roArg.quest_type() == KKSG::GUILD)
	{
		// 是否自己的工会
		if(roArg.garden_id() != CGuildMgr::Instance()->GetGuildIdByRoleId(role->GetID()))
		{
			LogWarn("guild id is err ,guild id : %llu ,role id is : %llu",roArg.garden_id(),role->GetID());
			roRes.set_result(KKSG::ERR_INVALID_REQUEST);
			return ;
		}

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

	// 1.检查是否偷过
	bool steal = false;
	Rcit_const rcit ;
	Rcit_const rcit_end ;
	bool pre_steal = true;
	if (roArg.quest_type() == KKSG::GUILD)
	{
		rcit = guild_plant->GetPlantInfoStore().plantinfo_record.find(roArg.farmland_id());
		rcit_end = guild_plant->GetPlantInfoStore().plantinfo_record.end();
		//pre_steal = guild_plant->GetPlantInfoStore().steal;
	}
	else
	{
		rcit = garden->GetPlantInfoStore().plantinfo_record.find(roArg.farmland_id());
		rcit_end = garden->GetPlantInfoStore().plantinfo_record.end();
		pre_steal = garden->GetPlantInfoStore().steal;
	}

	if  (rcit != rcit_end)
	{
		auto itlog =  rcit->second->record_event_log.begin();
		auto itlog_end =  rcit->second->record_event_log.end();
		for (; itlog != itlog_end; ++itlog)
		{
			if ( itlog->event_type() == friendSteal 
				&& itlog->role_id() == role->GetID())
			{
				steal = true;
			}
		}
	}

	if (steal)
	{
		SSError<<" role  :"<< role->GetID()<<" have steal " <<END;
		roRes.set_result(KKSG::ERR_GARDEN_STEALEDTIMES_EXCEED);
		return ;
	}

	std::vector<ItemDesc> steal_items;
	if (roArg.quest_type() == KKSG::GUILD)
	{
		if (role->GetMapID() == GUILD_MAP_ID)
		{
			result = guild_plant->StealHarvest(role->GetID(),role->GetName(),roArg.farmland_id(),steal_items);	
		}
		else
		{
			result = KKSG::ERR_INVFIGHT_ME_SCENE;
		}
	}
	else
	{	
		OneOwnerScene* owner_scene = CSceneManager::Instance()->FindOwnerSceneBySceneId(FAMILYGARDEN_MAP_ID,role->GetSceneID());
		OneOwnerScene* garden_scene = CSceneManager::Instance()->FindOwnerScene(FAMILYGARDEN_MAP_ID,roArg.garden_id());
		if (owner_scene == NULL  || garden_scene == NULL)
		{
			roRes.set_result(KKSG::ERR_INVFIGHT_ME_SCENE);
			return; 
		}		
		if (owner_scene->sceneid  == garden_scene->sceneid)
		{
			result = garden->StealHarvest(role->GetID(),role->GetName(),roArg.farmland_id(),steal_items);
		}
		else
		{
			result = KKSG::ERR_INVFIGHT_ME_SCENE;
		}
	}

	if (result == KKSG::ERR_SUCCESS)
	{
		if (!pre_steal && roArg.quest_type() != KKSG::GUILD)
		{
			// 第一次偷菜
			EventMgr::Instance()->AddEvent(role->GetID(), ACH_EVENT_OFFSET+ACHIEVE_COM_GARDEN,firstSteal);
			// 偷过菜了，设置标记
			mygarden->SetSteal();
		}

		// 更新自己偷菜次数
		mygarden->AddStealCount();
		// 将偷菜奖励发给gs
		RpcM2G_MsGiveItem* rpc = RpcM2G_MsGiveItem::CreateRpc();
		rpc->m_oArg.set_roleid(role->GetID());
		rpc->m_oArg.set_reason(ItemFlow_Garden);
		rpc->m_oArg.set_subreason(ItemFlow_Garden_PlantSeed);

		auto it = steal_items.begin();
		auto it_end = steal_items.end();
		for (; it != it_end; ++it)
		{
			KKSG::ItemBrief* itme  = roRes.add_items();
			itme->set_itemid(it->itemID);
			itme->set_itemcount((UINT32)it->itemCount);

			// 发给gs的数据
			KKSG::ItemBrief* item = rpc->m_oArg.add_items();
			item->set_itemid(it->itemID);
			item->set_itemcount((UINT32)it->itemCount);
		}

		GSLink::Instance()->SendToLine(role->GetGsLine(), *rpc);

		TGardenFlow oLog(TLogMgr::Instance()->GetTagString());
		oLog.m_uRoleID = role->GetID();
		oLog.m_uGardenID = roArg.garden_id();
		oLog.m_iQuestType = roArg.quest_type();
		oLog.m_iOp = Tx_Garden_Steal;
		oLog.Do();

		RoleTaskMgr::Instance()->UpdateTask(role->GetID(),KKSG::TaskOper_Add,KKSG::TaskConn_Activity,KKSG::TaskActType_GardenSteal);
	}

	roRes.set_result((KKSG::ErrorCode)result);
}

void RpcC2M_GardenSteal::OnDelayReplyRpc(const GardenStealArg &roArg, GardenStealRes &roRes, const CUserData &roUserData)
{
}
