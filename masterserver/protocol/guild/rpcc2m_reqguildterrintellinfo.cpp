#include "pch.h"
#include "guild/rpcc2m_reqguildterrintellinfo.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "../share/util/gametime.h"
#include "../share/util/XCommon.h"
#include "guild/guildterritorybattle.h"
#include "role/role.h"
#include "role/rolemanager.h"

#include "foreach.h"

// generate by ProtoGen at date: 2017/2/8 20:47:23

RPC_SERVER_IMPLEMETION(RpcC2M_ReqGuildTerrIntellInfo, ReqGuildTerrIntellInfoArg, ReqGuildTerrIntellInfoRes)

void RpcC2M_ReqGuildTerrIntellInfo::OnCall(const ReqGuildTerrIntellInfoArg &roArg, ReqGuildTerrIntellInfoRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		SSWarn << "RpcC2M_ReqGuildTerrIntellInfo role is null !" << END;
		return ;
	}

	UINT64 guildId = CGuildMgr::Instance()->GetGuildIdByRoleId(pRole->GetID());

	std::map<UINT32, CityUnit>& guildTerrData = CGuildTerritoryBattle::Instance()->GetCityMap();
	foreach (i in guildTerrData)
	{
		if (i->second.m_guildId == guildId) // 自己的
		{
			Guild* pGuild = CGuildMgr::Instance()->GetGuild(i->second.m_guildId);
			if (pGuild != NULL)
			{
				KKSG::TerrData* pData = roRes.add_intellinfo();
				pData->set_id(i->first);
				pData->set_guildid(pGuild->GetID());
				pData->set_name(pGuild->GetName());
				pData->set_icon(pGuild->GetIcon());
				break;
			}
		}
	}

	for (std::map<UINT32, CityUnit>::reverse_iterator iter = guildTerrData.rbegin(); iter != guildTerrData.rend(); ++iter)
	{
		if (iter->second.m_guildId != guildId)
		{
			Guild* pGuild = CGuildMgr::Instance()->GetGuild(iter->second.m_guildId);
			if (pGuild != NULL)
			{
				KKSG::TerrData* pData = roRes.add_intellinfo();
				pData->set_id(iter->first);
				pData->set_guildid(pGuild->GetID());
				pData->set_name(pGuild->GetName());
				pData->set_icon(pGuild->GetIcon());
			}
		}
	}
}

void RpcC2M_ReqGuildTerrIntellInfo::OnDelayReplyRpc(const ReqGuildTerrIntellInfoArg &roArg, ReqGuildTerrIntellInfoRes &roRes, const CUserData &roUserData)
{
}
