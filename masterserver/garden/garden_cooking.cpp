#include "pch.h"
#include "garden/garden_cooking.h"
#include "garden/garden_config.h"
#include "garden/garden_manager.h"
#include "goalawards/goalawardsmgr.h"
///////////////////////////////////////////

GardenCooking::GardenCooking(const UINT64 garden_id)
{
	cooking_level_ = 1;
	cooking_total_times_ = 0;
	cooking_treasure_totaltimes_ = 0;
	cooking_experiences_ = 0;
	cook_food_.clear();
	garden_id_ = garden_id;
}

GardenCooking:: ~GardenCooking() 
{

}

bool GardenCooking::IsChanged() 
{
	return swithc_.TestAndReset();
}

bool GardenCooking::Load(const KKSG::GardenInfo& garden_info) 
{
	if (!garden_info.has_cooking() || garden_info.cooking().empty())
	{
		return false;
	}

	KKSG::CookingInfo cooking_info;
	if (!cooking_info.ParseFromString(garden_info.cooking()))
	{
		return false;
	}
	cooking_level_ = cooking_info.cooking_level();
	cooking_experiences_ = cooking_info.cooking_exp();
	cooking_total_times_ = cooking_info.cooking_total_times();
	cooking_treasure_totaltimes_ = cooking_info.cooking_treasure_total_times();

	int cook_food_size = cooking_info.cooking_food_size();
	for ( int i =0; i < cook_food_size ; ++i)
	{
		const  KKSG::MapIntItem& item = cooking_info.cooking_food(i);
		cook_food_.insert(std::make_pair((UINT32)item.key(),item.value()));
	}

	int cook_books_size = cooking_info.cook_book_id_size();
	for ( int i =0; i < cook_books_size ; ++i)
	{
		UINT32 cook_book_id  = cooking_info.cook_book_id(i);
		cook_books_.insert(cook_book_id);		
	}

	return true;
}


void GardenCooking::Save(KKSG::GardenInfo& garden_info) 
{
	garden_info.mutable_cooking()->assign(ToBinary());
}

const std::string GardenCooking::ToBinary() 
{
	KKSG::CookingInfo cooking_info;
	cooking_info.set_cooking_level(cooking_level_);
	cooking_info.set_cooking_exp(cooking_experiences_);
	cooking_info.set_cooking_total_times(cooking_total_times_);
	cooking_info.set_cooking_treasure_total_times(cooking_treasure_totaltimes_);

	auto it = cook_food_.begin();
	auto it_end = cook_food_.end();
	for (; it != it_end; ++it)
	{
		KKSG::MapIntItem* item = cooking_info.add_cooking_food();
		item->set_key(it->first);
		item->set_value(it->second);
	}

	auto bookit = cook_books_.begin();
	auto bookit_end = cook_books_.end();
	for ( ; bookit != bookit_end;++bookit )
	{
		cooking_info.add_cook_book_id(*bookit);
	}

	return cooking_info.SerializeAsString();
}

bool GardenCooking::Refresh()
{
	// 清理之前一天烹饪信息
	auto it = cook_food_.begin();
	auto it_end = cook_food_.end();
	for (; it != it_end; ++it)
	{
		it->second = 0;
	}

	swithc_.Set();
	GardenManager::Instance()->SetChanged(garden_id_);
	
	return true;
}

void GardenCooking::OnTimer()
{
	return;
}


///////////////////////////////////////////


bool GardenCooking::ExistCookBook(const UINT32 cook_book_id)
{
	auto rowdata = GardenConfig::Instance()->GetByCookBookID(cook_book_id);
	if (rowdata == NULL)
	{
		SSWarn<<" no this cook_book_id : "<<cook_book_id<<END;
		return false;
	}

	if (rowdata->CookBookID > 0)
	{
		std::set<UINT32>::iterator it = cook_books_.find(cook_book_id);
		if (it == cook_books_.end())
		{
			return false;
		}
	}

	return true;
}


UINT32 GardenCooking::CheckCookingCondion(const UINT32 food_id)
{
	auto config = GardenConfig::Instance()->GetCooking().GetByFoodID(food_id);
	if (config == NULL)
	{
		SSWarn<<"config is  NULL, food_id:"<<food_id<<END;
		return KKSG::ERR_INVALID_REQUEST;
	}

	// 等级检查
	if (cooking_level_ < config->Level)
	{
		SSWarn<<"cook_level , cook_level:"<<cooking_level_<<END;
		return KKSG::ERR_GARDEN_COOKINGLEVEL_LOW; 
	}

	// 检查次数
	auto it = cook_food_.find(food_id);
	if (it != cook_food_.end())
	{
		if (config->Frequency > 0 && it->second >= config->Frequency)
		{
			return KKSG::ERR_GARDEN_COOKING_EXCEED;  ;
		}
	}

	return KKSG::ERR_SUCCESS;
}


UINT32 GardenCooking::CookingFoodComplete(const UINT32 food_id)
{
	// 增加次数
	auto it = cook_food_.find(food_id);
	if (it == cook_food_.end())
	{
		cook_food_.insert(std::make_pair(food_id,1));
	}
	else
	{
		it->second +=1;
	}

	auto config = GardenConfig::Instance()->GetCooking().GetByFoodID(food_id);
	if (config == NULL)
	{
		SSWarn<<"config is  NULL, food_id:"<<food_id<<END;
		return KKSG::ERR_INVALID_REQUEST;
	}
	cooking_experiences_ += config->AddExp;
	UINT32 temp_level = cooking_level_;
	UINT32 exp = GardenConfig::Instance()->GetExperiencesByLevel(temp_level);
	while (cooking_experiences_ >= exp)
	{
		cooking_level_ += 1;
		cooking_experiences_ -= exp;
		temp_level = cooking_level_;
        GoalAwardsMgr::Instance()->FreshValue(garden_id_, GOAL_AWARD_ID_801, temp_level, 0);
		exp  = GardenConfig::Instance()->GetExperiencesByLevel(temp_level);
		if (0 == exp)
		{
			break;
		}
	}

	swithc_.Set();
	GardenManager::Instance()->SetChanged(garden_id_);

	return KKSG::ERR_SUCCESS;
}


void GardenCooking::AddCookBook(const UINT32 cook_book_id)
{
	cook_books_.insert(cook_book_id);

	auto rowdata = GardenConfig::Instance()->GetByCookBookID(cook_book_id);
	if (rowdata == NULL)
	{
		SSWarn<<" no this cook_book_id : "<<cook_book_id<<END;
		return ;
	}
	cook_food_.insert(std::make_pair(rowdata->FoodID,0));

	swithc_.Set();
	GardenManager::Instance()->SetChanged(garden_id_);
}

void GardenCooking::SetCookLevel(const UINT32 cook_level)
{
   cooking_level_ = cook_level;
}
