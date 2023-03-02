#include "pch.h"
#include "guild/ptcm2g_noticeguildbossattr.h"
#include "guild/guildboss.h"
#include "scene/scene.h"
#include "scene/scenemanager.h"
#include "gamelogic/noticemgr.h"
#include "table/PowerPointMgr.h"
#include "guild/ptcg2c_notifyguildbossaddattr.h"
#include "foreach.h"

// generate by ProtoGen at date: 2017/1/15 15:42:35

void PtcM2G_NoticeGuildBossAttr::Process(UINT32 dwConnID)
{
	const std::unordered_map<UINT64, vector<GuildBossInfo*> >& infoMap = GuildBossMgr::Instance()->GetInfoMap();
	auto iter = infoMap.find(m_Data.guildid());
	if (iter != infoMap.end())
	{
		UINT32 dwCount =  m_Data.count();
		LogInfo("guildboss addbuff count guild=%llu buffcount=%u",m_Data.guildid(),dwCount);
	    GuildBossMgr::Instance()->SetAttrCount(m_Data.guildid(),dwCount); 
		foreach (i in iter->second)
		{
			if ((*i) != NULL)
			{
				Scene* pScene = SceneManager::Instance()->FindBySceneID((*i)->GetSceneId());
				if (pScene==NULL)
				{
					continue;
				}
				PtcG2C_NotifyGuildBossAddAttr oNotifyAddAttr;
				oNotifyAddAttr.m_Data.set_count(dwCount);

				const std::list<Role*>& roles = pScene->GetAllRoles();
				for (auto i = roles.begin(); i != roles.end(); ++i)
				{
					Role* role = *i;
					for (auto j =  GetGlobalConfig().GuildBossAddAttr.begin(); j !=  GetGlobalConfig().GuildBossAddAttr.end(); ++j)
					{
						PowerPointMgr::Instance()->AdjustOneAttributeByCoeffcient(role, j->seq[0], (float)(j->seq[1])/100.0f);
					}
					role->Send(oNotifyAddAttr);
				}	
			}
		}
	}
}
