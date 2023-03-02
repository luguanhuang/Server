#include "pch.h"
#include "commondef.h"
#include "scene/scene.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "scene/scenemanager.h"
#include "table/globalconfig.h"
#include "table/PowerPointMgr.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "scene/sceneworldboss.h"
#include "worldboss/ptcg2c_worldbossguildaddattrsyncclientntf.h"
#include "worldboss/ptcm2g_worldbossguildaddattrsyncgsntf.h"

// generate by ProtoGen at date: 2017/4/10 14:54:22

void PtcM2G_WorldBossGuildAddAttrSyncGsNtf::Process(UINT32 dwConnID)
{
	SSInfo<<"sync attr, role:"<<m_Data.roleid()<<" guildid:"<<m_Data.guildid()<<" count:"<<m_Data.count()<<END;

	///> 通知个人
	if (0 != m_Data.roleid())
	{
		Role* role = RoleManager::Instance()->FindByRoleID(m_Data.roleid());
		if (NULL == role)
		{
			SSWarn<<"not find role:"<<m_Data.roleid()<<END;
			return;
		}
		Scene* scene = role->GetCurrScene();
		if (NULL != scene && KKSG::SCENE_WORLDBOSS == scene->GetSceneType())
		{
			PtcG2C_WorldBossGuildAddAttrSyncClientNtf ntf;
			ntf.m_Data.set_count(m_Data.count());
			role->Send(ntf);
		}
		return;
	}

	///> 通知所有场景
	GuildSimpleMgr::Instance()->SetGuildAttr(m_Data.guildid(), m_Data.count());

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
		if (NULL != handler)
		{
			std::list<Role*> roles = scene->GetAllRoles();
			for (auto j = roles.begin(); j != roles.end(); ++j)
			{
				Role* role = *j;
				CGuildSimple* guild = GuildSimpleMgr::Instance()->GetGuildByRoleId(role->GetID());
				if (NULL != guild && guild->GetGuildId() == m_Data.guildid())
				{
					// add attr
					for (auto i = GetGlobalConfig().WorldBossGuildAddAttr.begin(); i != GetGlobalConfig().WorldBossGuildAddAttr.end(); ++i)
					{
						//float coef = m_Data.count() >= (UINT32)GetGlobalConfig().WorldBossAttrCount ? 5.5f : 3.0f;
						float coef = 1.0f;
						PowerPointMgr::Instance()->AdjustOneAttributeByCoeffcient(role, i->seq[0], (float)(i->seq[1]*coef)/100.0f);
					}
					// notify client
					PtcG2C_WorldBossGuildAddAttrSyncClientNtf ntf;
					ntf.m_Data.set_count(m_Data.count());
					role->Send(ntf);

					SSInfo<<"guild:"<<guild->GetGuildId()<<" roleid:"<<role->GetID()<<" count:"<<m_Data.count()<<END;
				}
			}
		}
	}
}
