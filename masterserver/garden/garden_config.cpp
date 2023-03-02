#include "pch.h"
#include "garden/garden_config.h"

INSTANCE_SINGLETON(GardenConfig);

GardenConfig::GardenConfig()
{

}

GardenConfig::~GardenConfig()
{

}


bool GardenConfig::CheckFile()
{
	PlantSeed tmep_plant_seed;
	if (!tmep_plant_seed.LoadFile("table/PlantSeed.txt"))
	{
		SSWarn<<"check file table/PlantSeed.txt failed"<<END;
		return false;
	}

	PlantSprite temp_plant_sprite;
	if (!temp_plant_sprite.LoadFile("table/PlantSprite.txt"))
	{
		SSWarn<<"check file table/PlantSprite.txt failed"<<END;
		return false;
	}
	temp_plant_sprite.Clear();

	GardenFishConfig temp_gardenfish;
	if (!temp_gardenfish.LoadFile("table/GardenFishing.txt"))
	{
		SSWarn<<"check file table/GardenFishing.txt failed"<<END;
		return false;
	}
	temp_gardenfish.Clear();

	FishInfo temp_fish_info;
	if (!temp_fish_info.LoadFile("table/FishInfo.txt"))
	{
		SSWarn<<"check file table/FishInfo.txt failed"<<END;
		return false;
	}

	CookingLevel temp_cooking_level;
	if (!temp_cooking_level.LoadFile("table/CookingLevel.txt"))
	{
		SSWarn<<"check file table/CookingLevel.txt failed"<<END;
		return false;
	}

	CookingFoodInfo temp_cooking_food;
	if (!temp_cooking_food.LoadFile("table/CookingFoodInfo.txt"))
	{
		SSWarn<<"check file table/CookingFoodInfo.txt failed"<<END;
		return false;
	}
	GardenBanquetCfg temp_banquet;
	if (!temp_banquet.LoadFile("table/GardenBanquetCfg.txt"))
	{
		SSWarn<<"check file table/GardenBanquetCfg.txt failed"<<END;
		return false;
	}
	return true;
}


bool GardenConfig::LoadFile()
{
	bool bRet = CheckFile();
	if (bRet == false)
	{
		return false;
	}

	plant_seed_.Clear();
	if (!plant_seed_.LoadFile("table/PlantSeed.txt"))
	{
		SSWarn<<"load file table/PlantSeed.txt failed"<<END;
		return false;
	}

	plant_sprite_.Clear();
	if (!plant_sprite_.LoadFile("table/PlantSprite.txt"))
	{
		SSWarn<<"load file table/PlantSprite.txt failed"<<END;
		return false;
	}

	gardenfish_config_.Clear();
	if (!gardenfish_config_.LoadFile("table/GardenFishing.txt"))
	{
		SSWarn<<"load file table/GardenFishing.txt failed"<<END;
		return false;
	}

	fish_info_.Clear();
	if (!fish_info_.LoadFile("table/FishInfo.txt"))
	{
		SSWarn<<"load file table/FishInfo.txt failed"<<END;
		return false;
	}

	cooking_level_.Clear();
	if (!cooking_level_.LoadFile("table/CookingLevel.txt"))
	{
		SSWarn<<"load file table/CookingLevel.txt failed"<<END;
		return false;
	}

	cooking_food_.Clear();
	if (!cooking_food_.LoadFile("table/CookingFoodInfo.txt"))
	{
		SSWarn<<"load file table/CookingFoodInfo.txt failed"<<END;
		return false;
	}

	banquet_.Clear();
	if (!banquet_.LoadFile("table/GardenBanquetCfg.txt"))
	{
		SSWarn<<"load file table/GardenBanquetCfg.txt failed"<<END;
		return false;
	}
	return true;
}

bool GardenConfig::Init()
{
	return LoadFile();
}

void GardenConfig::Uninit()
{

}

UINT32  GardenConfig::GetCookingLevelByExperiences(const UINT32 experiences)
{
	UINT32 level =0;
	UINT32 temp_experiences = experiences;
	auto it = cooking_level_.Table.begin();
	auto it_end = cooking_level_.Table.end();
	for (; it != it_end; ++it)
	{
		if (temp_experiences > (UINT32)((*it)->Experiences) && level <= (UINT32)((*it)->CookLevel) )
		{
			auto cfg = cooking_level_.GetByCookLevel((*it)->CookLevel+1);
			if (cfg != NULL)
			{
				level = (*it)->CookLevel;
				temp_experiences -= (*it)->Experiences;
			}
		}
	}		

	return level;
}


UINT32 GardenConfig::GetExperiencesByLevel(const UINT32 level)
{
	auto rowdate = cooking_level_.GetByCookLevel(level);
	if (rowdate == NULL)
	{	
		if (level > 0)
		{
			int size = cooking_level_.Table.size();
			if (size >= 1)
			{
				return cooking_level_.Table[size-1]->Experiences;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}

	return rowdate->Experiences;
}


CookingFoodInfo::RowData* GardenConfig::GetByCookBookID(UINT32 cook_book_id)
{
	auto it = cooking_food_.Table.begin();
	auto it_end = cooking_food_.Table.end();
	for ( ; it != it_end; ++it)
	{
		if ((*it)->CookBookID == cook_book_id)
		{
			return *it;
		}
	}
	return NULL;
}