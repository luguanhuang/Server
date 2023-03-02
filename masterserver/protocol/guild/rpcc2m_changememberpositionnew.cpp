#include "pch.h"
#include "guild/rpcc2m_changememberpositionnew.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "config.h"

// generate by ProtoGen at date: 2016/9/20 17:44:57

RPC_SERVER_IMPLEMETION(RpcC2M_ChangeMemberPositionNew, ChangeGuildPositionArg, ChangeGuildPositionRes)

	static GuildPermission ChangePositionNeedPermission(GuildPosition pos)
{
	if (pos == GPOS_LEADER) return GPEM_CHANGELEADER;
	if (pos == GPOS_VICELEADER) return GPEM_SET_VICELEADER;
	if (pos == GPOS_OFFICER) return GPEM_SET_OFFICER;
	if (pos == GPOS_ELITEMEMBER) return GPEM_SET_ELITEMEMBER;
	return GPEM_MAX;
}

void RpcC2M_ChangeMemberPositionNew::OnCall(const ChangeGuildPositionArg &roArg, ChangeGuildPositionRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
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
	GuildMember *pOther = pGuild->GetMember(roArg.roleid());

	if (pOther == NULL || pMember == NULL)
	{
		roRes.set_result(KKSG::ERR_GUILD_MEMBER_NOT_EXIST);
		return;
	}

	GuildPosition pos = (GuildPosition)roArg.position();
	if (pos == pOther->position)
	{
		roRes.set_result(KKSG::ERR_SUCCESS);
		return;
	}

	if (pos > pOther->position)
	{
		pos = pOther->position;
	}

	if (!CGuildConfig::Instance()->HasPermission(pMember->position, ChangePositionNeedPermission(pos)))
	{
		roRes.set_result(KKSG::ERR_GUILD_NO_PERMISSION);
		return;
	}

	int ret = pGuild->SetMemberPos(roArg.roleid(), (GuildPosition)roArg.position());
	roRes.set_result((KKSG::ErrorCode)ret);
}

void RpcC2M_ChangeMemberPositionNew::OnDelayReplyRpc(const ChangeGuildPositionArg &roArg, ChangeGuildPositionRes &roRes, const CUserData &roUserData)
{
}
