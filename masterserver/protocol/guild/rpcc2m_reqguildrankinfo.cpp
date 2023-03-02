#include "pch.h"
#include "guild/rpcc2m_reqguildrankinfo.h"
#include "config.h"
#include "table/globalconfig.h"
#include "guild/guildmgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "../share/util/XCommon.h"
#include <time.h>

// generate by ProtoGen at date: 2016/10/10 17:23:35

RPC_SERVER_IMPLEMETION(RpcC2M_ReqGuildRankInfo, ReqGuildRankInfoArg, ReqGuildRankInfoRes)

void RpcC2M_ReqGuildRankInfo::OnCall(const ReqGuildRankInfoArg &roArg, ReqGuildRankInfoRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		return ;
	}

	UINT64 guildId = CGuildMgr::Instance()->GetGuildIdByRoleId(pRole->GetID());
	UINT32 endTime = XCommon::GetDayBeginTime(MSConfig::Instance()->GetGameServerOpenTimeStamp()) + (GetGlobalConfig().GuildRankRewardTime) * (24*60*60);
	UINT32 keepTime = XCommon::GetDayBeginTime(MSConfig::Instance()->GetGameServerOpenTimeStamp()) + (GetGlobalConfig().GuildRankRewardTime - GetGlobalConfig().GuildRankRewardKeepTime) * (24*60*60);
	UINT32 rank = CGuildMgr::Instance()->GetRankNum(GuildSortByPrestige, guildId);
	roRes.set_endtime(endTime);
	roRes.set_keeptime(keepTime);
	roRes.set_rank(rank);
	roRes.set_nowtime(time(NULL));
}

void RpcC2M_ReqGuildRankInfo::OnDelayReplyRpc(const ReqGuildRankInfoArg &roArg, ReqGuildRankInfoRes &roRes, const CUserData &roUserData)
{
}
