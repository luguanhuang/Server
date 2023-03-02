#include "pch.h"
#include "guild/rpcc2m_guildarenasimpledeploynew.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildarena.h"
#include "foreach.h"

// generate by ProtoGen at date: 2016/9/22 16:23:48

RPC_SERVER_IMPLEMETION(RpcC2M_GuildArenaSimpleDeployNew, GuildArenaSimpleDeployArg, GuildArenaSimpleDeployRes)

void RpcC2M_GuildArenaSimpleDeployNew::OnCall(const GuildArenaSimpleDeployArg &roArg, GuildArenaSimpleDeployRes &roRes)
{
	/*
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		SSWarn<<"RpcC2G_GuildArenaSimpleDeploy:: pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuild == NULL)
	{
		SSWarn<<"RpcC2G_GuildArenaSimpleDeploy:: pGuild is null"<<END;
		return;
	}

	CGuildArenaTeam* pTeam = CGuildWarMgr::Instance()->GetTeam(pGuild->GetID());
	if (pTeam == NULL)
	{
		SSWarn<<"RpcC2G_GuildArenaSimpleDeploy:: pTeam is null"<<END;
		return;
	}

	pTeam->SimpleDeploy();


	std::map<UINT32, fightUnit> unitMap = pTeam->GetFigthRoleMap();
	foreach (i in unitMap)
	{
		KKSG::GuildDarenaUnit* pData = roRes.add_fightunit();
		pData->set_roleid(i->second.roleId);
		pData->set_state(i->second.state);
	}
	*/
}

void RpcC2M_GuildArenaSimpleDeployNew::OnDelayReplyRpc(const GuildArenaSimpleDeployArg &roArg, GuildArenaSimpleDeployRes &roRes, const CUserData &roUserData)
{
}
