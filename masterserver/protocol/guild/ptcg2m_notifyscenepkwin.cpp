#include "pch.h"
#include "guild/ptcg2m_notifyscenepkwin.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildladder.h"


// generate by ProtoGen at date: 2016/10/1 20:23:37

void PtcG2M_NotifyScenePkWin::Process(UINT32 dwConnID)
{
	UINT64 guildId = CGuildMgr::Instance()->GetGuildIdByRoleId(m_Data.roleid());
	GuildGroup* pGroup = CGuildLadder::Instance()->GetGuildGroup(guildId);
	if (pGroup != NULL)
	{
		pGroup->AddTime(m_Data.roleid());
	}
}
