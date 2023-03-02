#include "pch.h"
#include "cardmatchmgr.h"
#include "time.h"
#include "define/itemdef.h"
#include "mail/mailconfig.h"
#include "mail/maildata.h"
#include "mail/mailmgr.h"
#include "notice/noticemgr.h"
#include "define/systemiddef.h"
#include "role/rolesummarymgr.h"
#include "table/MultActivityMgr.h"
#include "pokertournament/ptcm2c_pokertournamentendrefund.h"
#include "role/role.h"
#include "role/rolemanager.h"

#define  POKER_TOURNAMENT_CHECK  1*1000
INSTANCE_SINGLETON(CardMatchMgr)

	CardMatchMgr::CardMatchMgr()
{
}

CardMatchMgr::~CardMatchMgr()
{
}

bool CardMatchMgr::Init()
{
	if (!porker_reward_.LoadFile("table/PokerTournamentReward.txt"))
	{
		SSWarn<<"load file table/PokerTournamentReward.txt failed"<<END;
		return false;
	}

	role_match_.clear();
	role_signup_num_.clear();
	match_roles_.clear();
	all_matchs_.clear();
	remove_matchs_.clear();

	timer_handle_ = INVALID_HTIMER;

	return true;
}
void CardMatchMgr::Uninit()
{
	Cancel();
}
GuildCardMatch* CardMatchMgr::GetCardMatchByRoleID(const UINT64 role_id)
{
	auto it =  role_match_.find(role_id);
	if (it != role_match_.end())
	{
		return it->second;
	}

	return NULL;
}

void CardMatchMgr::AddRoleTOCardMatch(const UINT64 role_id)
{
	match_roles_.insert(role_id);
	// 报名次数
	auto signup_it = role_signup_num_.find(role_id);
	if (signup_it != role_signup_num_.end())
	{
		signup_it->second += 1;
	}
	else
	{
		role_signup_num_.insert(std::make_pair(role_id,1));
	}

	// 开启定时器，查看每场比赛是否可以删除了。
	StartTimer();

	// gc 配置的大小
	if (match_roles_.size() >= GlobalConfig::GetInstance().PokerTournamentPlayNum )
	{
		GuildCardMatch* card_match = new GuildCardMatch(match_roles_);		
		all_matchs_.insert(card_match);
		auto it = match_roles_.begin();
		auto it_end = match_roles_.end();
		for (; it != it_end; ++it)
		{
			role_match_.insert(std::make_pair(*it,card_match));
		}
		// 比赛开始
		card_match->Run();
		match_roles_.clear();
	}
}

UINT32 CardMatchMgr::GetSignUpNum(const UINT64 role_id)
{
	auto it = role_signup_num_.find(role_id);
	if (it != role_signup_num_.end())
	{
		return it->second;
	}
	return 0;
}

bool CardMatchMgr::CheckRoleExist(const UINT64 role_id)
{
	auto it = match_roles_.find(role_id);
	if (it != match_roles_.end())
	{
		return true;
	}
	LogDebug("role:%llu not in  match_roles_",role_id);

	auto it_match = role_match_.find(role_id);
	if (it_match != role_match_.end())
	{
		return true;
	}
	LogDebug("role:%llu not in role_match_",role_id);

	return false;
}
void CardMatchMgr::DeleteFromCardMatch(const UINT64 role_id)
{
	role_match_.erase(role_id);
}

void CardMatchMgr::StartTimer()
{
	if (INVALID_HTIMER == timer_handle_)
	{
		timer_handle_ = CTimerMgr::Instance()->SetTimer(this, 0, POKER_TOURNAMENT_CHECK, -1, __FILE__, __LINE__);
	}
}
void CardMatchMgr::DispatchPokerTournamentReward(std::set<CardPoint>& match_points)
{	
	UINT32 rank = 1;
	auto rank_it = match_points.begin();
	auto rank_it_end = match_points.end();
	for (; rank_it != rank_it_end; ++rank_it)
	{
		std::vector<ItemDesc> items;
		auto it = porker_reward_.Table.begin();
		auto it_end = porker_reward_.Table.end();
		for (; it != it_end; ++it)
		{			
			if (rank >= (*it)->Rank[0] && rank <= (*it)->Rank[1])
			{
				auto item_it = (*it)->Reward.begin();
				auto item_it_end = (*it)->Reward.end();
				for (; item_it != item_it_end; ++item_it)
				{
					items.push_back(ItemDesc(item_it->seq[0], item_it->seq[1]));
				}
			}
		}
		if (items.size() > 0)
		{
			// send mail 
			const MailConf* conf = MailConfig::Instance()->GetMailConf(MailConf_PokerTournament);
			if (NULL != conf)
			{
				std::string content = MailConfig::Instance()->ReplaceString("0", ToString(rank), conf->m_content);
				MailData mail(conf->m_title, content, items, MAIL_EXPIRE_TIME);
				mail.SetReason(ItemFlow_PokerTournamentAward);

				CMailMgr::Instance()->SendMail(rank_it->roleid, mail);
			}
		}
		++rank;
	}
}

void CardMatchMgr::Cancel()
{
	if (INVALID_HTIMER != timer_handle_)
	{
		CTimerMgr::Instance()->KillTimer(timer_handle_);
		timer_handle_ = INVALID_HTIMER;
	}

	match_roles_.clear();
	role_match_.clear();	
	role_signup_num_.clear();
}

void CardMatchMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	std::unordered_set<UINT64> refund_roles;

	auto remove_it = remove_matchs_.begin();
	auto remove_it_end = remove_matchs_.end();
	for (; remove_it != remove_it_end; ++remove_it)
	{
		auto role_add = (*remove_it)->GetAddRoleId();
		auto roles = (*remove_it)->GetRoles();
		if ( role_add.size() != roles.size())
		{
			// 需要退费
			for(auto it = roles.begin(); it != roles.end(); ++it)
			{
				auto it_exist = role_add.find(*it);
				if (it_exist == role_add.end())
				{
					refund_roles.insert(*it);
				}
			}
		}

		delete *remove_it;
	}
	remove_matchs_.clear();

	auto it = all_matchs_.begin();
	auto it_end = all_matchs_.end();
	for ( ; it != it_end; )
	{
		if ( KKSG::CardMatch_StateDummy == (*it)->GetState())
		{
			remove_matchs_.insert(*it);			   
			all_matchs_.erase(it++);
		}
		else
		{
			++it;
		}
	}

	bool open = MultActivityMgr::Instance()->IsActivityTimeOpen(MULT_ACTIVITY_POKERTOUNAMENT);
	if (!open)
	{
		PtcM2C_PokerTournamentEndReFund ntf;
		for (auto it = match_roles_.begin(); it != match_roles_.end(); ++it)
		{
			refund_roles.insert(*it);	

			CRole* role = CRoleManager::Instance()->GetByRoleID(*it);
			if (role != NULL)
			{
				role->Send(ntf);
			}
		}
	}
	// 没有比赛，且活动结束，就不用定时器了
	if (all_matchs_.size() < 1 && remove_matchs_.size() < 1 && !open)
	{
		Cancel();
	}

	// 报名消耗，退还
	if (refund_roles.size() > 0)
	{
		std::vector<ItemDesc> items;
		auto it_item = GlobalConfig::GetInstance().PokerTournamentSignUpCost.begin();
		auto it_item_end = GlobalConfig::GetInstance().PokerTournamentSignUpCost.end();
		for (;it_item != it_item_end; ++it_item)
		{
			items.push_back(ItemDesc(it_item->seq[0], it_item->seq[1]));
		}

		const MailConf* conf = MailConfig::Instance()->GetMailConf(MailConf_PokerTournament_Refund);
		if (NULL != conf)
		{
			MailData mail(conf->m_title, conf->m_content, items, MAIL_EXPIRE_TIME);
			for (auto it = refund_roles.begin(); it != refund_roles.end(); ++it)
			{			
				CMailMgr::Instance()->SendMail(*it, mail);	
			}
		}

		refund_roles.clear();
	}
}
