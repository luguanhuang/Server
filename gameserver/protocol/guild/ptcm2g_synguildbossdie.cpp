#include "pch.h"
#include "guild/ptcm2g_synguildbossdie.h"
#include "guild/guildboss.h"
#include "foreach.h"

// generate by ProtoGen at date: 2017/1/14 11:44:15

void PtcM2G_SynGuildBossDie::Process(UINT32 dwConnID)
{
	const std::unordered_map<UINT64, vector<GuildBossInfo*> >& infoMap = GuildBossMgr::Instance()->GetInfoMap();
	auto iter = infoMap.find(m_Data.guildid());
	if (iter != infoMap.end())
	{
		foreach (i in iter->second)
		{
			if ((*i) != NULL)
			{
				(*i)->DoBossDie();
			}
		}
	}
}
