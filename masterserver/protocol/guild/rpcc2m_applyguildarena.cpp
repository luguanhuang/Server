#include "pch.h"
#include "guild/rpcc2m_applyguildarena.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildarena.h"
#include "loghelper/tlogr.h"
#include "define/tlogenum.h"

// generate by ProtoGen at date: 2016/11/3 19:36:08

RPC_SERVER_IMPLEMETION(RpcC2M_applyguildarena, applyguildarenaarg, applyguildarenares)

void RpcC2M_applyguildarena::OnCall(const applyguildarenaarg &roArg, applyguildarenares &roRes)
{
	if (!CGuildArenaIntegralMgr::Instance()->IsApplyTime())
	{
		return ;
	}

	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(ERR_UNKNOWN);
		return ;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuild == NULL)
	{
		roRes.set_errorcode(ERR_UNKNOWN);
		return ;
	}

	if (!pGuild->isLeader(pRole->GetID()) && !pGuild->isViceLeader(pRole->GetID()))
	{
		roRes.set_errorcode(ERR_UNKNOWN);
		return ;
	}

	if (pGuild->GetLevel() < GetGlobalConfig().GuildArenaSignupLevel)
	{
		roRes.set_errorcode(ERR_UNKNOWN);
		return ;
	}

	CGuildArenaIntegralMgr::Instance()->Apply(pGuild->GetID());
	roRes.set_errorcode(ERR_SUCCESS);

	TGuildFlowLog oLog(TLogMgr::Instance()->GetTagString());
	pGuild->DoTxLog(oLog);
	oLog.m_iActType = TX_GUILD_ACT_TYPE_10;
	oLog.m_uRoleID = pRole->GetID();
	oLog.Do();
	return ;
}

void RpcC2M_applyguildarena::OnDelayReplyRpc(const applyguildarenaarg &roArg, applyguildarenares &roRes, const CUserData &roUserData)
{
}
