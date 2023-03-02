#include "pch.h"
#include "garden/rpcc2m_gardenoverview.h"
#include "garden/garden_manager.h"
#include "role/rolemanager.h"
#include "network/gslink.h"
#include "define/tlogenum.h"
#include "garden/garden_config.h"
#include "friend/friendop.h"
#include "role/rolesummarymgr.h"
#include "garden/garden_utility.h"

// generate by ProtoGen at date: 2016/10/15 18:50:00



RPC_SERVER_IMPLEMETION(RpcC2M_GardenOverview, GardenOverviewArg, GardenOverviewRes)

	void RpcC2M_GardenOverview::OnCall(const GardenOverviewArg &roArg, GardenOverviewRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"role is NULL, session id:"<<m_sessionID<<END;
		return;
	}
	//  目前只能获取自己家园的总览
	Garden* garden = GardenUtility::Instance()->GetMyGarden(role->GetID(),role->GetName());
	if (NULL == garden)
	{
		LogWarn("not find the garden,role id is : %llu",role->GetID());
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	const PlantInfoStore&  plant_info =garden->GetPlantInfoStore();
	const FishInfoStore& fish_info =garden->GetFishInfo();
	// 1.被访问次数
	// 2.钓鱼等级
	// 3.种植数量
	// 4.烹饪等级
	roRes.set_visited_times(garden->GetVisitTimes());	
	roRes.set_fish_level(fish_info.fish_level);	
	roRes.set_fish_experiences(fish_info.experiences);
	//roRes.set_plant_amount(plant_info.total_plant_times);	
	// 允许种的地块
	{ 
		std::vector<UINT32> farmland_list;
		CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(role->GetID());
		if (NULL == summary)
		{			
			roRes.set_result(KKSG::ERR_INVALID_REQUEST);
			SSWarn<<"role is NULL, role->GetID() id:"<<role->GetID()<<END;
		}	

		UINT32 level =  summary->GetLevel();
		auto it = GetGlobalConfig().BreakFarmlandLevel.begin();
		auto it_end = GetGlobalConfig().BreakFarmlandLevel.end();
		for (; it != it_end; ++it)
		{
			UINT32 farmland = it->seq[0];
			UINT32 roleleve = it->seq[1];
			if (level >= roleleve)
			{
				garden->AddFarmland(farmland);
			}			
		}		
	}

	roRes.set_plant_farmland_max(plant_info.allow_plantid.size());

	auto itrecord =  plant_info.plantinfo_record.begin();
	auto itrecord_end =  plant_info.plantinfo_record.end();
	for (;itrecord !=  itrecord_end; ++itrecord )
	{
		KKSG::PlantInfo* info =  roRes.add_plant_info();
		info->set_farmland_id(itrecord->first);
		info->set_seed_id(itrecord->second->seed_id);
		info->set_plant_grow_state((KKSG::PlantGrowState)itrecord->second->plant_grow_state);
		info->set_grow_times(itrecord->second->grow_times);
		info->set_growup_amount(itrecord->second->growup_amount);
		info->set_mature_duration(itrecord->second->mature_duration);
		//info->set_start_time(itrecord->second->sta)
		info->set_stealed_times(itrecord->second->stealed_times);
		info->set_growup_cd(itrecord->second->growup_cd);
		// log
	}

	FoodInfo food_info;
	garden->GetGardenCookingInfo(food_info);
	auto foodit = food_info.food_id.begin();
	auto foodit_end = food_info.food_id.end();
	for (; foodit != foodit_end; ++foodit)
	{
		KKSG::MapIntItem* item = roRes.add_food_id();
		item->set_key(foodit->first);
		item->set_value(foodit->second);
	}
	roRes.set_cooking_level(food_info.cooking_level);
	roRes.set_cooking_experiences(food_info.cooking_experiences);

	// 5.3个好友信息
	FriendOp op(role->GetID());
	std::set<FriendInfo> friendlist;
	op.GetFriendListSorted(friendlist);

	UINT32 count =0;
	auto it = friendlist.begin();
	auto it_end = friendlist.end();
	for (; it != it_end && count < GARDEN_TOP_FRIEND; ++it,++count)
	{		
		CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(it->m_roleid);
		if (NULL == summary)
		{
			roRes.set_result(KKSG::ERR_INVALID_REQUEST);			
			SSWarn<<"summary is NULL, role  id:"<<it->m_roleid<<END;
			return;
		}

		KKSG::FriendPlantLog* friend_log = roRes.add_friend_log();
		friend_log->set_role_id(summary->GetID());
		friend_log->set_role_name(summary->GetName());	
		friend_log->set_profession_id(summary->GetProfession());

		Garden* garden = GardenManager::Instance()->GetGarden(summary->GetID(),summary->GetName());
		if (NULL == garden)
		{
			continue;
		}
		const PlantInfoStore& all_record = garden->GetPlantInfoStore();
		auto rcit = all_record.plantinfo_record.begin();
		auto rcit_end = all_record.plantinfo_record.end();
		for (; rcit != rcit_end; ++rcit)
		{
			if (rcit->second->plant_grow_state == KKSG::growCD )
			{
				friend_log->set_abnormal_state(true);
			}
			if (rcit->second->plant_grow_state == KKSG::growMature )
			{
				friend_log->set_mature(true);
			}
		}
		// 好友帮忙次数
		auto ithelp = all_record.help_info.find(summary->GetID());
		auto ithelp_end = all_record.help_info.end();
		if (ithelp != ithelp_end)
		{
			friend_log->set_help_times(ithelp->second);
		}
		if (all_record.sprite_id>0 && all_record.sprite_durations >0 )
		{
			friend_log->set_exist_sprite(true);
		}
	}

	// 6.家园日志
	std::list<KKSG::GardenEventLog*> log;
	garden->GetEventLog(log);
	auto etlog = log.begin();
	auto etlog_end = log.end();
	for (; etlog != etlog_end; ++etlog)
	{
		KKSG::GardenEventLog* eventlog = roRes.add_event_log();		
		eventlog->set_role_id((*etlog)->role_id());
		eventlog->set_role_name((*etlog)->role_name());
		eventlog->set_event_type((*etlog)->event_type());
		eventlog->set_occur_time((*etlog)->occur_time());
		eventlog->set_target((*etlog)->target());
		eventlog->set_result((*etlog)->result());
	}

	LogDebug("garden eventlog size is %d",roRes.event_log().size());

	if (garden->GetPlantInfoStore().sprite_id > 0 &&
		garden->GetPlantInfoStore().sprite_durations > 0)
	{
		roRes.set_sprite_id(garden->GetPlantInfoStore().sprite_id);
	}

	roRes.set_server_time((UINT32)GameTime::GetTime());
	roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcC2M_GardenOverview::OnDelayReplyRpc(const GardenOverviewArg &roArg, GardenOverviewRes &roRes, const CUserData &roUserData)
{
}
