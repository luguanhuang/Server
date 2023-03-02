#include "pch.h"
#include "garden/rpcc2m_fetchplantinfo.h"
#include "garden/garden_manager.h"
#include "role/rolemanager.h"
#include "garden/garden_utility.h"
// generate by ProtoGen at date: 2016/10/2 20:17:40

RPC_SERVER_IMPLEMETION(RpcC2M_FetchPlantInfo, FetchPlantInfoArg, FetchPlantInfoRes)

	void RpcC2M_FetchPlantInfo::OnCall(const FetchPlantInfoArg &roArg, FetchPlantInfoRes &roRes)
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

	Rcit_const it_end ;
	if (roArg.quest_type() == KKSG::GUILD)
	{
		it_end= guild_plant->GetPlantInfoStore().plantinfo_record.end();
	}
	else
	{
		it_end= garden->GetPlantInfoStore().plantinfo_record.end();
	}
	if (roArg.farmland_id() > 0 )
	{
		Rcit_const it;
		if (roArg.quest_type() == KKSG::GUILD)
		{
			it = guild_plant->GetPlantInfoStore().plantinfo_record.find(roArg.farmland_id());
		}
		else
		{
			it = garden->GetPlantInfoStore().plantinfo_record.find(roArg.farmland_id());
		}
		if (it != it_end)
		{
			GardenUtility::Instance()->AddPlantInfoToRes(roArg.farmland_id(),*(it->second),roRes);		
		}
	}
	else
	{
		Rcit_const it;		
		if (roArg.quest_type() == KKSG::GUILD)
		{
			it = guild_plant->GetPlantInfoStore().plantinfo_record.begin();	
		}
		else
		{
			it = garden->GetPlantInfoStore().plantinfo_record.begin();	
		}
		for ( ; it !=it_end; ++it)
		{
			GardenUtility::Instance()->AddPlantInfoToRes(it->first,*(it->second),roRes);					
		}

		std::set<UINT32>::iterator idit;
		std::set<UINT32>::iterator idit_end;
		if (roArg.quest_type() == KKSG::GUILD)
		{
			idit = guild_plant->GetPlantInfoStore().allow_plantid.begin();
			idit_end = guild_plant->GetPlantInfoStore().allow_plantid.end();
		}
		else
		{
			idit = garden->GetPlantInfoStore().allow_plantid.begin();
			idit_end = garden->GetPlantInfoStore().allow_plantid.end();
		}
		for (; idit != idit_end; ++idit)
		{
			roRes.add_farmland_id(*idit);
		}			
	}

	if (roArg.quest_type() == KKSG::GUILD)
	{
		if (guild_plant->GetPlantInfoStore().sprite_id > 0 &&
			guild_plant->GetPlantInfoStore().sprite_durations > 0)
		{
			roRes.set_sprite_id(guild_plant->GetPlantInfoStore().sprite_id);
		}
	}
	else
	{
		if (garden->GetPlantInfoStore().sprite_id > 0 &&
			garden->GetPlantInfoStore().sprite_durations > 0)
		{
			roRes.set_sprite_id(garden->GetPlantInfoStore().sprite_id);
		}
	}

	/*std::vector<GardenPlantStatus> plant_status;
	garden->GetPlantInfo(plant_status);
	*/
	roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcC2M_FetchPlantInfo::OnDelayReplyRpc(const FetchPlantInfoArg &roArg, FetchPlantInfoRes &roRes, const CUserData &roUserData)
{
}
