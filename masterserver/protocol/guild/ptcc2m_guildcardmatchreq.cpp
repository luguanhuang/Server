#include "pch.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildcard.h"
#include "guild/guildcardmatch.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/ptcc2m_guildcardmatchreq.h"
#include "guild/ptcm2c_guildcardmatchntf.h"
#include "table/MultActivityMgr.h"
#include "guild/guildcardmgr.h"
#include "loghelper/tlogr.h"
#include "guild/rpcm2g_queryrolecardmatch.h"
#include "network/gslink.h"

// generate by ProtoGen at date: 2016/10/2 17:38:51

void PtcC2M_GuildCardMatchReq::Process(UINT32 dwConnID)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		SSWarn<<"role is NULL, sessionid:"<<m_sessionID<<END;
		return;
	}
	if(role->CheckIdipAndNotify(KKSG::PUNISH_USER_MULTI_ACTIVITY))
	{
		return ;
	}

	Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
	if (NULL == guild)
	{
		SSWarn<<"role has no guild, roleid:"<<role->GetID()<<END;
		return;
	}
	CGuildCard* guildcard = guild->Get<CGuildCard>();
	GuildCardMatch* match = guildcard->GetMatch();
	bool isbegin = true;
	if (NULL == match || !match->IsBegin())
	{
		isbegin = false;
	}

	PtcM2C_GuildCardMatchNtf ntf;	
	ntf.m_Data.set_op(m_Data.op());
	ntf.m_Data.set_state(NULL != match ? (KKSG::CardMatchState)match->GetState() : KKSG::CardMatch_StateDummy);

	///> 查询当前状态
	if (m_Data.op() == KKSG::CardMatch_Query)
	{
		ntf.m_Data.set_isbegin(isbegin);
		ntf.m_Data.set_iscanbegin(MultActivityMgr::Instance()->IsActivityTimeOpen(MULT_ACTIVITY_CARDMATCH));
		role->Send(ntf);
		return;
	}

	///> 会长开启
	if (m_Data.op() == KKSG::CardMatch_Begin)
	{
		if (!(guild->isLeader(role->GetID()) || guild->isViceLeader(role->GetID())))
		{
			ntf.m_Data.set_errorcode(KKSG::ERR_CARDMATCH_BEGINFAILED);
			return;
		}
		if (!MultActivityMgr::Instance()->IsActivityTimeOpen(MULT_ACTIVITY_CARDMATCH))
		{
			ntf.m_Data.set_errorcode(KKSG::ERR_CARDMATCH_BEGINFAILED);
			return;
		}
		if (guildcard->BeginMatch())
		{
			ntf.m_Data.set_errorcode(KKSG::ERR_SUCCESS);
		}
		else
		{
			ntf.m_Data.set_errorcode(KKSG::ERR_CARDMATCH_BEGINFAILED);
		}
		role->Send(ntf);
		return;
	}
	///> 是否开启
	if (!isbegin)
	{
		ntf.m_Data.set_errorcode(KKSG::ERR_CARDMATCH_NOBEGIN);
		role->Send(ntf);
		return;
	}
	ntf.m_Data.set_round(match->GetRound());
	switch (m_Data.op())
	{
	///> 参赛
	case KKSG::CardMatch_Add:
		{
			//UINT32 timeleft = 0;
			//UINT32 round = (UINT32)(-1);
			//if (match->AddRole(role->GetID(), timeleft, round))
			//{
			//	ntf.m_Data.set_timeleft(timeleft);
			//	ntf.m_Data.set_round(round);
			//	ntf.m_Data.set_errorcode(KKSG::ERR_SUCCESS);
			//}
			//else
			//{
			//	ntf.m_Data.set_errorcode(KKSG::ERR_CARDMATCH_ENDSOON);
			//}
			//role->Send(ntf);
			RpcM2G_QueryRoleCardMatch* rpc = RpcM2G_QueryRoleCardMatch::CreateRpc();
			rpc->m_oArg.set_guildid(guild->GetID());
			rpc->m_oArg.set_roleid(role->GetID());
			GSLink::Instance()->SendToLine(role->GetGsLine(), *rpc);

			break;
		}
	///> 离开
	case KKSG::CardMatch_Del:
		{
			match->DelRole(role->GetID());
			break;
		}
	///> 换牌
	case KKSG::CardMatch_RoundChange:
		{
			bool canchange = false;
			std::vector<UINT32>* cards = match->ChangeRoundRole(role->GetID(), m_Data.card(), canchange);
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
			UINT32 result = match->EndRoundRole(role->GetID());
			ntf.m_Data.set_result(result);
			role->Send(ntf);
			break;
		}
	default:
		{
			SSWarn<<"invalid op:"<<m_Data.op()<<END;
			break;
		}
	}
	TGuildCardMatch oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_uGuildID = guild->GetID();
	oLog.m_uRoleID = role->GetID();
	oLog.m_strOpenId = role->GetAccount();
	oLog.m_uRound = ntf.m_Data.round()==UINT32(-1)?0:ntf.m_Data.round()+1;
	oLog.m_iResult =  ntf.m_Data.has_result()?ntf.m_Data.result():-1;
	oLog.m_iOp = ntf.m_Data.op();
	oLog.Do();
}
