#include "pch.h"
#include "guild/rpcc2m_recalliance.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guildterritorybattle.h"

// generate by ProtoGen at date: 2016/12/20 11:11:05

RPC_SERVER_IMPLEMETION(RpcC2M_RecAlliance, RecAllianceArg, RecAllianceRes)

void RpcC2M_RecAlliance::OnCall(const RecAllianceArg &roArg, RecAllianceRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSWarn << "role is NULL and session is :" << m_sessionID <<END;
		roRes.set_allianceid(0);
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return ;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuild == NULL)
	{
		roRes.set_allianceid(0);
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return ;
	}

	UINT32 errorcode = CGuildTerritoryBattle::Instance()->RcvAlliance(pRole->GetID(), roArg.guildid());
	if (errorcode == KKSG::ERR_SUCCESS)
	{
		roRes.set_allianceid(roArg.guildid());
	}
	else
	{
		roRes.set_allianceid(0);
	}
	roRes.set_errorcode((KKSG::ErrorCode)errorcode);
}

void RpcC2M_RecAlliance::OnDelayReplyRpc(const RecAllianceArg &roArg, RecAllianceRes &roRes, const CUserData &roUserData)
{
}
