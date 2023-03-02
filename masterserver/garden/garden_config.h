#ifndef _HOME_GARDEN_CONFIG_
#define _HOME_GARDEN_CONFIG_

#include "table/PlantSeed.h"
#include "table/PlantSprite.h"
#include "table/GardenFishConfig.h"
#include "table/FishInfo.h"
#include "table/CookingLevel.h"
#include "table/CookingFoodInfo.h"
#include "table/GardenBanquetCfg.h"

class GardenConfig
{
	GardenConfig();
	~GardenConfig();
	DECLARE_SINGLETON(GardenConfig);
public:
	bool CheckFile();
	bool LoadFile();
	bool Init();
	void Uninit();	

	PlantSeed& GetSeed() 
	{
		return plant_seed_;
	}

	PlantSprite&  GetSprite()
	{
		return plant_sprite_;
	}

	GardenFishConfig& GetFish()
	{
		return gardenfish_config_;
	}

	FishInfo&  GetFishInfo()
	{
		return fish_info_;
	}
	UINT32  GetCookingLevelByExperiences(const UINT32 experiences);	
	UINT32 GetExperiencesByLevel(const UINT32 level);
	CookingFoodInfo& GetCooking()
	{
		return cooking_food_;
	}

	CookingFoodInfo::RowData* GetByCookBookID(UINT32 cook_book_id);

	GardenBanquetCfg& GetBanquet()
	{
		return banquet_;
	}
private:
	// 种子成长的配置信息
	PlantSeed	plant_seed_; 

	PlantSprite plant_sprite_;

	GardenFishConfig gardenfish_config_;
	FishInfo fish_info_;

	CookingLevel cooking_level_;
	CookingFoodInfo cooking_food_;

	GardenBanquetCfg banquet_;
};
#endif  // _HOME_GARDEN_CONFIG_