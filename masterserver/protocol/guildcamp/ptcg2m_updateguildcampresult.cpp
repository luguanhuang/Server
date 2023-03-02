#include "pch.h"
#include "guildcamp/ptcg2m_updateguildcampresult.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "guild/guildcamp.h"


// generate by ProtoGen at date: 2016/11/16 14:34:11

void PtcG2M_UpdateGuildCampResult::Process(UINT32 dwConnID)
{
	Guild* pGuild = CGuildMgr::Instance()->GetGuild(m_Data.guildid());
	if (pGuild)
	{
		pGuild->Get<CGuildCamp>()->UpdateRankInfo(m_Data);
	}
}
