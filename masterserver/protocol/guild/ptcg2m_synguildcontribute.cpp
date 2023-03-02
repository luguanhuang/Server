#include "pch.h"
#include "guild/ptcg2m_synguildcontribute.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"

// generate by ProtoGen at date: 2016/9/24 17:05:29

void PtcG2M_SynGuildContribute::Process(UINT32 dwConnID)
{
	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(m_Data.roleid());
	if (pGuild == NULL)
	{
		return ;
	}

	pGuild->AddRoleContribute(m_Data.roleid(), m_Data.num());
}
