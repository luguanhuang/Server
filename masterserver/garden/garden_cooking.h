#ifndef __HOME_GARDEN_COOKING_H__
#define __HOME_GARDEN_COOKING_H__
#include <string>
#include "garden/garden_define.h"
#include "role/switch.h"

class GardenCooking : public IGardenSerializerInterface 
{
public:
	GardenCooking(const UINT64 garden_id);
	virtual ~GardenCooking() ;

	virtual bool IsChanged() ;
	virtual bool Load(const KKSG::GardenInfo& garden_info) ;
	virtual void Save(KKSG::GardenInfo& garden_info) ;
	virtual const std::string ToBinary() ;
	virtual void OnTimer();
	
public:
	bool Refresh();
public:
	// 是否存在这个cook_book_id
	bool ExistCookBook(const UINT32 cook_book_id);
	// 检查烹饪条件
	UINT32 CheckCookingCondion(const UINT32 food_id);
	// 烹饪完成数据的更新
	UINT32 CookingFoodComplete(const UINT32 food_id);

	void GetGardenCookingInfo(FoodInfo& info)
	{
		info.cooking_level = cooking_level_;
		info.cooking_experiences = cooking_experiences_;
		auto it = cook_food_.begin();
		auto it_end = cook_food_.end();
		for (; it != it_end; ++it)
		{
			info.food_id.insert(std::make_pair(it->first,it->second));
		}
	}
	void AddCookBook(const UINT32 cook_book_id);
public:
	// for test
	void SetCookLevel(const UINT32 cook_level);


	void SetCookExperiences(const UINT32 experiences)
	{
		cooking_experiences_ = experiences;
	}

private:
	Switch swithc_;	
	UINT32 cooking_level_;
	
	UINT32 cooking_total_times_;
	UINT32 cooking_treasure_totaltimes_;
	UINT32 cooking_experiences_;
	std::unordered_map<UINT32,UINT32>  cook_food_;
	std::set<UINT32> cook_books_;

	UINT64 garden_id_;
};

#endif // __HOME_GARDEN_COOKING_H__