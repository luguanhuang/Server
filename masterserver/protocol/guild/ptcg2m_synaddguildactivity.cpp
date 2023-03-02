#include "pch.h"
#include "guild/ptcg2m_synaddguildactivity.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildactivity.h"

// generate by ProtoGen at date: 2016/9/16 17:12:09

void PtcG2M_SynAddGuildActivity::Process(UINT32 dwConnID)
{
	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(m_Data.roleid());
	if (pGuild == NULL)
	{
		return ;
	}

	pGuild->Get<CGuildActivity>()->AddActivity(m_Data.roleid(), m_Data.guildactivitynum());
}
