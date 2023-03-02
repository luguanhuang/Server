#include "pch.h"
#include "gamelogic/military_rank_record.h"
#include "util/gametime.h"
#include "util/XCommon.h"
#include "unit/role.h"
#include "gamelogic/military_rank_manager.h"
#include "ranklist/rpcg2m_updatemsranklist.h"
#include "gamelogic/herobattlerecord.h"
#include "gamelogic/pvprecord.h"
#include "unit/roleoutlook.h"
#include "rolesummary/rolesummarybase.h"
#include "define/tlogenum.h"
#include "militaryrank/ptcg2c_militaryrankntf.h"
#include "gamelogic/bagtransition.h"

const UINT32 MILITARY_ITEM_ID = 12;
MilitaryRankRecord::MilitaryRankRecord(Role* role)
	:role_(role)
{
	military_rank_ = 0;
	military_max_rank_his_ = 0;
	military_exploit_ = 0;
	military_max_exploit_his_ = 0;
	m_lastUpdateTime = 0;
	m_isModify.Set();	
	achieve_rank_.clear();
}
MilitaryRankRecord::~MilitaryRankRecord()
{

}

bool MilitaryRankRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (!poRoleAllInfo->has_military())
	{
		return true;
	}

	const KKSG::MilitaryRecord& military_record = poRoleAllInfo->military();
	if (military_exploit_ < military_record.military_exploit())
	{
		military_exploit_ = military_record.military_exploit();
	}	
	if (military_max_exploit_his_ < military_record.military_exploit_his())
	{
		military_max_exploit_his_ =  military_record.military_exploit_his();
	}
	if (military_rank_  <  military_record.military_rank())
	{
		military_rank_  =  military_record.military_rank();
	}
	if (military_max_rank_his_ <  military_record.military_rank_his())
	{
		military_max_rank_his_ =  military_record.military_rank_his();
	}

	m_lastUpdateTime = military_record.last_update_time();

	for (UINT32 i = 0; i < military_record.achieve_rank_size(); ++i)
	{
		achieve_rank_.insert(military_record.achieve_rank(i));
	}

	LogDebug("military_rank_ :%d,update:%d\n",military_rank_,m_lastUpdateTime);

	return true;
}
void MilitaryRankRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{	
	Update();
	if (!m_isModify.TestAndReset())
	{
		return;
	}

	KKSG::MilitaryRecord* record = poRoleAllInfo->mutable_military();
	record->Clear();

	record->set_military_rank(military_rank_);
	record->set_military_rank_his(military_max_rank_his_);
	record->set_military_exploit(military_exploit_);
	record->set_military_exploit_his(military_max_exploit_his_);
	record->set_last_update_time(m_lastUpdateTime);

	for ( auto it = achieve_rank_.begin(); it != achieve_rank_.end(); ++it)
	{
		record->add_achieve_rank(*it);
	}

	LogDebug("military_rank_ :%d,update:%d\n",military_rank_,m_lastUpdateTime);
	roChanged.insert(record);
}

void MilitaryRankRecord::Update()
{
	time_t now = GameTime::GetTime();
	time_t offset = time_t(GetGlobalConfig().PkWeekOffset);//和pk排行榜一致
	if (!XCommon::IsMonthSame(time_t(m_lastUpdateTime) + offset, now + offset, false))
	{
		m_lastUpdateTime = (UINT32)now;
		military_rank_ = 0;
		military_exploit_ = 0;
		m_isModify.Set();
		NotifyClientMilitaryChange();
		ConsumeItemForMilitary(military_exploit_);
		achieve_rank_.clear();
		LogDebug("military_rank_ :%d,update:%d\n",military_rank_,m_lastUpdateTime);
	}	
}

UINT32 MilitaryRankRecord::GetMilitaryExploitByType(const KKSG::SceneType scene_type)
{
	Update();
	return  MilitaryRankManager::Instance()->GetMilitaryExploit(scene_type,military_rank_);
}

UINT32 MilitaryRankRecord::GetReduceMilitaryExploitByType(const KKSG::SceneType scene_type)
{
	Update();
	return  MilitaryRankManager::Instance()->GetReduceMilitaryExploit(scene_type,military_rank_);
}

UINT32 MilitaryRankRecord::AddBattleRecord(const KKSG::SceneType scene_type)
{
	if(!role_->Get<CRoleSystem>()->IsSystemOpened(SYS_MILITARY_RANK))
	{
		return 0;//系统未开启
	}

	LogInfo("AddBattleRecord type is :%d,military_rank is %d,military_exploit is %d",
		scene_type,military_rank_,military_exploit_);

	Update();

	UINT32 exploit = GetMilitaryExploitByType(scene_type);
	if (exploit > 0)
	{
		SetMilitaryExploit(exploit);
	}
	return exploit;
}

void MilitaryRankRecord::UpdateMSRankList()
{
	RpcG2M_UpdateMSRankList* rpc = RpcG2M_UpdateMSRankList::CreateRpc();
	rpc->m_oArg.set_ranktype(KKSG::MilitaryRank);
	KKSG::RankData* data = rpc->m_oArg.mutable_info();
	data->set_roleid(role_->GetRoleID());	
	data->set_time((UINT32)GameTime::GetTime());
	KKSG::MilitaryRankData* military = data->mutable_military_info();
	military->set_military_exploit(military_exploit_);
	military->set_military_rank(military_rank_);
	// 设置总的场次 
	UINT32 total_num = role_->Get<CHeroBattleRecord>()->GetTotalNum() + role_->Get<MyPvpRecord>()->GetCountAll();
	military->set_total_num(total_num);
	role_->SendToMS(*rpc);
}

void MilitaryRankRecord::SetMilitaryExploit(UINT32 military_exploit)
{
	Update();

	GiveItemTransacionForMilitary(military_exploit);
	military_exploit_ += military_exploit;

	UINT32 new_rank = MilitaryRankManager::Instance()->GetMilitaryRankByExploit(military_exploit_);
	if (new_rank > military_rank_)
	{
		// 给首次达到的奖励，通知客户端
		while ( military_rank_ < new_rank)
		{
			++military_rank_;
			auto it = achieve_rank_.find(military_rank_);
			if (it == achieve_rank_.end())
			{
				MilitaryRankManager::Instance()->GiveRewardForFirstAchieveRank(role_->GetRoleID(),military_rank_);
				achieve_rank_.insert(military_rank_);				
			}
			NotifyClientMilitaryChange();
		}
		military_rank_ = new_rank;

		// 如果军阶创历史则更新
		if (military_rank_ > military_max_rank_his_)
		{
			military_max_rank_his_ = military_rank_;
		}
	}
	// 如果军功创历史则更新
	if (military_exploit_ > military_max_exploit_his_)
	{
		military_max_exploit_his_ = military_exploit_;
	}

	m_isModify.Set();
	// 更新排行榜
	UpdateMSRankList();
	LogDebug("military_rank_ :%d\n",military_rank_);
}


// 通知客户端
void MilitaryRankRecord::NotifyClientMilitaryChange()
{	
	PtcG2C_MilitaryrankNtf ntf;
	ntf.m_Data.set_military_rank(military_rank_);
	ntf.m_Data.set_military_rank_his(military_max_rank_his_);
	ntf.m_Data.set_military_exploit(military_exploit_);
	ntf.m_Data.set_military_exploit_his(military_max_exploit_his_);
	role_->Send(ntf);

	role_->OnMilitaryChange();

	RoleOutLook outlook(role_);
	outlook.Fill(SUMMARY_MILITARY_RANK, role_);
	outlook.BroadCast();
}


void MilitaryRankRecord::DebugClear()
{
	ConsumeItemForMilitary(military_exploit_);
	military_rank_ = 0;
	military_exploit_ = 1;
	m_isModify.Set();
	UpdateMSRankList();
	NotifyClientMilitaryChange();
	LogDebug("military_rank_ :%d\n",military_rank_);
}

// 只需要一个瓢字效果而已
void MilitaryRankRecord::GiveItemTransacionForMilitary(UINT32 count)
{
	BagGiveItemTransition trans(role_);
	trans.SetReason(ItemFlow_Military,ItemFlow_Military_Exchange);
	trans.GiveItem(MILITARY_ITEM_ID, count);
	trans.NotifyClient();
}

void MilitaryRankRecord::ConsumeItemForMilitary(UINT32 military_exploit)
{
	if (military_exploit > 0)
	{
		BagTakeItemTransition transition(role_);
		transition.SetReason(ItemFlow_Military,ItemFlow_Military_Exchange);

		if (!transition.TakeItem(MILITARY_ITEM_ID,military_exploit))
		{
			transition.RollBack();	
			return;
		}

		transition.NotifyClient();	
	}
}


UINT32 MilitaryRankRecord::ReduceBattleRecord(const KKSG::SceneType scene_type)
{
	if(!role_->Get<CRoleSystem>()->IsSystemOpened(SYS_MILITARY_RANK))
	{
		return 0;//系统未开启
	}

	LogInfo("ReduceBattleRecord type is :%d,military_rank is %d,military_exploit is %d",
		scene_type,military_rank_,military_exploit_);

	Update();

	UINT32 exploit = GetReduceMilitaryExploitByType(scene_type);
	if (exploit > 0)
	{
		ReduceMilitaryExploit(exploit);
	}
	return exploit;
}


void MilitaryRankRecord::ReduceMilitaryExploit(UINT32 military_exploit)
{
	Update();

	UINT32 consume_count = 0;
	if (military_exploit_ > military_exploit)
	{
		consume_count = military_exploit;
		military_exploit_ -= military_exploit;		
	}
	else
	{
		consume_count = military_exploit_;
		military_exploit_ = 0;
	}

	ConsumeItemForMilitary(consume_count);

	UINT32 new_rank = MilitaryRankManager::Instance()->GetMilitaryRankByExploit(military_exploit_);
	if (new_rank != military_rank_)
	{
		military_rank_ = new_rank;		
	}

	NotifyClientMilitaryChange();		
	m_isModify.Set();
	// 更新排行榜
	UpdateMSRankList();
	LogDebug("military_rank_ :%d\n",military_rank_);
}
