#include "pch.h"
#include "guild/rpcc2m_changeguildsettingnew.h"
#include "guild/ptcm2c_guildsetingnotify.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "table/guildConfig.h"

// generate by ProtoGen at date: 2016/9/15 22:33:09

RPC_SERVER_IMPLEMETION(RpcC2M_ChangeGuildSettingNew, ChangeGuildSettingArg, ChangeGuildSettingRes)

void RpcC2M_ChangeGuildSettingNew::OnCall(const ChangeGuildSettingArg &roArg, ChangeGuildSettingRes &roRes)
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

	if (roArg.has_annoucement())
	{
		if (!CGuildConfig::Instance()->HasPermission(pMember->position, GPEM_ANNOUNCEMENT))
		{
			roRes.set_result(KKSG::ERR_GUILD_NO_PERMISSION);
			return;
		}

		pGuild->SetAnnoucement(roArg.annoucement());
	}
	else
	{
		if (!CGuildConfig::Instance()->HasPermission(pMember->position, GPEM_SETTINGS))
		{
			roRes.set_result(KKSG::ERR_GUILD_NO_PERMISSION);
			return;
		}

		if (roArg.has_icon())
		{
			pGuild->SetIcon(roArg.icon());
		}

		if (roArg.has_powerpoint())
		{
			pGuild->SetRecruitPowerPoint(roArg.powerpoint());
		}

		if (roArg.has_needapproval())
		{
			pGuild->SetNeedApproval(roArg.needapproval());
		}
	}
	PtcM2C_GuildSetingNotify ntf;
	ntf.m_Data.set_icon(pGuild->GetIcon());
	ntf.m_Data.set_annoucement(pGuild->GetAnnoucement());
	ntf.m_Data.set_recuitppt(pGuild->GetRecuritPPT());
	ntf.m_Data.set_needapproval(pGuild->GetNeedApproval());
	pGuild->BroadCast(ntf);
	roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcC2M_ChangeGuildSettingNew::OnDelayReplyRpc(const ChangeGuildSettingArg &roArg, ChangeGuildSettingRes &roRes, const CUserData &roUserData)
{
}
