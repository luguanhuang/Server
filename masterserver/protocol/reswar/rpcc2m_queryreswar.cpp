#include "pch.h"
#include "reswar/rpcc2m_queryreswar.h"
#include "reswar/reswarmgr.h"
#include "role/rolemanager.h"
#include "team/teammgr.h"
#include "team/team.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"

RPC_SERVER_IMPLEMETION(RpcC2M_QueryResWar, QueryResWarArg, QueryResWarRes)

void RpcC2M_QueryResWar::OnCall(const QueryResWarArg &roArg, QueryResWarRes &roRes)
{
	CRole *pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (!pRole)  
	{
		roRes.set_error(ERR_UNKNOWN);
		return;
	}

	Guild *pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (!pGuild)
	{
		roRes.set_error(KKSG::ERR_NOTGUILD);
		return;
	}
	GuildTeam*pGuildTeam = ResWarMgr::Instance()->GetGuildTeam(pGuild->GetID());
	if (!pGuildTeam)
	{
		bool bOpen = ResWarMgr::Instance()->GetState();
		if (!bOpen)
			roRes.set_error(KKSG::ERR_RESWAR_ACTIVITY);
		else
			roRes.set_error(KKSG::ERR_RESWAR_GROUP);
		return;
	}
	if (roArg.param()== KKSG::RESWAR_FLOWAWARD)
	{
		if ( ResWarMgr::Instance()->GetStep()==eGuildResFlowAward)
		{
			if (pGuildTeam->pGroupGuildInfo)
			{
				UINT32 nGroupID = pGuildTeam->pGroupGuildInfo->nGroupID;
				ResWarMgr::Instance()->GetGuildWarRank(nGroupID, m_oRes.mutable_finalrank());
				ResWarMgr::Instance()->SetIconState(pRole);
				roRes.set_error(KKSG::ERR_SUCCESS);
			}else
			{
				roRes.set_error(KKSG::ERR_RESWAR_GROUP);
			}
		}else
		{
			roRes.set_error(KKSG::ERR_RESWAR_ACTIVITY);
		}
	}else
	{
		Team *pTeam = TeamMgr::Instance()->FindTeam(pRole->GetTeamID());
		GuildFightTeam *pFightTeam = NULL;
		if (pTeam)
		{
			pFightTeam =  ResWarMgr::Instance()->GetGuildFightTeam(pGuild->GetID(), pTeam->GetID());
		}
		bool bOpen = ResWarMgr::Instance()->GetState();
		if (!bOpen)
		{
			roRes.set_error(KKSG::ERR_RESWAR_ACTIVITY);
			return;
		}
		KKSG::ResWarGuildBrief *pBrief = roRes.mutable_data();
		ResWarMgr::Instance()->GetResWarGuildBrief(pGuildTeam,pFightTeam,pBrief);
		ResWarMgr::Instance()->GetResWarGuildRank(pGuildTeam,pBrief);
		ResWarMgr::Instance()->GetResWarGuildBuff(pGuildTeam,pBrief);
		ResWarMgr::Instance()->GetNoticeInfo(pGuildTeam, pBrief);
		ResWarMgr::Instance()->AddRoleJoin(pRole->GetID());
	}

	roRes.set_error(KKSG::ERR_SUCCESS);
}

void RpcC2M_QueryResWar::OnDelayReplyRpc(const QueryResWarArg &roArg, QueryResWarRes &roRes, const CUserData &roUserData)
{
}
