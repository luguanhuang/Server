#include "pch.h"
#include "guildcard.h"
#include "guild/guild_garden.h"
#include "guild/guild.h"
#include <time.h>

GuildGarden::GuildGarden(Guild* guild)
{
	m_guild = guild;
	guild_level_ = guild->GetLevel();
	add_log_func_ = std::bind(
		&GuildGarden::AddEventLog,this, 
		std::placeholders::_1,		
		std::placeholders::_2, 
		std::placeholders::_3, 		
		std::placeholders::_4,
		std::placeholders::_5,
		std::placeholders::_6);

	guild_plant_ = new GuildGardenPlant();
	guild_plant_->SetAddLogFunc(add_log_func_);
}

GuildGarden::~GuildGarden()
{
	if ( NULL != guild_plant_)
	{
		delete guild_plant_;
		guild_plant_ = NULL;
	}

	auto logit = garden_log_.event_log.begin();
	auto logit_end = garden_log_.event_log.end();
	for (; logit != logit_end; ++logit)
	{
		delete *logit;
	}
}

bool GuildGarden::IsChanged()
{
	return guild_plant_->IsChanged();
}

bool GuildGarden::Load(const KKSG::GuildAllDBData& guilddata)
{
	if (guilddata.has_garden() && !guilddata.garden().empty())
	{		
		KKSG::GuildGardenInfo info;
		info.ParseFromString(guilddata.garden());

		std::string guild_name = guilddata.name();
		UINT64 guild_id = guilddata.gid();

		if (NULL != guild_plant_)
		{
			guild_plant_->LoadGuildInfo(guild_id,guild_name, info);
		}
		else
		{
			return false;
		}

		if (!info.has_eventlog() && info.eventlog().empty())
		{
			return true;
		}

		KKSG::GardenStoreLog  garden_evnetlog;
		if (!garden_evnetlog.ParseFromString(info.eventlog()))
		{
			return  false;
		}

		int eventlog_count = garden_evnetlog.garden_eventlog_size();
		for (int i=0; i < eventlog_count; ++i)
		{	
			const KKSG::GardenEventLog& event_log = garden_evnetlog.garden_eventlog(i);
			KKSG::GardenEventLog* log = new KKSG::GardenEventLog();		

			log->set_role_id(event_log.role_id());
			log->set_role_name(event_log.role_name());
			log->set_event_type(event_log.event_type());
			log->set_occur_time(event_log.occur_time());	
			log->set_target(event_log.target());
			log->set_result(event_log.result());
			garden_log_.event_log.push_back(log);
		}
	}

	return true;
}

void GuildGarden::Save(KKSG::GuildAllDBData& guilddata)
{
	guilddata.mutable_garden()->assign(ToBinary());
}

const std::string GuildGarden::ToBinary()
{
	KKSG::GuildGardenInfo info;
	info.set_plant(guild_plant_->ToBinary());
	return info.SerializeAsString();
}


void GuildGarden::AddEventLog(
	PlantInforRecord* record,
	const UINT64 role_id,
	const std::string& role_name,
	const UINT32 event_type,
	const UINT32 target,
	const bool result)
{
	KKSG::GardenEventLog* log = new KKSG::GardenEventLog();
	log->set_role_id(role_id);
	log->set_role_name(role_name);
	log->set_event_type(event_type);
	log->set_occur_time((UINT32)time(NULL));
	log->set_target(target);
	log->set_result(result);
	if (record != NULL)
	{
		record->record_event_log.push_back(*log);
	}
	while (garden_log_.event_log.size() > GetGlobalConfig().GardenLogMax)
	{		
		KKSG::GardenEventLog* temp = garden_log_.event_log.back();
		delete temp;
		garden_log_.event_log.pop_back();
	}
	garden_log_.event_log.push_front(log);
}


GuildGardenPlant* GuildGarden::GetGuildPlant()
{
	UINT32 level =  m_guild->GetLevel();
	if (level != guild_level_ 
		|| guild_plant_->GetPlantInfoStore().allow_plantid.size() == 0)
	{
		std::set<UINT32> farmland;
		auto it = GetGlobalConfig().BreakFarmlandLevel_Guild.begin();
		auto it_end = GetGlobalConfig().BreakFarmlandLevel_Guild.end();
		for (; it != it_end; ++it)
		{
			if (it->seq[1] <= level)
			{
				farmland.insert(it->seq[0]);
			}
		}

		if (guild_plant_->GetPlantInfoStore().allow_plantid.size() < farmland.size())
		{
			guild_plant_->SetFarmland(farmland);
		}

		guild_level_ = level;
	}

	return guild_plant_;
}



void GuildGarden::GetPlantInfo(std::vector<GardenPlantStatus>& plant_status)
{
	const PlantInfoStore& store =GetPlantInfoStore();
	auto it = store.plantinfo_record.begin();
	auto it_end = store.plantinfo_record.end();		
	for ( ; it !=it_end; ++it)
	{
		UINT32 farmland_id = it->first;		
		UINT32 seed_id = it->second->seed_id;
		UINT32 state = it->second->plant_grow_state;	
		UINT32 sprite_id =0;

		UINT32 buffid = 0;
		switch (state)
		{
		case  KKSG::growDrought:
			{
				buffid = GetGlobalConfig().PlantGrowDroughtBuff;
			} 
			break;
		case  KKSG::growPest:
			{
				buffid = GetGlobalConfig().PlantGrowPestBuff;
			}
			break;
		case KKSG::growSluggish:
			{
				buffid = GetGlobalConfig().PlantGrowSluggish;
			}
			break;
		case KKSG::growMature:
			{
				buffid = GetGlobalConfig().PlantGrowMatureBuff;
			}
			break;
		default:
			break;
		}

		auto rowdata = GardenConfig::Instance()->GetSeed().GetBySeedID(seed_id);
		if (rowdata == NULL)
		{
			return ;
		}

		UINT32 stage = it->second->notice_times;
		if (stage == PLANT_GROWUP_FIRST)
		{
			sprite_id = rowdata->FirstGrowModelSpriteId;
		}
		else if (stage == PLANT_GROWUP_SECOND)
		{
			sprite_id = rowdata->SecondGrowSpriteId;
		}
		else 
		{
			sprite_id = rowdata->RipeSpriteID;
		}

		GardenPlantStatus plant;
		plant.farmland_id = farmland_id;
		plant.sprite_id = sprite_id;
		plant.buff_id = buffid;

		plant_status.push_back(plant);
	}
}

// plant 
void GuildGarden::AddGrownAmount(
	const UINT32 farmland_id,
	const UINT32 amount)
{
	guild_plant_->AddGrownAmount(farmland_id,amount);
}