#include "pch.h"
#include "guild/rpcc2m_getguildwagereward.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildwage.h"
// generate by ProtoGen at date: 2016/10/26 17:24:40

RPC_SERVER_IMPLEMETION(RpcC2M_GetGuildWageReward, GetGuildWageRewardArg, GetGuildWageReward)

void RpcC2M_GetGuildWageReward::OnCall(const GetGuildWageRewardArg &roArg, GetGuildWageReward &roRes)
{
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

		CGuildWage* pGuildWage = pGuild->Get<CGuildWage>();
		if (pGuildWage == NULL)
		{
			roRes.set_errorcode(ERR_UNKNOWN);
			return ;
		}

		if (pGuildWage->GetReward(pRole) == true)
		{
			roRes.set_errorcode(ERR_SUCCESS);
		}
		else
		{
			roRes.set_errorcode(ERR_UNKNOWN);
		}
}

void RpcC2M_GetGuildWageReward::OnDelayReplyRpc(const GetGuildWageRewardArg &roArg, GetGuildWageReward &roRes, const CUserData &roUserData)
{
}
