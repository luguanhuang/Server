#include "pch.h"
#include "guild/rpcc2m_reqguildterrchallinfo.h"
#include "guild/guildterritorybattle.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"

#include "foreach.h"

// generate by ProtoGen at date: 2016/12/16 20:22:56

RPC_SERVER_IMPLEMETION(RpcC2M_ReqGuildTerrChallInfo, ReqGuildTerrChallInfoArg, ReqGuildTerrChallInfoRes)

void RpcC2M_ReqGuildTerrChallInfo::OnCall(const ReqGuildTerrChallInfoArg &roArg, ReqGuildTerrChallInfoRes &roRes)
{
	CGuildTerritoryBattle::Instance()->ReSort(roArg.id());
	std::map<UINT32, std::set<TerritoryGuild> >& challMap = CGuildTerritoryBattle::Instance()->GetChallList();
	auto iter = challMap.find(roArg.id());
	if (iter != challMap.end())
	{
		foreach (i in iter->second)
		{
			Guild* pGuild = CGuildMgr::Instance()->GetGuild(i->m_guildId);
			if (pGuild != NULL)
			{
				KKSG::GuildTerrChallInfo* pChallInfo = roRes.add_challinfo();
				pChallInfo->set_guildid(i->m_guildId);
				pChallInfo->set_allianceid(i->m_allianceId);
				pChallInfo->set_guildname(pGuild->GetName());
				foreach (j in i->m_reqAllianceSet)
				{
					pChallInfo->add_tryallianceid(j->first);
				}
			}
		}
	}
	roRes.set_cdtime(CGuildTerritoryBattle::Instance()->GetCDTime());
}

void RpcC2M_ReqGuildTerrChallInfo::OnDelayReplyRpc(const ReqGuildTerrChallInfoArg &roArg, ReqGuildTerrChallInfoRes &roRes, const CUserData &roUserData)
{
}
