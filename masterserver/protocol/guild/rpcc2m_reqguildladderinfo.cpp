#include "pch.h"
#include "guild/rpcc2m_reqguildladderinfo.h"
#include "guild/rpcm2g_getguildladderrewardtime.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildladder.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "network/gslink.h"
#include "foreach.h"

// generate by ProtoGen at date: 2016/9/30 11:18:03

RPC_SERVER_IMPLEMETION(RpcC2M_ReqGuildLadderInfo, ReqGuildLadderInfoAgr, ReqGuildLadderInfoRes)

void RpcC2M_ReqGuildLadderInfo::OnCall(const ReqGuildLadderInfoAgr &roArg, ReqGuildLadderInfoRes &roRes)
{
	roRes.set_nowtime(time(NULL));
	if (!CGuildLadder::Instance()->IsOpen())
	{
		roRes.set_errorcode(KKSG::ERR_GUILD_LADDER_NOT_OPEN);
		return ;
	}

	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roArg.roleid());
	if (pRole == NULL)
	{
		return ;
	}

	RpcM2G_GetGuildLadderRewardTime* pMsg =  RpcM2G_GetGuildLadderRewardTime::CreateRpc();
	pMsg->m_oArg.set_rpcid(DelayRpc());
	pMsg->m_oArg.set_roleid(roArg.roleid());
	GSLink::Instance()->SendToLine(pRole->GetGsLine(), *pMsg);
	return ;
	
	/*
	UINT64 guildId = CGuildMgr::Instance()->GetGuildIdByRoleId(roArg.roleid());
	GuildGroup* pData = CGuildLadder::Instance()->GetGuildGroup(guildId);
	if (pData != NULL)
	{
		UINT32 lastTime = pData->m_rewardTimes[roArg.roleid()] >= GetGlobalConfig().GuildLadderRewardTimes ? 0 : GetGlobalConfig().GuildLadderRewardTimes - pData->m_rewardTimes[roArg.roleid()]; 
		roRes.set_lestrewardtimes(lastTime);

		for (std::multimap<UINT32, UINT64>::reverse_iterator i = pData->m_guildRank.rbegin(); i != pData->m_guildRank.rend(); ++i)
		{
			Guild* pGuild = CGuildMgr::Instance()->GetGuild(i->second);
			if (pGuild == NULL)
			{
				continue;
			}

			KKSG::GuildLadderRank* pGuildRank = roRes.add_guildranks();
			pGuildRank->set_guildid(pGuild->GetID());
			pGuildRank->set_guildname(pGuild->GetName());
			pGuildRank->set_icon(pGuild->GetIcon());
			pGuildRank->set_wintimes(i->first);
		}

		foreach (j in pData->m_roleRank)
		{
			CRoleSummary* pRole = CRoleSummaryMgr::Instance()->GetRoleSummary(j->second);
			if (pRole == NULL)
			{
				continue;
			}
			UINT64 guildId = CGuildMgr::Instance()->GetGuildIdByRoleId(j->second);
			if (guildId == 0)
			{
				continue;
			}

			KKSG::GuildLadderRoleRank* pRoleRank = roRes.add_roleranks();
			pRoleRank->set_roleid(j->second);
			pRoleRank->set_wintimes(j->first);
			pRoleRank->set_name(pRole->GetName());
			pRoleRank->set_guildid(guildId);
		}
		roRes.set_lasttime(CGuildLadder::Instance()->GetLastTime());
		roRes.set_errorcode(ERR_SUCCESS);
	}
	else
	{
		roRes.set_errorcode(KKSG::ERR_GUILD_LADDER_NOT_OPEN);
	}
	*/
}

void RpcC2M_ReqGuildLadderInfo::OnDelayReplyRpc(const ReqGuildLadderInfoAgr &roArg, ReqGuildLadderInfoRes &roRes, const CUserData &roUserData)
{
	UINT64 guildId = CGuildMgr::Instance()->GetGuildIdByRoleId(roArg.roleid());
	GuildGroup* pData = CGuildLadder::Instance()->GetGuildGroup(guildId);
	if (pData != NULL)
	{
		if (pData->m_rewardTimes[roArg.roleid()] < roUserData.m_dwUserData)
		{
			pData->m_rewardTimes[roArg.roleid()] = roUserData.m_dwUserData; 
		}

		UINT32 lastTime = pData->m_rewardTimes[roArg.roleid()] >= GetGlobalConfig().GuildLadderRewardTimes ? 0 : GetGlobalConfig().GuildLadderRewardTimes - pData->m_rewardTimes[roArg.roleid()]; 
		roRes.set_lestrewardtimes(lastTime);

		for (std::multimap<UINT32, UINT64>::reverse_iterator i = pData->m_guildRank.rbegin(); i != pData->m_guildRank.rend(); ++i)
		{
			Guild* pGuild = CGuildMgr::Instance()->GetGuild(i->second);
			if (pGuild == NULL)
			{
				continue;
			}

			KKSG::GuildLadderRank* pGuildRank = roRes.add_guildranks();
			pGuildRank->set_guildid(pGuild->GetID());
			pGuildRank->set_guildname(pGuild->GetName());
			pGuildRank->set_icon(pGuild->GetIcon());
			pGuildRank->set_wintimes(i->first);
		}

		foreach (j in pData->m_roleRank)
		{
			CRoleSummary* pRole = CRoleSummaryMgr::Instance()->GetRoleSummary(j->second);
			if (pRole == NULL)
			{
				continue;
			}
			UINT64 guildId = CGuildMgr::Instance()->GetGuildIdByRoleId(j->second);
			if (guildId == 0)
			{
				continue;
			}

			KKSG::GuildLadderRoleRank* pRoleRank = roRes.add_roleranks();
			pRoleRank->set_roleid(j->second);
			pRoleRank->set_wintimes(j->first);
			pRoleRank->set_name(pRole->GetName());
			pRoleRank->set_guildid(guildId);
		}
		roRes.set_lasttime(CGuildLadder::Instance()->GetLastTime());
		roRes.set_errorcode(ERR_SUCCESS);
	}
	else
	{
		roRes.set_errorcode(KKSG::ERR_GUILD_LADDER_NOT_OPEN);
	}
}
