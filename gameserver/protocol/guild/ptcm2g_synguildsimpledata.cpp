#include "pch.h"
#include "guild/ptcm2g_synguildsimpledata.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "foreach.h"

// generate by ProtoGen at date: 2016/9/12 20:33:45

void PtcM2G_SynGuildSimpleData::Process(UINT32 dwConnID)
{

	foreach (i in m_Data.datas())
	{
		if (i->has_isdel() && i->isdel())
		{
			GuildSimpleMgr::Instance()->DeleteGuildSimple(i->guildid());
			continue;
		}

		CGuildSimple& guildSimple = GuildSimpleMgr::Instance()->AddGuildSimple(i->guildid());

		guildSimple.SetGuildId(i->guildid());

		if (i->has_guildlvl())
		{
			guildSimple.SetGuildLvl(i->guildlvl());
		}
		/*if (i->has_guildleaderid())
		{
			guildSimple.SetGuildLeaderId(i->guildleaderid());
		}*/
		if (i->has_guildname())
		{
			guildSimple.SetGuildName(i->guildname());
		}
		if (i->has_icon())
		{
			guildSimple.SetGuildIcon(i->icon());
		}

		for (UINT32 cnt = 0; cnt < i->addmembers_size(); ++cnt)
		{
			guildSimple.AddGuildMember(i->addmembers(cnt).roleid(), i->addmembers(cnt).position());
		}

		for (UINT32 cnt = 0; cnt < i->delmembers_size(); ++cnt)
		{
			guildSimple.DeleteMember(i->delmembers(cnt));
		}
	}

}
