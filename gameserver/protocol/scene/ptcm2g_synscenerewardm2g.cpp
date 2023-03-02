#include "pch.h"
#include "scene/ptcm2g_synscenerewardm2g.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"

// generate by ProtoGen at date: 2016/8/5 19:32:09

void PtcM2G_SynSceneRewardM2G::Process(UINT32 dwConnID)
{
	/*for (UINT32 i = 0; i < m_Data.guildexps_size(); ++i)
	{
		const KKSG::GuildExpData& data = m_Data.guildexps(i);
		Guild* pGuild = GuildMgr::Instance()->FindGuild(data.guildid());
		if (pGuild != NULL)
		{
			pGuild->AddExp(data.guildexp());
		}
	}

	std::set<UINT64> roleSet;
	for (UINT32 j = 0; j < m_Data.guildcons_size(); ++j)
	{
		const KKSG::GuildConData& data = m_Data.guildcons(j);
		Guild* pGuild = GuildMgr::Instance()->FindGuildByRoleId(data.roleid());
		if (pGuild != NULL)
		{
			pGuild->AddRoleContribute(data.roleid(), data.guildcon());
			roleSet.insert(data.roleid());
		}
	}

	GuildMgr::Instance()->DoGuildDareReward(roleSet, m_Data.scenename());*/
}
