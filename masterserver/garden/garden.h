#ifndef __HOME_GARDEN_H__
#define __HOME_GARDEN_H__

#include "garden/garden_define.h"
#include "role/role.h"
#include "garden/garden_config.h"
#include "garden/garden_plant.h"
#include "garden/guild_garden_plant.h"
#include "table/globalconfig.h"
#include <unordered_map>
#include "util/gametime.h"
enum GardenPlayType
{
	plant_play=1,
	fish_play,
	cooking_play,
	banquet_play,
	guild_plant_play,
};
class Garden 
{
public:
	Garden(const UINT64 garden_id,
		const std::string& garden_name);
	~Garden();

	void InitSerializer();
	void Init();
	void InsertTODB();
	void SaveToDB();
	void LoadFromDB(const KKSG::GardenInfo& garden_info);
	void OnTimer();
	const std::string ToBinary();

	void Refresh();
public:
	void LoadGuildGarden(
		const UINT64 guild_id,
		const std::string& guild_name,
		const KKSG::GuildGardenInfo& info);

public:	
	// �����ֲ����
	UINT32 CheckPlantCondition(
		const UINT32 farmland_id,
		const UINT32 seed_id);

	// ��ֲ
	UINT32 TryPlantSeed(
		const UINT64 role_id,
		const UINT32 farmland_id,
		const UINT32 seed_id);

	UINT32 CanclePlant(const UINT32 farmland_id);
	// ������ֲ��¼
	void UpdatePlantRecord(
		const UINT32 farmland_id,
		const UINT64 enemy_uid);

	// ��ȡ��ֲ��¼
	const PlantInfoStore& GetPlantInfoStore();

	// ��������
	UINT32 PlantCultivation(	
		const UINT32 farmland_id,
		const UINT32 operate_type);

	// ����ֲ��
	UINT32 PlantCultivationByPlayer(
		const UINT64 role_id,
		const std::string& role_name,
		const UINT32 farmland_id,
		const UINT32 operate_type);

	// �ո�
	UINT32 PlantHarvest(
		const UINT32 farmland_id,
		std::vector<ItemDesc>& gain_items,
		bool& harvest,
		bool& extra);

	// ����С��
	UINT32 ExpelSprite(
		const UINT64 role_id,
		const std::string& role_name,
		const UINT32 sprite_id,
		std::vector<ItemDesc>& award_items);

	// ����͵��
	UINT32 StealHarvest(
		const UINT64 role_id,
		const std::string& role_name,
		const UINT32 farmland_id,
		std::vector<ItemDesc>& steal_items);

	// �����µؿ�
	void AddFarmland(const UINT32 farmland_id);
	void SetSteal();
	void AddStealCount();
public:
	// ����
	UINT32 TryFishing(std::vector<ItemDesc>& items);

	// ��ȡ������Ϣ
	FishInfoStore& GetFishInfo();
public:
	// ���ڲ���
	bool ExistCookBook(const UINT32 cook_book_id);

	// ����������
	UINT32 CheckCookingCondion(const UINT32 food_id);

	// ����ˣ����¼�¼
	UINT32 CookingFoodComplete(const UINT32 food_id);

	// ��ȡ������Ϣ
	void GetGardenCookingInfo(FoodInfo& info);

	// ���Ӳ���
	void AddCookBook(const UINT32 cook_book_id);
public:
	// ���
	void ServeBanquet(const UINT32 banquet_id,const std::vector<UINT64>& guests);

	// ��ȡ���״̬
	bool GetBanquetStatus();
	UINT32 GetBanquetTimes();
	UINT32 GetAwardTimesTSWK();
	void AddBanquetTimes();

public:
	// �ι���־
	void VisitLog(	
		const UINT64 role_id,
		const std::string& role_name)
	{
		// ���ӱ����ʴ���
		garden_log_.visit_times += 1;
		AddEventLog(NULL,role_id,role_name,friendVisit,0,true);
	}
public:

	// ��ȡ�ι���־
	INT32 GetVisitTimes()
	{
		return garden_log_.visit_times;
	}

	// ��ȡ��־��Ϣ
	void GetEventLog(std::list<KKSG::GardenEventLog*>& log)
	{
		// �¼���־
		auto logit		= garden_log_.event_log.begin();
		auto logit_end	= garden_log_.event_log.end();
		for (; logit != logit_end; ++logit)
		{
			log.push_back(*logit);
		}	
	}

public:
	// for test
	void SetCookLevel(const UINT32 cook_level);
	void SetCookExperiences(const UINT32 experiences);

	// fish
	void SetFishLevel(const UINT32 fish_level);
	void SetFishExperiences(const UINT32 experiences);

	// plant 
	void AddGrownAmount(
		const UINT32 farmland_id,
		const UINT32 amount);

public:
	UINT64 GetGardenID() {return garden_id_; }
	UINT32 GetExpelspriteTimes();
	void AddExpelspriteTimes();
private:
	UINT64 garden_id_;
	std::string garden_name_;
	std::unordered_map<int,IGardenSerializerInterface*> all_garden_info_;
public:
	// ��־����
	void AddEventLog(
		PlantInforRecord* record,
		const UINT64 role_id,
		const std::string& role_name,
		const UINT32 event_type,
		const UINT32 target,
		const bool result);
public:
	void GetPlantInfo(std::vector<GardenPlantStatus>& plant_status);

private:
	// ���˵���ֲ��¼
	GardenPlant* plant_;
	Switch swithc_;
	GardenStoreLogInfo garden_log_;
	AddLogFunc add_log_func_;
};

#endif // __HOME_GARDEN_H__