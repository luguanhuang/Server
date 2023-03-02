#include "pch.h"
#include "guild/rpcc2m_tryalliance.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guildterritorybattle.h"

// generate by ProtoGen at date: 2016/12/20 11:19:14

RPC_SERVER_IMPLEMETION(RpcC2M_TryAlliance, TryAllianceArg, TryAlliance)

void RpcC2M_TryAlliance::OnCall(const TryAllianceArg &roArg, TryAlliance &roRes)
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

	UINT32 cityId = CGuildTerritoryBattle::Instance()->GetDeclareCityId(pGuild->GetID());
	if (cityId == 0)
	{
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return ;
	}

	roRes.set_errorcode((KKSG::ErrorCode)CGuildTerritoryBattle::Instance()->ReqAlliance(pRole->GetID(), roArg.guild(), cityId));
}

void RpcC2M_TryAlliance::OnDelayReplyRpc(const TryAllianceArg &roArg, TryAlliance &roRes, const CUserData &roUserData)
{
}
