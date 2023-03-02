#include "pch.h"
#include "garden/garden_banquet.h"
#include "garden/garden_config.h"
#include "garden/garden_utility.h"
#include "mail/mailmgr.h"
#include "util/XCommon.h"
#include "garden/garden_utility.h"
#include "garden/garden.h"
#include "garden/garden_manager.h"
#include "goalawards/goalawardsmgr.h"
///////////////////////////////////////////

Banquet::Banquet()
{
	banquet_id_ = 0;
	stage_counter_ = 0;
	stage_id_ = 0;
	garden_id_ = 0;
	runing = false;
	banquet_stage_.clear();
	guests_.clear();	
}

Banquet::~Banquet()
{
}

void Banquet::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	auto it = banquet_stage_.find(stage_id_);
	if (it != banquet_stage_.end())
	{
		if (++stage_counter_ >= it->second)
		{
			UINT32 removeid = stage_id_;

			// 如果不是最后一条，还需要继续
			if (banquet_stage_.size()> 0)
			{
				stage_id_ += 1;				
			}
			else
			{
				stage_id_ = 0;					
			}
			stage_counter_ = 0;
			banquet_stage_.erase(removeid);
			
			KKSG::GardenPlayStageChange arg;
			arg.set_event_type(KKSG::BANQUET_STAGE);	
			if (BANQUET_FINISH_STAGE == stage_id_)
			{
				arg.set_banquet_stage(0);				
			}
			else
			{
				arg.set_banquet_stage(stage_id_);
			}

			auto guestIt = guests_.begin();
			auto guestIt_end  = guests_.end();
			for (; guestIt != guestIt_end; ++guestIt)
			{	
				KKSG::MapIntItem* guest = arg.add_guests();
				guest->set_key(guestIt->key());
				guest->set_value(guestIt->value());					
			}
			arg.set_banquet_id(banquet_id_);

			GardenUtility::Instance()->NotifyEvnetChangeNtf(arg,garden_id_);
				
			// 第四阶段，发奖励
			if (BANQUET_FINISH_STAGE == stage_id_)
			{
				auto cfg = GardenConfig::Instance()->GetBanquet().GetByBanquetID(banquet_id_);
				if (NULL == cfg)
				{
					return ;
				}

				std::vector<ItemDesc> gain_items;
				std::vector<ItemDesc> gain_buff_items;
				auto itawrd =  cfg->BanquetAwards.begin();
				auto itawrd_end =  cfg->BanquetAwards.end();
				for (; itawrd != itawrd_end; ++itawrd)
				{
					ItemDesc des ;
					des.itemID = itawrd->seq[0];
					des.itemCount = itawrd->seq[1];
					gain_items.push_back(des);
				}	

				gain_buff_items.clear();
				/*ItemDesc buff_item_des ;
				buff_item_des.itemID = cfg->StuffBuffID[0];
				buff_item_des.itemCount = cfg->StuffBuffID[1];;
				buff_item_des.isbind = true;
				gain_buff_items.push_back(buff_item_des);*/

				auto git =  guests_.begin();
				auto git_end =  guests_.end();
				for (; git != git_end; ++git)
				{
					Garden*  garden = GardenUtility::Instance()->GetGardenByRoleId(git->key());
					if (NULL == garden)
					{		
						LogWarn("not find the garden,role id is : %llu",git->key());		
						continue;
					}
					UINT32 times = garden->GetAwardTimesTSWK();
					if (times > GlobalConfig::GetInstance().GardenBanquetAwardTimesTSWK )
					{
						CMailMgr::Instance()->SendMail(git->key(),MailConf_BANQUET_OVERTIMES, gain_buff_items);
					}
					else
					{
						CMailMgr::Instance()->SendMail(git->key(),MailConf_BANQUET, gain_items);
					}	
					garden->AddBanquetTimes();
				}
					
				Cancel();
			}
		}		
	}	
}

void Banquet::Run()
{
	garden_timehandle_ = CTimerMgr::Instance()->SetTimer(this ,0,GARDEN_BANQUET_TIME, -1, __FILE__, __LINE__);
	runing = true;
}

void Banquet::Cancel()
{
	if (INVALID_HTIMER != garden_timehandle_)
	{
		CTimerMgr::Instance()->KillTimer(garden_timehandle_);
		garden_timehandle_ = INVALID_HTIMER;
	}
	//	清理
	stage_id_= 0;
	banquet_id_ = 0;
	stage_counter_ = 0;
	banquet_stage_.clear();
	guests_.clear();
	runing = false;
}


void Banquet::SetBanquet(
	const UINT32 banquet_id,
	const std::vector<UINT64>& guests)
{
	auto cfg = GardenConfig::Instance()->GetBanquet().GetByBanquetID(banquet_id);
	if (NULL == cfg)
	{
		return ;
	}

	banquet_id_ = banquet_id;
	stage_id_ = 1;
	stage_counter_ = 0;

	banquet_stage_.insert(std::make_pair(1,cfg->VoiceOver1Duration));
	banquet_stage_.insert(std::make_pair(2,cfg->VoiceOver2Duration));
	banquet_stage_.insert(std::make_pair(3,cfg->VoiceOver3Duration));
	banquet_stage_.insert(std::make_pair(4,cfg->VoiceOver4Duration));
	banquet_stage_.insert(std::make_pair(5,0));

	auto guestIt = guests.begin();
	auto guestIt_end  = guests.end();
	for (; guestIt != guestIt_end; ++guestIt)
	{
		Garden*  garden = GardenUtility::Instance()->GetGardenByRoleId(*guestIt);
		if (NULL == garden)
		{		
			LogWarn("not find the garden,role id is : %llu",*guestIt);		
			continue;
		}

		UINT32 times = garden->GetAwardTimesTSWK();
		KKSG::MapIntItem info;
		info.set_key(*guestIt);
		info.set_value(times);
		guests_.push_back(info);		
	}	
}

GardenBanquet::GardenBanquet(const UINT64 garden_id)
{	
	garden_id_ = garden_id;
}

GardenBanquet:: ~GardenBanquet() 
{
}

bool GardenBanquet::IsChanged() 
{
	return swithc_.TestAndReset();
}

bool GardenBanquet::Load(const KKSG::GardenInfo& garden_info) 
{
	if (!garden_info.has_banquet() || garden_info.banquet().empty())
	{
		return false;
	}

	KKSG::BanquetInfo banquet_info;
	if (!banquet_info.ParseFromString(garden_info.banquet()))
	{
		return false;
	}

	banquet_store_.banquet_times_ = banquet_info.banquet_times();
	banquet_store_.time = banquet_info.time();
	banquet_store_.joinbanquet_times = banquet_info.joinbanquet_times();

	return true;
}


void GardenBanquet::Save(KKSG::GardenInfo& garden_info) 
{
	garden_info.mutable_banquet()->assign(ToBinary());
}

const std::string GardenBanquet::ToBinary() 
{
	KKSG::BanquetInfo banquet_info;
	banquet_info.set_banquet_times(banquet_store_.banquet_times_);
	banquet_info.set_joinbanquet_times(banquet_store_.joinbanquet_times);
	banquet_info.set_time(banquet_store_.time);

	return banquet_info.SerializeAsString();
}

void GardenBanquet::OnTimer()
{
	return;
}


///////////////////////////////////////////
void GardenBanquet::ServeBanquet(
	const UINT32 banquet_id,
	const std::vector<UINT64>& guests)
{
	if (banquet_.runing)
	{
		SSWarn << "banquet_->runing  : "<<banquet_.runing<<END;
		return ;
	}
	
	// notice
	KKSG::GardenPlayStageChange arg;
	auto guestIt = guests.begin();
	auto guestIt_end  = guests.end();
	for (; guestIt != guestIt_end; ++guestIt)
	{
		Garden*  garden = GardenUtility::Instance()->GetGardenByRoleId(*guestIt);
		if (NULL == garden)
		{		
			LogWarn("not find the garden,role id is : %llu",*guestIt);		
			continue;
		}

		UINT32 times = garden->GetAwardTimesTSWK();		
		KKSG::MapIntItem* info = arg.add_guests();
		info->set_key(*guestIt);
		info->set_value(times);
	}

	arg.set_event_type(KKSG::BANQUET_STAGE);		
	arg.set_banquet_id(banquet_id);
	arg.set_banquet_stage(1);
	GardenUtility::Instance()->NotifyEvnetChangeNtf(arg,garden_id_);

	// start
	banquet_.SetBanquet(banquet_id,guests);
	banquet_.SetGardenID(garden_id_);
	banquet_.Run();

	++banquet_store_.banquet_times_;
    GoalAwardsMgr::Instance()->FreshValue(garden_id_, GOAL_AWARD_ID_804, 1.f, 0);
	swithc_.Set();
	GardenManager::Instance()->SetChanged(garden_id_);
}


UINT32 GardenBanquet::GetAwardTimesTSWK()
{
	UINT32 now = (UINT32)time(0);
	if (XCommon::IsWeekSame(banquet_store_.time, now))
	{
		return banquet_store_.joinbanquet_times;
	}

	return 1;
}

void GardenBanquet::AddBanquetTimes()
{
	UINT32 now = (UINT32)GameTime::GetTime();
	if (XCommon::IsWeekSame(banquet_store_.time, now))
	{
		++banquet_store_.joinbanquet_times ;
	}
	else
	{
		banquet_store_.joinbanquet_times  = 2;
		banquet_store_.time = now;
	}

	swithc_.Set();
	GardenManager::Instance()->SetChanged(garden_id_);
}