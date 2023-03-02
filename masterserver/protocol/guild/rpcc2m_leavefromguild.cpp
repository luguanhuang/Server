#include "pch.h"
#include "guild/rpcc2m_leavefromguild.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildarena.h"
#include "table/globalconfig.h"
#include "guild/guildterritorybattle.h"
#include "loghelper/tlogr.h"
#include "define/tlogenum.h"
#include "reswar/reswarmgr.h"

// generate by ProtoGen at date: 2016/8/27 10:34:46

RPC_SERVER_IMPLEMETION(RpcC2M_LeaveFromGuild, LeaveGuildArg, LeaveGuildRes)

void RpcC2M_LeaveFromGuild::OnCall(const LeaveGuildArg &roArg, LeaveGuildRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	if (roArg.has_roleid() && roArg.roleid() != pRole->GetID())
	{
		Guild *pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
		if (pGuild == NULL)
		{
			roRes.set_result(KKSG::ERR_GUILD_NOT_EXIST);
			return;
		}

		GuildMember *pMember = pGuild->GetMember(pRole->GetID());
		GuildMember *pOther = pGuild->GetMember(roArg.roleid());
		if (pMember == NULL||pOther == NULL)
		{
			roRes.set_result(KKSG::ERR_GUILD_NOT_IN_GUILD);
			return;
		}

		if (CGuildWarMgr::Instance()->IsGuildWaring(pGuild->GetID())
			|| CGuildArenaIntegralMgr::Instance()->IsGuildWaring((pGuild->GetID())))
		{
			roRes.set_result(KKSG::CanNotDelInGuildArena);
			return;
		}

		if (CGuildTerritoryBattle::Instance()->IsInWaring(pGuild->GetID()))
		{
			roRes.set_result(KKSG::ERR_CAN_NOT_DEL_IN_GUILD_TERRITOYR);
			return;
		}

		if(ResWarMgr::Instance()->IsInResWar(pGuild->GetID()))
		{
			roRes.set_result(KKSG::ERR_TEAM_STATE_ERROR);
			return;
		}

		if (pMember->position > pOther->position)
		{
			roRes.set_result(KKSG::ERR_GUILD_NO_PERMISSION);
			return;
		}

		if (!CGuildConfig::Instance()->HasPermission(pMember->position, GPEM_FIREMEMBER))
		{
			roRes.set_result(KKSG::ERR_GUILD_NO_PERMISSION);
			return;
		}

		pGuild->RemoveMember(roArg.roleid());
		pGuild->DelExp(GetGlobalConfig().DelGuildExp);
		pGuild->DelPrestige(GetGlobalConfig().DelGuildPre);

		roRes.set_result(KKSG::ERR_SUCCESS);

		//do log
		TGuildFlowLog oLog(TLogMgr::Instance()->GetTagString());
		pGuild->DoTxLog(oLog);
		oLog.m_iActType = TX_GUILD_ACT_TYPE_11;
		oLog.m_uRoleID = roArg.roleid();
		oLog.m_RoleIDT = pRole->GetID();
		oLog.Do();
	}
	else
	{
		Guild *pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
		if (pGuild == NULL)
		{
			roRes.set_result(KKSG::ERR_GUILD_NOT_EXIST);
			return;
		}

		if ((CGuildWarMgr::Instance()->IsGuildWaring(pGuild->GetID())
			|| CGuildArenaIntegralMgr::Instance()->IsGuildWaring((pGuild->GetID())))
			&& pGuild->GetGuildMember().size() == 1)
		{
			roRes.set_result(KKSG::CanNotDelInGuildArena);
			return;
		}

		if (CGuildTerritoryBattle::Instance()->IsInWaring(pGuild->GetID()) && pGuild->GetGuildMember().size() == 1)
		{
			roRes.set_result(KKSG::ERR_CAN_NOT_DEL_IN_GUILD_TERRITOYR);
			return;
		}

		if(ResWarMgr::Instance()->IsInResWar(pGuild->GetID()))
		{
			roRes.set_result(KKSG::ERR_TEAM_STATE_ERROR);
			return;
		}


		GuildMember *pMember = pGuild->GetMember(roArg.roleid());
		if (pMember)
		{
			pGuild->RemoveMember(roArg.roleid());
			roRes.set_result(KKSG::ERR_SUCCESS);
		}
		else
		{
			roRes.set_result(KKSG::ERR_GUILD_NOT_IN_GUILD);
		}
	}
}

void RpcC2M_LeaveFromGuild::OnDelayReplyRpc(const LeaveGuildArg &roArg, LeaveGuildRes &roRes, const CUserData &roUserData)
{
}
