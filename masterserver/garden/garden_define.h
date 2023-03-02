#ifndef _HOME_GARDEN_DEFINE_
#define _HOME_GARDEN_DEFINE_

#include <string>
#include "pb/project.pb.h"
#include "define.h"
#include <list>
#include <unordered_map>
#include "define/itemdef.h"
//#include "garden/garden_utility.h"
#include "garden/garden_config.h"
#include "table/globalconfig.h"

enum GardenDBOperateType
{
	dbCreate = 0,
	dbUpdate,	
	dbDelete,
};


const UINT32 PLANT_GROWUP_FIRST = 1;
const UINT32 PLANT_GROWUP_SECOND = 2;
const UINT32 BANQUET_FINISH_STAGE = 5;
const UINT32 GARDEN_TOP_FRIEND = 3;
const UINT32 FISH_STOSH_ID = 5500;
const UINT32 QUALITY_FOUR = 4;
const UINT32 FISH_LEVEL_FOUR = 4;
const UINT32 FISH_LEVEL_FIVE = 5;
const UINT32 COOK_LEVEL_FOUR = 4;
const UINT32 BANQUET_TEN = 10;
const UINT32 GARDEN_SYSTEM_ID = 150;

enum GardenAchievementType
{
	firstFish = 1,
	firstFourQuality,	
	firstHavest,
	firstSteal,
	firstExtra,	
	firstCook,	
};

enum GardenDesignation
{
	fishLevel2  = 1,
	fishLevel5,
	firstRichCorp,
	cookLevel2,
	cookLevel4,
	firstBanquet,
	tenBanquet,
	firstJoinBanquet,
};
enum GardenEventType
{
	friendWater = 1,
	friendDisinsection,
	friendFertilize,
	friendSteal,
	spriteTrouble,
	spriteExpel,
	spritedisappear,
	friendVisit,
};

struct FoodInfo
{
	UINT32 cooking_level;
	UINT32 cooking_experiences;
	std::unordered_map<UINT32,UINT32> food_id;
};

struct GardenBanquetStore
{
	GardenBanquetStore()
	{
		banquet_times_ = 0;
		time = 0;
		joinbanquet_times = 1;
	}

	UINT32 banquet_times_;
	UINT32 time;
	UINT32 joinbanquet_times;
};
struct PlantInforRecord
{
	PlantInforRecord()
	{
		owner = 0 ;
		seed_id=0;
		plant_grow_state=0;
		growup_amount=0;
		mature_duration=0;
		grow_times=0;
		start_time=0;		
		growup_cd=0;		
		stealed_times =0;
		notice_times =0; 
		record_event_log.clear();
	}
	UINT64 owner;
	UINT32 seed_id;
	UINT32 plant_grow_state;
	float  growup_amount;
	UINT32 mature_duration;
	UINT32 grow_times;
	UINT32 start_time;
	UINT32 growup_cd;
	//每个作物被偷的次数
	UINT32 stealed_times;
	UINT32 notice_times; 
	std::vector<KKSG::GardenEventLog> record_event_log;
};

struct PlantInfoStore
{	
	PlantInfoStore()
	{
		plantinfo_record.clear();
		allow_plantid.clear();
		help_info.clear();		
		harvest  = false;
		steal  = false; 
		extra  = false;
		rich_crop = false;
		sprite_appeartimes= 0;
		sprite_durations = 0;
		sprite_id = 0;
		sprite_appear_cd = 0;
		steal_count = 0;
	}

	// 允许种植的地块
	std::set<UINT32> allow_plantid;
	// 是否收获
	bool harvest;
	// 是否偷菜
	bool steal;
	// 是否额外奖励
	bool extra;
	// 是否丰收
	bool rich_crop;
	// 总的偷菜次数
	UINT32 steal_count;
	// 小妖出现的次数
	UINT32 sprite_appeartimes;
	// 持续的时长
	UINT32 sprite_durations;
	// 小妖id
	UINT32 sprite_id;
	UINT32 sprite_appear_cd;
	std::unordered_map<UINT64,UINT32> help_info;	
	std::unordered_map<UINT32,PlantInforRecord*> plantinfo_record;
};

struct FishInfoStore
{
	FishInfoStore()
	{
		fish_level = 1;
		experiences = 0;
		quality_four = false;
	}

	// 钓鱼等级
	UINT32 fish_level;
	//经验
	UINT32 experiences;
	// 钓到过品质4的鱼
	bool quality_four;
};

struct GardenCookingInfoStore
{
	UINT32 cook_level;
	UINT32 experiences;
	std::unordered_map<UINT32,UINT32> cooking_info;
};

struct GardenStoreLogInfo
{
	UINT32 visit_times;
	UINT32 expelsprite_times;
	UINT32 refresh_time;
	std::unordered_map<UINT64,UINT32> visit_log;
	std::list<KKSG::GardenEventLog*> event_log;
	GardenStoreLogInfo()
	{
		visit_times = 0;
		expelsprite_times = 0;
		visit_log.clear();
		event_log.clear();
		refresh_time  = 0;
	}
};

struct GardenPlantStatus
{
	GardenPlantStatus()
	{
		farmland_id = 0;
		sprite_id = 0;
		buff_id = 0; 
	}

	UINT32 farmland_id;
	UINT32 sprite_id;
	UINT32 buff_id;
};

class IGardenSerializerInterface
{
public:
	virtual ~IGardenSerializerInterface() {}

	virtual bool IsChanged() = 0;
	virtual bool Load(const KKSG::GardenInfo& garden_info) = 0;
	virtual void Save(KKSG::GardenInfo& garden_info) = 0;
	virtual const std::string ToBinary() = 0;
	virtual void  OnTimer()=0;
};

typedef std::unordered_map<UINT32,PlantInforRecord*>::const_iterator  Rcit_const;

typedef std::function<void(PlantInforRecord*,const UINT64,const std::string&,const UINT32,const UINT32,const bool)> AddLogFunc;


#endif // _HOME_GARDEN_DEFINE_