#include "pch.h"
#include "time.h"
#include "guildcampparty.h"
#include "util/gametime.h"
#include "globalconfig.h"
#include "scene/scene.h"
#include "scene/scenemanager.h"
#include "util/XRandom.h"
#include "unit/enemymanager.h"
#include "guild/ptcg2c_guildcamppartynotify.h"
#include "guild/ptcg2c_guildcamppartytradenotify.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/bag.h"
#include "gamelogic/bagtransition.h"
#include "guild/guildrecord.h"
#include "gamelogic/MultActivityMgr.h"
#include "guild/guildmgr.h"
#include "gamelogic/noticemgr.h"
#include "define/systemiddef.h"
#include "activity/ptcg2c_halliconsntf.h"
#include "util/timespecificmgr.h"
#include "table/OpenSystemMgr.h"

INSTANCE_SINGLETON(CGuildCampPartyMgr);

///////////////////////////
CGuildCampPartyMgr::CGuildCampPartyMgr()
{
	state_ = ActiveEnd;
	time_handler_ = INVALID_HTIMER;	
}
CGuildCampPartyMgr::~CGuildCampPartyMgr()
{

}
bool CGuildCampPartyMgr::Init()
{
	if (!GSConfig::Instance()->IsHallGS())
	{
		return true; //非主城服务器不允许运行这些逻辑
	}

	if (time_handler_ == INVALID_HTIMER)
	{
		time_handler_ = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);
	}
	Reset();
	return LoadFile();
}

void CGuildCampPartyMgr::Uninit()
{
	if (INVALID_HTIMER != time_handler_ )
	{
		CTimerMgr::Instance()->KillTimer(time_handler_);
	}
	FinishActivities();
}

bool CGuildCampPartyMgr::Reload()
{
	Uninit();	
	return Init();
}
bool CGuildCampPartyMgr::CheckFile()
{
	GuildCampPartyReward tmpRewrad;
	if(!tmpRewrad.LoadFile("table/GuildCampPartyReward.txt"))
	{
		SSWarn << " load GuildCampPartyReward.txt failed " << END;
		return false;
	}

	return true;
}

bool CGuildCampPartyMgr::LoadFile()
{
	if(!CheckFile())
	{
		return false;
	}

	ClearFile();

	party_reward_.LoadFile("table/GuildCampPartyReward.txt");	

	return true;
}
void CGuildCampPartyMgr::ClearFile()
{
	party_reward_.Clear();
}
void CGuildCampPartyMgr::Reset()
{
	state_ = ActiveEnd;	
	sprite_wave_appear_.clear();
	sprite_time_ = 0;
	sprite_delete_time_ = 0;
	reward_continued_time_ = 0;
	lottery_list_.clear();
	last_time_.clear();	
	targe_roles_.clear();

	scene_sprite_list_.clear();
	transaction_roles_.clear();

	sprite_index_ = 0;
	trade_data_.clear();
	summons_spirit_status_.clear();
	scene_summonspirit_.clear();
}
void CGuildCampPartyMgr::OpenActivities()
{
	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_GUILD_CAMP_PARTY_NTF))
	{
		return;
	}
	HalliconsNotify(SYS_GUILD_CAMP_PARTY_NTF,true);
	//HalliconsNotify();
}

void CGuildCampPartyMgr::HalliconsNotify(UINT32 system_id,bool show)
{
	PtcG2C_HallIconSNtf ntf;
	ntf.m_Data.set_state(show ? KKSG::HICONS_BEGIN : KKSG::HICONS_END);
	ntf.m_Data.set_systemid(system_id);

	auto itend = RoleManager::Instance()->End();
	for(auto it = RoleManager::Instance()->Begin(); it != itend; ++it)
	{
		Role *role = *it;
		if(NULL == role)
		{
			continue;
		}
		if (!role->Get<CRoleSystem>()->IsSystemOpened(SYS_GUILD_CAMP_PARTY_NTF))
		{
			SSInfo<<"system close"<<END;
			continue;
		}
		role->Send(ntf);
	}	
}

bool  CGuildCampPartyMgr::PreNotice(UINT32 now_time)
{
	UINT32 open_time = MultActivityMgr::Instance()->GetOpenTime(MULT_ACTIVITY_GUILDCAMP_PARTY);
	if (now_time >= open_time -  GetGlobalConfig().GuildCampPartyPreNoticeTime)
	{
		return true;
	}

	return false;
}

void CGuildCampPartyMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{	
	UINT32 week_day = XCommon::GetWeekNum(GameTime::GetTime());
	if (!MultActivityMgr::Instance()->IsActivityOnDay(MULT_ACTIVITY_GUILDCAMP_PARTY, week_day))
	{
		return ;
	}

	SetTimerEvent();
	UINT32 nowtime = (UINT32)GameTime::GetTime();
	switch(state_)
	{	
	case BeginPreWaiting:
		{
			if (nowtime >=  MultActivityMgr::Instance()->GetOpenTime(MULT_ACTIVITY_GUILDCAMP_PARTY) &&
				nowtime < MultActivityMgr::Instance()->GetEndTime(MULT_ACTIVITY_GUILDCAMP_PARTY))
			{
				InitPartyConf();
				state_ = SpriteAppear;				
			}
		}
		break;
	case SpriteAppear:
		{
			if (nowtime > sprite_time_)
			{
				CreateSprite();

				auto wave_it = sprite_wave_appear_.begin();
				auto wave_it_end = sprite_wave_appear_.end();
				if (wave_it != wave_it_end)
				{
					sprite_time_ = nowtime + wave_it->second;
					sprite_delete_time_ = nowtime + GetGlobalConfig().GuildCampPartySpriteExistTime;

					LogDebug("sprite_time_ :%d  sprite_delete_time_ :%d  and wave is %d ,id %d",sprite_time_,sprite_delete_time_,sprite_wave_appear_.size(),wave_it->first);
					// 获取后就可以清理掉了
					sprite_wave_appear_.erase(wave_it->first);
				}				
				else
				{
					state_ = SpriteEnd; 
				}
			}

			if (nowtime > sprite_delete_time_)
			{
				DestroySprite();
			}			
		}
		break;
	case SpriteEnd:
		{
			if (nowtime > sprite_delete_time_)
			{
				DestroySprite();
				state_ = ActiveContinue;
			}
		}
		break;	
	default:
		break;
	}	

	ClearSummonSpirit(nowtime);
}

void CGuildCampPartyMgr::FinishActivities()
{
	// 通知 交易结束
	PtcG2C_GuildCampPartyTradeNotify ntf;
	ntf.m_Data.set_notify_type(KKSG::UPDATA_TRADE_STATUS);
	ntf.m_Data.set_open_trade(false);
	for (auto it = trade_data_.begin(); it != trade_data_.end(); ++it)
	{
		TradeData* data = *it;
		if (NULL != data)
		{
			Role* lauch_role = RoleManager::Instance()->FindByRoleID(data->lauch_role_id);
			Role* targe_role = RoleManager::Instance()->FindByRoleID(data->target_role_id);

			if (NULL != lauch_role)
			{
				lauch_role->Send(ntf);
			}
			if (NULL != targe_role)
			{
				targe_role->Send(ntf);
			}

			ClearRoleInTransactionStatus(data->lauch_role_id);
			ClearRoleInTransactionStatus(data->target_role_id);

			LogDebug("delete role id :%llu",data->lauch_role_id);
			LogDebug("delete role id :%llu",data->target_role_id);
			delete data;
		}		
	}

	//WorldSendNotice(GuildCampPartyEnd);	
	Reset();
}

void CGuildCampPartyMgr::CreateSprite()
{	
	DestroySprite();

	SceneManager::scene_iter itScene = SceneManager::Instance()->Begin();
	SceneManager::scene_iter itSceneEnd = SceneManager::Instance()->End();
	for(; itScene != itSceneEnd; ++itScene)
	{
		Scene* scene = itScene->second;

		if (scene->GetSceneType() == KKSG::SCENE_GUILD_HALL)
		{
			UINT32 enemy_count = (UINT32)(scene->GetRoleCount() * GetGlobalConfig().GuildCampRandomEnemyFrequency) +1;
			std::map<UINT32,KKSG::GuildCampSpriteInfo> sprites;
			while(sprites.size() < enemy_count)
			{
				UINT32 random_index  = XRandom::randInt(0,GetGlobalConfig().GuildCampPartyBonus.size());				
				auto it = GetGlobalConfig().GuildCampPartyBonus.find(random_index);
				if (it != GetGlobalConfig().GuildCampPartyBonus.end())
				{
					LogDebug("rand pos source x  %f ",it->second.seq[0]);
					KKSG::GuildCampSpriteInfo sprite_info;										
					sprite_info.set_sprite_id(++sprite_index_);

					float set_x = (float)XRandom::randDouble(it->second.seq[0]-it->second.seq[3],it->second.seq[0]+it->second.seq[3]);
					float set_z = (float)XRandom::randDouble(it->second.seq[2]-it->second.seq[3],it->second.seq[2]+it->second.seq[3]);

					int posxz = (int)(floor(set_z*100+0.5f));
					posxz |= ((int)(floor(set_x*100+0.5f)) << 16);

					sprite_info.set_position(posxz);
					sprites.insert(std::make_pair(sprite_index_,sprite_info));
					//LogDebug("create sprite id= %d vec  x:%f , y:%f,z:%f",sprite_index_,vec->x(),vec->y(),vec->z());
				}				
			}

			auto exist_it = scene_sprite_list_.find(scene->GetSceneID());
			if (exist_it != scene_sprite_list_.end())
			{
				auto new_it = sprites.begin();
				auto new_it_end = sprites.end();
				for (; new_it != new_it_end; ++new_it)
				{
					exist_it->second.insert(std::make_pair(new_it->first,new_it->second));
				}
			}
			else
			{
				scene_sprite_list_.insert(std::make_pair(scene->GetSceneID(),sprites));	
			}
		}
	}

	UpdateNpcSprite();
	GuildSendNotice(GuildCampPartySpriteAppear);
}

void CGuildCampPartyMgr::DestroySprite()
{
	UINT32 serverID = GSConfig::Instance()->GetServerID();	
	UINT32 new_npc_id =  serverID  << 16;

	auto it = scene_sprite_list_.begin();
	auto it_end = scene_sprite_list_.end();
	for (; it != it_end; ++it)
	{
		std::vector<UINT32> sprite_list;
		for (auto exist_it = it->second.begin(); exist_it != it->second.end(); ++exist_it)
		{
			if (exist_it->first < new_npc_id )
			{
				sprite_list.push_back(exist_it->first);
			}
		}

		for ( auto del_it = sprite_list.begin(); del_it != sprite_list.end(); ++del_it)
		{
			it->second.erase(*del_it);
		}
	}

	UpdateNpcSprite();
}

bool CGuildCampPartyMgr::CheckSpriteExist(UINT32 scene_id,UINT32 sprite_id,Sequence<float,3>& sequence)
{
	auto it = scene_sprite_list_.find(scene_id);
	if (it == scene_sprite_list_.end())
	{
		return false;
	}
	auto exist_it = it->second.find(sprite_id);
	if (exist_it != it->second.end())
	{
		/*sequence.seq[0] = (0xFFFF0000 & exist_it->second) >> 16 ;
		sequence.seq[1] = 0;
		sequence.seq[2] = (0x0000FFFF &exist_it->second);*/
		int hight_x = (0xFFFF0000 & exist_it->second.position()) >> 16;
		sequence.seq[0] = (float)hight_x  / 100 +  hight_x % 100 * 0.01;

		int low_z = 0x0000FFFF & exist_it->second.position();
		sequence.seq[2] = low_z /100 + low_z %100 * 0.01;
		return true;
	}	

	return false;		
}

KKSG::ErrorCode CGuildCampPartyMgr::DeleteSprite(Scene *scene,UINT32 sprite_id,UINT64 guild_id)
{
	if (NULL == scene)
	{
		return KKSG::ERR_INVFIGHT_ME_SCENE; 
	}

	auto it = scene_sprite_list_.find(scene->GetSceneID());
	if (it == scene_sprite_list_.end())
	{
		return KKSG::ERR_INVFIGHT_ME_SCENE;
	}
	auto& sprite_list = it->second;
	auto exist_it = sprite_list.find(sprite_id);
	if (exist_it == sprite_list.end())
	{
		return KKSG::ERR_GUILDCAMP_SPRITE_NOTEXIST;
	}
	sprite_list.erase(sprite_id);
	LogDebug("DeleteSprite  sprite id %d ",sprite_id);

	// 更新小妖信息
	UpdateSprite(scene);

	// 小妖被抓完的公告
	if (sprite_list.size () < 1 )
	{		
		Notice notice(GuildCampPartySpriteDisAppear);
		notice.SendGuild(guild_id);
	}
	return KKSG::ERR_SUCCESS;
}

void CGuildCampPartyMgr::UpdateSprite(Scene *scene)
{
	if (NULL == scene)
	{
		return;
	}

	auto it = scene_sprite_list_.find(scene->GetSceneID());
	if (it == scene_sprite_list_.end())
	{
		return ;
	}

	// 通知场景内
	PtcG2C_guildcamppartyNotify ntf;
	ntf.m_Data.set_notify_type(1);
	if (reward_continued_time_ > (UINT32)GameTime::GetTime())
	{
		ntf.m_Data.set_left_time(reward_continued_time_ - (UINT32)GameTime::GetTime());
	}
	auto lottery_it = lottery_list_.begin();
	auto lottery_it_end = lottery_list_.end();
	for (; lottery_it != lottery_it_end; ++lottery_it)
	{
		ntf.m_Data.add_lottery_list(*lottery_it);
	}

	for (auto sprite_it = it->second.begin(); sprite_it !=  it->second.end(); ++sprite_it)
	{
		KKSG::GuildCampSpriteInfo*  sprite_info = ntf.m_Data.add_sprite_list();	
		sprite_info->CopyFrom(sprite_it->second);
	}
	scene->Broadcast(ntf);	
}

void CGuildCampPartyMgr::ExchangeItemReq(Role *launch_role,Role * target_role)
{
	if (NULL == launch_role || NULL == target_role)
	{
		return;
	}

	UINT64 target_role_id  = target_role->GetRoleID();
	UINT64 launch_role_id = launch_role->GetRoleID();

	UINT32 lauch_count = 1;
	auto exist_it = targe_roles_.find(target_role_id);
	if (exist_it != targe_roles_.end())
	{
		exist_it->second.insert(std::make_pair(launch_role_id,(UINT32)GameTime::GetTime()));
		lauch_count = exist_it->second.size();
	}
	else
	{
		std::unordered_map<UINT64,UINT32> launch_list;
		launch_list.insert(std::make_pair(launch_role_id,(UINT32)GameTime::GetTime()));
		targe_roles_.insert(std::make_pair(target_role_id,launch_list));
	}
	// 通知target
	PtcG2C_GuildCampPartyTradeNotify ntf;	
	ntf.m_Data.set_notify_type(KKSG::TRADE_INVITATION);
	ntf.m_Data.set_lauch_count(lauch_count);
	target_role->Send(ntf);
}

void CGuildCampPartyMgr::GetExchangeInfoByTargeId(UINT64 role_id,KKSG::GetGuildCampPartyExchangeInfoRes& res)
{
	auto it = targe_roles_.find(role_id);
	if (it == targe_roles_.end())
	{
		return;
	}
	auto exist_it = it->second.begin();
	auto exist_end = it->second.end();
	for (; exist_it != exist_end; ++exist_it)
	{
		Role* role = RoleManager::Instance()->FindByRoleID(exist_it->first);
		if (NULL == role)
		{				
			SSWarn<<"pRole is NULL, roleid id:"<< exist_it->first<<END;				
			continue;
		}

		KKSG::PartyExchangeItemInfo *info = res.add_infos();
		info->set_role_id(exist_it->first);
		info->set_time((UINT32)GameTime::GetTime()-exist_it->second);
		info->set_level(role->GetLevel());
		info->set_name(role->GetName());
		info->set_profession_id(role->GetProfession());
	}
}

bool CGuildCampPartyMgr::DefendPlug(UINT64 role_id)
{
	if (lottery_list_.size() < 1)
	{
		return false;
	}

	auto it = last_time_.find(role_id);
	if (it != last_time_.end())
	{
		UINT32 now_time = (UINT32)GameTime::GetTime();
		if (it->second + GetGlobalConfig().GuildCollecLotteryWaitTime - 2  < now_time)
		{
			it->second = now_time;
			return true;
		}
		return false;
	}

	last_time_.insert(std::make_pair(role_id,(UINT32)GameTime::GetTime()));

	return true;
}
bool CGuildCampPartyMgr::CheckRoleTransactionStatus(UINT64 role_id)
{
	auto it = transaction_roles_.find(role_id);
	if (it != transaction_roles_.end())
	{
		return true;
	}
	return false;
}

void  CGuildCampPartyMgr::SetRoleInTransactionStatus(UINT64 role_id)
{
	transaction_roles_.insert(role_id);	
	LogDebug("insert role id :%llu",role_id);
}

void  CGuildCampPartyMgr::ClearRoleInTransactionStatus(UINT64 role_id)
{
	transaction_roles_.erase(role_id);	
	LogDebug("delete role id :%llu",role_id);
}

void CGuildCampPartyMgr::ClearTradeInvitation(UINT64 target_role_id ,UINT64 lauch_role_id,bool clear_all)
{	
	UINT32 count = 0;
	if (clear_all)
	{
		targe_roles_.erase(target_role_id);
	}
	else
	{
		auto it = targe_roles_.find(target_role_id);
		if (it != targe_roles_.end())
		{		
			auto exist_it = it->second.begin();
			auto exist_it_end = it->second.end();
			for (;exist_it != exist_it_end; ++exist_it)
			{	
				if (exist_it->first == lauch_role_id)
				{		
					it->second.erase(exist_it->first);
					count = it->second.size();					
					break ;
				}
			}
		}
	}

	if (0 == count)
	{
		targe_roles_.erase(target_role_id);
	}

	Role* role = RoleManager::Instance()->FindByRoleID(target_role_id);
	if (NULL != role)
	{
		// 通知target
		PtcG2C_GuildCampPartyTradeNotify ntf;	
		ntf.m_Data.set_notify_type(KKSG::TRADE_INVITATION);
		ntf.m_Data.set_lauch_count(count);
		role->Send(ntf);
	}
}

UINT32 CGuildCampPartyMgr::GetTardeInvitationCount(UINT64 role_id)
{
	UINT32 count = 0;
	auto it = targe_roles_.find(role_id);
	if (it != targe_roles_.end())
	{
		count = it->second.size();
	}
	return count;
}

void CGuildCampPartyMgr::NoticeTradeInvitationDisappear(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	// 如果没有邀请记录了，就通知icon消失
	UINT32 role_invation =   CGuildCampPartyMgr::Instance()->GetTardeInvitationCount(role->GetRoleID());
	if (role_invation < 1)
	{
		// 通知target
		PtcG2C_GuildCampPartyTradeNotify ntf;	
		ntf.m_Data.set_notify_type(KKSG::TRADE_INVITATION);
		ntf.m_Data.set_lauch_count(0);
		role->Send(ntf);
	}
}
KKSG::ErrorCode CGuildCampPartyMgr::CheckPartyConditon(Role *role)
{
	// 对方role为空才会有这个处理，自身在第一步就已经判断了。
	if (NULL == role)
	{
		return KKSG::ERR_ROLE_NOT_ONLINE;
	}

	if(role->CheckIdipAndNotify(KKSG::PUNISH_USER_MULTI_ACTIVITY))
	{
		SSWarn<<"activeity  PUNISH_USER_MULTI_ACTIVITY :"<<role->GetID()<<END;
		return KKSG::ERR_FAILED;		
	}

	// 系统开放
	if (!role->Get<CRoleSystem>()->IsSystemOpened(SYS_GUILD_CAMP_PARTY_NTF) || OpenSystemMgr::Instance()->IsClosedSystem(SYS_GUILD_CAMP_PARTY_NTF))
	{
		SSInfo<<"system close"<<END;
		return KKSG::ERR_SYS_NOTOPEN;
	}

	if ((UINT32)GameTime::GetTime() > reward_continued_time_)
	{
		SSWarn<<"activeity is not open :"<<role->GetID()<<END;
		return KKSG::ERR_GUILDACMPPATY_NOTOPEN;		
	}	

	// 当前应该在公会场景
	if (NULL == role->GetCurrScene())
	{
		SSWarn<<" currscene is null ,role id :"<< role->GetID()<<END;
		return KKSG::ERR_INVALID_REQUEST;
	}
	if(role->GetCurrScene()->GetSceneTemplateID() != GUILD_MAP_ID)
	{
		SSWarn<<" currscene is not in GUILD_MAP_ID ,role id :"<< role->GetID()<<END; 
		return KKSG::ERR_INVFIGHT_OTHER_SCENE;	
	}

	//　检查还是不是工会成员
	if (role->getGuildId() == 0)
	{
		return KKSG::ERR_QA_NO_GUILD;		
	}

	return KKSG::ERR_SUCCESS;
}

// 清理掉目标玩家交易请求,生成正式的交易记录
void  CGuildCampPartyMgr::MakeTradeRecord(Role *launch_role,Role * target_role)
{	
	if (NULL == launch_role || NULL == target_role)
	{
		return;
	}

	ClearTradeInvitation(target_role->GetRoleID(),launch_role->GetRoleID(),false);

	TradeData* data = new TradeData();
	data->lauch_role_id = launch_role->GetRoleID();
	data->target_role_id = target_role->GetRoleID();
	trade_data_.insert(data);
}

UINT32 CGuildCampPartyMgr::CheckSwingUpItem(Role *role,UINT64 uid)
{
	if (NULL == role)
	{
		return 0;
	}

	Bag* bag = role->Get<Bag>();
	if(NULL == bag)
	{
		return 0;
	}
	XItem* item = bag->FindUniqueItemInBag(uid);
	if(NULL == item)
	{
		return 0;
	}
	// 能否交易 目前定的是33 卡牌类型
	if (item->type != 33 )
	{
		return 0;
	}
	return item->itemID;
}

void CGuildCampPartyMgr::SwingUpItem(Role *role,UINT64 item_uid,UINT32 item_id,Role *launch_role,Role * target_role)
{	
	if (NULL == role || NULL == launch_role || NULL == target_role)
	{
		return;
	}

	TradeData* data = trade_data_.find<LaunchIndex>(role->GetRoleID());
	if (data != NULL )
	{
		data->lauch_item_uid = item_uid;
		data->lauch_item_id = item_id;
	}
	else
	{
		data = trade_data_.find<TargetIndex>(role->GetRoleID());
		if (data != NULL )
		{
			data->target_item_uid = item_uid;
			data->target_item_id = item_id;
		}
	}

	NoticeTradeStatus(launch_role,target_role);
}


void CGuildCampPartyMgr::MakeSureTrade(Role *role,bool status,Role *launch_role,Role * target_role)
{
	if (NULL == role || NULL == launch_role || NULL == target_role)
	{
		return;
	}

	TradeData* data = trade_data_.find<LaunchIndex>(role->GetRoleID());
	if (data != NULL )
	{
		data->lauch_status = status;
	}
	else
	{
		data = trade_data_.find<TargetIndex>(role->GetRoleID());
		if (data != NULL )
		{
			data->target_status = status;
		}
	}

	NoticeTradeStatus(launch_role,target_role);
	UpdateTransaction(launch_role,target_role);
}


void CGuildCampPartyMgr::UpdateTransaction(Role *launch_role,Role * target_role)
{
	if (NULL == launch_role || NULL == target_role)
	{
		return;
	}

	TradeData* lauch_data = trade_data_.find<LaunchIndex>(launch_role->GetRoleID());
	TradeData* targe_data = trade_data_.find<TargetIndex>(target_role->GetRoleID());
	if (lauch_data != NULL && targe_data != NULL)
	{
		if (lauch_data->lauch_status && targe_data->target_status )
		{
			// 交易达成
			Sequence<uint, 2> lauch_item,targe_item;
			lauch_item.seq[0]= lauch_data->lauch_item_id;
			lauch_item.seq[1]= 1;
			targe_item.seq[0]= lauch_data->target_item_id;
			targe_item.seq[1]= 1;

			std::vector<Sequence<uint, 2>> items;
			items.push_back(lauch_item);
			ConsumeItems(launch_role,items);
			items.clear();

			items.push_back(targe_item);
			ConsumeItems(target_role,items);
			items.clear();

			items.push_back(targe_item);
			GiveItems(launch_role,items);
			items.clear();

			items.push_back(lauch_item);
			GiveItems(target_role,items);
			items.clear();

			ClearRoleInTransactionStatus(launch_role->GetRoleID());
			ClearRoleInTransactionStatus(target_role->GetRoleID());

			// 通知客户端关闭界面
			PtcG2C_GuildCampPartyTradeNotify trade_ntf;
			trade_ntf.m_Data.set_notify_type(KKSG::UPDATA_TRADE_STATUS);
			trade_ntf.m_Data.set_open_trade(false);
			launch_role->Send(trade_ntf);
			target_role->Send(trade_ntf);

			trade_data_.erase<LaunchIndex>(launch_role->GetRoleID());
			delete lauch_data;	

			LogDebug("delete role id :%llu",launch_role->GetRoleID());
			LogDebug("delete role id :%llu",target_role->GetRoleID());
		}
	}
}


void CGuildCampPartyMgr::CancleTrade(Role *role,Role *launch_role,Role * target_role)
{
	if (NULL == role || NULL == launch_role || NULL == target_role)
	{
		return;
	}

	TradeData* data = trade_data_.find<LaunchIndex>(launch_role->GetRoleID());	
	if (NULL != data)
	{			
		trade_data_.erase<LaunchIndex>(launch_role->GetRoleID());	
		delete data;

		// 通知 交易结束
		PtcG2C_GuildCampPartyTradeNotify ntf;
		ntf.m_Data.set_notify_type(KKSG::UPDATA_TRADE_STATUS);
		ntf.m_Data.set_open_trade(false);
		launch_role->Send(ntf);
		target_role->Send(ntf);

		ClearRoleInTransactionStatus(launch_role->GetRoleID());
		ClearRoleInTransactionStatus(target_role->GetRoleID());				

		LogDebug("delete role id :%llu",launch_role->GetRoleID());
		LogDebug("delete role id :%llu",target_role->GetRoleID());
	}
	else
	{
		PtcG2C_GuildCampPartyTradeNotify ntf;
		ntf.m_Data.set_notify_type(KKSG::UPDATA_TRADE_STATUS);
		ntf.m_Data.set_open_trade(false);
		launch_role->Send(ntf);
		target_role->Send(ntf);
	}
}

void  CGuildCampPartyMgr::DeleteTrade(UINT64 role_id)
{
	TradeData* data = trade_data_.find<LaunchIndex>(role_id);	
	if (NULL != data)
	{			
		ClearRoleInTransactionStatus(data->lauch_role_id);
		ClearRoleInTransactionStatus(data->target_role_id);

		trade_data_.erase<LaunchIndex>(role_id);	
		delete data;
	}
	else
	{
		data = trade_data_.find<TargetIndex>(role_id);	
		if (NULL != data)
		{
			ClearRoleInTransactionStatus(data->lauch_role_id);
			ClearRoleInTransactionStatus(data->target_role_id);

			trade_data_.erase<TargetIndex>(role_id);	
			delete data;
		}
	}
}

TradeData *CGuildCampPartyMgr::GetTradeDataByRole(UINT64 role_id)
{
	TradeData* data = trade_data_.find<LaunchIndex>(role_id);
	if (data != NULL )
	{
		return data;
	}
	else
	{
		data = trade_data_.find<TargetIndex>(role_id);
		if (data != NULL )
		{
			return data;
		}
	}

	return NULL;
}
void CGuildCampPartyMgr::NoticeTradeStatus(Role *launch_role,Role *target_role)
{
	if (NULL == launch_role || NULL == target_role)
	{
		return;
	}

	TradeData* data = trade_data_.find<LaunchIndex>(launch_role->GetRoleID());
	if (NULL == data )
	{
		return;
	}

	// 通知target
	PtcG2C_GuildCampPartyTradeNotify ntf;
	ntf.m_Data.set_notify_type(KKSG::UPDATA_TRADE_STATUS);
	ntf.m_Data.set_open_trade(true);

	ntf.m_Data.set_lauch_role_id(launch_role->GetRoleID());
	ntf.m_Data.set_lauch_item_id(data->lauch_item_id);
	ntf.m_Data.set_lauch_item_uid(data->lauch_item_uid);
	ntf.m_Data.set_lauch_confirm(data->lauch_status);
	ntf.m_Data.set_lauch_name(launch_role->GetName());
	ntf.m_Data.set_lauch_profession(launch_role->GetProfession());
	// lauch chat
	ntf.m_Data.mutable_lauch_chat_info()->set_audio_id(data->lauch_chat.audio_id);
	ntf.m_Data.mutable_lauch_chat_info()->set_audio_time(data->lauch_chat.audio_time);
	ntf.m_Data.mutable_lauch_chat_info()->set_chat_text(data->lauch_chat.chat_text);

	ntf.m_Data.set_target_role_id(target_role->GetRoleID());
	ntf.m_Data.set_target_item_id(data->target_item_id);
	ntf.m_Data.set_target_item_uid(data->target_item_uid);
	ntf.m_Data.set_target_confirm(data->target_status);
	ntf.m_Data.set_target_name(target_role->GetName());
	ntf.m_Data.set_target_profession(target_role->GetProfession());
	// target chat
	ntf.m_Data.mutable_target_chat_info()->set_audio_id(data->target_chat.audio_id);
	ntf.m_Data.mutable_target_chat_info()->set_audio_time(data->target_chat.audio_time);
	ntf.m_Data.mutable_target_chat_info()->set_chat_text(data->target_chat.chat_text);

	launch_role->Send(ntf);
	target_role->Send(ntf);
}

UINT32 CGuildCampPartyMgr::ConsumeItems(Role *role,std::vector<Sequence<uint, 2>>& items)
{
	if (NULL == role)
	{
		return KKSG::ERR_FAILED;;
	}

	BagTakeItemTransition transition(role);
	transition.SetReason(ItemFlow_GuildCampParty, ItemFlow_GuildCampParty_ExchangeItem);
	for (auto it = items.begin(); it != items.end(); ++it)
	{
		if (it->seq[0] > 0 )
		{		
			if (!transition.TakeItem(it->seq[0],it->seq[1]))
			{
				transition.RollBack();				
				return KKSG::ERR_ITEM_NOT_ENOUGH;
			}

			SSInfo<<"itemid:"<<it->seq[0]<<" count:"<<it->seq[1]<<END;
		}
	}

	transition.NotifyClient();
	return KKSG::ERR_SUCCESS;
}

void  CGuildCampPartyMgr::GiveItems(Role *role,std::vector<Sequence<uint, 2>>& items)
{
	if (NULL == role)
	{
		return;
	}

	BagGiveItemTransition trans(role);
	trans.SetReason(ItemFlow_GuildCampParty,ItemFlow_GuildCampParty_ExchangeItem);
	auto it = items.begin();
	auto it_end = items.end();
	for (; it != it_end; ++it)
	{
		if (it->seq[0] > 0)
		{
			trans.GiveItem(it->seq[0], it->seq[1]);
			SSInfo<<"itemid:"<<it->seq[0] <<" count:"<< it->seq[1] <<END;
		}
	}
	trans.NotifyClient();
}

KKSG::ErrorCode CGuildCampPartyMgr::CheckRoleRewardItems(Role *role,UINT32 reward_id,vector<Sequence<uint, 2>>& items,vector<Sequence<uint, 2>>& reward)
{
	if (NULL == role)
	{
		return KKSG::ERR_INVALID_REQUEST;
	}

	items.clear();
	reward.clear();

	bool exist = false;
	UINT32 can_reward_count = 0;
	auto it = party_reward_.Table.begin();
	auto it_end = party_reward_.Table.end();
	for (; it != it_end; ++it)
	{
		if ((*it)->ID == reward_id)
		{
			exist = true;
			can_reward_count = (*it)->Count;
			auto item_it = (*it)->Items.begin();
			auto item_it_end = (*it)->Items.end();
			for (; item_it != item_it_end; ++item_it)
			{
				Sequence<uint, 2> temp_item;
				temp_item.seq[0] = item_it->seq[0];
				temp_item.seq[1] = item_it->seq[1];
				items.push_back(temp_item);
			}

			for (auto reward_it = (*it)->Reward.begin() ; reward_it != (*it)->Reward.end(); ++reward_it)
			{
				Sequence<uint, 2> reward_item;
				reward_item.seq[0] = reward_it->seq[0];
				reward_item.seq[1] = reward_it->seq[1];
				reward.push_back(reward_item);
			}
		}
	}
	// 本次奖励ID是否存在
	if (!exist)
	{
		return KKSG::ERR_REWARD_NOTEXIST;
	}

	UINT32 have_count =   role->Get<CGuildRecord>()->GetPartyRewardCount(reward_id);
	if (have_count >= can_reward_count)
	{
		return KKSG::ERR_UNKNOWN;
	}

	//检查物品是否足够
	for (auto exist_it = items.begin(); exist_it != items.end(); ++exist_it)
	{
		UINT32 count = role->Get<Bag>()->CountItemInBag(exist_it->seq[0]);
		if (count < exist_it->seq[1])
		{
			return KKSG::ERR_ITEM_NOT_ENOUGH;
		}
	}	

	return KKSG::ERR_SUCCESS;
}
KKSG::ErrorCode CGuildCampPartyMgr::GetReward(Role *role,UINT32 reward_id)
{
	if (NULL == role)
	{
		return KKSG::ERR_INVALID_REQUEST;
	}

	// 1.检查兑奖次数是否满了
	// 2.检查物品是否够
	// 3.扣除物品，发放奖励
	// 4.保存兑奖奖励
	vector<Sequence<uint, 2>>	items;
	vector<Sequence<uint, 2>>	reward;
	KKSG::ErrorCode result = CheckRoleRewardItems(role,reward_id,items,reward);
	if (result != KKSG::ERR_SUCCESS)
	{
		return result;
	}

	// 扣物品，给奖励
	ConsumeItems(role,items);
	GiveItems(role,reward);

	role->Get<CGuildRecord>()->AddPartyRewardCount(reward_id);
	return  KKSG::ERR_SUCCESS;
}

void CGuildCampPartyMgr::WorldSendNotice(CNoticeType notice_type)
{
	Notice notice(notice_type);
	notice.Send();
}

void CGuildCampPartyMgr::GuildSendNotice(CNoticeType notice_type)
{
	std::unordered_map<UINT64, CGuildSimple>& guildSimpleMap = GuildSimpleMgr::Instance()->GetAllGuild();
	for (auto iter = guildSimpleMap.begin(); iter != guildSimpleMap.end(); ++iter)
	{					
		Notice notice(notice_type);
		notice.SendGuild(iter->second.GetGuildId());
	}
}

void CGuildCampPartyMgr::UpdateNpcSprite()
{
	PtcG2C_guildcamppartyNotify ntf;
	ntf.m_Data.set_notify_type(1);	
	if (reward_continued_time_ > GameTime::GetTime())
	{
		ntf.m_Data.set_left_time(reward_continued_time_ - (UINT32)GameTime::GetTime());
	}
	auto lottery_it = lottery_list_.begin();
	auto lottery_it_end = lottery_list_.end();
	for (; lottery_it != lottery_it_end; ++lottery_it)
	{
		ntf.m_Data.add_lottery_list(*lottery_it);
	}

	SceneManager::scene_iter itScene = SceneManager::Instance()->Begin();
	SceneManager::scene_iter itSceneEnd = SceneManager::Instance()->End();
	for(; itScene != itSceneEnd; ++itScene)
	{
		Scene* scene = itScene->second;
		if (scene->GetSceneType() == KKSG::SCENE_GUILD_HALL)
		{
			// 先清理上次的数据
			ntf.m_Data.clear_sprite_list();

			Scene* scene = itScene->second;
			auto exist_scene_id = scene_sprite_list_.find(scene->GetSceneID());
			if (exist_scene_id != scene_sprite_list_.end())
			{				
				for (auto sprite_it = exist_scene_id->second.begin(); sprite_it != exist_scene_id->second.end(); ++sprite_it )
				{					
					KKSG::GuildCampSpriteInfo* sprite_info = ntf.m_Data.add_sprite_list();
					sprite_info->CopyFrom(sprite_it->second);
				}
			}			
			scene->Broadcast(ntf);			
		}
	}
}

void CGuildCampPartyMgr::SystemOpenNotify(Role *role)
{
	if (NULL == role)
	{
		return;
	}
	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_GUILD_CAMP_PARTY_NTF))
	{
		return;
	}
	PtcG2C_HallIconSNtf ntf;	
	ntf.m_Data.set_systemid(SYS_GUILD_CAMP_PARTY_NTF);

	UINT32 now_time = (UINT32)GameTime::GetTime();
	if (now_time >=   MultActivityMgr::Instance()->GetOpenTime(MULT_ACTIVITY_GUILDCAMP_PARTY) &&
		now_time < MultActivityMgr::Instance()->GetEndTime(MULT_ACTIVITY_GUILDCAMP_PARTY) )
	{
		ntf.m_Data.set_state(KKSG::HICONS_BEGIN);
		role->Send(ntf);
	}
}

void CGuildCampPartyMgr::UpdatePartyStauts(Role *role,UINT32 scene_id)
{
	if (NULL == role)
	{
		return ;
	}

	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_GUILD_CAMP_PARTY_NTF))
	{
		return;
	}
	if (NULL != role && role->Get<CRoleSystem>()->IsSystemOpened(SYS_GUILD_CAMP_PARTY_NTF))
	{
		UINT32 now_time = (UINT32)GameTime::GetTime();
		if (now_time >=   MultActivityMgr::Instance()->GetOpenTime(MULT_ACTIVITY_GUILDCAMP_PARTY) &&
			now_time < reward_continued_time_ )
		{
			PtcG2C_guildcamppartyNotify ntf;
			ntf.m_Data.set_notify_type(1);	
			if (reward_continued_time_ > now_time)
			{
				ntf.m_Data.set_left_time(reward_continued_time_ - now_time);
			}
			auto lottery_it = lottery_list_.begin();
			auto lottery_it_end = lottery_list_.end();
			for (; lottery_it != lottery_it_end; ++lottery_it)
			{
				ntf.m_Data.add_lottery_list(*lottery_it);
			}

			auto exist_scene_id = scene_sprite_list_.find(scene_id);
			if (exist_scene_id != scene_sprite_list_.end())
			{
				for (auto sprite_it = exist_scene_id->second.begin(); sprite_it != exist_scene_id->second.end(); ++sprite_it )
				{					
					KKSG::GuildCampSpriteInfo* sprite_info = ntf.m_Data.add_sprite_list();
					sprite_info->CopyFrom(sprite_it->second);
				}
			}
			role->Send(ntf);	

			bool open_tarde = false;
			Role *lauch_role = NULL;
			Role *target_role = NULL;

			TradeData* data = trade_data_.find<LaunchIndex>(role->GetRoleID());
			if (NULL != data )
			{
				target_role = RoleManager::Instance()->FindByRoleID(data->target_role_id);
				if (NULL == target_role)
				{
					open_tarde = false;
				}
				else
				{
					open_tarde = true;
					lauch_role = role;
				}
			}
			else
			{
				data = trade_data_.find<TargetIndex>(role->GetRoleID());
				if (NULL != data)
				{
					target_role = role;
					lauch_role = RoleManager::Instance()->FindByRoleID(data->lauch_role_id);
					if (NULL == lauch_role)
					{
						open_tarde = false;
					}
					else
					{
						open_tarde = true;
					}
				}
			}

			if (open_tarde)
			{
				NoticeTradeStatus(lauch_role,target_role);
			}
			else
			{
				PtcG2C_GuildCampPartyTradeNotify trade_ntf;
				trade_ntf.m_Data.set_notify_type(KKSG::UPDATA_TRADE_STATUS);
				trade_ntf.m_Data.set_open_trade(false);
				role->Send(trade_ntf);
			}
		}
	}
}

void CGuildCampPartyMgr::ClearTradeInfo(Role *role)
{
	if (NULL != role && role->Get<CRoleSystem>()->IsSystemOpened(SYS_GUILD_CAMP_PARTY_NTF))
	{
		TradeData * data = CGuildCampPartyMgr::Instance()->GetTradeDataByRole(role->GetRoleID());
		if (data == NULL)
		{
			return;
		}

		// 通知在线的对方，关闭交易界面
		PtcG2C_GuildCampPartyTradeNotify ntf;
		ntf.m_Data.set_notify_type(KKSG::UPDATA_TRADE_STATUS);
		ntf.m_Data.set_open_trade(false);
		if (data->target_role_id == role->GetRoleID())
		{
			Role *launch_role =  RoleManager::Instance()->FindByRoleID(data->lauch_role_id);
			if (NULL != launch_role)
			{
				launch_role->Send(ntf);
			}
		}
		else
		{
			Role *target_role =  RoleManager::Instance()->FindByRoleID(data->target_role_id);
			if (NULL != target_role)
			{
				target_role->Send(ntf);
			}
		}

		// 清理这笔交易
		CGuildCampPartyMgr::Instance()->DeleteTrade(role->GetRoleID());

	}
}
void CGuildCampPartyMgr::SetTimerEvent()
{
	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_GUILD_CAMP_PARTY_NTF))
	{
		return;
	}

	time_t now_time =  GameTime::GetTime();
	if (now_time >= MultActivityMgr::Instance()->GetEndTime(MULT_ACTIVITY_GUILDCAMP_PARTY))
	{
		return ;
	}
	//LogDebug("now time : %d",now_time);
	// 活动状态不是开启前，无需处理
	KKSG::MulActivityTimeState activity_state = MultActivityMgr::Instance()->GetOpenTimeState(MULT_ACTIVITY_GUILDCAMP_PARTY);
	if (KKSG::MULACTIVITY_BEfOREOPEN != activity_state)
	{
		return;
	}

	if (state_ == ActiveEnd  && 0 == timers_.size())
	{
		LogDebug("now time : %d",now_time);
		for (auto it = timers_.begin(); it != timers_.end(); ++it)
		{
			CTimeSpecificMgr::Instance()->UnRegisterEvent(*it);
		}
		timers_.clear();

		// 注册预告通知
		UINT32 open_time = MultActivityMgr::Instance()->GetOpenTime(MULT_ACTIVITY_GUILDCAMP_PARTY);	
		time_t pre5minit = open_time -  GetGlobalConfig().GuildCampPartyPreNoticeTime;
		if (now_time < pre5minit)
		{
			tm* pre5time = localtime(&pre5minit);	
			TimeSpecific start_pre5(pre5time->tm_hour, pre5time->tm_min, 0);
			UINT32 pre5time_timerId = CTimeSpecificMgr::Instance()->RegisterEvent(start_pre5, std::bind(&CGuildCampPartyMgr::Pre5minitNotice, this));
			timers_.push_back(pre5time_timerId);
		}

		// 通知物品交换
		time_t down_noticetime = open_time +  GetGlobalConfig().GuildCampPartyCountDownNoticeTime;
		if (now_time < down_noticetime)
		{
			tm* down_notice = localtime(&down_noticetime);	
			TimeSpecific start_down_notice(down_notice->tm_hour, down_notice->tm_min, 0);
			UINT32 down_time_id = CTimeSpecificMgr::Instance()->RegisterEvent(start_down_notice, std::bind(&CGuildCampPartyMgr::DownNotice, this));
			timers_.push_back(down_time_id);
		}

		// 清理抽奖机
		time_t clear_lottery_time  = open_time +  GetGlobalConfig().GuildCampPartyContinuedTime2;
		if (now_time < clear_lottery_time)
		{
			tm*  clear_notice = localtime(&clear_lottery_time);	
			TimeSpecific clear_npc(clear_notice->tm_hour, clear_notice->tm_min, 0);
			UINT32 clear_npc_time_id = CTimeSpecificMgr::Instance()->RegisterEvent(clear_npc, std::bind(&CGuildCampPartyMgr::ClearLottery, this));
			timers_.push_back(clear_npc_time_id);
		}

		// 活动配置时间结束奖机
		time_t active_end_time  = MultActivityMgr::Instance()->GetEndTime(MULT_ACTIVITY_GUILDCAMP_PARTY);
		if (now_time < active_end_time)
		{
			tm*  avtive_end_notice = localtime(&active_end_time);	
			TimeSpecific avtive_end_notice_time(avtive_end_notice->tm_hour, avtive_end_notice->tm_min, 0);
			UINT32 active_end_time_id = CTimeSpecificMgr::Instance()->RegisterEvent(avtive_end_notice_time, std::bind(&CGuildCampPartyMgr::ActiveIocnNotice, this));
			timers_.push_back(active_end_time_id);
		}

		//活动结束通知
		time_t finish_noticetime = open_time +  GetGlobalConfig().GuildCampPartyContinuedTime;
		tm* finish_notice = localtime(&finish_noticetime);	
		TimeSpecific finish_down(finish_notice->tm_hour, finish_notice->tm_min, 0);
		UINT32 finish_down_time_id = CTimeSpecificMgr::Instance()->RegisterEvent(finish_down, std::bind(&CGuildCampPartyMgr::FinishNotice, this));
		timers_.push_back(finish_down_time_id);

		state_ = BeginPreWaiting ;
	}
}

void CGuildCampPartyMgr::InitPartyConf()
{
	UINT32 open_time  = MultActivityMgr::Instance()->GetOpenTime(MULT_ACTIVITY_GUILDCAMP_PARTY);
	// 兑奖持续时间
	reward_continued_time_ = open_time + GetGlobalConfig().GuildCampPartyContinuedTime;
	// 抽奖机
	auto lottery_it = GetGlobalConfig().GuildCampPartyNPC.begin();
	auto lottery_it_end = GetGlobalConfig().GuildCampPartyNPC.end();
	for (; lottery_it != lottery_it_end; ++lottery_it)
	{
		lottery_list_.insert((UINT32)lottery_it->seq[0]);
	}

	// 刷npc时间
	auto sprite_it =  GetGlobalConfig().GuildCampPartySpriteTimeStap.begin();
	auto sprite_it_end = GetGlobalConfig().GuildCampPartySpriteTimeStap.end();
	for (; sprite_it != sprite_it_end; ++sprite_it)
	{
		sprite_wave_appear_.insert(std::make_pair(sprite_it->seq[0],sprite_it->seq[1]));
	}

	UINT32 nowtime = (UINT32)GameTime::GetTime();
	auto wave_it = sprite_wave_appear_.begin();
	auto wave_it_end = sprite_wave_appear_.end();
	if (wave_it != wave_it_end)
	{
		sprite_time_ =  nowtime + wave_it->second;
		sprite_delete_time_ = nowtime + GetGlobalConfig().GuildCampPartySpriteExistTime;
		// 获取后就可以清理掉了
		sprite_wave_appear_.erase(wave_it->first);
	}
	//　通知抽奖机可见
	UpdateNpcSprite();
	// 广播
	WorldSendNotice(GuildCampPartySysStart);
	GuildSendNotice(GuildCampPartyGuildStart);
	LogDebug("-----InitPartyConf--------");
}
bool CGuildCampPartyMgr::Pre5minitNotice()
{
	LogDebug("-----Pre5minitNotice--------");
	WorldSendNotice(GuildCampPartyPre5);
	return true;
}

bool CGuildCampPartyMgr::DownNotice()
{
	LogDebug("-----DownNotice--------");
	GuildSendNotice(GuildCampPartyExchangeItem);
	return true;
}

bool CGuildCampPartyMgr::ClearLottery()
{
	LogDebug("-----ClearLottery--------");
	lottery_list_.clear();
	UpdateNpcSprite();
	return true;
}

bool CGuildCampPartyMgr::ActiveIocnNotice()
{
	LogDebug("-----ActiveIocnNotice--------");
	HalliconsNotify(SYS_GUILD_CAMP_PARTY_NTF,false);
	return true;
}
bool CGuildCampPartyMgr::FinishNotice()
{
	LogDebug("-----FinishNotice--------");

	// 通知 交易结束
	PtcG2C_GuildCampPartyTradeNotify ntf;
	ntf.m_Data.set_notify_type(KKSG::UPDATA_TRADE_STATUS);
	ntf.m_Data.set_open_trade(false);
	for (auto it = trade_data_.begin(); it != trade_data_.end(); ++it)
	{
		TradeData* data = *it;
		if (NULL != data)
		{
			Role* lauch_role = RoleManager::Instance()->FindByRoleID(data->lauch_role_id);
			Role* targe_role = RoleManager::Instance()->FindByRoleID(data->target_role_id);

			if (NULL != lauch_role)
			{
				lauch_role->Send(ntf);
			}
			if (NULL != targe_role)
			{
				targe_role->Send(ntf);
			}

			ClearRoleInTransactionStatus(data->lauch_role_id);
			ClearRoleInTransactionStatus(data->target_role_id);

			LogDebug("delete role id :%llu",data->lauch_role_id);
			LogDebug("delete role id :%llu",data->target_role_id);
			delete data;
		}		
	}
	trade_data_.clear();
	WorldSendNotice(GuildCampPartyEnd);	

	for (auto it = timers_.begin(); it != timers_.end(); ++it)
	{
		CTimeSpecificMgr::Instance()->UnRegisterEvent(*it);
	}
	timers_.clear();

	//　通知活动结束
	PtcG2C_guildcamppartyNotify end_ntf;
	end_ntf.m_Data.set_notify_type(0);
	auto itend = RoleManager::Instance()->End();
	for(auto it = RoleManager::Instance()->Begin(); it != itend; ++it)
	{
		Role *role = *it;
		if(NULL == role)
		{
			continue;
		}
		if (!role->Get<CRoleSystem>()->IsSystemOpened(SYS_GUILD_CAMP_PARTY_NTF))
		{
			SSInfo<<"system close"<<END;
			continue;
		}

		role->Send(end_ntf);
	}	

	// 通知target，活动结束，申请界面也可以关闭了
	PtcG2C_GuildCampPartyTradeNotify end_invation_ntf;	
	end_invation_ntf.m_Data.set_notify_type(KKSG::TRADE_INVITATION);
	end_invation_ntf.m_Data.set_lauch_count(0);
	for (auto it = targe_roles_.begin(); it != targe_roles_.end(); ++it)
	{
		Role* role = RoleManager::Instance()->FindByRoleID(it->first);
		if (NULL == role )
		{
			continue;
		}
		if (!role->Get<CRoleSystem>()->IsSystemOpened(SYS_GUILD_CAMP_PARTY_NTF))
		{			
			continue;
		}
		role->Send(end_invation_ntf);
	}

	state_ = ActiveEnd;	
	sprite_wave_appear_.clear();
	sprite_time_ = 0;
	sprite_delete_time_ = 0;
	reward_continued_time_ = 0;
	lottery_list_.clear();

	last_time_.clear();	

	scene_sprite_list_.clear();
	transaction_roles_.clear();

	sprite_index_ = 0;
	trade_data_.clear();

	summons_spirit_status_.clear();
	scene_summonspirit_.clear();

	return true;
}


void CGuildCampPartyMgr::AudioChat(Role *role,UINT64 audio_id,UINT32 audio_time, std::string translate_text,Role *launch_role,Role * target_role)
{
	if (NULL == role || NULL == launch_role || NULL == target_role)
	{
		return;
	}

	TradeData* data = trade_data_.find<LaunchIndex>(role->GetRoleID());
	if (data != NULL )
	{	
		data->lauch_chat.audio_id = audio_id;
		data->lauch_chat.audio_time = audio_time;
		data->lauch_chat.chat_text = translate_text;
	}
	else
	{
		data = trade_data_.find<TargetIndex>(role->GetRoleID());
		if (data != NULL )
		{	
			data->target_chat.audio_id = audio_id;
			data->target_chat.audio_time = audio_time;
			data->target_chat.chat_text = translate_text;
		}
	}

	NoticeTradeStatus(launch_role,target_role);
	UpdateTransaction(launch_role,target_role);
}

void CGuildCampPartyMgr::TextChat(Role *role,std::string text_chat, Role *launch_role,Role * target_role)
{
	if (NULL == role || NULL == launch_role || NULL == target_role)
	{
		return;
	}

	TradeData* data = trade_data_.find<LaunchIndex>(role->GetRoleID());
	if (data != NULL )
	{			
		data->lauch_chat.chat_text = text_chat;
		data->lauch_chat.audio_id = 0;
		data->lauch_chat.audio_time = 0;
	}
	else
	{
		data = trade_data_.find<TargetIndex>(role->GetRoleID());
		if (data != NULL )
		{			
			data->target_chat.chat_text = text_chat;
			data->target_chat.audio_id = 0;
			data->target_chat.audio_time = 0;
		}
	}

	NoticeTradeStatus(launch_role,target_role);
	UpdateTransaction(launch_role,target_role);
}

bool CGuildCampPartyMgr::CheckSummonSpiritExist(UINT32 scene_id)
{
	auto it = scene_sprite_list_.find(scene_id);
	if (it == scene_sprite_list_.end())
	{
		return false;
	}

	UINT32 serverID = GSConfig::Instance()->GetServerID();	
	for (auto exist_it = it->second.begin(); exist_it != it->second.end(); ++exist_it)
	{
		if ( exist_it->second.sprite_id() >= serverID)
		{
			return true;
		}
	}

	return false;
}

bool CGuildCampPartyMgr::CheckHaveSummonSpiritIng(UINT64 guild_id)
{
	auto it = summons_spirit_status_.find(guild_id);
	if (it != summons_spirit_status_.end())
	{
		UINT32 now_time = GameTime::GetTime() ;
		if (now_time - it->second > GetGlobalConfig().GuildCampPartySpriteExistTime)
		{
			summons_spirit_status_.erase(it);
			return false;
		}

		return true;
	}

	return false;
}

bool CGuildCampPartyMgr::CheckSummonSpiritTime()
{
	UINT32 now_time = GameTime::GetTime() ;
	if (reward_continued_time_ > now_time)
	{
		if (reward_continued_time_ - now_time > GetGlobalConfig().GetInt("GuildCampSummonSpiritTime" , 300))
		{
			return true;
		}
	}
	return false;
}

void CGuildCampPartyMgr::SetHaveSummonSpiritIng(UINT64  guild_id)
{
	summons_spirit_status_.insert(std::make_pair(guild_id,(UINT32)GameTime::GetTime()));
}
void CGuildCampPartyMgr::ClearSummonSpiritIng(UINT64  guild_id)
{
	auto it = summons_spirit_status_.find(guild_id);
	if (it != summons_spirit_status_.end())
	{
		summons_spirit_status_.erase(it);
	}
}

bool CGuildCampPartyMgr::ClearSummonSpirit(time_t  now_time)
{
	std::unordered_set<UINT32> scene_list;	
	auto it = scene_summonspirit_.begin();
	auto it_end = scene_summonspirit_.end();
	for (; it != it_end; ++it)
	{
		if (now_time > it->second)
		{
			scene_list.insert(it->first);
		}
	}


	UINT32 serverID = GSConfig::Instance()->GetServerID();	
	UINT32 new_npc_id =  serverID  << 16;
	for (auto clear_it = scene_list.begin(); clear_it != scene_list.end(); ++clear_it)
	{		
		scene_summonspirit_.erase(*clear_it);

		auto scene_it = scene_sprite_list_.find(*clear_it);
		if (scene_it != scene_sprite_list_.end())
		{	
			std::vector<UINT32> npc_id;
			for (auto exist_it = scene_it->second.begin(); exist_it != scene_it->second.end(); ++exist_it)
			{
				if ( exist_it->first >= new_npc_id)
				{
					npc_id.push_back(exist_it->first);
				}
			}

			for (auto del_it = npc_id.begin(); del_it != npc_id.end(); ++del_it)
			{
				scene_it->second.erase(*del_it);
			}
			Scene* scene = SceneManager::Instance()->FindBySceneID(scene_it->first);
			if (NULL != scene)
			{
				UpdateSceneSpirte(scene);
			}		
		}
	}

	return true;
}
void CGuildCampPartyMgr::SummonSpirit(Role *role)
{
	std::map<UINT32,KKSG::GuildCampSpriteInfo> sprites;
	Scene* scene = role->GetCurrScene();

	UINT32 GuildCampSummonSpiritCount = GetGlobalConfig().GetInt("GuildCampSummonSpiritCount" , 10);
	UINT32 new_id = GSConfig::Instance()->GetServerID();		
	UINT32 serverID = new_id << 16;		
	for (UINT32 i = 0 ; i < GuildCampSummonSpiritCount; ++i)
	{
		UINT32 random_index  = XRandom::randInt(0,GetGlobalConfig().GuildCampPartyBonus.size());				
		auto it = GetGlobalConfig().GuildCampPartyBonus.find(random_index);
		if (it != GetGlobalConfig().GuildCampPartyBonus.end())
		{					
			KKSG::GuildCampSpriteInfo sprite_info;										
			sprite_info.set_sprite_id(++serverID);

			float set_x = (float)XRandom::randDouble(it->second.seq[0]-it->second.seq[3],it->second.seq[0]+it->second.seq[3]);
			float set_z = (float)XRandom::randDouble(it->second.seq[2]-it->second.seq[3],it->second.seq[2]+it->second.seq[3]);

			int posxz = (int)(floor(set_z*100+0.5f));
			posxz |= ((int)(floor(set_x*100+0.5f)) << 16);

			sprite_info.set_position(posxz);
			sprite_info.set_summoner(role->GetName());
			sprites.insert(std::make_pair(serverID,sprite_info));					
		}						
	}
	auto it = scene_sprite_list_.find(scene->GetSceneID());
	if (it != scene_sprite_list_.end())
	{
		auto new_it = sprites.begin();
		auto new_it_end = sprites.end();
		for (; new_it != new_it_end; ++new_it)
		{
			it->second.insert(std::make_pair(new_it->first,new_it->second));						
		}
	}
	else
	{
		scene_sprite_list_.insert(std::make_pair(scene->GetSceneID(),sprites));		
	}

	UpdateSceneSpirte(scene);

	Notice notice(GuildCampPartySummonSpirit);
	notice.Replace("$R",role->GetName());
	notice.SendGuild(role->getGuildId());

	// 清理召唤的小妖
	time_t  clear_time  = GameTime::GetTime() +  GetGlobalConfig().GuildCampPartySpriteExistTime;	
	scene_summonspirit_.insert(std::make_pair(scene->GetSceneID(),clear_time));
}


void CGuildCampPartyMgr::UpdateSceneSpirte(Scene *scene)
{
	if (NULL == scene)
	{
		return;
	}

	PtcG2C_guildcamppartyNotify ntf;
	ntf.m_Data.set_notify_type(1);	
	if (reward_continued_time_ > GameTime::GetTime())
	{
		ntf.m_Data.set_left_time(reward_continued_time_ - (UINT32)GameTime::GetTime());
	}
	auto lottery_it = lottery_list_.begin();
	auto lottery_it_end = lottery_list_.end();
	for (; lottery_it != lottery_it_end; ++lottery_it)
	{
		ntf.m_Data.add_lottery_list(*lottery_it);
	}

	// 先清理上次的数据
	ntf.m_Data.clear_sprite_list();		
	auto exist_scene_id = scene_sprite_list_.find(scene->GetSceneID());
	if (exist_scene_id != scene_sprite_list_.end())
	{				
		for (auto sprite_it = exist_scene_id->second.begin(); sprite_it != exist_scene_id->second.end(); ++sprite_it )
		{					
			KKSG::GuildCampSpriteInfo* sprite_info = ntf.m_Data.add_sprite_list();
			sprite_info->CopyFrom(sprite_it->second);
		}
	}			
	scene->Broadcast(ntf);	
}

bool CGuildCampPartyMgr::ActiveIsFinish()
{
	UINT32 now_time = (UINT32)GameTime::GetTime() ;
	if (reward_continued_time_ > now_time)
	{
		return false;
	}

	return true;
}