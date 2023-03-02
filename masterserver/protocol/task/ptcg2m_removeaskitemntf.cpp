#include "pch.h"
#include "task/ptcg2m_removeaskitemntf.h"
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


// generate by ProtoGen at date: 2016/12/6 10:50:43

void PtcG2M_RemoveAskItemNtf::Process(UINT32 dwConnID)
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
	donate->RemoveAskInfo(pRole->GetID(), m_Data.taskid());
}
