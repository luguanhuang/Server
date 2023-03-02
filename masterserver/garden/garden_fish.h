#ifndef __HOME_GARDEN_FISH_H__
#define __HOME_GARDEN_FISH_H__
#include <string>
#include "garden/garden_define.h"
#include "role/switch.h"
#include "garden/garden_config.h"


class GardenFish : public IGardenSerializerInterface 
{
public:
	GardenFish(const UINT64 garden_id);
	virtual ~GardenFish() ;

	virtual bool IsChanged() ;
	virtual bool Load(const KKSG::GardenInfo& garden_info) ;
	virtual void Save(KKSG::GardenInfo& garden_info) ;
	virtual const std::string ToBinary() ;
	virtual void  OnTimer();
	
public:

	// 尝试钓鱼
	UINT32 TryFishing(std::vector<ItemDesc>& items);
	// 获取钓鱼信息
	FishInfoStore& GetFishInfo() { return fishinfo_store_;}

public:
	// for test 
	void SetFishLevel(const UINT32 fish_level);


	void SetFishExperiences(const UINT32 experiences)
	{
		fishinfo_store_.experiences = experiences;
	}
private:
	Switch swithc_;
	
	FishInfoStore  fishinfo_store_;
	
	UINT64 garden_id_;
};

#endif // __HOME_GARDEN_PLANT_H__