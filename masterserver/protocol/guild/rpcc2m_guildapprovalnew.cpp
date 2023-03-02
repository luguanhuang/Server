#include "pch.h"
#include "guild/rpcc2m_guildapprovalnew.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "table/guildConfig.h"
#include "guild/guildapps.h"

// generate by ProtoGen at date: 2016/9/15 21:03:11

RPC_SERVER_IMPLEMETION(RpcC2M_GuildApprovalNew, GuildApprovalArg, GuildApprovalRes)

void RpcC2M_GuildApprovalNew::OnCall(const GuildApprovalArg &roArg, GuildApprovalRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_result(ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	UINT64 gid = CGuildMgr::Instance()->GetRoleGuild(pRole->GetID());
	if (gid == INVALID_GUILDID)
	{
		return;
	}

	Guild *pGuild = CGuildMgr::Instance()->GetGuild(gid);
	if (pGuild == NULL)
	{
		roRes.set_result(KKSG::ERR_GUILD_NOT_EXIST);
		return;
	}

	GuildMember *pMember = pGuild->GetMember(pRole->GetID());
	if (!CGuildConfig::Instance()->HasPermission(pMember->position, GPEM_APPROVAL))
	{
		roRes.set_result(KKSG::ERR_GUILD_NO_PERMISSION);
		return;
	}

	if (roArg.type() == 0)
	{
		if (!pGuild->Get<GuildApplication>()->Accept(roArg.roleid()))
		{
			roRes.set_result(ERR_GUILD_FULL);
			return;
		}
	}
	else if (roArg.type() == 1)
	{
		pGuild->Get<GuildApplication>()->Reject(roArg.roleid());
	}
	else
	{
		pGuild->Get<GuildApplication>()->RejectAll();
	}
	roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcC2M_GuildApprovalNew::OnDelayReplyRpc(const GuildApprovalArg &roArg, GuildApprovalRes &roRes, const CUserData &roUserData)
{
}
