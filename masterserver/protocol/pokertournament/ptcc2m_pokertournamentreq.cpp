#include "pch.h"
#include "pokertournament/ptcc2m_pokertournamentreq.h"
#include "guild/guildcardmatch.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "pokertournament/cardmatchmgr.h"
#include "guild/ptcm2c_guildcardmatchntf.h"
#include "pokertournament/rpcm2g_pokertournamentsignupcost.h"
#include "table/MultActivityMgr.h"
#include "network/gslink.h"
#include "define/tlogenum.h"
// generate by ProtoGen at date: 2016/12/10 17:02:23

void PtcC2M_PokerTournamentReq::Process(UINT32 dwConnID)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		SSWarn<<"role is NULL, sessionid:"<<m_sessionID<<END;
		return;
	}
	LogDebug("m_Data.op() : %d",m_Data.op());

	GuildCardMatch* card_match = CardMatchMgr::Instance()->GetCardMatchByRoleID(role->GetID());
	PtcM2C_GuildCardMatchNtf ntf;
	// 比赛类型有别于工会的
	ntf.m_Data.set_match_type(1);
	ntf.m_Data.set_op(m_Data.op());
	ntf.m_Data.set_state(NULL != card_match ? (KKSG::CardMatchState)card_match->GetState() : KKSG::CardMatch_StateDummy);

	///> 查询当前状态
	if (m_Data.op() == KKSG::CardMatch_Query)
	{
		// 1.活动是否开启
		ntf.m_Data.set_isbegin(MultActivityMgr::Instance()->IsActivityTimeOpen(MULT_ACTIVITY_POKERTOUNAMENT));
		// 2.是否报名
		bool exist = CardMatchMgr::Instance()->CheckRoleExist(role->GetID());
		ntf.m_Data.set_sign_up(exist);	
		UINT32 sign_up_num = CardMatchMgr::Instance()->GetSignUpNum(role->GetID());
		ntf.m_Data.set_sign_up_num(sign_up_num);
		role->Send(ntf);
		return;
	}
	///> 报名
	if (m_Data.op() == KKSG::CardMatch_SignUp)
	{
		// 1.活动开启		
		bool open = MultActivityMgr::Instance()->IsActivityTimeOpen(MULT_ACTIVITY_POKERTOUNAMENT);
		if (!open)
		{
			ntf.m_Data.set_isbegin(MultActivityMgr::Instance()->IsActivityTimeOpen(MULT_ACTIVITY_POKERTOUNAMENT));
			role->Send(ntf);
			return;
		}

		// 2.是否已经报名
		bool exist = CardMatchMgr::Instance()->CheckRoleExist(role->GetID());
		if (exist)
		{
			ntf.m_Data.set_sign_up(exist);		
			role->Send(ntf);
			return ;
		}

		UINT32 sign_up_num = CardMatchMgr::Instance()->GetSignUpNum(role->GetID());
		ntf.m_Data.set_sign_up_num(sign_up_num);
		if (sign_up_num >= GlobalConfig::GetInstance().PokerTournamentSignUpNum)
		{
			ntf.m_Data.set_errorcode(KKSG::ERR_CARDMATCH_SIGNUP_LIMIT);
			role->Send(ntf);
			return ;
		}

		RpcM2G_PokerTournamentSignUpCost* rpc = RpcM2G_PokerTournamentSignUpCost::CreateRpc();
		rpc->m_oArg.set_role_id(role->GetID());
		rpc->m_oArg.set_reason(ItemFlow_PokerTournament);
		rpc->m_oArg.set_subreason(ItemFlow_PokerTournamentCost);

		// 报名消耗物品检查
		auto it = GlobalConfig::GetInstance().PokerTournamentSignUpCost.begin();
		auto it_end = GlobalConfig::GetInstance().PokerTournamentSignUpCost.end();
		for ( ; it != it_end; ++it)
		{
			KKSG::ItemBrief* item = rpc->m_oArg.add_items();
			item->set_itemid(it->seq[0]);
			item->set_itemcount(it->seq[1]);
		}

		GSLink::Instance()->SendToLine(role->GetGsLine(), *rpc);	
		return ;
	}
	if (NULL != card_match)
	{
		ntf.m_Data.set_round(card_match->GetRound());
	}
	switch (m_Data.op())
	{
		///> 参赛
	case KKSG::CardMatch_Add:
		{
			if (NULL != card_match)
			{
				ntf.m_Data.set_round(card_match->GetRound());
				UINT32 timeleft = 0;
				UINT32 round = (UINT32)(-1);
				if (card_match->AddRole(role->GetID(), timeleft, round))
				{
					card_match->SaveAddRoleId(role->GetID());

					ntf.m_Data.set_timeleft(timeleft);
					ntf.m_Data.set_round(round);
					ntf.m_Data.set_errorcode(KKSG::ERR_SUCCESS);

					// 如果本局切出去又回来，这个累计获得需要立马显示
					const auto& total_awards = card_match->GetTotalAwards();
					auto it = total_awards.find(role->GetID());
					if (it != total_awards.end())
					{
						auto item_it = it->second.begin();
						auto item_it_end = it->second.end();
						for (; item_it != item_it_end; ++item_it)
						{
							KKSG::ItemBrief* brief = ntf.m_Data.add_items();
							brief->set_itemid(item_it->first);
							brief->set_itemcount(item_it->second);
						}
					}
				}
				else
				{
					ntf.m_Data.set_errorcode(KKSG::ERR_POK_MATCH_ENDSOON); 
				}
			}

			role->Send(ntf);
		}
		break;
	case KKSG::CardMatch_Del:
		{
			if (NULL != card_match)
			{
				card_match->DelRole(role->GetID());
			}			
			break;
		}
		///> 换牌
	case KKSG::CardMatch_RoundChange:
		{
			bool canchange = false;
			std::vector<UINT32>* cards = NULL;
			if (NULL != card_match)
			{
				cards = card_match->ChangeRoundRole(role->GetID(), m_Data.card(), canchange);
			}				
			if (!canchange)
			{
				ntf.m_Data.set_errorcode(KKSG::ERR_CARDMATCH_CHANGELIMIT);
			}
			if (NULL != cards)
			{
				for (auto i = cards->begin(); i != cards->end(); ++i)
				{
					ntf.m_Data.add_cards(*i);
				}
				ntf.m_Data.set_result(GuildCardMgr::Instance()->GetResult(*cards));
			}
			role->Send(ntf);
			break;
		}
		///> 结算
	case KKSG::CardMatch_RoundEnd:
		{
			if (NULL != card_match)
			{
				UINT32 result = card_match->EndRoundRole(role->GetID());
				ntf.m_Data.set_result(result);	

				// 领取奖励，累计获得需要立马显示
				const auto& total_awards = card_match->GetTotalAwards();
				auto it = total_awards.find(role->GetID());
				if (it != total_awards.end())
				{
					auto item_it = it->second.begin();
					auto item_it_end = it->second.end();
					for (; item_it != item_it_end; ++item_it)
					{
						KKSG::ItemBrief* brief = ntf.m_Data.add_items();
						brief->set_itemid(item_it->first);
						brief->set_itemcount(item_it->second);
					}
				}
			}

			role->Send(ntf);
			break;
		}
	default:
		{
			SSWarn<<"invalid op:"<<m_Data.op()<<END;
			break;
		}
	}
}
