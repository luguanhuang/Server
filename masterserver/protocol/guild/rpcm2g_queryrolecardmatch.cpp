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
#include "guild/rpcm2g_queryrolecardmatch.h"

// generate by ProtoGen at date: 2016/10/26 21:19:07

RPC_CLIENT_IMPLEMETION(RpcM2G_QueryRoleCardMatch, QueryRoleCardMatchArg, QueryRoleCardMatchRes)

void RpcM2G_QueryRoleCardMatch::OnReply(const QueryRoleCardMatchArg &roArg, const QueryRoleCardMatchRes &roRes, const CUserData &roUserData)
{
	CRole* role = CRoleManager::Instance()->GetByRoleID(roArg.roleid());
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
	ntf.m_Data.set_op(KKSG::CardMatch_Add);
	ntf.m_Data.set_state(NULL != match ? (KKSG::CardMatchState)match->GetState() : KKSG::CardMatch_StateDummy);

	///> 是否开启
	if (!isbegin)
	{
		ntf.m_Data.set_errorcode(KKSG::ERR_CARDMATCH_NOBEGIN);
		role->Send(ntf);
		return;
	}

	///> GS条件没过
	if (KKSG::ERR_SUCCESS != roRes.errorcode())
	{
		ntf.m_Data.set_errorcode(KKSG::ERR_CARDMATCH_ENDSOON);
		role->Send(ntf);
		return;
	}

	ntf.m_Data.set_round(match->GetRound());
	UINT32 timeleft = 0;
	UINT32 round = (UINT32)(-1);
	if (match->AddRole(role->GetID(), timeleft, round))
	{
		ntf.m_Data.set_timeleft(timeleft);
		ntf.m_Data.set_round(round);
		ntf.m_Data.set_errorcode(KKSG::ERR_SUCCESS);
	}
	else
	{
		ntf.m_Data.set_errorcode(KKSG::ERR_CARDMATCH_ENDSOON);
	}
	role->Send(ntf);

	TGuildCardMatch oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_uGuildID = guild->GetID();
	oLog.m_uRoleID = role->GetID();
	oLog.m_strOpenId = role->GetAccount();
	oLog.m_uRound = ntf.m_Data.round()==UINT32(-1)?0:ntf.m_Data.round()+1;
	oLog.m_iResult =  ntf.m_Data.has_result()?ntf.m_Data.result():-1;
	oLog.m_iOp = ntf.m_Data.op();
	oLog.Do();
}

void RpcM2G_QueryRoleCardMatch::OnTimeout(const QueryRoleCardMatchArg &roArg, const CUserData &roUserData)
{
}
