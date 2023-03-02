#include "pch.h"
#include "garden/guild_garden_plant.h"
#include "util/XRandom.h"
#include "util/gametime.h"
#include "mail/mailmgr.h"
#include "garden/ptcm2c_spritestatechangentf.h"
#include "role/rolemanager.h"
#include "garden/garden_utility.h"
#include "common/rpcm2g_msgiveitem.h"

GuildGardenPlant::GuildGardenPlant()
{	
	// 工会默认地块
	plant_info_store_.allow_plantid.clear();
}

GuildGardenPlant:: ~GuildGardenPlant() 
{
	auto rcit = plant_info_store_.plantinfo_record.begin();
	auto rcit_end = plant_info_store_.plantinfo_record.end();
	for (; rcit != rcit_end; ++rcit)
	{
		delete rcit->second;
	}
}

void GuildGardenPlant::SetGuildNameID(
	const std::string& garden_name,
	const UINT64 garden_id)
{
	garden_name_ = garden_name;
	garden_id_ = garden_id;
}

const std::string& GuildGardenPlant::GetName()
{
	return garden_name_;
}

bool GuildGardenPlant::IsChanged() 
{
	return swithc_.TestAndReset();
}

bool GuildGardenPlant::LoadGuildInfo(
	const UINT64 guild_id,
	const std::string& guild_name,
	const KKSG::GuildGardenInfo& info) 
{
	if (!info.has_plant() || info.plant().empty())
	{
		return false;
	}

	KKSG::GardenPlantInfo garden_plantinfo;
	if (!garden_plantinfo.ParseFromString(info.plant()))
	{
		return false;
	}

	// 
	int  allow_plantid = garden_plantinfo.allow_plantid_size();
	for (int i=0; i < allow_plantid; ++i)
	{
		plant_info_store_.allow_plantid.insert(garden_plantinfo.allow_plantid(i));
	}

	//plant_info_store_.total_plant_times = garden_plantinfo.total_plant_times();
	//plant_info_store_.total_harverst_amount = garden_plantinfo.total_harverst_amount();
	plant_info_store_.sprite_id = garden_plantinfo.sprite_id();
	plant_info_store_.sprite_durations = garden_plantinfo.sprite_durations();
	plant_info_store_.sprite_appeartimes =  garden_plantinfo.sprite_appeartimes();
	plant_info_store_.sprite_appear_cd = garden_plantinfo.sprite_appear_cd();

	// 种植信息
	int plan_info_count = garden_plantinfo.plant_info().size();
	for (int i=0; i < plan_info_count; ++i)
	{
		PlantInforRecord* info_record = new PlantInforRecord();
		const KKSG::PlantInfo& plant_info = garden_plantinfo.plant_info(i);
		info_record->owner = plant_info.owner();
		info_record->seed_id = plant_info.seed_id();
		info_record->plant_grow_state = plant_info.plant_grow_state();
		info_record->growup_amount = plant_info.growup_amount();
		info_record->mature_duration = plant_info.mature_duration();
		info_record->grow_times = plant_info.grow_times();
		info_record->notice_times = plant_info.notice_times();
		info_record->stealed_times = plant_info.stealed_times();
		info_record->growup_cd = plant_info.growup_cd();
		info_record->start_time = plant_info.start_time();
		for (int index =0 ; index < plant_info.event_log_size(); ++index)
		{
			const KKSG::GardenEventLog& log = plant_info.event_log(index);
			info_record->record_event_log.push_back(log);
		}
		plant_info_store_.plantinfo_record.insert(std::make_pair(plant_info.farmland_id(),info_record));
	}

	// 帮助信息
	int help_info_cout = garden_plantinfo.help_info_size();
	for (int i = 0; i < help_info_cout; ++ i)
	{
		const KKSG::MapIntItem&  item = garden_plantinfo.help_info(i);
		plant_info_store_.help_info.insert(std::make_pair(item.key(),item.value()));
	}

	return true;
}

void GuildGardenPlant::SaveGuildGardenInfo(KKSG::GuildGardenInfo& info)
{
	info.mutable_plant()->assign(ToBinary());
}
const std::string GuildGardenPlant::ToBinary() 
{
	KKSG::GardenPlantInfo garden_plantinfo;
	//	garden_plantinfo.set_allow_plantid_max(plant_info_store_.allow_plantid_max);
	//garden_plantinfo.set_total_plant_times(plant_info_store_.total_plant_times);
	//garden_plantinfo.set_total_harverst_amount(plant_info_store_.total_harverst_amount);
	garden_plantinfo.set_sprite_id(plant_info_store_.sprite_id);
	garden_plantinfo.set_sprite_durations(plant_info_store_.sprite_durations);
	garden_plantinfo.set_sprite_appeartimes(plant_info_store_.sprite_appeartimes);
	garden_plantinfo.set_sprite_appear_cd(plant_info_store_.sprite_appear_cd);

	// 种植记录
	auto it = plant_info_store_.plantinfo_record.begin();
	auto it_end = plant_info_store_.plantinfo_record.end();
	for (; it != it_end; ++it)
	{
		KKSG::PlantInfo* plant = garden_plantinfo.add_plant_info();
		plant->set_owner(it->second->owner);
		plant->set_farmland_id(it->first);
		plant->set_seed_id(it->second->seed_id);
		plant->set_plant_grow_state((KKSG::PlantGrowState)it->second->plant_grow_state);
		plant->set_growup_amount(it->second->growup_amount);
		plant->set_mature_duration(it->second->mature_duration);		
		plant->set_stealed_times(it->second->stealed_times);
		plant->set_growup_cd(it->second->growup_cd);
		plant->set_grow_times(it->second->grow_times);
		plant->set_start_time(it->second->start_time);
		plant->set_notice_times(it->second->notice_times); 

		std::vector<KKSG::GardenEventLog>::iterator logit = it->second->record_event_log.begin();
		std::vector<KKSG::GardenEventLog>::iterator logit_end = it->second->record_event_log.end();
		for (; logit != logit_end; ++logit)
		{
			KKSG::GardenEventLog* event_log = plant->add_event_log();
			event_log->set_role_id(logit->role_id());
			event_log->set_role_name(logit->role_name());
			event_log->set_event_type(logit->event_type());
			event_log->set_occur_time(logit->occur_time());
			event_log->set_target(logit->target());
			event_log->set_result(logit->result());
		}
	}

	auto itid = plant_info_store_.allow_plantid.begin();
	auto itid_end = plant_info_store_.allow_plantid.end();	
	for (; itid != itid_end; ++itid)
	{
		garden_plantinfo.add_allow_plantid(*itid);
	}


	// 帮助信息
	auto helpit = plant_info_store_.help_info.begin();
	auto helpit_end = plant_info_store_.help_info.end();	
	for (; helpit != helpit_end; ++helpit)
	{
		KKSG::MapIntItem*  item = garden_plantinfo.add_help_info();
		item->set_key(helpit->first);
		item->set_value(helpit->second);		
	}

	return garden_plantinfo.SerializeAsString();
}


void GuildGardenPlant::SetFarmland(std::set<UINT32>& farmland)
{
	auto it = farmland.begin();
	auto it_end = farmland.end();
	for (; it != it_end; ++it)
	{
		plant_info_store_.allow_plantid.insert(*it);
	}	
}

// 检查地块种植的条件
UINT32 GuildGardenPlant::CheckPlantCondition(
	const UINT32 farmland_id,
	const UINT32 seed_id)
{
	// 检查本地块是否开启
	auto itid = plant_info_store_.allow_plantid.find(farmland_id);
	if ( itid  ==  plant_info_store_.allow_plantid.end())
	{
		return KKSG::ERR_GARDEN_NOALLOW;
	}

	// 1.是否已经种上了
	auto it = plant_info_store_.plantinfo_record.find(farmland_id);
	if (it != plant_info_store_.plantinfo_record.end())
	{
		return KKSG::ERR_GARDEN_PLANT_CD;
	}

	// 2.种子是否在配置中
	auto  rowdata = GardenConfig::Instance()->GetSeed().GetBySeedID(seed_id);
	if (rowdata == NULL )
	{
		return KKSG::ERR_GARDEN_NOSEED;
	}

	return KKSG::ERR_SUCCESS ;
}

// 在制定地块种植种子
UINT32 GuildGardenPlant::TryPlantSeed(
	const UINT64 role_id,
	const UINT32 farmland_id,
	const UINT32 seed_id)
{
	auto rowdata = GardenConfig::Instance()->GetSeed().GetBySeedID(seed_id);
	if (rowdata == NULL)
	{
		return KKSG::ERR_GARDEN_NOTEXIST_SEEDID;
	}

	UINT32 random = XRandom::randInt(1,4);

	PlantInforRecord* info_record = new PlantInforRecord();	
	info_record->plant_grow_state = (KKSG::PlantGrowState)random;	
	info_record->owner = role_id;
	info_record->seed_id = seed_id;	
	info_record->notice_times =PLANT_GROWUP_FIRST;
	info_record->start_time = (UINT32)GameTime::GetTime();
	info_record->growup_amount = (float)rowdata->InitUpAmount;
	info_record->mature_duration =0;
	info_record->growup_cd = 0;
	info_record->stealed_times = 0;
	info_record->record_event_log.clear();

	plant_info_store_.plantinfo_record.insert(std::make_pair(farmland_id,info_record));

	// 通知buf 		
	KKSG::GardenPlayStageChange arg;					
	arg.set_event_type(KKSG::PLANT_STATE_CHANGE);			
	arg.set_farmland_id(farmland_id);
	arg.set_grow_state((KKSG::PlantGrowState)(info_record->plant_grow_state));					
	GardenUtility::Instance()->NotifyEvnetChangeNtf(arg,garden_id_,true);

	// 数据有变化
	swithc_.Set();

	return random ;
}

UINT32 GuildGardenPlant::CanclePlant(const UINT32 farmland_id)
{
	auto rcit = plant_info_store_.plantinfo_record.find(farmland_id);
	auto rcit_end = plant_info_store_.plantinfo_record.end();
	if (rcit != rcit_end)
	{
		if (rcit->second->plant_grow_state == KKSG::growMature)
		{
			return KKSG::ERR_TEAM_IDIP;
		}

		CRole *role = CRoleManager::Instance()->GetByRoleID(rcit->second->owner);
		if (NULL == role)
		{
			return KKSG::ERR_INVALID_REQUEST;
		}
		
		// 退种子
		RpcM2G_MsGiveItem* rpc = RpcM2G_MsGiveItem::CreateRpc();
		rpc->m_oArg.set_roleid(role->GetID());
		rpc->m_oArg.set_reason(ItemFlow_Garden);
		rpc->m_oArg.set_subreason(ItemFlow_Garden_PlantSeed);
		KKSG::ItemBrief *item = rpc->m_oArg.add_items();
		item->set_itemid(rcit->second->seed_id);
		item->set_itemcount(1);
		item->set_isbind(false);

		role->SendMsgToGS(*rpc);

		// 删除种植的作物
		DeletePlantRecordByFarmland(farmland_id);

		return KKSG::ERR_SUCCESS;
	}

	return KKSG::ERR_GARDEN_NOTEXIST_FARMLAND;
}
void GuildGardenPlant::UpdatePlantRecord(
	const UINT32 farmland_id,
	const UINT64 enemy_uid)
{
	auto rcit = plant_info_store_.plantinfo_record.find(farmland_id);
	auto rcit_end = plant_info_store_.plantinfo_record.end();
	if (rcit == rcit_end)
	{
		SSWarn << " not exist this record ,farmland_id is  : "<<farmland_id <<END;
		return;
	}
}

// 获取已有的种植记录
const PlantInfoStore& GuildGardenPlant::GetPlantInfoStore()
{
	return plant_info_store_;
}

// 种植的操作
UINT32 GuildGardenPlant::PlantCultivation(	
	const UINT32 farmland_id,
	const UINT32 operate_type)
{
	auto rcit = plant_info_store_.plantinfo_record.find(farmland_id);
	auto rcit_end = plant_info_store_.plantinfo_record.end();
	if (rcit == rcit_end)
	{
		return KKSG::ERR_GARDEN_NOTEXIST_FARMLAND;
	}

	// 已经培养过了
	if (rcit->second->plant_grow_state == KKSG::growCD)
	{
		return KKSG::ERR_GARDEN_PLANT_CD;
	}

	UINT32 result = KKSG::ERR_SUCCESS;

	if (operate_type  !=  rcit->second->plant_grow_state)
	{
		auto rowdata = GardenConfig::Instance()->GetSeed().GetBySeedID(rcit->second->seed_id);
		if (rowdata == NULL)
		{
			return KKSG::ERR_GARDEN_NOTEXIST_SEEDID;
		}
		if (rcit->second->growup_amount > rowdata->ReduceGrowUpRate)
		{
			rcit->second->growup_amount -= rowdata->ReduceGrowUpRate;
		}
		else
		{
			rcit->second->growup_amount = 0;
		}

		result = KKSG::ERR_GARDEN_PLANT_CUL_ERR;
	}	

	rcit->second->plant_grow_state = KKSG::growCD;
	rcit->second->growup_cd = 0;

	// 通知buf清理异常状态		
	KKSG::GardenPlayStageChange arg;					
	arg.set_event_type(KKSG::PLANT_STATE_CHANGE);			
	arg.set_farmland_id(farmland_id);
	arg.set_grow_state(KKSG::growCD);					
	GardenUtility::Instance()->NotifyEvnetChangeNtf(arg,garden_id_,true);

	if (KKSG::ERR_SUCCESS == result )
	{
		// 如果处理正确，添加正确的buff					
		arg.set_event_type(KKSG::PLANT_STATE_CHANGE);			
		arg.set_farmland_id(farmland_id);
		arg.set_grow_state(KKSG::growCorrect);					
		GardenUtility::Instance()->NotifyEvnetChangeNtf(arg,garden_id_,true);
	}

	// 数据有变化
	swithc_.Set();

	return result;
}


UINT32 GuildGardenPlant::PlantCultivationByPlayer(
	const UINT64 role_id,
	const std::string& role_name,
	const UINT32 farmland_id,
	const UINT32 operate_type)
{
	INT32 result = PlantCultivation(farmland_id,operate_type);

	auto rcit = plant_info_store_.plantinfo_record.find(farmland_id);
	auto rcit_end = plant_info_store_.plantinfo_record.end();
	if (rcit == rcit_end)
	{
		return KKSG::ERR_GARDEN_NOTEXIST_FARMLAND;
	}

	bool operate_result = false;
	if (KKSG::ERR_SUCCESS == result )
	{
		operate_result = true;
	}

	add_log_func_(rcit->second,role_id,role_name,operate_type,rcit->second->seed_id,operate_result);

	if (role_id != garden_id_)
	{
		auto it = plant_info_store_.help_info.find(role_id);
		auto it_end = plant_info_store_.help_info.end();
		if (it != it_end)
		{
			it->second += 1;
		}
		else
		{
			plant_info_store_.help_info.insert(std::make_pair(role_id,1));
		}
	}

	// 数据有变化
	swithc_.Set();

	return result;
}
// 检查是否需要主动发收获的物品
bool GuildGardenPlant::CheckNeedSendHarvestItem(
	PlantInforRecord& record,
	const UINT32 duration)
{
	if(++record.mature_duration < duration)
	{
		return false;
	}

	return true;
}

// 制定地块收割
UINT32 GuildGardenPlant::PlantHarvest(
	const UINT32 farmland_id,
	std::vector<ItemDesc>& gain_items,
	bool& harvest,
	bool& extra)
{
	auto rcit = plant_info_store_.plantinfo_record.find(farmland_id);
	auto rcit_end = plant_info_store_.plantinfo_record.end();
	if (rcit == rcit_end)
	{
		return KKSG::ERR_GARDEN_NOTEXIST_FARMLAND;
	}
	if (rcit->second->plant_grow_state != KKSG::growMature)
	{
		return KKSG::ERR_GARDEN_NOHARVESTSTATE;
	}

	auto rowdata = GardenConfig::Instance()->GetSeed().GetBySeedID(rcit->second->seed_id);
	if (rowdata == NULL )
	{
		SSWarn << "get seed config err ,the seed is : "<<rcit->second->seed_id <<END;
		return KKSG::ERR_GARDEN_NOTEXIST_SEEDID; ;
	}

	GetPlantHarvestItems(*rcit->second,*rowdata,gain_items,harvest,extra);		

	DeletePlantRecordByFarmland(farmland_id);

	return KKSG::ERR_SUCCESS;
}


// 好友偷菜
UINT32 GuildGardenPlant::StealHarvest(
	const UINT64 role_id,
	const std::string& role_name,
	const UINT32 farmland_id,
	std::vector<ItemDesc>& steal_items)
{
	auto rcit = plant_info_store_.plantinfo_record.find(farmland_id);
	auto rcit_end = plant_info_store_.plantinfo_record.end();
	if (rcit == rcit_end)
	{
		return KKSG::ERR_GARDEN_NOTEXIST_FARMLAND;
	}
	if (rcit->second->plant_grow_state != KKSG::growMature)
	{
		return KKSG::ERR_GARDEN_NOHARVESTSTATE;
	}

	auto rowdata = GardenConfig::Instance()->GetSeed().GetBySeedID(rcit->second->seed_id);
	if (rowdata == NULL)
	{
		return KKSG::ERR_GARDEN_NOTEXIST_SEEDID;
	}

	if (rcit->second->stealed_times >= rowdata->CanStealMaxTimes)
	{
		return KKSG::ERR_GARDEN_STEALEDTIMES_EXCEED;
	}

	rcit->second->stealed_times += 1;
	add_log_func_(rcit->second,role_id,role_name,friendSteal,rcit->second->seed_id,true);

	ItemDesc item(rowdata->StealAward.seq[0],rowdata->StealAward.seq[1]);
	steal_items.push_back(item);


	// 数据有变化
	swithc_.Set();

	return KKSG::ERR_SUCCESS;
}

UINT32 GuildGardenPlant::ExpelSprite(
	const UINT64 role_id,
	const std::string& role_name,
	const UINT32 sprite_id,
	std::vector<ItemDesc>& award_items)
{
	if (sprite_id != plant_info_store_.sprite_id)
	{
		return KKSG::ERR_GARDEN_NOTEXIST_SPRITE;
	}

	auto sprite_cfg = GardenConfig::Instance()->GetSprite().GetBySpriteID(sprite_id);
	if (sprite_cfg == NULL)
	{
		SSWarn << "get sprite config err ,the id is : "<<plant_info_store_.sprite_id <<END;
		return KKSG::ERR_GARDEN_NOTEXIST_SPRITE;
	}

	plant_info_store_.sprite_id = 0;
	plant_info_store_.sprite_appear_cd = 1;
	plant_info_store_.sprite_durations = 0;

	add_log_func_(NULL,role_id,role_name,spriteExpel,sprite_id,true);

	auto it = sprite_cfg->AwardItems.begin();
	auto it_end = sprite_cfg->AwardItems.end();
	for (; it != it_end; ++it)
	{
		ItemDesc item(it->seq[0],it->seq[1]);
		award_items.push_back(item);
	}	


	// 通知小妖消失
	KKSG::GardenPlayStageChange arg;
	arg.set_sprite_id(sprite_id);
	arg.set_exist(false);
	arg.set_event_type(KKSG::PLANT_SPRITE);		
	GardenUtility::Instance()->NotifyEvnetChangeNtf(arg,garden_id_,true);	

	// 数据有变化
	swithc_.Set();

	return KKSG::ERR_SUCCESS;
}


void GuildGardenPlant::AddFarmland(const UINT32 farmland_id)
{
	auto it = plant_info_store_.allow_plantid.find(farmland_id);
	auto it_end = plant_info_store_.allow_plantid.end();
	if (it == it_end)
	{
		plant_info_store_.allow_plantid.insert(farmland_id);
		// 数据有变化
		swithc_.Set();
	}
}
/////////////////////////////////////////////////////////////////////////////////////////


// 植物成熟状态下的处理
bool  GuildGardenPlant::PlantHarvesState(
	PlantInforRecord& record,
	const PlantSeed::RowData& config)
{
	if (!CheckNeedSendHarvestItem(record,config.MatureDuration))
	{
		return false;
	}

	std::vector<ItemDesc> gain_items;
	bool harvest = false;
	bool extra = false;

	GetPlantHarvestItems(record,config,gain_items,harvest,extra);

	// guild  --> owner 
	CMailMgr::Instance()->SendMail(record.owner,MailConf_PLANT, gain_items);

	return true;
}

// 获取丰收的物品
void GuildGardenPlant::GetPlantHarvestItems(
	const PlantInforRecord& record,
	const PlantSeed::RowData& config,
	std::vector<ItemDesc>& gain_items,
	bool& harvest,
	bool& extra)
{
	// 查看丰收和额外掉落
	if (record.grow_times < config.GrowUpAmount)
	{
		//有概率丰收
		UINT32 random = XRandom::randInt(1,101);
		if (random < config.HarvestRate)
		{
			if (config.HarvestPlant.seq[0] > 0 && config.HarvestPlant.seq[1]>0)
			{
				ItemDesc item(config.HarvestPlant.seq[0],config.HarvestPlant.seq[1]);
				gain_items.push_back(item);
				harvest = true;				
			}

		}
	}

	// 正常收获
	if (config.PlantID.seq[0] >0 && config.PlantID.seq[1] > 0)
	{
		ItemDesc item(config.PlantID.seq[0],config.PlantID.seq[1]);
		gain_items.push_back(item);
	}

	// 额外掉落
	UINT32 random = XRandom::randInt(1,101);
	if (random < config.ExtraDropRate)
	{
		if (config.ExtraDropItem.seq[0] > 0 &&
			config.ExtraDropItem.seq[1] > 0)
		{
			ItemDesc item(config.ExtraDropItem.seq[0],config.ExtraDropItem.seq[1]);
			gain_items.push_back(item);
			extra = true;
		}
	}

	// 数据有变化
	swithc_.Set();
}

void GuildGardenPlant::DeletePlantRecordByFarmland(const UINT32 farmland_id)
{
	auto record = plant_info_store_.plantinfo_record.find(farmland_id);
	if (record != plant_info_store_.plantinfo_record.end())
	{
		record->second->record_event_log.clear();
		delete record->second;
		plant_info_store_.plantinfo_record.erase(farmland_id);

		// 通知给gs，销毁对应的怪
		KKSG::GardenPlayStageChange arg;
		arg.set_farmland_id(farmland_id);
		arg.set_event_type(KKSG::PLANT_DELETE);		
		GardenUtility::Instance()->NotifyEvnetChangeNtf(arg,garden_id_,true);

		// 数据有变化
		swithc_.Set();
	}
}

void  GuildGardenPlant::PlantSprite()
{
	// 1.检查是否有小妖影响
	if (plant_info_store_.sprite_durations > 0 )
	{
		SpriteInfluence();
		return ;
	}

	// 数据有变化
	swithc_.Set();

	// 2. 是否CD期间
	plant_info_store_.sprite_appear_cd  += 1;

	if (plant_info_store_.sprite_id >0 && 
		plant_info_store_.sprite_appear_cd < GetGlobalConfig().PlantSpriteCD)
	{
		return;
	}

	// 3.时间区间是否到了	
	time_t now = GameTime::GetTime();
	struct tm *timeinfo = localtime(&now);
	if ((UINT32)timeinfo->tm_hour < GetGlobalConfig().PlantSpriteAppearTimeSection[0] && 
		(UINT32)timeinfo->tm_hour >= GetGlobalConfig().PlantSpriteAppearTimeSection[1] )
	{	
		return;
	}

	// 4 当前是否还有出现次数
	if (plant_info_store_.sprite_appeartimes >= GetGlobalConfig().PlantSpriteMaxTimes)
	{
		return ;
	}

	// 5.种植了作物,植物是否CD状态
	{
		bool growState = false;
		auto it =   plant_info_store_.plantinfo_record.begin();
		auto it_end = plant_info_store_.plantinfo_record.end();
		for (; it != it_end; ++it)
		{
			if (it->second->plant_grow_state == KKSG::growCD )
			{
				growState = true;			
			}
		}

		if (!growState)
		{
			return;
		}
	}

	// 6,小妖可以出现了
	UINT32 weights = 0;
	auto& sprite_cfg = GardenConfig::Instance()->GetSprite().Table;
	auto it = sprite_cfg.begin();
	auto it_end = sprite_cfg.end();
	for (; it != it_end; ++it)
	{	
		weights = (*it)->OccurrenceWeitght;
	}

	UINT32 random_weight = XRandom::randInt(1,weights+1);
	weights = 0;	

	it = sprite_cfg.begin();
	it_end = sprite_cfg.end();
	for (; it != it_end; ++it)
	{
		weights += (*it)->OccurrenceWeitght;
		if (weights >= random_weight)
		{
			// 小妖出现
			plant_info_store_.sprite_id =(*it)->SpriteID;
			plant_info_store_.sprite_durations = 1;
			plant_info_store_.sprite_appeartimes += 1;
			plant_info_store_.sprite_appear_cd =0;
			add_log_func_(NULL,garden_id_,garden_name_,spriteTrouble,0,true);

			// 通知给gs，
			KKSG::GardenPlayStageChange arg;
			arg.set_sprite_id(plant_info_store_.sprite_id);
			arg.set_exist(true);
			arg.set_event_type(KKSG::PLANT_SPRITE);		
			GardenUtility::Instance()->NotifyEvnetChangeNtf(arg,garden_id_,true);

			LogDebug("sent GardenPlayStageChange  sprite_id %d  ", plant_info_store_.sprite_id);

			break;
		}
	}
}


bool GuildGardenPlant::SpriteInfluence()
{
	auto sprite_cfg = GardenConfig::Instance()->GetSprite().GetBySpriteID(plant_info_store_.sprite_id);
	if (sprite_cfg == NULL)
	{
		SSWarn << "get sprite config err ,the id is : "<<plant_info_store_.sprite_id <<END;
		return false;
	}

	plant_info_store_.sprite_durations  += 1; //GetGlobalConfig().PlantSpriteCheckTimes;
	if (plant_info_store_.sprite_durations >= sprite_cfg->Duration)
	{
		// 清理状态		
		plant_info_store_.sprite_durations = 0;		

		// 非成熟状态植物扣点
		auto it  = plant_info_store_.plantinfo_record.begin();
		auto it_end = plant_info_store_.plantinfo_record.end();
		for (; it != it_end; ++it)
		{
			if (it->second->plant_grow_state != KKSG::growMature)
			{
				it->second->growup_amount =  (float)(100-sprite_cfg->DamageRate);				
			}
		}

		//// 通知小妖消失 需要通知场景里的所有人！

		// 通知给gs，
		KKSG::GardenPlayStageChange arg;
		arg.set_sprite_id(plant_info_store_.sprite_id);
		arg.set_exist(false);
		arg.set_event_type(KKSG::PLANT_SPRITE);		
		GardenUtility::Instance()->NotifyEvnetChangeNtf(arg,garden_id_,true);

		LogDebug("disparrs  GardenPlayStageChange  sprite_id %d  ", plant_info_store_.sprite_id); 

		plant_info_store_.sprite_durations = 0;

		// 数据有变化
		swithc_.Set();
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
void  GuildGardenPlant::OnTimer()
{
	// 1.检查作物状态
	// 2.根据状态，增加成长点
	// 3.根据成长点，更新状态
	std::vector<UINT32> remove_id;
	auto it = plant_info_store_.plantinfo_record.begin();
	auto it_end = plant_info_store_.plantinfo_record.end();
	for (;it != it_end; ++it)
	{	
		/*LogDebug("====================farmland_id:%d ,seed_id:%d,plant_grow_state:%d ,growup_amount: %f,mature_duration:%d ,grow_times:%d ,enemy_uid: %llu,start_time:%d ,growup_cd:%d ,stealed_times:%d ,notice_times:%d =================", 
		it->first, 
		it->second->seed_id,
		it->second->plant_grow_state,
		it->second->growup_amount,
		it->second->mature_duration,
		it->second->grow_times,
		it->second->enemy_uid,
		it->second->start_time,
		it->second->growup_cd,
		it->second->stealed_times,
		it->second->notice_times); 
		*/

		auto rowdata = GardenConfig::Instance()->GetSeed().GetBySeedID(it->second->seed_id);
		if (rowdata == NULL )
		{
			SSWarn << "get seed config err ,the seed is : "<<it->second->seed_id <<END;
			return ;
		}
		// 成熟状态的，检查是否需要把收获物品邮件发送
		if (it->second->plant_grow_state == KKSG::growMature)
		{			
			if (PlantHarvesState(*(it->second),*rowdata))
			{
				remove_id.push_back(it->first);
			}
		}
		else
		{
			// 未成熟的此刻成长一次
			float grow_rate = (float)rowdata->GrowUpAmountPerMinute;
			if (it->second->plant_grow_state  != KKSG::growCD)		
			{
				grow_rate = grow_rate *(float) (100.00-rowdata->BadStateGrowUpRate)/100;						
			}
			it->second->growup_amount += (float)(rowdata->GrowUpAmountPerMinute*grow_rate);
			//  
			it->second->grow_times += 1;
			// 检查成长阶段
			if (it->second->notice_times == PLANT_GROWUP_FIRST)
			{
				INT32 rate = rowdata->SecondGrowSection[0];
				float threshold = (float)rowdata->GrowUpAmount * rate/100;
				if (it->second->growup_amount >= threshold)
				{
					// 进入第二阶段，发通知
					it->second->notice_times = PLANT_GROWUP_SECOND;

					UINT64 garden_id = garden_id_;
					UINT32 farmland_id = it->first;					
					UINT32 sprite_id = rowdata->SecondGrowSpriteId;
					GardenUtility::Instance()->RpcPlantChangeStage(
						garden_id,
						farmland_id,					
						sprite_id,
						it->second->plant_grow_state,
						true);

					continue;
				}				
			}

			else if (it->second->notice_times == PLANT_GROWUP_SECOND)
			{
				INT32 rate = rowdata->RipeSection[0];
				float threshold = (float)(rowdata->GrowUpAmount* rate/100);
				if (it->second->growup_amount >= threshold)
				{
					it->second->notice_times +=1;
					// 进入第第三阶段					

					UINT64 garden_id = garden_id_;
					UINT32 farmland_id = it->first;				
					UINT32 sprite_id = rowdata->RipeSpriteID;
					GardenUtility::Instance()->RpcPlantChangeStage(
						garden_id,
						farmland_id,						
						sprite_id,
						it->second->plant_grow_state,
						true);	

					continue;
				}
			}

			// 即使成长值到了，也要看阶段
			if ((UINT32)it->second->growup_amount >= rowdata->GrowUpAmount
				&&  PLANT_GROWUP_SECOND+1 == it->second->notice_times )
			{
				it->second->plant_grow_state = KKSG::growMature;	

				// 通知成熟了
				KKSG::GardenPlayStageChange arg;
				arg.set_event_type(KKSG::PLANT_MATURE);
				arg.set_farmland_id(it->first);			
				GardenUtility::Instance()->NotifyEvnetChangeNtf(arg,garden_id_,true);
			}	
			if (it->second->plant_grow_state == KKSG::growCD)
			{
				it->second->growup_cd += 1;
				if (it->second->growup_cd  >= rowdata->PlantStateCD)
				{
					// 又来异常状态
					UINT32 random = XRandom::randInt(1,4);				
					it->second->plant_grow_state = (KKSG::PlantGrowState)random;	
					KKSG::GardenPlayStageChange arg;					
					arg.set_event_type(KKSG::PLANT_STATE_CHANGE);			
					arg.set_farmland_id(it->first);
					arg.set_grow_state((KKSG::PlantGrowState)(it->second->plant_grow_state));					
					GardenUtility::Instance()->NotifyEvnetChangeNtf(arg,garden_id_,true);

				}
			}

			swithc_.Set();
		}
	}

	// 清理已经收获了的记录
	auto rmit = remove_id.begin();
	auto rmit_end = remove_id.end();
	for (; rmit != rmit_end; ++rmit)
	{
		DeletePlantRecordByFarmland(*rmit);
	}

	//// 小妖	 工会目前没有小妖需求
	//if (plant_info_store_.plantinfo_record.size() > 0 )
	//{
	//	PlantSprite();
	//}
}
