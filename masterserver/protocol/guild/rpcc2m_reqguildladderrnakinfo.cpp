#include "pch.h"
#include "guild/rpcc2m_reqguildladderrnakinfo.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildladder.h"
#include "foreach.h"

// generate by ProtoGen at date: 2016/9/30 11:27:13

RPC_SERVER_IMPLEMETION(RpcC2M_ReqGuildLadderRnakInfo, ReqGuildLadderRnakInfoArg, ReqGuildLadderRnakInfoRes)

void RpcC2M_ReqGuildLadderRnakInfo::OnCall(const ReqGuildLadderRnakInfoArg &roArg, ReqGuildLadderRnakInfoRes &roRes)
{
	if (!CGuildLadder::Instance()->IsOpen())
	{
		roRes.set_errorcode(KKSG::ERR_GUILD_LADDER_NOT_OPEN);
		return ;
	}

	UINT64 guildId = CGuildMgr::Instance()->GetGuildIdByRoleId(roArg.roleid());
	GuildGroup* pData = CGuildLadder::Instance()->GetGuildGroup(guildId);
	if (pData != NULL)
	{
		for (std::multimap<UINT32, UINT64>::reverse_iterator i = pData->m_guildRank.rbegin(); i != pData->m_guildRank.rend(); ++i)
		{
			Guild* pGuild = CGuildMgr::Instance()->GetGuild(i->second);
			if (pGuild == NULL)
			{
				continue;
			}

			KKSG::GuildLadderRank* pRank = roRes.add_guildrank();
			pRank->set_guildid(pGuild->GetID());
			pRank->set_guildname(pGuild->GetName());
			pRank->set_icon(pGuild->GetIcon());
			pRank->set_wintimes(i->first);
		}
		roRes.set_lasttime(CGuildLadder::Instance()->GetLastTime());
		roRes.set_errorcode(KKSG::ERR_SUCCESS);
	}
	else
	{
		roRes.set_errorcode(KKSG::ERR_GUILD_LADDER_NOT_OPEN);
	}
}

void RpcC2M_ReqGuildLadderRnakInfo::OnDelayReplyRpc(const ReqGuildLadderRnakInfoArg &roArg, ReqGuildLadderRnakInfoRes &roRes, const CUserData &roUserData)
{
}
