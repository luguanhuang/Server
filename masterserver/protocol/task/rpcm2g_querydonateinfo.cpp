#include "pch.h"
#include "task/rpcm2g_querydonateinfo.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildactivity.h"
#include "guild/guilddonate.h"
#include "task/rpcm2g_costdonateitem.h"
#include "mail/mailconfig.h"
#include "mail/maildata.h"
#include "mail/mailmgr.h"
#include "util/XCommon.h"
#include "table/ItemConfigMgr.h"

// generate by ProtoGen at date: 2016/11/16 19:01:33

RPC_CLIENT_IMPLEMETION(RpcM2G_QueryDonateInfo, QueryDonateInfoArg, QueryDonateInfoRes)

void RpcM2G_QueryDonateInfo::OnReply(const QueryDonateInfoArg &roArg, const QueryDonateInfoRes &roRes, const CUserData &roUserData)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		return;
	}
	Guild *pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuild == NULL)
	{
		return;
	}

	GuildDonate* donate = pGuild->Get<GuildDonate>();
	donate->UpdateDonateInfo(pRole->GetID(), roRes.todaydonatecount(), roRes.totaldonatecount());
}

void RpcM2G_QueryDonateInfo::OnTimeout(const QueryDonateInfoArg &roArg, const CUserData &roUserData)
{
}
