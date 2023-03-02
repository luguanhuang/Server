#include "pch.h"
#include "activity/rpcg2m_getmulactopeninfo.h"
#include "guild/guilddef.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildbonus.h"
#include "guild/guildcard.h"
#include "guild/guildcardmatch.h"
#include "guild/guildcardmgr.h"

// generate by ProtoGen at date: 2017/2/10 21:44:51

RPC_SERVER_IMPLEMETION(RpcG2M_GetMulActOpenInfo, GetMulActOpenInfoArg, GetMulActOpenInfoRes)

void RpcG2M_GetMulActOpenInfo::OnCall(const GetMulActOpenInfoArg &roArg, GetMulActOpenInfoRes &roRes)
{
	roRes.set_result(KKSG::ERR_FAILED);
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (!role)
	{
		return;
	}
	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
	if (!pGuild)
	{
		return;
	}
	roRes.set_result(KKSG::ERR_SUCCESS);
	auto guildBonusMgr = pGuild->Get<GuildBonusMgr>();
	roRes.set_last_bonus_time(guildBonusMgr->GetLastCheckinBonusTime());
	roRes.set_is_all_check_bonus_taked(guildBonusMgr->IsCheckInBonusAllGot());

	CGuildCard* guildcard = pGuild->Get<CGuildCard>();
	GuildCardMatch* match = guildcard->GetMatch();
	bool isbegin = true;
	if (NULL == match || !match->IsBegin())
	{
		isbegin = false;
	}
	roRes.set_guildcardopen(isbegin);
	roRes.set_guildcard_openstate(guildcard->GetMatchStatus());
}

void RpcG2M_GetMulActOpenInfo::OnDelayReplyRpc(const GetMulActOpenInfoArg &roArg, GetMulActOpenInfoRes &roRes, const CUserData &roUserData)
{
}
