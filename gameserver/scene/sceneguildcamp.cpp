#include "pch.h"
#include "sceneguildcamp.h"
#include "unit/enemy.h"
#include "scene/scene.h"
#include "foreach.h"
#include "unit/role.h"
#include <vector>
#include "guild/guildconfig.h"
#include "network/mslink.h"
#include "unit/rolemanager.h"
#include "scene/sceneteam.h"
#include "guildcamp/ptcg2m_updateguildcampresult.h"

SceneGuildCamp::SceneGuildCamp(Scene* scene)
{
	m_pScene = scene;
	m_guildID = 0;
}

SceneGuildCamp* SceneGuildCamp::CreateSceneGuildCamp(Scene* scene)
{
	if (scene->GetSceneType() == KKSG::SCENE_GUILD_CAMP)
	{
		return new SceneGuildCamp(scene);
	}

	return NULL;
}

void SceneGuildCamp::OnEnemyDie(Scene* pScene, Enemy* enemy)
{

}

void SceneGuildCamp::OnBeginFinish(Scene* pScene)
{
	if (m_pScene->GetSceneType() != KKSG::SCENE_GUILD_CAMP)
	{
		return;
	}

	SceneTeam* pTeam = m_pScene->GetSceneTeam();
	if (!pTeam)
	{
		return;
	}

	auto tabledata = GuildConfig::Instance()->GetGuildCampData(pTeam->GetExpID());
	if (!tabledata)
	{
		return;
	}
	
	if (m_pScene->IsSceneWinState())
	{
		PtcG2M_UpdateGuildCampResult oPtc;
		KKSG::GuildCampResult* campresult = &oPtc.m_Data;

		const std::list<Role*>& vecRole = m_pScene->GetAllRoles();
		if (vecRole.empty())
		{
			return;
		}
		foreach(i in vecRole)
		{
			KKSG::RoleBriefInfo* brief = campresult->add_roles();
			brief->set_roleid((*i)->GetID());
			brief->set_name((*i)->GetName());
			brief->set_type((KKSG::RoleType)(*i)->GetProfession());
		}
		switch(tabledata->Type)
		{
		case GUILDCAMP_COSTTIME:
		case GUILDCAMP_LIVETIME:
			campresult->set_var(m_pScene->CostTime()/* + m_pScene->GetAddedTimeSpan()*/);
			break;
		case GUILDCAMP_KILLNUM:
			campresult->set_var(m_pScene->GetKillEnemyScore());
			break;
		}

		campresult->set_guildid(m_guildID);
		campresult->set_curcampid(pTeam->GetExpID());
		MSLink::Instance()->SendTo(oPtc);
	}
}

void SceneGuildCamp::OnEnterSceneFinally(Scene *pScene, Role* role)
{
	if (!role)
	{
		return;
	}
	if (!m_guildID)
	{
		m_guildID = role->getGuildId();
	}
}

void SceneGuildCamp::notifyAll()
{
}


void SceneGuildCamp::onEnterScene(Role* role)
{
	if (m_pScene->GetSceneType() != KKSG::SCENE_GUILD_CAMP)
	{
		return;
	}
}


