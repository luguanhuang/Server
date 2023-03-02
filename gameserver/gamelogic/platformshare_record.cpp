#include "pch.h"
#include "gamelogic/platformshare_record.h"
#include "util/gametime.h"
#include "util/XCommon.h"
#include "unit/role.h"
#include "define/tlogenum.h"
#include "gamelogic/bagtransition.h"
#include "platformshareresult/ptcg2c_platformshareawardntf.h"
#include "util/timespecificmgr.h"

const UINT32 FIVE_HOURS = 5*60*60;

PlatformShareRecord::PlatformShareRecord(Role* role)
	:role_(role)
{
	m_lastUpdateTime = 0;
	frist_pass_share_list_.clear();
	weekly_award_ = false;
	disappear_redpoint_ = false;
	weekly_share_number_ = 0;
	have_notify_scene_.clear();
	m_isModify.Set();	

	TimeSpecific time(HOUR_ONE_DAY_BEGIN, 0, 0);
	time_handle_ = CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&PlatformShareRecord::ReSetWeekly, this));
	
	consume_dragoncoins_now_ = 0;
	consume_dragoncoins_before_1_= 0;
	consume_dragoncoins_before_2_= 0;
	consume_dragoncoins_before_3_= 0;
	consume_dragoncoins_before_4_= 0;
	consume_dragoncoins_before_5_= 0;
	consume_dragoncoins_before_6_= 0;
	consume_dragoncoins_before_7_= 0;
	last_update_consume_ = 0;
}
PlatformShareRecord::~PlatformShareRecord()
{
	if (time_handle_ > 0)
	{
		CTimeSpecificMgr::Instance()->UnRegisterEvent(time_handle_);
	}
}

bool PlatformShareRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (!poRoleAllInfo->has_platformshareresult())
	{
		return true;
	}

	const KKSG::PlatformShareResult& share_result_record = poRoleAllInfo->platformshareresult();

	m_lastUpdateTime = share_result_record.last_update_time();

	for (UINT32 index = 0; index < (UINT32)share_result_record.firstpass_share_list_size(); ++index)
	{
		const KKSG::MapIntItem &temp_result =  share_result_record.firstpass_share_list(index);
		bool award_result = false;
		if (temp_result.value() == 1)
		{
			award_result = true;
		}

		frist_pass_share_list_.insert(std::make_pair(temp_result.key(),award_result));
	}

	for (UINT32 i = 0;  i < (UINT32)share_result_record.have_notify_scene_size(); ++i)
	{
		have_notify_scene_.insert(share_result_record.have_notify_scene(i));
	}

	weekly_share_number_ = share_result_record.weekly_share_number();	
	weekly_award_ = share_result_record.weekly_award();	
	disappear_redpoint_ = share_result_record.disappear_redpoint();


	consume_dragoncoins_now_ = share_result_record.consume_dragoncoins_now();
	consume_dragoncoins_before_1_= share_result_record.consume_dragoncoins_before_1();
	consume_dragoncoins_before_2_= share_result_record.consume_dragoncoins_before_2();
	consume_dragoncoins_before_3_= share_result_record.consume_dragoncoins_before_3();
	consume_dragoncoins_before_4_= share_result_record.consume_dragoncoins_before_4();
	consume_dragoncoins_before_5_= share_result_record.consume_dragoncoins_before_5();
	consume_dragoncoins_before_6_= share_result_record.consume_dragoncoins_before_6();
	consume_dragoncoins_before_7_= share_result_record.consume_dragoncoins_before_7();
	last_update_consume_ = share_result_record.last_update_consume();
	return true;
}
void PlatformShareRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{	
	Update();
	if (!m_isModify.TestAndReset())
	{
		return;
	}

	KKSG::PlatformShareResult* record = poRoleAllInfo->mutable_platformshareresult();
	record->Clear();

	for (auto it = frist_pass_share_list_.begin(); it != frist_pass_share_list_.end(); ++it)
	{
		auto *item = record->add_firstpass_share_list();
		item->set_key(it->first);
		if (it->second)
		{
			item->set_value(1);
		}
		else
		{
			item->set_value(0);
		}
	} 

	for (auto it = have_notify_scene_.begin(); it != have_notify_scene_.end(); ++it)
	{
		record->add_have_notify_scene(*it);
	}

	record->set_last_update_time(m_lastUpdateTime);
	record->set_weekly_award(weekly_award_);
	record->set_weekly_share_number(weekly_share_number_);
	record->set_disappear_redpoint(disappear_redpoint_);


	record->set_consume_dragoncoins_now(consume_dragoncoins_now_);
	record->set_consume_dragoncoins_before_1(consume_dragoncoins_before_1_);
	record->set_consume_dragoncoins_before_2(consume_dragoncoins_before_2_);
	record->set_consume_dragoncoins_before_3(consume_dragoncoins_before_3_);
	record->set_consume_dragoncoins_before_4(consume_dragoncoins_before_4_);
	record->set_consume_dragoncoins_before_5(consume_dragoncoins_before_5_);
	record->set_consume_dragoncoins_before_6(consume_dragoncoins_before_6_);
	record->set_consume_dragoncoins_before_7(consume_dragoncoins_before_7_);
	record->set_last_update_consume(last_update_consume_);

	roChanged.insert(record);
}

void PlatformShareRecord::Update()
{
	time_t now = GameTime::GetTime() - (time_t)FIVE_HOURS;
	if (!XCommon::IsWeekSame(time_t(m_lastUpdateTime) , now , false))
	{
		m_lastUpdateTime = (UINT32)now;
		weekly_award_ = false;
		disappear_redpoint_ = false;
		weekly_share_number_ = 0;
		m_isModify.Set();
	}
	time_t now_time = GameTime::GetTime();
	if (!XCommon::IsDaySame(now_time,last_update_consume_,false))
	{
		UINT32 diff_day =  GameTime::GetDiffDayCount(last_update_consume_,now_time);
		switch (diff_day)
		{
		case 1:
			{
				consume_dragoncoins_before_7_ = consume_dragoncoins_before_6_;
				consume_dragoncoins_before_6_ = consume_dragoncoins_before_5_;
				consume_dragoncoins_before_5_ = consume_dragoncoins_before_4_;
				consume_dragoncoins_before_4_ = consume_dragoncoins_before_3_;
				consume_dragoncoins_before_3_ = consume_dragoncoins_before_2_;
				consume_dragoncoins_before_2_ = consume_dragoncoins_before_1_;
				consume_dragoncoins_before_1_ = consume_dragoncoins_now_;
				consume_dragoncoins_now_ = 0;
				
				break;
			}
		case 2:
			{
				consume_dragoncoins_before_7_ = consume_dragoncoins_before_5_;
				consume_dragoncoins_before_6_ = consume_dragoncoins_before_4_;
				consume_dragoncoins_before_5_ = consume_dragoncoins_before_3_;
				consume_dragoncoins_before_4_ = consume_dragoncoins_before_2_;
				consume_dragoncoins_before_3_ = consume_dragoncoins_before_1_;
				consume_dragoncoins_before_2_ = consume_dragoncoins_now_;
				consume_dragoncoins_before_1_ = 0;
				consume_dragoncoins_now_ = 0;

				break;
			}
		case 3:
			{
				consume_dragoncoins_before_7_ = consume_dragoncoins_before_4_;
				consume_dragoncoins_before_6_ = consume_dragoncoins_before_3_;
				consume_dragoncoins_before_5_ = consume_dragoncoins_before_2_;
				consume_dragoncoins_before_4_ = consume_dragoncoins_before_1_;
				consume_dragoncoins_before_3_ = consume_dragoncoins_now_;
				consume_dragoncoins_before_2_ = 0;
				consume_dragoncoins_before_1_ = 0;
				consume_dragoncoins_now_ = 0;

				break;
			}
		case 4:
			{
				consume_dragoncoins_before_7_ = consume_dragoncoins_before_3_;
				consume_dragoncoins_before_6_ = consume_dragoncoins_before_2_;
				consume_dragoncoins_before_5_ = consume_dragoncoins_before_1_;
				consume_dragoncoins_before_4_ = consume_dragoncoins_now_;
				consume_dragoncoins_before_3_ = 0;
				consume_dragoncoins_before_2_ = 0;
				consume_dragoncoins_before_1_ = 0;
				consume_dragoncoins_now_ = 0;

				break;
			}
		case 5:
			{
				consume_dragoncoins_before_7_ = consume_dragoncoins_before_2_;
				consume_dragoncoins_before_6_ = consume_dragoncoins_before_1_;
				consume_dragoncoins_before_5_ = consume_dragoncoins_now_;
				consume_dragoncoins_before_4_ = 0;
				consume_dragoncoins_before_3_ = 0;
				consume_dragoncoins_before_2_ = 0;
				consume_dragoncoins_before_1_ = 0;
				consume_dragoncoins_now_ = 0;

				break;
			}
		case 6:
			{
				consume_dragoncoins_before_7_ = consume_dragoncoins_before_1_;
				consume_dragoncoins_before_6_ = consume_dragoncoins_now_;
				consume_dragoncoins_before_5_ = 0;
				consume_dragoncoins_before_4_ = 0;
				consume_dragoncoins_before_3_ = 0;
				consume_dragoncoins_before_2_ = 0;
				consume_dragoncoins_before_1_ = 0;
				consume_dragoncoins_now_ = 0;

				break;
			}
		case 7:
			{
				consume_dragoncoins_before_7_ = consume_dragoncoins_now_;
				consume_dragoncoins_before_6_ = 0;
				consume_dragoncoins_before_5_ = 0;
				consume_dragoncoins_before_4_ = 0;
				consume_dragoncoins_before_3_ = 0;
				consume_dragoncoins_before_2_ = 0;
				consume_dragoncoins_before_1_ = 0;
				consume_dragoncoins_now_ = 0;

				break;
			}
		default:
			{
				consume_dragoncoins_before_7_ = 0;
				consume_dragoncoins_before_6_ = 0;
				consume_dragoncoins_before_5_ = 0;
				consume_dragoncoins_before_4_ = 0;
				consume_dragoncoins_before_3_ = 0;
				consume_dragoncoins_before_2_ = 0;
				consume_dragoncoins_before_1_ = 0;
				consume_dragoncoins_now_ = 0;
				break;
			}
		}
			
		last_update_consume_ = GameTime::GetTime();

		m_isModify.Set();
	}
}

void PlatformShareRecord::AddFirstShare(UINT32 scene_id)
{
	Update();

	auto it = frist_pass_share_list_.find(scene_id);
	if (it != frist_pass_share_list_.end())
	{
		return;
	}
	auto exist_it = GetGlobalConfig().FirstPassShareSceneList.find(scene_id);
	if (exist_it == GetGlobalConfig().FirstPassShareSceneList.end())
	{
		return;
	}
	frist_pass_share_list_.insert(std::make_pair(scene_id,false));
		
	weekly_share_number_++;
	m_isModify.Set();

	GetFirstShareReward(scene_id);

	NotifyPlatShareAward();
}

void PlatformShareRecord::AddShareNumWeekly()
{
	Update();
	weekly_share_number_++;
	if (weekly_share_number_ >=  GetGlobalConfig().WeekShareNum)
	{
		NotifyPlatShareAward();
	}
	m_isModify.Set();
}

void PlatformShareRecord::NotifyFirstShare(UINT32 scene_id)
{
	Update();
	auto it = frist_pass_share_list_.find(scene_id);
	if (it != frist_pass_share_list_.end())
	{
		return;
	}
	auto exist_it = GetGlobalConfig().FirstPassShareSceneList.find(scene_id);
	if (exist_it == GetGlobalConfig().FirstPassShareSceneList.end())
	{
		return;
	}

	auto have_notify_it = have_notify_scene_.find(scene_id);
	if (have_notify_it != have_notify_scene_.end())
	{
		return;
	}
	else
	{
		have_notify_scene_.insert(scene_id);
	}

	PtcG2C_PlatformShareAwardNtf ntf;	
	if (!weekly_award_)
	{
		if (weekly_share_number_ >= GetGlobalConfig().WeekShareNum)
		{
			ntf.m_Data.set_weekly_award(true);
		}
	}
	ntf.m_Data.set_weekly_share_number(weekly_share_number_);

	auto now_time = GameTime::GetTime();
	UINT32 num =  XCommon::GetWeekNum(now_time);
	if (num == 1)
	{
		ntf.m_Data.set_disappear_redpoint(disappear_redpoint_);
	}
	else
	{
		ntf.m_Data.set_disappear_redpoint(true);
	}
	ntf.m_Data.set_share_scene_id(scene_id);
	role_->Send(ntf);

	m_isModify.Set();
}

void PlatformShareRecord::GetFirstShareReward(const UINT32 scene_id)
{
	auto exist_it = GetGlobalConfig().FirstPassShareSceneList.find(scene_id);
	if (exist_it == GetGlobalConfig().FirstPassShareSceneList.end())
	{
		return;
	}

	auto it = frist_pass_share_list_.find(scene_id);
	if (it == frist_pass_share_list_.end())
	{
		return;
	}
	if (it->second)
	{
		return;
	}

	auto item_it = GetGlobalConfig().FirstPassShareReward.begin();
	auto item_it_end = GetGlobalConfig().FirstPassShareReward.end();

	BagGiveItemTransition transition(role_);
	transition.SetReason(ItemFlow_PlatShareResult,ItemFlow_PlatShare_Award);
	for (; item_it != item_it_end; ++item_it)
	{
		transition.GiveItem(item_it->seq[0], item_it->seq[1]);
	}
	transition.NotifyClient();	

	it->second = true;

	m_isModify.Set();
}

void PlatformShareRecord::GetWeeklyShareReward()
{
	Update();
	if (weekly_share_number_ < GetGlobalConfig().WeekShareNum)
	{
		return;
	}
	if (weekly_award_)
	{
		return;
	}
	auto item_it = GetGlobalConfig().WeekShareReward.begin();
	auto item_it_end = GetGlobalConfig().WeekShareReward.end();

	BagGiveItemTransition transition(role_);
	transition.SetReason(ItemFlow_PlatShareResult,ItemFlow_PlatShare_Award);
	for (; item_it != item_it_end; ++item_it)
	{
		transition.GiveItem(item_it->seq[0], item_it->seq[1]);
	}
	transition.NotifyClient();	

	weekly_award_ = true;
	m_isModify.Set();

	NotifyPlatShareAward();
}


void PlatformShareRecord::NotifyPlatShareAward()
{
	Update();
	PtcG2C_PlatformShareAwardNtf ntf;	
	if (!weekly_award_)
	{
		if (weekly_share_number_ >= GetGlobalConfig().WeekShareNum)
		{
			ntf.m_Data.set_weekly_award(true);
		}
	}
	ntf.m_Data.set_weekly_share_number(weekly_share_number_);

	auto now_time = GameTime::GetTime();
	UINT32 num =  XCommon::GetWeekNum(now_time);
	if (num == 1)
	{
		ntf.m_Data.set_disappear_redpoint(disappear_redpoint_);
	}
	else
	{
		ntf.m_Data.set_disappear_redpoint(true);
	}

	role_->Send(ntf);
}


void PlatformShareRecord::SetDisappearRedPoint()
{
	disappear_redpoint_ = true;
	m_isModify.Set();
}

void PlatformShareRecord::DebugTest()
{
	frist_pass_share_list_.clear();
	have_notify_scene_.clear();
	weekly_award_ = false;
	weekly_share_number_ = 0;
	m_isModify.Set();
}

bool PlatformShareRecord::ReSetWeekly()
{
	auto now_time = GameTime::GetTime();
	UINT32 num =  XCommon::GetWeekNum(now_time);
	if (num == 1)
	{
		NotifyPlatShareAward();
	}
	
	return true;
}

void PlatformShareRecord::AddDragonCoinsConsume(UINT32 dragon_coins)
{
	Update();

	consume_dragoncoins_now_ += dragon_coins;
	m_isModify.Set();
}

UINT32 PlatformShareRecord::GetAverageConsume7Day()
{
	Update();

	SSInfo << " roleid = " << role_->GetRoleID() << " consume_dragoncoins_before_1_ = " << consume_dragoncoins_before_1_
		<< " consume_dragoncoins_before_2_ = " << consume_dragoncoins_before_2_ 
		<<" consume_dragoncoins_before_3_ = " << consume_dragoncoins_before_3_
		<<" consume_dragoncoins_before_4_ = " << consume_dragoncoins_before_4_
		<<" consume_dragoncoins_before_5_ = " << consume_dragoncoins_before_5_
		<<" consume_dragoncoins_before_6_ = " << consume_dragoncoins_before_6_
		<<" consume_dragoncoins_before_7_ = " << consume_dragoncoins_before_7_
		<< END;

	UINT64 all_consume =  consume_dragoncoins_before_1_ +
	 consume_dragoncoins_before_2_ +
	 consume_dragoncoins_before_3_ +
	 consume_dragoncoins_before_4_ +
	 consume_dragoncoins_before_5_ +
	 consume_dragoncoins_before_6_ +
	 consume_dragoncoins_before_7_ ;

	return (UINT32) (all_consume/7);
}