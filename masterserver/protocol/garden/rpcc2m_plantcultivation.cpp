#include "pch.h"
#include "garden/rpcc2m_plantcultivation.h"
#include "garden/garden_manager.h"
#include "role/rolemanager.h"
#include "garden/garden_utility.h"
#include "network/gslink.h"
#include "define/tlogenum.h"
#include "loghelper/tlogr.h"
#include "common/rpcm2g_msgiveitem.h"

// generate by ProtoGen at date: 2016/10/3 16:07:07

RPC_SERVER_IMPLEMETION(RpcC2M_PlantCultivation, PlantCultivationArg, PlantCultivationRes)

	void RpcC2M_PlantCultivation::OnCall(const PlantCultivationArg &roArg, PlantCultivationRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"role is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	Garden* garden = NULL;
	GuildGardenPlant* guild_plant = NULL;
	UINT32 result = 0;
	bool need_award = false;
	UINT32 seed_id = 0;
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
	else if (roArg.quest_type() == KKSG::FRIEND)
	{
		garden = GardenUtility::Instance()->GetFriendGarden(roArg.garden_id(),garden_name);
		if (NULL == garden)
		{
			LogWarn("not find the garden,role id is : %llu",roArg.garden_id());
			roRes.set_result(KKSG::ERR_INVALID_REQUEST);
			return;
		}
		need_award = true;
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
		result = guild_plant->PlantCultivationByPlayer(
			role->GetID(),
			role->GetName(),
			roArg.farmland_id(),
			roArg.operate_type());

		// growup 
		const PlantInfoStore&  plant_info =guild_plant->GetPlantInfoStore();	
		auto it = plant_info.plantinfo_record.find(roArg.farmland_id());
		auto it_end = plant_info.plantinfo_record.end();
		if (it != it_end)
		{
			roRes.set_notice_times(it->second->notice_times);
			roRes.set_growup_amount(it->second->growup_amount);
			if (it->second->owner != role->GetID())
			{
				need_award = true;	
				seed_id = it->second->seed_id;
			}
		}
	}
	else
	{
		result = garden->PlantCultivationByPlayer(
			role->GetID(),
			role->GetName(),
			roArg.farmland_id(),
			roArg.operate_type());

		// growup 
		const PlantInfoStore&  plant_info =garden->GetPlantInfoStore();	
		auto it = plant_info.plantinfo_record.find(roArg.farmland_id());
		auto it_end = plant_info.plantinfo_record.end();
		if (it != it_end)
		{
			roRes.set_growup_amount(it->second->growup_amount);
			seed_id = it->second->seed_id;
		}
	}
	if (result != KKSG::ERR_SUCCESS)
	{
		roRes.set_result((KKSG::ErrorCode)result);
		return ;
	}

	if (need_award && result == KKSG::ERR_SUCCESS)
	{
		// 奖品
		PlantSeed::RowData* rowdata = GardenConfig::Instance()->GetSeed().GetBySeedID(seed_id);
		if (rowdata == NULL)
		{
			roRes.set_result(KKSG::ERR_GARDEN_NOTEXIST_SEEDID);
			return ;
		}
		// 帮助培养的奖品
		RpcM2G_MsGiveItem* rpc = RpcM2G_MsGiveItem::CreateRpc();
		rpc->m_oArg.set_roleid(role->GetID());
		rpc->m_oArg.set_reason(ItemFlow_Garden);
		rpc->m_oArg.set_subreason(ItemFlow_Garden_PlantSeed);
		
		KKSG::ItemBrief* item = rpc->m_oArg.add_items();
		item->set_itemid(rowdata->TrainAward.seq[0]);
		item->set_itemcount(rowdata->TrainAward.seq[1]);

		GSLink::Instance()->SendToLine(role->GetGsLine(), *rpc);
	}
	
	roRes.set_result(KKSG::ERR_SUCCESS);

	TGardenFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_uRoleID = role->GetID();
	oLog.m_iQuestType = roArg.quest_type();
	oLog.m_iOp = Tx_Garden_Cultivation;
	oLog.m_uGardenID = roArg.garden_id();
	oLog.Do();
}

void RpcC2M_PlantCultivation::OnDelayReplyRpc(const PlantCultivationArg &roArg, PlantCultivationRes &roRes, const CUserData &roUserData)
{
}
