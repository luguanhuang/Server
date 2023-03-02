#include "pch.h"
#include "garden/rpcc2m_startplant.h"
#include "common/rpcm2g_msconsumeitems.h"
#include "garden/garden_manager.h"
#include "role/rolemanager.h"
#include "network/gslink.h"
#include "define/tlogenum.h"
#include "scene/scenemanager.h"
#include "garden/rpcm2g_gardenplayevent.h"
#include "garden/garden_utility.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "loghelper/tlogr.h"

// generate by ProtoGen at date: 2016/10/1 21:47:49

RPC_SERVER_IMPLEMETION(RpcC2M_StartPlant, StartPlantArg, StartPlantRes)

	void RpcC2M_StartPlant::OnCall(const StartPlantArg &roArg, StartPlantRes &roRes)
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
		LogWarn("err type ,can't plant in friend garden");
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return ;
	}

	UINT32 result = 0;
	UINT64 garden_id = role->GetID();
	std::string garden_name = role->GetName();
	if (roArg.quest_type() == KKSG::MYSELF)
	{
		Garden* garden = GardenUtility::Instance()->GetMyGarden(garden_id,garden_name);
		if (NULL == garden)
		{
			LogWarn("not find the garden,role id is : %llu",role->GetID());
			roRes.set_result(KKSG::ERR_INVALID_REQUEST);
			return;
		}
		if (roArg.cancel())
		{
			result = garden->CanclePlant(roArg.farmland_id());
		}
		else
		{
			result = garden->CheckPlantCondition(roArg.farmland_id(),roArg.seed_id());
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
		auto guild_plant =  guild_garden->GetGuildPlant();
		if (NULL == guild_plant)
		{
			LogWarn("not find the guild garden plant info ,role id is : %llu",role->GetID());
			roRes.set_result(KKSG::ERR_INVALID_REQUEST);
			return ;
		}
		if (roArg.cancel())
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

			result = guild_plant->CanclePlant(roArg.farmland_id());
		}
		else
		{
			result = guild_plant->CheckPlantCondition(roArg.farmland_id(),roArg.seed_id());
		}
	}
	else 
	{
		LogWarn("err type ,can't plant in friend garden");
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
	}

	// 取消种植的情况下，就需要返回了
	if (roArg.cancel())
	{
		roRes.set_result((KKSG::ErrorCode)result);
		return ;
	}
	if (result != KKSG::ERR_SUCCESS)
	{
		roRes.set_result((KKSG::ErrorCode)result);
		return ;
	}

	auto rowdata = GardenConfig::Instance()->GetSeed().GetBySeedID(roArg.seed_id());
	if (rowdata == NULL)
	{	
		roRes.set_result(KKSG::ERR_GARDEN_NOTEXIST_SEEDID);
		return;
	}

	// gs上去扣除种子
	OneOwnerScene* onescene = NULL;
	if (roArg.quest_type() == KKSG::MYSELF)
	{
		onescene = CSceneManager::Instance()->FindOwnerScene(FAMILYGARDEN_MAP_ID,role->GetID());
	}
	else if (roArg.quest_type() == KKSG::GUILD)
	{
		UINT64 gid = CGuildMgr::Instance()->GetRoleGuild(role->GetID());
		if (gid != INVALID_GUILDID)
		{
			onescene = CSceneManager::Instance()->FindOwnerScene(GUILD_MAP_ID,gid);
		}
		else
		{
			LogWarn("not find the guild garden,role id is : %llu",role->GetID());
			roRes.set_result(KKSG::ERR_INVALID_REQUEST);
			return;
		}
	}

	RpcM2G_GardenPlayEvent* msg = RpcM2G_GardenPlayEvent::CreateRpc();
	msg->m_oArg.set_rpcid(DelayRpc());
	msg->m_oArg.set_role_id(role->GetID());
	msg->m_oArg.set_scene_uid(NULL == onescene ? 0 : onescene->sceneid);
	msg->m_oArg.set_event_type(KKSG::PLANT);
	msg->m_oArg.set_farmland_id(roArg.farmland_id());
	msg->m_oArg.set_seed_id(roArg.seed_id());
	msg->m_oArg.set_sprite_id(rowdata->FirstGrowModelSpriteId);
	if (roArg.quest_type() == KKSG::GUILD)
	{
		msg->m_oArg.set_isguild(true);
	}

	GSLink::Instance()->SendToLine(role->GetGsLine(), *msg);

	LogDebug("START_PLANT farmland:%d ,scene_uid:%d, seed_id:%d", 
		msg->m_oArg.farmland_id(), 
		msg->m_oArg.scene_uid(),
		msg->m_oArg.seed_id()); 
}

void RpcC2M_StartPlant::OnDelayReplyRpc(const StartPlantArg &roArg, StartPlantRes &roRes, const CUserData &roUserData)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"role is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	if ( KKSG::ERR_SUCCESS  !=  (KKSG::ErrorCode)roUserData.m_dwUserData)
	{
		roRes.set_result((KKSG::ErrorCode)roUserData.m_dwUserData);
		return;
	}

	UINT32 result = 0;
	UINT64 garden_id = role->GetID();
	std::string garden_name = role->GetName();
	if (roArg.quest_type() == KKSG::MYSELF)
	{
		Garden* garden = GardenUtility::Instance()->GetMyGarden(garden_id,garden_name);
		if (NULL == garden)
		{
			LogWarn("not find the garden,role id is : %llu",role->GetID());
			roRes.set_result(KKSG::ERR_INVALID_REQUEST);
			return ;
		}		
		result = garden->TryPlantSeed(role->GetID(),roArg.farmland_id(),roArg.seed_id());		
	}
	else if (roArg.quest_type() == KKSG::GUILD)
	{	
		GuildGarden* guild_garden = GardenUtility::Instance()->GetGuildGarden(roArg.garden_id());
		if (NULL == guild_garden)
		{
			LogWarn("not find the guild garden,role id is : %llu",role->GetID());
			roRes.set_result(KKSG::ERR_INVALID_REQUEST);
			return;
		}
		auto guild_plant =  guild_garden->GetGuildPlant();
		if (NULL == guild_plant)
		{
			LogWarn("not find the guild garden plant info ,role id is : %llu",role->GetID());
			roRes.set_result(KKSG::ERR_INVALID_REQUEST);
			return ;
		}

		result = guild_plant->TryPlantSeed(role->GetID(),roArg.farmland_id(),roArg.seed_id());
	}

	roRes.set_grow_state((KKSG::PlantGrowState)result);
	roRes.set_result(KKSG::ERR_SUCCESS);

	TGardenFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_uRoleID = role->GetID();
	oLog.m_uGardenID = roArg.garden_id();
	oLog.m_uSeedID = roArg.seed_id();
	oLog.m_iOp = roArg.quest_type() == KKSG::MYSELF?TX_Garden_PlantPlay:TX_Garden_GuildPlantPlay;
	oLog.m_iQuestType = roArg.quest_type();
	oLog.Do();
}
