#include "pch.h"
#include "guild/rpcc2g_reqguildbosstimeleft.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildboss.h"

// generate by ProtoGen at date: 2016/10/8 15:31:27

RPC_SERVER_IMPLEMETION(RpcC2G_ReqGuildBossTimeLeft, getguildbosstimeleftArg, getguildbosstimeleftRes)

void RpcC2G_ReqGuildBossTimeLeft::OnCall(const getguildbosstimeleftArg &roArg, getguildbosstimeleftRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		SSWarn << "role is NULL and session is :" << m_sessionID <<END;
		return;
	}

	roRes.set_timeleft(GuildBossMgr::Instance()->GetEndTimeLeft());
	roRes.set_addattrcount(GuildBossMgr::Instance()->GetAttrCount(GuildSimpleMgr::Instance()->GetGuildId(role->GetID())));
}

void RpcC2G_ReqGuildBossTimeLeft::OnDelayReplyRpc(const getguildbosstimeleftArg &roArg, getguildbosstimeleftRes &roRes, const CUserData &roUserData)
{
}
