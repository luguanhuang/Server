#include "pch.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "role/rolemanager.h"
#include "ranklist/ranklistmgr.h"
#include "worldboss/worldbossmgr.h"
#include "ranklist/worldbossguildranklist.h"
#include "worldboss/rpcc2m_getworldbosstimeleft.h"

// generate by ProtoGen at date: 2016/8/9 0:43:42

RPC_SERVER_IMPLEMETION(RpcC2M_GetWorldBossTimeLeft, GetWorldBossTimeLeftArg, GetWorldBossTimeLeftRes)

void RpcC2M_GetWorldBossTimeLeft::OnCall(const GetWorldBossTimeLeftArg &roArg, GetWorldBossTimeLeftRes &roRes)
{
	UINT32 roletimeleft = (UINT32)(-1);
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL != role)
	{
		roletimeleft = WorldBossMgr::Instance()->GetRoleEndTimeLeft(role->GetID());
	}

	UINT32 timeleft = WorldBossMgr::Instance()->GetEndTimeLeft();
	roRes.set_timeleft(timeleft > roletimeleft ? roletimeleft : timeleft);

	if (NULL != role)
	{
		Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
		if (NULL != guild)
		{
			WorldBossGuildRankList* ranklist = (WorldBossGuildRankList*)CRankListMgr::Instance()->GetRankList(WorldBossGuildRank);
			if (NULL != ranklist)
			{
				auto i = ranklist->m_guild2info.find(guild->GetID());
				if (i != ranklist->m_guild2info.end())
				{
					roRes.set_guildrolecount(i->second.size());
				}
			}
		}
	}
}

void RpcC2M_GetWorldBossTimeLeft::OnDelayReplyRpc(const GetWorldBossTimeLeftArg &roArg, GetWorldBossTimeLeftRes &roRes, const CUserData &roUserData)
{
}
