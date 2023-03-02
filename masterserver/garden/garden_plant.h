#ifndef __HOME_GARDEN_PLANT_H__
#define __HOME_GARDEN_PLANT_H__

#include "garden/garden_define.h"
#include "role/switch.h"
#include "garden/garden_plant.h"
#include <string>

class GardenPlant : public IGardenSerializerInterface 
{
public:
	GardenPlant(
		const UINT64 role_id,
		const std::string& role_name);

	virtual ~GardenPlant() ;

	virtual bool IsChanged() ;
	virtual bool Load(const KKSG::GardenInfo& garden_info) ;
	virtual void Save(KKSG::GardenInfo& garden_info) ;
	virtual const std::string ToBinary() ;
	
	virtual void  OnTimer();
	
	bool Refresh();
public:
	UINT32 CheckPlantCondition(
		const UINT32 farmland_id,
		const UINT32 seed_id);

	UINT32 TryPlantSeed(
		const UINT32 farmland_id,
		const UINT32 seed_id);

	UINT32 CanclePlant(const UINT32 farmland_id);

	const PlantInfoStore& GetPlantInfoStore();

	// 更新作物状态
	void UpdatePlantRecord(
		const UINT32 farmland_id,
		const UINT64 enemy_uid);

	UINT32 PlantCultivation(	
		const UINT32 farmland_id,
		const UINT32 operate_type);

	UINT32 PlantHarvest(
		const UINT32 farmland_id,
		std::vector<ItemDesc>& gain_items,
		bool& harvest,
		bool& extra);

	// 增加新的地块
	void AddFarmland(const UINT32 farmland_id);
public:
	// 朋友偷菜
	UINT32 StealHarvest(
		const UINT64 role_id,
		const std::string& role_name,
		const UINT32 farmland_id,
		std::vector<ItemDesc>& steal_items);

	// 培养接口
	UINT32 PlantCultivationByPlayer(
		const UINT64 role_id,
		const std::string& role_name,
		const UINT32 farmland_id,
		const UINT32 operate_type);

	// 赶走小妖
	UINT32 ExpelSprite(
		const UINT64 role_id,
		const std::string& role_name,
		const UINT32 sprite_id,
		std::vector<ItemDesc>& award_items);

private:
	bool PlantHarvesState(
		PlantInforRecord& record,
		const PlantSeed::RowData& config);

	void GetPlantHarvestItems(
		const PlantInforRecord& record,
		const PlantSeed::RowData& config,
		std::vector<ItemDesc>& gain_items,
		bool& harvest,
		bool& extra);

	bool CheckNeedSendHarvestItem(
		PlantInforRecord& record,
		const UINT32 duration);

	void DeletePlantRecordByFarmland(const UINT32 farmland_id);

	void PlantSprite();
	bool SpriteInfluence();

public:
	// 设置日志函数
	void SetAddLogFunc(AddLogFunc& func)
	{
		add_log_func_ = func;
	}

	// 设置偷菜标记
	void SetSteal();
	
	// 增加偷菜次数
	void AddStealCount();
public:
	// test 
	void AddGrownAmount(
		const UINT32 farmland_id,
		const UINT32 amount)
	{
		auto rcit = plant_info_store_.plantinfo_record.find(farmland_id);
		auto rcit_end = plant_info_store_.plantinfo_record.end();
		if (rcit == rcit_end)
		{
			SSWarn << "AddGrownAmount not exist this record ,farmland_id is  : "<<farmland_id <<END;
			return;
		}

		rcit->second->growup_amount += amount;
	}
private:
	Switch swithc_;
	UINT64 garden_id_;
	std::string garden_name_;
	PlantInfoStore  plant_info_store_;
	
	AddLogFunc add_log_func_;
};

#endif // __HOME_GARDEN_PLANT_H__