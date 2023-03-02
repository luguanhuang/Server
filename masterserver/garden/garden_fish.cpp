#include "pch.h"
#include "garden/garden_fish.h"
#include "util/XRandom.h"
#include "garden/garden_manager.h"
#include "goalawards/goalawardsmgr.h"
GardenFish::GardenFish(const UINT64 garden_id)
{
	fishinfo_store_.fish_level = 1;	
	fishinfo_store_.experiences = 0;
	garden_id_ = garden_id;
}

GardenFish:: ~GardenFish() 
{
}

bool GardenFish::IsChanged() 
{
	return swithc_.TestAndReset();
}

bool GardenFish::Load(const KKSG::GardenInfo& garden_info) 
{
	if (!garden_info.has_fishing() || garden_info.fishing().empty())
	{
		return false;
	}

	KKSG::GardenFishInfo garden_fishinfo;
	if (!garden_fishinfo.ParseFromString(garden_info.fishing()))
	{
		return false;
	}

	fishinfo_store_.fish_level = garden_fishinfo.fish_level();
	fishinfo_store_.experiences = garden_fishinfo.fish_exp();	
	fishinfo_store_.quality_four = garden_fishinfo.quality_four();

	return true;
}

void GardenFish::Save(KKSG::GardenInfo& garden_info) 
{
	garden_info.mutable_fishing()->assign(ToBinary());
}

const std::string GardenFish::ToBinary() 
{
	KKSG::GardenFishInfo garden_fishinfo;
	garden_fishinfo.set_fish_level(fishinfo_store_.fish_level);
	garden_fishinfo.set_fish_exp(fishinfo_store_.experiences);
	garden_fishinfo.set_quality_four(fishinfo_store_.quality_four);

	return garden_fishinfo.SerializeAsString();
}

UINT32 GardenFish::TryFishing(std::vector<ItemDesc>& items)
{
	auto fish_config = GardenConfig::Instance()->GetFish().GetByFishLeve(fishinfo_store_.fish_level);
	UINT32 fishid = 0;

	UINT32 success_rand = XRandom::randInt(1,101);

	LogDebug("cfg rate is %d,and the rand rate is %d",fish_config->SuccessRate,success_rand);

	if (success_rand < fish_config->SuccessRate)
	{
		UINT32 total_weight = 0;
		auto it = fish_config->FishWeight.begin();
		for (; it != fish_config->FishWeight.end(); ++it)
		{
			total_weight += it->seq[1];
		}

		UINT32 weight_rand = XRandom::randInt(1,total_weight+1);
		total_weight =0;

		auto fish_it = fish_config->FishWeight.begin();
		auto fish_it_end = fish_config->FishWeight.end();
		for (; fish_it != fish_it_end; ++fish_it)
		{
			total_weight += fish_it->seq[1];
			if (total_weight >= weight_rand)
			{
				fishid = fish_it->seq[0];
				break;
			}
		}
	}

	if (fishid >0)
	{				
		// 经验 等级的变化
		auto  fish_info = GardenConfig::Instance()->GetFishInfo().GetByFishID(fishid);
		if (fish_info == NULL)
		{
			SSWarn<<" no this fish  fishid  : "<<fishid<<END;
			return 0;		
		}
		// 钓到的鱼的信息
		ItemDesc item ;
		item.itemID = fishid;
		item.itemCount = 1;
		item.isbind = fish_info->isbind == 0 ? false:true;
		items.push_back(item);

		fishinfo_store_.experiences += fish_info->Experience;	

		auto cfg = GardenConfig::Instance()->GetFish().GetByFishLeve(fishinfo_store_.fish_level);
		if (cfg == NULL)
		{
			SSWarn<<" fishinfo_store_.fish_level err  : "<<fishinfo_store_.fish_level<<END;
			return 0;
		}
		UINT32 exp = cfg->Experience;
		while (fishinfo_store_.experiences >= exp)
		{
			cfg  = GardenConfig::Instance()->GetFish().GetByFishLeve(fishinfo_store_.fish_level+1);
			if (NULL == cfg)
			{
				break;
			}
			fishinfo_store_.experiences -= exp;
			fishinfo_store_.fish_level += 1;
            GoalAwardsMgr::Instance()->FreshValue(garden_id_, GOAL_AWARD_ID_803, fishinfo_store_.fish_level, 0);
			exp = cfg->Experience;
		}		
	}

	// 钓到了鱼
	if (items.size()>0)
	{
		swithc_.Set();
		GardenManager::Instance()->SetChanged(garden_id_);
	}

	return 0;
}

void GardenFish::SetFishLevel(const UINT32 fish_level)
{
    fishinfo_store_.fish_level = fish_level;
}

void  GardenFish::OnTimer()
{
	return ;
}