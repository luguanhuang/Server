#include "pch.h"
#include "guild/rpcc2m_reqguildterrallianceinfo.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildterritorybattle.h"
#include <time.h>

#include "foreach.h"
// generate by ProtoGen at date: 2016/12/15 20:38:00

RPC_SERVER_IMPLEMETION(RpcC2M_ReqGuildTerrAllianceInfo, ReqGuildTerrAllianceInfoArg, ReqGuildTerrAllianceInfoRes)

void RpcC2M_ReqGuildTerrAllianceInfo::OnCall(const ReqGuildTerrAllianceInfoArg &roArg, ReqGuildTerrAllianceInfoRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSWarn << "role is NULL and session is :" << m_sessionID <<END;
		return ;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuild == NULL)
	{
		return ;
	}

	const TerritoryGuild* pData = CGuildTerritoryBattle::Instance()->GetUnit(pGuild->GetID());
	if (pData != NULL)
	{
		UINT32 nowTime = time(NULL);
		roRes.set_allianceid(pData->m_allianceId);
		foreach (i in pData->m_reqAllianceSet)
		{
			Guild* pGuild = CGuildMgr::Instance()->GetGuild(i->first);
			if (pGuild != NULL)
			{
				KKSG::GuildTerrAllianceInfo* pInfo = roRes.add_allianceinfo();
				pInfo->set_guildname(pGuild->GetName());
				pInfo->set_guildrolenum(pGuild->GetMemberCount());
				pInfo->set_guildlvl(pGuild->GetLevel());
				pInfo->set_time(nowTime - i->second);
				pInfo->set_guildid(pGuild->GetID());
			}
		}
	}
}

void RpcC2M_ReqGuildTerrAllianceInfo::OnDelayReplyRpc(const ReqGuildTerrAllianceInfoArg &roArg, ReqGuildTerrAllianceInfoRes &roRes, const CUserData &roUserData)
{
}
