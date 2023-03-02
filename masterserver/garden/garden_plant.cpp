#include "pch.h"
#include "garden/garden_plant.h"
#include "util/XRandom.h"
#include "util/gametime.h"
#include "mail/mailmgr.h"
#include "garden/ptcm2c_spritestatechangentf.h"
#include "role/rolemanager.h"
#include "garden/garden_utility.h"
#include "loghelper/tlogr.h"
#include "garden/garden_manager.h"
#include "common/rpcm2g_msgiveitem.h"

GardenPlant::GardenPlant(
	const UINT64 garden_id,
	const std::string& garden_name)
{	
	garden_id_ = garden_id;
	garden_name_ = garden_name;
	swithc_.Set();	
}

GardenPlant:: ~GardenPlant() 
{
	auto rcit = plant_info_store_.plantinfo_record.begin();
	auto rcit_end = plant_info_store_.plantinfo_record.end();
	for (; rcit != rcit_end; ++rcit)
	{
		delete rcit->second;
	}
}

bool GardenPlant::IsChanged() 
{
	return swithc_.TestAndReset();
}

bool GardenPlant::Load(const KKSG::GardenInfo& garden_info) 
{
	if (!garden_info.has_plant() || garden_info.plant().empty())
	{
		return false;
	}

	KKSG::GardenPlantInfo garden_plantinfo;
	if (!garden_plantinfo.ParseFromString(garden_info.plant()))
	{
		return false;
	}

	// 
	int  allow_plantid = garden_plantinfo.allow_plantid_size();
	for (int i=0; i < allow_plantid; ++i)
	{
		plant_info_store_.allow_plantid.insert(garden_plantinfo.allow_plantid(i));
	}

	plant_info_store_.harvest = garden_plantinfo.harvest();
	plant_info_store_.extra = garden_plantinfo.extra();
	plant_info_store_.rich_crop = garden_plantinfo.rich_crop();
	plant_info_store_.steal = garden_plantinfo.steal();

	plant_info_store_.sprite_id = garden_plantinfo.sprite_id();
	plant_info_store_.sprite_durations = garden_plantinfo.sprite_durations();
	plant_info_store_.sprite_appeartimes =  garden_plantinfo.sprite_appeartimes();
	plant_info_store_.sprite_appear_cd = garden_plantinfo.sprite_appear_cd();
	plant_info_store_.steal_count = garden_plantinfo.steal_count();
	// ��ֲ��Ϣ
	int plan_info_count = garden_plantinfo.plant_info().size();
	for (int i=0; i < plan_info_count; ++i)
	{
		PlantInforRecord* info_record = new PlantInforRecord();
		const KKSG::PlantInfo& plant_info = garden_plantinfo.plant_info(i);
		// ��ͨ�ļ�԰���Ǹ��˵�
		info_record->owner = garden_id_;
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

	// ������Ϣ
	int help_info_cout = garden_plantinfo.help_info_size();
	for (int i = 0; i < help_info_cout; ++ i)
	{
		const KKSG::MapIntItem&  item = garden_plantinfo.help_info(i);
		plant_info_store_.help_info.insert(std::make_pair(item.key(),item.value()));
	}

	return true;
}

void GardenPlant::Save(KKSG::GardenInfo& garden_info) 
{
	garden_info.mutable_plant()->assign(ToBinary());
}

const std::string GardenPlant::ToBinary() 
{
	KKSG::GardenPlantInfo garden_plantinfo;

	garden_plantinfo.set_harvest(plant_info_store_.harvest);
	garden_plantinfo.set_extra(plant_info_store_.extra);
	garden_plantinfo.set_rich_crop(plant_info_store_.rich_crop);
	garden_plantinfo.set_steal(plant_info_store_.steal);

	garden_plantinfo.set_sprite_id(plant_info_store_.sprite_id);
	garden_plantinfo.set_sprite_durations(plant_info_store_.sprite_durations);
	garden_plantinfo.set_sprite_appeartimes(plant_info_store_.sprite_appeartimes);
	garden_plantinfo.set_sprite_appear_cd(plant_info_store_.sprite_appear_cd);
	garden_plantinfo.set_steal_count(plant_info_store_.steal_count);
	// ��ֲ��¼
	auto it = plant_info_store_.plantinfo_record.begin();
	auto it_end = plant_info_store_.plantinfo_record.end();
	for (; it != it_end; ++it)
	{
		KKSG::PlantInfo* plant = garden_plantinfo.add_plant_info();
		// ��ͨ�ļ�԰���Ǹ��˵�
		plant->set_owner(garden_id_);
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

		auto logit = it->second->record_event_log.begin();
		auto logit_end = it->second->record_event_log.end();
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

	// ������Ϣ
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


// �賿��Ҫ���������
bool GardenPlant::Refresh()
{
	plant_info_store_.sprite_appeartimes = 0;
	plant_info_store_.steal_count = 0;

	swithc_.Set();
	GardenManager::Instance()->SetChanged(garden_id_);

	return true;
}
// ���ؿ���ֲ������
UINT32 GardenPlant::CheckPlantCondition(
	const UINT32 farmland_id,
	const UINT32 seed_id)
{
	// ��鱾�ؿ��Ƿ���
	auto itid = plant_info_store_.allow_plantid.find(farmland_id);
	if ( itid  ==  plant_info_store_.allow_plantid.end())
	{
		return KKSG::ERR_GARDEN_NOALLOW;
	}

	// 1.�Ƿ��Ѿ�������
	auto it = plant_info_store_.plantinfo_record.find(farmland_id);
	if (it != plant_info_store_.plantinfo_record.end())
	{
		return KKSG::ERR_GARDEN_PLANT_CD;
	}

	// 2.�����Ƿ���������
	auto  rowdata = GardenConfig::Instance()->GetSeed().GetBySeedID(seed_id);
	if (rowdata == NULL )
	{
		return KKSG::ERR_GARDEN_NOSEED;
	}

	return KKSG::ERR_SUCCESS ;
}

// ���ƶ��ؿ���ֲ����
UINT32 GardenPlant::TryPlantSeed(
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
	info_record->owner = garden_id_;
	info_record->plant_grow_state = (KKSG::PlantGrowState)random;	
	info_record->seed_id = seed_id;	
	info_record->notice_times = PLANT_GROWUP_FIRST;
	info_record->start_time = (UINT32)GameTime::GetTime();
	info_record->growup_amount = (float)rowdata->InitUpAmount;
	info_record->mature_duration = 0;
	info_record->growup_cd = 0;
	info_record->stealed_times = 0;
	info_record->record_event_log.clear();

	plant_info_store_.plantinfo_record.insert(std::make_pair(farmland_id,info_record));

	// ֪ͨbuf 		
	KKSG::GardenPlayStageChange arg;					
	arg.set_event_type(KKSG::PLANT_STATE_CHANGE);
	arg.set_farmland_id(farmland_id);
	arg.set_grow_state((KKSG::PlantGrowState)(info_record->plant_grow_state));					
	GardenUtility::Instance()->NotifyEvnetChangeNtf(arg,garden_id_);

	// �����б仯
	swithc_.Set();
	GardenManager::Instance()->SetChanged(garden_id_);

	return random ;
}

UINT32 GardenPlant::CanclePlant(const UINT32 farmland_id)
{
	auto rcit = plant_info_store_.plantinfo_record.find(farmland_id);
	auto rcit_end = plant_info_store_.plantinfo_record.end();
	if (rcit != rcit_end)
	{
		if (rcit->second->plant_grow_state == KKSG::growMature)
		{
			return KKSG::ERR_TEAM_IDIP;
		}

		CRole *role = CRoleManager::Instance()->GetByRoleID(garden_id_);
		if (NULL == role)
		{
			return KKSG::ERR_INVALID_REQUEST;
		}
		
		if (GetGlobalConfig().PlantReturn > 0)
		{
			// ������
			RpcM2G_MsGiveItem* rpc = RpcM2G_MsGiveItem::CreateRpc();
			rpc->m_oArg.set_roleid(role->GetID());
			rpc->m_oArg.set_reason(ItemFlow_Garden);
			rpc->m_oArg.set_subreason(ItemFlow_Garden_PlantSeed);
			KKSG::ItemBrief *item = rpc->m_oArg.add_items();
			item->set_itemid(rcit->second->seed_id);
			item->set_itemcount(1);
			item->set_isbind(true);

			role->SendMsgToGS(*rpc);
		}

		// ɾ����ֲ������
		DeletePlantRecordByFarmland(farmland_id);

		return KKSG::ERR_SUCCESS;
	}

	return KKSG::ERR_GARDEN_NOTEXIST_FARMLAND;
}
void GardenPlant::UpdatePlantRecord(
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

// ��ȡ���е���ֲ��¼
const PlantInfoStore& GardenPlant::GetPlantInfoStore()
{
	return plant_info_store_;
}

// ��ֲ�Ĳ���
UINT32 GardenPlant::PlantCultivation(	
	const UINT32 farmland_id,
	const UINT32 operate_type)
{
	auto rcit = plant_info_store_.plantinfo_record.find(farmland_id);
	auto rcit_end = plant_info_store_.plantinfo_record.end();
	if (rcit == rcit_end)
	{
		return KKSG::ERR_GARDEN_NOTEXIST_FARMLAND;
	}

	// �Ѿ���������
	if (rcit->second->plant_grow_state == KKSG::growCD)
	{
		return KKSG::ERR_GARDEN_PLANT_CD;
	}

	UINT32 result = KKSG::ERR_SUCCESS;

	if (operate_type  !=  rcit->second->plant_grow_state)
	{
		PlantSeed::RowData* rowdata = GardenConfig::Instance()->GetSeed().GetBySeedID(rcit->second->seed_id);
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
			rcit->second->growup_amount =0;
		}

		result = KKSG::ERR_GARDEN_PLANT_CUL_ERR;
	}	

	rcit->second->plant_grow_state = KKSG::growCD;
	rcit->second->growup_cd = 0;
	// ֪ͨbuf�����쳣״̬		
	KKSG::GardenPlayStageChange arg;					
	arg.set_event_type(KKSG::PLANT_STATE_CHANGE);
	arg.set_farmland_id(farmland_id);
	arg.set_grow_state(KKSG::growCD);					
	GardenUtility::Instance()->NotifyEvnetChangeNtf(arg,garden_id_);

	if (KKSG::ERR_SUCCESS == result )
	{
		// ���������ȷ�������ȷ��buff					
		arg.set_event_type(KKSG::PLANT_STATE_CHANGE);		
		arg.set_farmland_id(farmland_id);
		arg.set_grow_state(KKSG::growCorrect);					
		GardenUtility::Instance()->NotifyEvnetChangeNtf(arg,garden_id_);
	}

	// �����б仯
	swithc_.Set();
	GardenManager::Instance()->SetChanged(garden_id_);

	return result;
}


UINT32 GardenPlant::PlantCultivationByPlayer(
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

	// �����б仯
	swithc_.Set();
	GardenManager::Instance()->SetChanged(garden_id_);

	return result;
}
// ����Ƿ���Ҫ�������ջ����Ʒ
bool GardenPlant::CheckNeedSendHarvestItem(
	PlantInforRecord& record,
	const UINT32 duration)
{
	if(++record.mature_duration < duration)
	{
		return false;
	}

	return true;
}

// �ƶ��ؿ��ո�
UINT32 GardenPlant::PlantHarvest(
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


// ����͵��
UINT32 GardenPlant::StealHarvest(
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

	// �����б仯
	swithc_.Set();
	GardenManager::Instance()->SetChanged(garden_id_);

	return KKSG::ERR_SUCCESS;
}

UINT32 GardenPlant::ExpelSprite(
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

	LogDebug("ExpelSprite  sprite_id %d   disapper ", plant_info_store_.sprite_id);

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

	// ֪ͨС����ʧ
	KKSG::GardenPlayStageChange arg;
	arg.set_sprite_id(sprite_id);
	arg.set_exist(false);
	arg.set_event_type(KKSG::PLANT_SPRITE);		
	GardenUtility::Instance()->NotifyEvnetChangeNtf(arg,garden_id_);	

	// �����б仯
	swithc_.Set();
	GardenManager::Instance()->SetChanged(garden_id_);

	return KKSG::ERR_SUCCESS;
}


void GardenPlant::AddFarmland(const UINT32 farmland_id)
{
	auto it = plant_info_store_.allow_plantid.find(farmland_id);
	auto it_end = plant_info_store_.allow_plantid.end();
	if (it == it_end)
	{
		plant_info_store_.allow_plantid.insert(farmland_id);
		// �����б仯
		swithc_.Set();
		GardenManager::Instance()->SetChanged(garden_id_);
	}
}
/////////////////////////////////////////////////////////////////////////////////////////


// ֲ�����״̬�µĴ���
bool  GardenPlant::PlantHarvesState(
	PlantInforRecord& record,
	const PlantSeed::RowData& config)
{
	if (!CheckNeedSendHarvestItem(record,config.MatureDuration))
	{
		return false;
	}

	std::vector<ItemDesc> gain_items;
	bool havest = false;
	bool extra = false;
	GetPlantHarvestItems(record,config,gain_items,havest,extra);

	// 
	CMailMgr::Instance()->SendMail(garden_id_,MailConf_PLANT, gain_items);

	return true;
}

// ��ȡ���յ���Ʒ
void GardenPlant::GetPlantHarvestItems(
	const PlantInforRecord& record,
	const PlantSeed::RowData& config,
	std::vector<ItemDesc>& gain_items,
	bool& harvest,
	bool& extra)
{
	int item_amount=0;

	// �鿴���պͶ������
	if (record.grow_times < config.GrowUpAmount)
	{
		//�и��ʷ���
		UINT32 random = XRandom::randInt(1,101);
		if (random < config.HarvestRate)
		{
			if (config.HarvestPlant.seq[0] > 0 && config.HarvestPlant.seq[1]>0)
			{
				ItemDesc item(config.HarvestPlant.seq[0],config.HarvestPlant.seq[1]);
				gain_items.push_back(item);
				harvest = true;

				plant_info_store_.rich_crop = true;
			}

		}
	}

	// �����ջ�
	if (config.PlantID.seq[0] >0 && config.PlantID.seq[1] > 0)
	{
		ItemDesc item(config.PlantID.seq[0],config.PlantID.seq[1]);
		gain_items.push_back(item);

		plant_info_store_.harvest = true;
	}

	// �������
	UINT32 random = XRandom::randInt(1,101);
	if (random < config.ExtraDropRate)
	{
		if (config.ExtraDropItem.seq[0] > 0 &&
			config.ExtraDropItem.seq[1] > 0)
		{
			ItemDesc item(config.ExtraDropItem.seq[0],config.ExtraDropItem.seq[1]);
			gain_items.push_back(item);
			extra = true;
			plant_info_store_.extra = true;
		}
	}

	// �����б仯
	swithc_.Set();
	GardenManager::Instance()->SetChanged(garden_id_);
}

void GardenPlant::DeletePlantRecordByFarmland(const UINT32 farmland_id)
{
	auto record = plant_info_store_.plantinfo_record.find(farmland_id);
	if (record != plant_info_store_.plantinfo_record.end())
	{
		record->second->record_event_log.clear();
		delete record->second;
		plant_info_store_.plantinfo_record.erase(farmland_id);

		// ֪ͨ��gs�����ٶ�Ӧ�Ĺ�
		KKSG::GardenPlayStageChange arg;	
		arg.set_farmland_id(farmland_id);
		arg.set_event_type(KKSG::PLANT_DELETE);		
		GardenUtility::Instance()->NotifyEvnetChangeNtf(arg,garden_id_);

		// �����б仯
		swithc_.Set();
		GardenManager::Instance()->SetChanged(garden_id_);
	}
}

void  GardenPlant::PlantSprite()
{
	LogDebug("sprite_id:%d,sprite_appear_cd:%d,sprite_appeartimes:%d,sprite_durations:%d,PlantSpriteMaxTimes:%d,PlantSpriteCD:%d", 
		plant_info_store_.sprite_id, 
		plant_info_store_.sprite_appear_cd, 
		plant_info_store_.sprite_appeartimes, 
		plant_info_store_.sprite_durations,
		GetGlobalConfig().PlantSpriteMaxTimes,
		GetGlobalConfig().PlantSpriteCD); 

	// 1.����Ƿ���С��Ӱ��
	if (plant_info_store_.sprite_durations > 0 )
	{
		SpriteInfluence();
		return ;
	}

	// �����б仯
	swithc_.Set();
	GardenManager::Instance()->SetChanged(garden_id_);

	// 2. �Ƿ�CD�ڼ�
	// 2.1 ���С������
	if (plant_info_store_.sprite_id > 0) 
	{
		if (++plant_info_store_.sprite_appear_cd < GetGlobalConfig().PlantSpriteCD)
		{
			return;
		}
	}
	else
	{
		// ���С���Ѿ�������,����CDʱ�䣬����ֱ��ȥ�鿴С����������
		if (plant_info_store_.sprite_appear_cd > 0)
		{
			if (++plant_info_store_.sprite_appear_cd < GetGlobalConfig().PlantSpriteCD)
			{
				return;
			}
			// CDʱ����󣬿��Բ鿴С����������
			plant_info_store_.sprite_appear_cd  = 0;
		}
	}

	// 3.ʱ�������Ƿ���
	bool sprite_appear	= false;
	time_t now			= GameTime::GetTime();
	struct tm *oTime	= localtime(&now);
	UINT32 hour			= oTime->tm_hour;
	UINT32 start_time	= GetGlobalConfig().PlantSpriteAppearTimeSection[0];
	UINT32 end_time		= GetGlobalConfig().PlantSpriteAppearTimeSection[1];
	// û����
	if (start_time < end_time)
	{
		if (hour >= start_time && hour < end_time)
		{
			sprite_appear = true;
		}		
	}
	else // ������
	{
		if (hour >= start_time || hour < end_time)
		{
			sprite_appear = true;
		}
	}

	if (!sprite_appear)
	{
		return;
	}

	// 4 ��ǰ�Ƿ��г��ִ���
	if (plant_info_store_.sprite_appeartimes >= GetGlobalConfig().PlantSpriteMaxTimes)
	{
		return ;
	}

	// 5.��ֲ������,ֲ���Ƿ�CD״̬
	{
		bool growState=false;
		auto  it =   plant_info_store_.plantinfo_record.begin();
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

	// 6,С�����Գ�����
	UINT32 weights = 0;
	auto& sprite_cfg = GardenConfig::Instance()->GetSprite().Table;
	auto it = sprite_cfg.begin();
	auto it_end = sprite_cfg.end();
	for (; it != it_end; ++it)
	{	
		weights += (*it)->OccurrenceWeitght;
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
			// С������
			plant_info_store_.sprite_id =(*it)->SpriteID;
			plant_info_store_.sprite_durations = 1;
			plant_info_store_.sprite_appeartimes += 1;
			plant_info_store_.sprite_appear_cd = 0;
			add_log_func_(NULL,garden_id_,garden_name_,spriteTrouble,0,true);

			// ֪ͨ��gs��
			KKSG::GardenPlayStageChange arg;
			arg.set_sprite_id(plant_info_store_.sprite_id);
			arg.set_exist(true);
			arg.set_event_type(KKSG::PLANT_SPRITE);		
			GardenUtility::Instance()->NotifyEvnetChangeNtf(arg,garden_id_);

			LogDebug("PlantSprite sprite appear   sprite_id %d  ", plant_info_store_.sprite_id);

			break;
		}
	}
}


bool GardenPlant::SpriteInfluence()
{
	auto sprite_cfg = GardenConfig::Instance()->GetSprite().GetBySpriteID(plant_info_store_.sprite_id);
	if (sprite_cfg == NULL)
	{
		SSWarn << "get sprite config err ,the id is : "<<plant_info_store_.sprite_id <<END;
		return false;
	}

	plant_info_store_.sprite_durations  += 1; 
	if (plant_info_store_.sprite_durations >= sprite_cfg->Duration)
	{
		// ����״̬		
		plant_info_store_.sprite_durations =0;		

		// �ǳ���״ֲ̬��۵�
		auto it  = plant_info_store_.plantinfo_record.begin();
		auto it_end = plant_info_store_.plantinfo_record.end();
		for (; it != it_end; ++it)
		{
			if (it->second->plant_grow_state != KKSG::growMature)
			{
				it->second->growup_amount = it->second->growup_amount * (100-sprite_cfg->DamageRate)/100;				
			}
		}

		//// ֪ͨС����ʧ ��Ҫ֪ͨ������������ˣ�

		// ֪ͨ��gs��
		KKSG::GardenPlayStageChange arg;
		arg.set_sprite_id(plant_info_store_.sprite_id);
		arg.set_exist(false);
		arg.set_event_type(KKSG::PLANT_SPRITE);		
		GardenUtility::Instance()->NotifyEvnetChangeNtf(arg,garden_id_);

		LogDebug("SpriteInfluence  disparrs sprite_id %d  ", plant_info_store_.sprite_id); 
		// �����б仯
		swithc_.Set();
		GardenManager::Instance()->SetChanged(garden_id_);
	}

	return true;
}

// ����͵�˱��
void GardenPlant::SetSteal()
{
	plant_info_store_.steal = true;	
	swithc_.Set();
	GardenManager::Instance()->SetChanged(garden_id_);
}

void GardenPlant::AddStealCount()
{
	plant_info_store_.steal_count += 1;
	swithc_.Set();
	GardenManager::Instance()->SetChanged(garden_id_);
}

/////////////////////////////////////////////////////////////////////////////////////////
void  GardenPlant::OnTimer()
{
	// 1.�������״̬
	// 2.����״̬�����ӳɳ���
	// 3.���ݳɳ��㣬����״̬
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
		// ����״̬�ģ�����Ƿ���Ҫ���ջ���Ʒ�ʼ�����
		if (it->second->plant_grow_state == KKSG::growMature)
		{			
			if (PlantHarvesState(*(it->second),*rowdata))
			{
				remove_id.push_back(it->first);
			}
		}
		else
		{
			// δ����Ĵ˿̳ɳ�һ��
			float grow_rate = (float)rowdata->GrowUpAmountPerMinute;
			if (it->second->plant_grow_state  != KKSG::growCD)		
			{
				grow_rate = grow_rate *(float) (100.00-rowdata->BadStateGrowUpRate)/100;						
			}
			it->second->growup_amount += (float)(rowdata->GrowUpAmountPerMinute*grow_rate);
			//  
			it->second->grow_times +=1;
			// ���ɳ��׶�
			if (it->second->notice_times == PLANT_GROWUP_FIRST)
			{
				INT32 rate = rowdata->SecondGrowSection[0];
				float threshold = (float)rowdata->GrowUpAmount * rate/100;
				if (it->second->growup_amount >= threshold)
				{
					// ����ڶ��׶Σ���֪ͨ
					it->second->notice_times = PLANT_GROWUP_SECOND;

					UINT64 garden_id = garden_id_;
					UINT32 farmland_id = it->first;
					UINT32 sprite_id = rowdata->SecondGrowSpriteId;
					GardenUtility::Instance()->RpcPlantChangeStage(
						garden_id,
						farmland_id,					
						sprite_id,
						it->second->plant_grow_state);	

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
					// ����ڵ����׶�					

					UINT64 garden_id = garden_id_;
					UINT32 farmland_id = it->first;		
					UINT32 sprite_id = rowdata->RipeSpriteID;
					GardenUtility::Instance()->RpcPlantChangeStage(
						garden_id,
						farmland_id,					
						sprite_id,
						it->second->plant_grow_state);	

					continue;
				}
			}

			LogDebug("onTime check ,it->second->growup_amount:%f,rowdata->GrowUpAmount:%d ,it->second->notice_times: %d ", 
				it->second->growup_amount,
				rowdata->GrowUpAmount,
				it->second->notice_times);

			// ��ʹ�ɳ�ֵ���ˣ�ҲҪ���׶�
			if ((UINT32)it->second->growup_amount >= rowdata->GrowUpAmount
				&&  PLANT_GROWUP_SECOND+1 == it->second->notice_times )
			{
				it->second->plant_grow_state = KKSG::growMature;	

				// ֪ͨ������
				KKSG::GardenPlayStageChange arg;
				arg.set_event_type(KKSG::PLANT_MATURE);
				arg.set_farmland_id(it->first);			
				GardenUtility::Instance()->NotifyEvnetChangeNtf(arg,garden_id_);
			}	
			if (it->second->plant_grow_state == KKSG::growCD)
			{
				it->second->growup_cd += 1;
				if (it->second->growup_cd  >= rowdata->PlantStateCD)
				{
					// �����쳣״̬
					UINT32 random = XRandom::randInt(1,4);				
					it->second->plant_grow_state = (KKSG::PlantGrowState)random;	
					KKSG::GardenPlayStageChange arg;					
					arg.set_event_type(KKSG::PLANT_STATE_CHANGE);			
					arg.set_farmland_id(it->first);
					arg.set_grow_state((KKSG::PlantGrowState)(it->second->plant_grow_state));					
					GardenUtility::Instance()->NotifyEvnetChangeNtf(arg,garden_id_);

				}
			}
		}

		swithc_.Set();
		GardenManager::Instance()->SetChanged(garden_id_);
	}

	// �����Ѿ��ջ��˵ļ�¼
	auto rmit = remove_id.begin();
	auto rmit_end = remove_id.end();
	for (; rmit != rmit_end; ++rmit)
	{
		DeletePlantRecordByFarmland(*rmit);
	}

	// С��	
	if (plant_info_store_.plantinfo_record.size() > 0 )
	{
		PlantSprite();
	}
}


