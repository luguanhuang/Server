#include "pch.h"
#include "guild/rpcc2m_askguildarenateaminfonew.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guildarena.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "foreach.h"


// generate by ProtoGen at date: 2016/9/22 15:42:44

RPC_SERVER_IMPLEMETION(RpcC2M_AskGuildArenaTeamInfoNew, AskGuildArenaTeamInfoArg, AskGuildArenaTeamInfoRes)

void RpcC2M_AskGuildArenaTeamInfoNew::OnCall(const AskGuildArenaTeamInfoArg &roArg, AskGuildArenaTeamInfoRes &roRes)
{
	/*
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);

	if (pRole == NULL)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		SSWarn<<" RpcC2G_AskGuildArenaTeamInfo:: pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuild == NULL)
	{
		SSWarn<<" RpcC2G_AskGuildArenaTeamInfo:: pGuild is NULL"<<END;
		return;
	}

	CGuildArenaTeam* pTeam = CGuildWarMgr::Instance()->GetTeam(pGuild->GetID());
	if (pTeam == NULL)
	{
		SSWarn<<" RpcC2G_AskGuildArenaTeamInfo:: pTeam is NULL"<<END;
		return;
	}

	std::map<UINT32, fightUnit>& unitMap = pTeam->GetFigthRoleMap();
	foreach (i in unitMap)
	{
		KKSG::GuildDarenaUnit* pData = roRes.add_fightunit();
		pData->set_roleid(i->second.roleId);
		pData->set_state(i->second.state);
	}

	const std::vector<GuildMember>& memberVec = pGuild->GetGuildMember();
	foreach (i in memberVec)
	{
		CRoleSummary *pRoleSum = CRoleSummaryMgr::Instance()->GetRoleSummary(i->qwRoleID);
		if (pRoleSum != NULL)
		{
			KKSG::GuildMemberData *pData = roRes.add_guildmember();
			pData->set_name(pRoleSum->GetName());
			pData->set_roleid(pRoleSum->GetID());
			pData->set_position(i->position);
			pData->set_contribute(i->contribute);
			pData->set_ppt(pRoleSum->GetPowerPoint());
			pData->set_level(pRoleSum->GetLevel());
			pData->set_profession((KKSG::RoleType)pRoleSum->GetProfession());
			pData->set_vip(pRoleSum->GetVipLevel());
			pData->set_flag(0);
			if (CRoleManager::Instance()->GetByRoleID(i->qwRoleID) != NULL)
			{
				pData->set_isonline(true);
			}
			else
			{
				pData->set_isonline(false);
			}
		}

	}
	*/
}

void RpcC2M_AskGuildArenaTeamInfoNew::OnDelayReplyRpc(const AskGuildArenaTeamInfoArg &roArg, AskGuildArenaTeamInfoRes &roRes, const CUserData &roUserData)
{
}
