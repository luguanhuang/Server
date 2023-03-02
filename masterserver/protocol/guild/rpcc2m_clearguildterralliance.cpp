#include "pch.h"
#include "guild/rpcc2m_clearguildterralliance.h"
#include "guild/guildterritorybattle.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "role/role.h"
#include "role/rolemanager.h"

// generate by ProtoGen at date: 2016/12/21 10:43:26

RPC_SERVER_IMPLEMETION(RpcC2M_ClearGuildTerrAlliance, ClearGuildTerrAllianceArg, ClearGuildTerrAllianceRes)

void RpcC2M_ClearGuildTerrAlliance::OnCall(const ClearGuildTerrAllianceArg &roArg, ClearGuildTerrAllianceRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSWarn << "role is NULL and session is :" << m_sessionID <<END;
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return ;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuild == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return ;
	}

	CGuildTerritoryBattle::Instance()->ClearAlliance(pGuild->GetID());
	roRes.set_errorcode(KKSG::ERR_SUCCESS);
}

void RpcC2M_ClearGuildTerrAlliance::OnDelayReplyRpc(const ClearGuildTerrAllianceArg &roArg, ClearGuildTerrAllianceRes &roRes, const CUserData &roUserData)
{
}
