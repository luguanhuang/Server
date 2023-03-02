#include "pch.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guildcamp/rpcc2m_guildcampinfo.h"
#include "guild/guildcamp.h"
#include "guild/guildcamprank.h"

// generate by ProtoGen at date: 2016/11/5 14:51:44

RPC_SERVER_IMPLEMETION(RpcC2M_GuildCampInfo, GuildCampInfoArg, GuildCampInfoRes)

void RpcC2M_GuildCampInfo::OnCall(const GuildCampInfoArg &roArg, GuildCampInfoRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (!role)
	{
		return;
	}
	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
	if(!pGuild)
	{
		return;
	}

	CGuildCamp* record = pGuild->Get<CGuildCamp>();
	record->FillGuildCampInfo(roRes);
}

void RpcC2M_GuildCampInfo::OnDelayReplyRpc(const GuildCampInfoArg &roArg, GuildCampInfoRes &roRes, const CUserData &roUserData)
{
}
