#include "pch.h"
#include "scene/ptcm2g_noticeguildbossendtogsn.h"
#include "scene/scenemanager.h"
#include "scene/scene.h"
#include "guild/ptcg2c_guildbosstimeout.h"
#include "unit/role.h"
#include "guild/guildboss.h"

#include "foreach.h"

// generate by ProtoGen at date: 2016/7/29 20:34:46

void PtcM2G_noticeguildbossendtogsn::Process(UINT32 dwConnID)
{
	const std::unordered_map<UINT64, vector<GuildBossInfo*> >& infoMap = GuildBossMgr::Instance()->GetInfoMap();
	foreach (i in infoMap)
	{
		foreach (j in i->second)
		{
			if ((*j) != NULL)
			{
				(*j)->SetEnd();
			}
		}
		/*
		if (i->second != NULL)
		{
			i->second->SetEnd();
		}
		*/
	}
}
