#include "pch.h"
#include "guild/rpcc2m_getguildintegralinfo.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildarena.h"

// generate by ProtoGen at date: 2016/11/4 14:28:20

RPC_SERVER_IMPLEMETION(RpcC2M_GetGuildIntegralInfo, GetGuildIntegralInfoArg, GetGuildIntegralInfoRes)

void RpcC2M_GetGuildIntegralInfo::OnCall(const GetGuildIntegralInfoArg &roArg, GetGuildIntegralInfoRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		return ;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuild == NULL)
	{
		return ;
	}

	roRes.set_isapply(CGuildArenaIntegralMgr::Instance()->IsApplye(pGuild->GetID()));
	roRes.set_battletype(CGuildArenaIntegralMgr::Instance()->GetTimeType());
	roRes.set_applytime(CGuildArenaIntegralMgr::Instance()->GetApplyEndTime());
	roRes.set_curturn(CGuildArenaIntegralMgr::Instance()->GetCurTurnNum());
}

void RpcC2M_GetGuildIntegralInfo::OnDelayReplyRpc(const GetGuildIntegralInfoArg &roArg, GetGuildIntegralInfoRes &roRes, const CUserData &roUserData)
{
}
