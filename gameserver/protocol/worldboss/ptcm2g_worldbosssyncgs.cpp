#include "pch.h"
#include "scene/sceneworldboss.h"
#include "scene/scene.h"
#include "scene/scenemanager.h"
#include "guild/guildmgr.h"
#include "worldboss/ptcm2g_worldbosssyncgs.h"

// generate by ProtoGen at date: 2016/8/8 11:43:12

void PtcM2G_WorldBossSyncGS::Process(UINT32 dwConnID)
{
	SSInfo<<"sync gs state:"<<m_Data.state()<<END;
	if (eBegin == m_Data.state())
	{
		GuildSimpleMgr::Instance()->ClearGuildAttr();
	}

	std::vector<Scene*> scenes;
	SceneManager::Instance()->FindByTemplateID(WORLDBOSS_MAP_ID, scenes);
	if (scenes.empty())
	{
		return;
	}
	for (auto i = scenes.begin(); i != scenes.end(); ++i)
	{
		Scene* scene = *i;
		WorldBossHandler* handler = scene->GetWorldBossHandler();
		if (NULL == handler)
		{
			SSError<<"world boss scene handle empty"<<END;
			return;
		}
		if (m_Data.has_state())	
		{
			switch(m_Data.state())
			{
			case eGoing:
				{
					break;
				}
			case eWaitEnd:
				{
					handler->OnSceneTimeWaitEnd();
					break;
				}
			case eEnd:
				{
					handler->OnSceneTimeEnd();
					break;
				}
			case eDummy:
				{
					break;
				}
			default:
				break;
			}
		}
		else if (m_Data.has_scenedamage())
		{
			if (scene->GetSceneID() != m_Data.scenedamage().sceneid())
			{
				handler->SubHp(m_Data.scenedamage().damage());
			}
		}
	}
}
