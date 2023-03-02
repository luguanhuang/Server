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
	// 检查种植条件
	UINT32 CheckPlantCondition(
		const UINT32 farmland_id,
		const UINT32 seed_id);

	// 种植
	UINT32 TryPlantSeed(
		const UINT64 role_id,
		const UINT32 farmland_id,
		const UINT32 seed_id);

	UINT32 CanclePlant(const UINT32 farmland_id);
	// 更新种植记录
	void UpdatePlantRecord(
		const UINT32 farmland_id,
		const UINT64 enemy_uid);

	// 获取种植记录
	const PlantInfoStore& GetPlantInfoStore();

	// 培养操作
	UINT32 PlantCultivation(	
		const UINT32 farmland_id,
		const UINT32 operate_type);

	// 照料植物
	UINT32 PlantCultivationByPlayer(
		const UINT64 role_id,
		const std::string& role_name,
		const UINT32 farmland_id,
		const UINT32 operate_type);

	// 收割
	UINT32 PlantHarvest(
		const UINT32 farmland_id,
		std::vector<ItemDesc>& gain_items,
		bool& harvest,
		bool& extra);

	// 赶走小妖
	UINT32 ExpelSprite(
		const UINT64 role_id,
		const std::string& role_name,
		const UINT32 sprite_id,
		std::vector<ItemDesc>& award_items);

	// 朋友偷菜
	UINT32 StealHarvest(
		const UINT64 role_id,
		const std::string& role_name,
		const UINT32 farmland_id,
		std::vector<ItemDesc>& steal_items);

	// 开垦新地块
	void AddFarmland(const UINT32 farmland_id);
	void SetSteal();
	void AddStealCount();
public:
	// 钓鱼
	UINT32 TryFishing(std::vector<ItemDesc>& items);

	// 获取钓鱼信息
	FishInfoStore& GetFishInfo();
public:
	// 存在菜谱
	bool ExistCookBook(const UINT32 cook_book_id);

	// 检查烹饪条件
	UINT32 CheckCookingCondion(const UINT32 food_id);

	// 做完菜，更新记录
	UINT32 CookingFoodComplete(const UINT32 food_id);

	// 获取做菜信息
	void GetGardenCookingInfo(FoodInfo& info);

	// 增加菜谱
	void AddCookBook(const UINT32 cook_book_id);
public:
	// 宴会
	void ServeBanquet(const UINT32 banquet_id,const std::vector<UINT64>& guests);

	// 获取宴会状态
	bool GetBanquetStatus();
	UINT32 GetBanquetTimes();
	UINT32 GetAwardTimesTSWK();
	void AddBanquetTimes();

public:
	// 参观日志
	void VisitLog(	
		const UINT64 role_id,
		const std::string& role_name)
	{
		// 增加被访问次数
		garden_log_.visit_times += 1;
		AddEventLog(NULL,role_id,role_name,friendVisit,0,true);
	}
public:

	// 获取参观日志
	INT32 GetVisitTimes()
	{
		return garden_log_.visit_times;
	}

	// 获取日志信息
	void GetEventLog(std::list<KKSG::GardenEventLog*>& log)
	{
		// 事件日志
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
	// 日志函数
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
	// 个人的种植记录
	GardenPlant* plant_;
	Switch swithc_;
	GardenStoreLogInfo garden_log_;
	AddLogFunc add_log_func_;
};

#endif // __HOME_GARDEN_H__