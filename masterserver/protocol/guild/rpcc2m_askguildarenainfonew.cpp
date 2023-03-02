#include "pch.h"
#include "guild/rpcc2m_askguildarenainfonew.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guildarena.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildapps.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include <time.h>
#include "foreach.h"


// generate by ProtoGen at date: 2016/9/22 15:41:18

RPC_SERVER_IMPLEMETION(RpcC2M_AskGuildArenaInfoNew, AskGuildArenaInfoArg, AskGuildArenaInfoReq)

void RpcC2M_AskGuildArenaInfoNew::OnCall(const AskGuildArenaInfoArg &roArg, AskGuildArenaInfoReq &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);

	if (pRole == NULL)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		SSWarn<<"RpcC2G_AskGuildArenaInfo :: pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	/*
	UINT32 timeState = CGuildWarMgr::Instance()->GetTimeState();
	if (timeState == GUILD_ARENA_BATTLE_ONE || timeState == GUILD_ARENA_BATTLE_TWO || timeState == GUILD_ARENA_BATTLE_FINAL)
	{
		UINT32 nowTime = time(NULL);
		UINT32 fightTime = CGuildWarMgr::Instance()->GetFightTime(timeState);
		if (nowTime > fightTime && (nowTime - fightTime) > GetGlobalConfig().GuildArenaDelayTime * 60)
		{
			++timeState;
		}
	}*/
	roRes.set_timestate(CGuildWarMgr::Instance()->GetTimeState());

	std::map<UINT32, CGuildArenaWar>& warMap = CGuildWarMgr::Instance()->GetWarMap();
	foreach (i in warMap)
	{
		KKSG::guildArenaWarData* pData = roRes.add_wardata();
		pData->set_wartype(i->first);

		std::map<UINT32, CGuildArenaBattle>& battleMap = i->second.GetBattleMap(); 
		foreach (j in battleMap)
		{
			KKSG::GuildArenaGroupData* pGroupData = pData->add_guildarenagroupdata();
			pGroupData->set_battleid(j->first);
			pGroupData->set_winerid(j->second.GetWinId());
			pGroupData->set_warstate(j->second.GetState());
			pGroupData->set_watchid(j->second.GetWatchId());
			pGroupData->set_state(j->second.GetFightState());

			std::map<UINT32, UINT64> teamMap = j->second.GetTeamMap(); 
			auto iter = teamMap.find(1);
			if (iter != teamMap.end())
			{
				pGroupData->set_guildoneid(iter->second);
			}
			iter = teamMap.find(2);
			if (iter != teamMap.end())
			{
				pGroupData->set_guildtwoid(iter->second);
			}
		}
	}

	foreach (i in CGuildWarMgr::Instance()->GetGuildSet())
	{
		Guild* pGuild = CGuildMgr::Instance()->GetGuild(*i);
		if (pGuild != NULL)
		{
			KKSG::GuildInfo* pInfo = roRes.add_allguildinfo();
			pInfo->set_id(pGuild->GetID());
			pInfo->set_name(pGuild->GetName());
			pInfo->set_leaderid(pGuild->GetLeaderID());
			CRoleSummary* pRoleSum = CRoleSummaryMgr::Instance()->GetRoleSummary(pGuild->GetLeaderID());
			if (pRoleSum != NULL)
			{
				pInfo->set_leadername(pRoleSum->GetName());
			}
			else
			{
				pInfo->set_leadername(pGuild->GetLeaderName());
			}
			pInfo->set_level(pGuild->GetLevel());
			pInfo->set_ppt(pGuild->GetRecuritPPT());
			pInfo->set_needapproval(pGuild->GetNeedApproval());
			pInfo->set_membercount(pGuild->GetMemberCount());
			pInfo->set_issendapplication(pGuild->Get<GuildApplication>()->IsContain(pRole->GetID()));
			pInfo->set_icon(pGuild->GetIcon());
			pInfo->set_capacity(pGuild->GetCapacity());
			pInfo->set_annoucement(pGuild->GetAnnoucement());
			pInfo->set_guildexp(pGuild->GetExp());
		}

	}

	CGuildWarMgr::Instance()->AddShowRole(pRole->GetID());
}

void RpcC2M_AskGuildArenaInfoNew::OnDelayReplyRpc(const AskGuildArenaInfoArg &roArg, AskGuildArenaInfoReq &roRes, const CUserData &roUserData)
{
}
