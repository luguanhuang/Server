#include "pch.h"
#include "garden.h"
#include "role/role.h"
#include "network/dblink.h"
#include "garden/garden_fish.h"
#include "garden/garden_cooking.h"
#include "garden/garden_banquet.h"
#include "guild/ptcm2d_saveguilddata.h"
#include "garden/ptcm2d_savegardeninfo.h"
#include "table/globalconfig.h"
#include "guild/guilddef.h"
#include "config.h"
#include "tablemgr/backflowtablemgr.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "garden/garden_manager.h"
#include "goalawards/goalawardsmgr.h"

///////////////////////////////////////////////////////////////////////
Garden::Garden(
	const UINT64 garden_id,
	const std::string& garden_name)
{
	garden_id_  = garden_id;
	garden_name_ = garden_name;	
	plant_ = NULL;
	garden_log_.event_log.clear();
	garden_log_.visit_times=0;

	add_log_func_ = std::bind(
		&Garden::AddEventLog,this, 
		std::placeholders::_1,		
		std::placeholders::_2, 
		std::placeholders::_3, 		
		std::placeholders::_4,
		std::placeholders::_5,
		std::placeholders::_6);

	InitSerializer();
}

Garden::~Garden()
{
	auto it = all_garden_info_.begin();
	auto it_end = all_garden_info_.end();
	for (; it != it_end; ++it)
	{
		delete it->second;
	}

	auto logit = garden_log_.event_log.begin();
	auto logit_end = garden_log_.event_log.end();
	for (; logit != logit_end; ++logit)
	{
		delete *logit;
	}
}

void Garden::InitSerializer()
{
	// 种植
	IGardenSerializerInterface* plant = new GardenPlant(garden_id_,garden_name_);
	all_garden_info_.insert(std::make_pair(plant_play,plant));	
	plant_ =   (GardenPlant*)plant;
	// 传入日志函数
	plant_->SetAddLogFunc(add_log_func_);

	// 钓鱼
	IGardenSerializerInterface* fish =new GardenFish(garden_id_);
	all_garden_info_.insert(std::make_pair(fish_play,fish));

	// 烹饪
	IGardenSerializerInterface* cooking  =new GardenCooking(garden_id_);
	all_garden_info_.insert(std::make_pair(cooking_play,cooking));	 

	// 宴会
	IGardenSerializerInterface* banquet  = new GardenBanquet(garden_id_);
	all_garden_info_.insert(std::make_pair(banquet_play,banquet));	
}

void Garden::Init()
{
	if (MSConfig::Instance()->IsBackFlowServer())
	{
		if (CRoleSummaryMgr::Instance()->GetAccountRoleNumSameServer(garden_id_) != 1)
		{
			return;
		}
		CRoleSummary* pSu = CRoleSummaryMgr::Instance()->GetRoleSummary(garden_id_);
		if (pSu == NULL)
		{
			return;
		}
		BackFlowServerConfig::RowData* pData = BackFlowTableMgr::Instance()->GetServerConfig(pSu->GetProfession(), MSConfig::Instance()->GetBackFlowLevel());
		if (pData == NULL)
		{
			return;
		}
		GardenFish* pFish = (GardenFish*)all_garden_info_[fish_play];
		GardenCooking* pCooking = (GardenCooking*)all_garden_info_[cooking_play];
		pFish->SetFishLevel(pData->FishLevel);
		pCooking->SetCookLevel(pData->CookLevel);
	}
}


void Garden::LoadFromDB(const KKSG::GardenInfo& garden_info)
{
	auto it =  all_garden_info_.begin();
	auto it_end = all_garden_info_.end();
	for (; it != it_end; ++it)
	{
		(it->second)->Load(garden_info);
	}

	if (!garden_info.has_eventlog() || garden_info.eventlog().empty())
	{
		return ;
	}

	KKSG::GardenStoreLog  garden_evnetlog;
	if (!garden_evnetlog.ParseFromString(garden_info.eventlog()))
	{
		return ;
	}
	garden_log_.visit_times = garden_evnetlog.visit_times();

	int visit_count = garden_evnetlog.visit_log_size();
	for (int i=0; i < visit_count; ++i)
	{	
		const KKSG::MapIntItem&  vist_item = garden_evnetlog.visit_log(i);	
		garden_log_.visit_log.insert(std::make_pair(vist_item.key(),vist_item.value()));
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

	garden_log_.expelsprite_times = garden_evnetlog.expelsprite_times();
	garden_log_.refresh_time = garden_evnetlog.refresh_time();
}

const std::string Garden::ToBinary() 
{
	KKSG::GardenStoreLog  garden_evnetlog;

	auto logit = garden_log_.event_log.begin();
	auto logit_end = garden_log_.event_log.end();
	for (; logit != logit_end; ++logit)
	{
		KKSG::GardenEventLog* log = garden_evnetlog.add_garden_eventlog();
		log->set_role_id((*logit)->role_id());
		log->set_role_name((*logit)->role_name());
		log->set_event_type((*logit)->event_type());
		log->set_occur_time((*logit)->occur_time());
		log->set_target((*logit)->target());
		log->set_result((*logit)->result());
	}

	auto it = garden_log_.visit_log.begin();
	auto it_end = garden_log_.visit_log.end();
	for (; it != it_end; ++it)
	{
		KKSG::MapIntItem* item  = garden_evnetlog.add_visit_log();
		item->set_key(it->first);
		item->set_value(it->second);
	}

	garden_evnetlog.set_visit_times(garden_log_.visit_times);

	garden_evnetlog.set_expelsprite_times(garden_log_.expelsprite_times);
	garden_evnetlog.set_refresh_time(garden_log_.refresh_time);

	return 	garden_evnetlog.SerializeAsString();
}

void Garden::InsertTODB()
{
	PtcM2D_SaveGardenInfo msg;
	msg.m_Data.set_role_id(garden_id_);
	msg.m_Data.set_role_name(garden_name_);
	msg.m_Data.set_db_operatetype(dbCreate);

	auto it =  all_garden_info_.begin();
	auto it_end =  all_garden_info_.end();
	for (; it != it_end; ++it)
	{
		(it->second)->Save(msg.m_Data);
	}	
	// 带上garden log
	msg.m_Data.set_eventlog(ToBinary());

	DBLink::Instance()->SendTo(msg);
}

void Garden::SaveToDB()
{
	bool change =false;
	PtcM2D_SaveGardenInfo  msg;
	auto it = all_garden_info_.begin();
	auto it_end = all_garden_info_.end();
	for (; it != it_end; ++it)
	{
		if ((it->second)->IsChanged())
		{
			(it->second)->Save(msg.m_Data);
			change = true;
		}
	}

	if (swithc_.TestAndReset())
	{
		change = true;
		msg.m_Data.set_eventlog(ToBinary());
	}
	if (change)
	{
		msg.m_Data.set_role_id(garden_id_);
		msg.m_Data.set_role_name(garden_name_);
		msg.m_Data.set_db_operatetype(dbUpdate);

		DBLink::Instance()->SendTo(msg);
	}
}


void Garden::OnTimer()
{
	auto it = all_garden_info_.begin();
	auto it_end = all_garden_info_.end();
	for (; it != it_end; ++it)
	{
		(it->second)->OnTimer();
	}	
}


void  Garden::Refresh()
{
	time_t nowtime = GameTime::GetTime();
	if (GameTime::IsInSameDay(nowtime, garden_log_.refresh_time, true))
	{
		return ;
	}
	
	auto it =  all_garden_info_.find(cooking_play);
	auto it_end =  all_garden_info_.end();
	if (it != it_end)
	{
		GardenCooking* cooking = (GardenCooking*)(it->second);
		 cooking->Refresh();
	}

	if (plant_ == NULL)
	{
		LogWarn("plant is null the garden id is %llu  garden_name is %s ",garden_id_,garden_name_.c_str());
		return ;
	}

	garden_log_.expelsprite_times = 0;
	garden_log_.refresh_time = (UINT32)nowtime;
	swithc_.Set();

	plant_->Refresh();

	// 立马保存，以免延时导致其他问题
	SaveToDB();
}

UINT32 Garden::CheckPlantCondition(
	const UINT32 farmland_id,
	const UINT32 seed_id)
{
	if (plant_ == NULL)
	{
		LogWarn("plant is null the garden id is %llu  garden_name is %s ",garden_id_,garden_name_.c_str());
		return KKSG::ERR_FAILED; ;
	}

	return plant_->CheckPlantCondition(farmland_id,seed_id);	
}

UINT32 Garden::TryPlantSeed(
	const UINT64 role_id,
	const UINT32 farmland_id,
	const UINT32 seed_id)
{
	if (plant_ == NULL)
	{
		LogWarn("plant is null the garden id is %llu  garden_name is %s ",garden_id_,garden_name_.c_str());
		return KKSG::ERR_FAILED; 
	}

	return plant_->TryPlantSeed(farmland_id,seed_id);	
}

UINT32 Garden::CanclePlant(const UINT32 farmland_id)
{
	if (plant_ == NULL)
	{
		LogWarn("plant is null the garden id is %llu  garden_name is %s ",garden_id_,garden_name_.c_str());
		return KKSG::ERR_FAILED; 
	}

	return plant_->CanclePlant(farmland_id);	
}

void Garden::UpdatePlantRecord(
	const UINT32 farmland_id,
	const UINT64 enemy_uid)
{
	if (plant_ == NULL)
	{
		LogWarn("plant is null the garden id is %llu  garden_name is %s ",garden_id_,garden_name_.c_str());
		return ;
	}

	SSDebug << " UpdatePlantRecord  farmland_id is " << farmland_id <<"  and the enemy_uid is :" << enemy_uid << END;
	return plant_->UpdatePlantRecord(farmland_id,enemy_uid);
}

const PlantInfoStore& Garden::GetPlantInfoStore()
{
	return plant_->GetPlantInfoStore();
}

UINT32 Garden::PlantCultivation(	
	const UINT32 farmland_id,
	const UINT32 operate_type)
{
	if (plant_ == NULL)
	{
		LogWarn("plant is null the garden id is %llu  garden_name is %s ",garden_id_,garden_name_.c_str());
		return KKSG::ERR_FAILED; ;
	}

	return plant_->PlantCultivation(farmland_id,operate_type);	
}

UINT32 Garden::PlantCultivationByPlayer(
	const UINT64 role_id,
	const std::string& role_name,
	const UINT32 farmland_id,
	const UINT32 operate_type)
{
	if (plant_ == NULL)
	{
		LogWarn("plant is null the garden id is %llu  garden_name is %s ",garden_id_,garden_name_.c_str());
		return KKSG::ERR_FAILED; ;
	}

	return plant_->PlantCultivationByPlayer(role_id,role_name,farmland_id,operate_type);	
}

UINT32 Garden::PlantHarvest(
	const UINT32 farmland_id,
	std::vector<ItemDesc>& gain_items,
	bool& harvest,
	bool& extra)
{
	if (plant_ == NULL)
	{
		LogWarn("plant is null the garden id is %llu  garden_name is %s ",garden_id_,garden_name_.c_str());
		return KKSG::ERR_FAILED; ;
	}
	return plant_->PlantHarvest(farmland_id,gain_items,harvest,extra);
}

// 偷菜
UINT32 Garden::StealHarvest(
	const UINT64 role_id,
	const std::string& role_name,
	const UINT32 farmland_id,
	std::vector<ItemDesc>& steal_items)
{
	if (plant_ == NULL)
	{
		LogWarn("plant is null the garden id is %llu  garden_name is %s ",garden_id_,garden_name_.c_str());
		return KKSG::ERR_FAILED; ;
	}
	return plant_->StealHarvest(role_id,role_name,farmland_id,steal_items);
}

// 赶走小妖
UINT32 Garden::ExpelSprite(
	const UINT64 role_id,
	const std::string& role_name,
	const UINT32 sprite_id,
	std::vector<ItemDesc>& award_items)
{
	if (plant_ == NULL)
	{
		LogWarn("plant is null the garden id is %llu  garden_name is %s ",garden_id_,garden_name_.c_str());
		return KKSG::ERR_FAILED; ;
	}
	return plant_->ExpelSprite(role_id,role_name,sprite_id,award_items);
}

void Garden::AddFarmland(const UINT32 farmland_id)
{
	if (plant_ == NULL)
	{
		LogWarn("plant is null the garden id is %llu  garden_name is %s ",garden_id_,garden_name_.c_str());
		return ;
	}
	return plant_->AddFarmland(farmland_id);
}

void Garden::SetSteal()
{
	if (plant_ == NULL)
	{
		LogWarn("plant is null the garden id is %llu  garden_name is %s ",garden_id_,garden_name_.c_str());
		return ;
	}
	 plant_->SetSteal();
}

void Garden::AddStealCount()
{
	if (plant_ == NULL)
	{
		LogWarn("plant is null the garden id is %llu  garden_name is %s ",garden_id_,garden_name_.c_str());
		return ;
	}
	plant_->AddStealCount();
}
UINT32 Garden::TryFishing(std::vector<ItemDesc>& items)
{
	auto it =  all_garden_info_.find(fish_play);
	auto it_end =  all_garden_info_.end();
	if (it != it_end)
	{
		GardenFish* fish = (GardenFish*)(it->second);
		return fish->TryFishing(items);	
	}

	return KKSG::ERR_FAILED;
}

FishInfoStore& Garden::GetFishInfo()
{
	GardenFish* fish = (GardenFish*)all_garden_info_[fish_play];
	return fish->GetFishInfo();
}

void Garden::GetGardenCookingInfo(FoodInfo& info)
{
	auto it =  all_garden_info_.find(cooking_play);
	auto it_end =  all_garden_info_.end();
	if (it != it_end)
	{
		GardenCooking* cooking = (GardenCooking*)(it->second);
		return cooking->GetGardenCookingInfo(info);
	}
}


bool Garden::ExistCookBook(const UINT32 cook_book_id)
{
	auto it =  all_garden_info_.find(cooking_play);
	auto it_end =  all_garden_info_.end();
	if (it != it_end)
	{
		GardenCooking* cooking = (GardenCooking*)(it->second);
		return cooking->ExistCookBook(cook_book_id);
	}

	return false;
}
UINT32 Garden::CheckCookingCondion(const UINT32 food_id)
{
	auto it =  all_garden_info_.find(cooking_play);
	auto it_end =  all_garden_info_.end();
	if (it != it_end)
	{
		GardenCooking* cooking = (GardenCooking*)(it->second);
		return cooking->CheckCookingCondion(food_id);	
	}

	return KKSG::ERR_FAILED;
}

UINT32 Garden::CookingFoodComplete(const UINT32 food_id)
{
	auto it =  all_garden_info_.find(cooking_play);
	auto it_end =  all_garden_info_.end();
	if (it != it_end)
	{
		GardenCooking* cooking = (GardenCooking*)(it->second);
		return cooking->CookingFoodComplete(food_id);	
	}

	return KKSG::ERR_FAILED;
}

void  Garden::AddCookBook(const UINT32 cook_book_id)
{
	auto it =  all_garden_info_.find(cooking_play);
	auto it_end =  all_garden_info_.end();
	if (it != it_end)
	{
		GardenCooking* cooking = (GardenCooking*)(it->second);
		cooking->AddCookBook(cook_book_id);		
	}
}

bool Garden::GetBanquetStatus()
{	
	auto it =  all_garden_info_.find(banquet_play);
	auto it_end =  all_garden_info_.end();
	if (it != it_end)
	{
		GardenBanquet* banquet = (GardenBanquet*)(it->second);
		return banquet->GetBanquetStatus();	
	}

	return false;
}

void Garden::ServeBanquet(const UINT32 banquet_id,const std::vector<UINT64>& guests)
{
	auto it =  all_garden_info_.find(banquet_play);
	auto it_end =  all_garden_info_.end();
	if (it != it_end)
	{
		GardenBanquet* banquet = (GardenBanquet*)(it->second);
		banquet->ServeBanquet(banquet_id,guests);
	}
}

UINT32 Garden::GetBanquetTimes()
{
	auto it =  all_garden_info_.find(banquet_play);
	auto it_end =  all_garden_info_.end();
	if (it != it_end)
	{
		GardenBanquet* banquet = (GardenBanquet*)(it->second);
		return banquet->GetBanquetTimes();
	}

	return 0;
}

UINT32 Garden::GetAwardTimesTSWK()
{
	auto it =  all_garden_info_.find(banquet_play);
	auto it_end =  all_garden_info_.end();
	if (it != it_end)
	{
		GardenBanquet* banquet = (GardenBanquet*)(it->second);
		return banquet->GetAwardTimesTSWK();
	}

	return 0;
}

void Garden::AddBanquetTimes()
{
	auto it =  all_garden_info_.find(banquet_play);
	auto it_end =  all_garden_info_.end();
	if (it != it_end)
	{
		GardenBanquet* banquet = (GardenBanquet*)(it->second);
		banquet->AddBanquetTimes();
	}
}

void Garden::AddEventLog(
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
	log->set_occur_time((UINT32)GameTime::GetTime());
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

	swithc_.Set();
}


void Garden::GetPlantInfo(std::vector<GardenPlantStatus>& plant_status)
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

/////////////////////////////////////////////////////////////
// for test
void Garden::SetCookLevel(const UINT32 cook_level)
{	
	auto it =  all_garden_info_.find(cooking_play);
	auto it_end =  all_garden_info_.end();
	if (it != it_end)
	{
		GardenCooking* cooking = (GardenCooking*)(it->second);
		cooking->SetCookLevel(cook_level);
	}
}

void Garden::SetCookExperiences(const UINT32 experiences)
{
	auto it =  all_garden_info_.find(cooking_play);
	auto it_end =  all_garden_info_.end();
	if (it != it_end)
	{
		GardenCooking* cooking = (GardenCooking*)(it->second);
		cooking->SetCookExperiences(experiences);
	}
}

void Garden::SetFishLevel(const UINT32 fish_level)
{
	auto it =  all_garden_info_.find(fish_play);
	auto it_end =  all_garden_info_.end();
	if (it != it_end)
	{
		GardenFish* fish  = (GardenFish*)(it->second);
		return fish->SetFishLevel(fish_level);
	}
}

void Garden::SetFishExperiences(const UINT32 experiences)
{
	auto it =  all_garden_info_.find(fish_play);
	auto it_end =  all_garden_info_.end();
	if (it != it_end)
	{
		GardenFish* fish  = (GardenFish*)(it->second);
		return fish->SetFishExperiences(experiences);
	}
}

void Garden::AddGrownAmount(
	const UINT32 farmland_id,
	const UINT32 amount)
{
	plant_->AddGrownAmount(farmland_id,amount);
}

UINT32 Garden::GetExpelspriteTimes()
{
	return  garden_log_.expelsprite_times;
}
void Garden::AddExpelspriteTimes()
{
	garden_log_.expelsprite_times += 1;
    GoalAwardsMgr::Instance()->FreshValue(garden_id_, GOAL_AWARD_ID_802, 1.f, 0);
	swithc_.Set();
	GardenManager::Instance()->SetChanged(garden_id_);
}