#include "pch.h"
#include "garden/garden_utility.h"
#include "garden/ptcm2g_gardeneventntf.h"
#include "garden/rpcm2g_gardenplayevent.h"
#include "garden/rpcm2g_gardenplantchangestage.h"
#include "role/rolesummarymgr.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "network/gslink.h"
#include "define/tlogenum.h"
#include "scene/scenemanager.h"
#include "friend/friendop.h"
#include "role/rolesummarymgr.h"
#include "role/rolemanager.h"
#include "garden/ptcm2c_gardenbanquetntf.h"
#include "garden/garden_manager.h"
#include "event/eventmgr.h"

INSTANCE_SINGLETON(GardenUtility);

GardenUtility::GardenUtility()
{
}

GardenUtility::~GardenUtility()
{
}

bool GardenUtility::GetGardenNameForGuild(
	const UINT64 role_id,
	const UINT64 garden_id,
	std::string& garden_name)
{
	bool isguild = false;
	UINT64 gid = CGuildMgr::Instance()->GetRoleGuild(role_id);
	if (gid != INVALID_GUILDID)
	{
		Guild *pGuild  = CGuildMgr::Instance()->GetGuild(gid);
		if (pGuild != NULL)
		{
			if ( garden_id == pGuild->GetID())
			{
				isguild = true;				
				garden_name = pGuild->GetName();
			}
		}
	}

	return isguild;
}

bool GardenUtility::GetGardenNameForFriend(
	const UINT64 garden_id,	
	std::string& garden_name)
{
	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(garden_id);
	if (NULL == summary)
	{			
		SSWarn<<"summary is NULL, role  id:"<<garden_id<<END;
		return false;
	}

	garden_name =summary->GetName();

	return true;
}

void GardenUtility::GetGardenName(
	const UINT64 role_id,
	const UINT64 garden_id,
	std::string& garden_name)
{
	bool result = GetGardenNameForGuild( 
		role_id,
		garden_id,
		garden_name);
	if (result)
	{
		return;
	}

	result= GetGardenNameForFriend(
		garden_id,	
		garden_name);

	if (!result)
	{
		SSWarn<<"not guild and not friend ,the roleid is :"<<role_id<<" and garden_id is :" << garden_id <<END;
		return;
	}
}

// 把plant记录添加到res中
void GardenUtility::AddPlantInfoToRes(
	const UINT32 farmland_id,
	const PlantInforRecord& record,
	KKSG::FetchPlantInfoRes &roRes)
{
	KKSG::PlantInfo* data = roRes.add_plant_info();
	data->set_owner(record.owner);
	data->set_farmland_id(farmland_id);
	data->set_seed_id(record.seed_id);
	data->set_plant_grow_state((KKSG::PlantGrowState)record.plant_grow_state);
	data->set_growup_amount(record.growup_amount);
	data->set_mature_duration(record.mature_duration);
	data->set_growup_cd(record.growup_cd);
	data->set_stealed_times(record.stealed_times);
	data->set_notice_times(record.notice_times);
	auto logit = record.record_event_log.begin();
	auto logit_end = record.record_event_log.end();
	for (; logit != logit_end; ++logit)
	{
		KKSG::GardenEventLog*  templog =  data->add_event_log();
		templog->set_role_id(logit->role_id());
		templog->set_role_name(logit->role_name());
		templog->set_event_type(logit->event_type());
		templog->set_occur_time(logit->occur_time());
		templog->set_target(logit->target());
		templog->set_result(logit->result());
	}		
}

void GardenUtility::NotifyEvnetChangeNtf(
	const KKSG::GardenPlayStageChange& arg,
	const INT64 garden_id,
	const bool guild_garden)
{
	OneOwnerScene* onescene = NULL;
	if (guild_garden)
	{
		onescene = CSceneManager::Instance()->FindOwnerScene(GUILD_MAP_ID,garden_id);		
	}
	else
	{
		onescene = CSceneManager::Instance()->FindOwnerScene(FAMILYGARDEN_MAP_ID,garden_id);
	}

	UINT32 scene_uid = (NULL == onescene) ? 0 : onescene->sceneid;
	if (0 == scene_uid)
	{
		return ;
	}

	CScene* scene = CSceneManager::Instance()->GetScene(scene_uid);
	if (scene == NULL)
	{
		return;
	}

	if (!guild_garden)
	{
		CheckGardenHintNotifyMS(arg,garden_id);
	}

	PtcM2G_GardenEventNtf msg;	
	msg.m_Data.set_scene_uid(scene_uid);
	msg.m_Data.set_event_type(arg.event_type());
	switch (arg.event_type())
	{	
	case KKSG::PLANT_STATE_CHANGE:	
		{
			msg.m_Data.set_farmland_id(arg.farmland_id());
			UINT32 buffid = 0;
			if (arg.grow_state()== KKSG::growDrought)
			{
				buffid = GetGlobalConfig().PlantGrowDroughtBuff;
			} 
			else if (arg.grow_state()== KKSG::growPest)
			{
				buffid = GetGlobalConfig().PlantGrowPestBuff;
			}
			else  if (arg.grow_state()== KKSG::growSluggish)
			{
				buffid = GetGlobalConfig().PlantGrowSluggish;
			} 
			else if (arg.grow_state()== KKSG::growCorrect)
			{
				buffid = GetGlobalConfig().PlantGrowNormalBuff;
			}
			else if (arg.grow_state()== KKSG::growCD)
			{
				buffid = 0;
			}		

			msg.m_Data.set_buff_id(buffid);

			LogDebug("JUST_SEE_PLANT_STATE_CHANGE farmland:%d ,scene_uid:%d, buffid:%d", 
				msg.m_Data.farmland_id(), 
				msg.m_Data.scene_uid(),
				msg.m_Data.buff_id()); 
		}
		break;
	case KKSG::PLANT_DELETE:
		{	
			msg.m_Data.set_garden_id(garden_id);
			msg.m_Data.set_farmland_id(arg.farmland_id());	
		}
		break;
	case KKSG::PLANT_SPRITE:
		{
			msg.m_Data.set_garden_id(garden_id);
			msg.m_Data.set_sprite_id(arg.sprite_id());
			msg.m_Data.set_exist(arg.exist());
			LogDebug("PLANT_SPRITE sprite_id :%d,exist:%d ",arg.sprite_id(),arg.exist()); 
		}
		break;
	case KKSG::PLANT_MATURE:
		{
			msg.m_Data.set_garden_id(garden_id);
			msg.m_Data.set_farmland_id(arg.farmland_id());

			LogDebug("PLANT_MATURE farmland:%d ",arg.farmland_id()); 
		}
		break;
	case KKSG::BANQUET_STAGE:
		{
			msg.m_Data.set_garden_id(garden_id);
			msg.m_Data.set_banquet_id(arg.banquet_id());
			msg.m_Data.set_banquet_stage(arg.banquet_stage());
			int guests_size = arg.guests_size();
			
			for (int i = 0 ; i < guests_size; ++i)
			{				
				KKSG::MapIntItem* guest = msg.m_Data.add_guests();
				guest->set_key(arg.guests(i).key());
				guest->set_value(arg.guests(i).value());	
				LogDebug(" role %llu and  this week banquettimes: %d ", msg.m_Data.guests(i).key(),msg.m_Data.guests(i).value());
			}

			// 
			if (0 == arg.banquet_stage())
			{
				std::string garden_name;
				GardenUtility::Instance()->GetGardenName(garden_id,
					garden_id,
					garden_name);
				Garden* garden = GardenManager::Instance()->GetGarden(garden_id,garden_name);
				if (NULL != garden)
				{
					if ( 1 == garden->GetBanquetTimes())
					{
						// 第一次举办宴会
						EventMgr::Instance()->AddEvent(garden_id, DESIGNATION_COM_TYPE_GARDEN,firstBanquet); 
					} 
					else if ( BANQUET_TEN == garden->GetBanquetTimes() )
					{						
						// 第10次举办宴会
						EventMgr::Instance()->AddEvent(garden_id, DESIGNATION_COM_TYPE_GARDEN,tenBanquet); 
					}
				}

				for (int i = 0 ; i < guests_size; ++i)
				{
					if (garden_id !=  arg.guests(i).key())
					{
						// 第一次参加宴会，如果不是第一次，这个头衔已经有了也不会重复加
						EventMgr::Instance()->AddEvent(arg.guests(i).key(), DESIGNATION_COM_TYPE_GARDEN,firstJoinBanquet); 
					}
				}
			}
		}
		break;
	case KKSG::FISH_FACE:
		// 借用了gardenid字段
		msg.m_Data.set_garden_id(arg.role_id());
		break;
	case KKSG::FISH_RESULT:
		{// 钓鱼的结果
			msg.m_Data.set_garden_id(arg.role_id());
			msg.m_Data.set_exist(arg.exist());
		}
		break;
	case KKSG::FISH_STOP:
		{
			// stop fish
			msg.m_Data.set_garden_id(arg.role_id());			
		}
		break;
	default:
		break;
	}
	
	GSLink::Instance()->SendToLine(scene->GetLine(), msg);	
}

void GardenUtility::RpcPlantChangeStage(
	const UINT64 garden_id,
	const UINT32 farmland_id,
	const UINT32 sprite_id,
	const UINT32 grow_state,
	bool guild_garden
	)
{
	OneOwnerScene* onescene = NULL;
	if (guild_garden)
	{
		onescene = CSceneManager::Instance()->FindOwnerScene(GUILD_MAP_ID,garden_id);
	}
	else
	{
		onescene = CSceneManager::Instance()->FindOwnerScene(FAMILYGARDEN_MAP_ID,garden_id);
	}
	UINT32 scene_uid = NULL == onescene ? 0 : onescene->sceneid;
	CScene* scene = CSceneManager::Instance()->GetScene(scene_uid);
	if (scene == NULL)
	{
		return;
	}

	UINT32 buffid = 0;
	if (KKSG::growDrought == grow_state )
	{
		buffid = GetGlobalConfig().PlantGrowDroughtBuff;
	} 
	else if (KKSG::growPest == grow_state )
	{
		buffid = GetGlobalConfig().PlantGrowPestBuff;
	}
	else  if (KKSG::growSluggish == grow_state)
	{
		buffid = GetGlobalConfig().PlantGrowSluggish;
	} 

	RpcM2G_GardenPlantChangeStage* rpc =  RpcM2G_GardenPlantChangeStage::CreateRpc();
	rpc->m_oArg.set_garden_id(garden_id);
	rpc->m_oArg.set_farmland_id(farmland_id);
	rpc->m_oArg.set_isguild(guild_garden);
	rpc->m_oArg.set_sprite_id(sprite_id);
	rpc->m_oArg.set_scene_uid(scene_uid);
	rpc->m_oArg.set_buff_id(buffid);

	GSLink::Instance()->SendToLine(scene->GetLine(), *rpc);

	LogDebug("RpcPlantChangeStage farmland:%d ,sprite_id:%d ,scene_uid:%d ,buff_id:%d ", 
		farmland_id, 
		sprite_id,
		scene_uid,
		buffid); 
}


void GardenUtility::FriendOrderByDegree(
	CRole* role,
	const std::vector<UINT64>& all,
	std::vector<UINT64>& guests)
{
	auto it = all.begin();
	auto it_end = all.end();
	std::multimap<UINT32,UINT64> friends;
	FriendOp op(role->GetID());
	for (; it != it_end; ++it)
	{
		if ( *it != role->GetID())
		{
			friends.insert(std::make_pair(op.GetDegree(*it),*it));
			LogDebug("FriendOrderByDegree role id  is %llu,degree:%d", *it,op.GetDegree(*it));
		}
	}

	// 先把自己加进来
	UINT32 count = 1;
	guests.push_back(role->GetID());

	auto git = friends.rbegin();
	auto git_end = friends.rend();
	for (; git != git_end && count < GetGlobalConfig().GardenBanquetGuests; ++git,++count)
	{
		guests.push_back(git->second);
		LogDebug("FriendOrderByDegree  guests  role id  is %llu ", git->second);
	}
}


// 通知玩家开始宴会
void GardenUtility::NotifyBanquetQuests(
	const UINT64 garden_id,
	const UINT32 banquet_id,
	const std::vector<UINT64>& guests)
{
	auto it = guests.begin();
	auto it_end = guests.end();
	for (; it != it_end; ++it)
	{
		CRole* crole = CRoleManager::Instance()->GetByRoleID(*it);
		if(crole == NULL)
		{
			LogError("Can't find role %llu when garden banquet  scene OnDelayReplyRpc", *it);
			return;
		}
		PtcM2C_GardenBanquetNtf ntf;		
		ntf.m_Data.set_banquet_id(banquet_id);
		ntf.m_Data.set_garden_id(garden_id);

		LogDebug("sent PtcM2C_GardenBanquetNtf  ntf garden_id %llu and  banquet_id is %d", garden_id,banquet_id);
		crole->Send(ntf);
	}
}

Garden* GardenUtility::GetMyGarden(
	const UINT64 garden_id ,
	const std::string& garden_name)
{
	return GardenManager::Instance()->GetGarden(garden_id,garden_name);
}

Garden* GardenUtility::GetFriendGarden(
	const UINT64 garden_id ,
	const std::string& garden_name)
{
	return GardenManager::Instance()->GetGarden(garden_id,garden_name);
}

GuildGarden* GardenUtility::GetGuildGarden(const UINT64 garden_id)
{
	GuildGarden* guild_garden = GardenManager::Instance()->GuildGetGarden(garden_id);
	if (NULL != guild_garden)
	{
		return guild_garden;
	}

	auto guild = CGuildMgr::Instance()->GetGuild(garden_id);
	if (NULL == guild)
	{
		return NULL;
	}

	guild_garden = guild->Get<GuildGarden>();
	const std::string& name = guild_garden->GetName();
	if (name.empty())
	{
		guild_garden->SetGuildNameID(guild->GetName(),guild->GetID());
	}

	GardenManager::Instance()->AddGuildGarden(garden_id,guild_garden);

	return guild_garden;
}

Garden* GardenUtility::GetGardenByRoleId(const UINT64 role_id )
{
	CRole* role = CRoleManager::Instance()->GetByRoleID(role_id);
	if (NULL == role)
	{
		SSWarn<<"role is NULL, garden_id id:"<<role_id<<END;
		return NULL;
	}

	Garden*  garden = GardenUtility::Instance()->GetMyGarden(role->GetID(),role->GetName());
	if (NULL == garden)
	{		
		LogWarn("not find the garden,role id is : %llu",role_id);		
		return NULL;
	}

	return garden;
}

UINT32 GardenUtility::CheckPlantStauts(const PlantInfoStore& store)
{
	UINT32 count = 0 ;
	auto it = store.plantinfo_record.begin();
	auto it_end = store.plantinfo_record.end();
	for (; it != it_end; ++it)
	{
		if (KKSG::growMature == it->second->plant_grow_state )
		{
			++count;
		}
	}

	return count;
}

void GardenUtility::CheckGardenHintNotifyMS(
	const KKSG::GardenPlayStageChange& arg,
	const UINT64 garden_id)
{
	// 进入了别人的家园
	std::string garden_name;
	GardenUtility::Instance()->GetGardenName(garden_id,
		garden_id,
		garden_name);

	Garden* garden = GardenManager::Instance()->GetGarden(garden_id,garden_name);
	if (NULL == garden)
	{
		return ;
	}
	const PlantInfoStore& store = garden->GetPlantInfoStore();
	UINT32 mature_count = CheckPlantStauts(store);

	bool exist_sprite = false;
	if (store.sprite_id > 0 && store.sprite_durations > 0)
	{
		exist_sprite = true;
	}

	bool hint = false;
	bool needNotice = true;
	switch (arg.event_type())
	{		
	case KKSG::PLANT_DELETE:
		// 没有成熟的地块，且没有小妖
		if (!exist_sprite && mature_count < 1)
		{
			hint = false;
		}
		else
		{
			hint = true;
		}
		break;
	case KKSG::PLANT_SPRITE:
		{
			// 小妖消失且没有成熟地块
			if (!arg.exist() && mature_count < 1)
			{
				hint = false;				
			}
			else
			{
				hint = true;
			}
		}
		break;
	case KKSG::PLANT_MATURE:
		{
			hint = true;
		}
		break;
	default:
		needNotice = false;
		break;
	}

	if (needNotice)
	{
		CRole* role = CRoleManager::Instance()->GetByRoleID(garden_id);
		if (NULL == role)
		{
			SSWarn<<"role is NULL, role id:"<<garden_id<<END;
			return;
		}	

		role->HintNotify(GARDEN_SYSTEM_ID,hint);
	}
}