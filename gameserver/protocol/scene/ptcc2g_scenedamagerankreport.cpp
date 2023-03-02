#include "pch.h"
#include "scene/ptcc2g_scenedamagerankreport.h"
#include "scene/scenemanager.h"
#include "scene/scene.h"
#include "unit/rolemanager.h"
#include "gamelogic/team.h"
#include "unit/dummyrole.h"
#include "scene/ptcg2c_scenedamagerankntf.h"
#include "scene/sceneskycity.h"
#include "scene/scenereswar.h"

// generate by ProtoGen at date: 2015/12/24 17:43:22

void PtcC2G_SceneDamageRankReport::Process(UINT32 dwConnID)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		return;
	}
	Scene* scene = role->GetCurrScene();
	if (NULL == scene)
	{
		return;
	}

	PtcG2C_SceneDamageRankNtf ntf;
	if(scene->GetSceneTeam()||scene->GetSceneResWarPVE())
	{
		const std::list<Role*>& roles = scene->GetAllRoles();
		for (auto i = roles.begin(); i != roles.end(); ++i)
		{
			Role* temprole = *i;
			ntf.m_Data.add_roleid(temprole->GetID());
			ntf.m_Data.add_name(temprole->GetName());
			ntf.m_Data.add_damage((float)temprole->GetStatistics().GetTotalDamage());
		}
		std::vector<DummyRole*> dummyroles;
		scene->GetDummyRoles(dummyroles);
		for (auto i = dummyroles.begin(); i != dummyroles.end(); ++i)
		{
			DummyRole* dummyrole = *i;
			ntf.m_Data.add_roleid(dummyrole->GetID());
			ntf.m_Data.add_name(dummyrole->GetName());
			ntf.m_Data.add_damage((float)dummyrole->GetStatistics().GetTotalDamage());
		}
	}
	else if(scene->GetSkyCityHandler())
	{
		std::vector<Role*> roles;
		scene->GetSkyCityHandler()->GetSameGroupRoles(role, roles);
		for (auto i = roles.begin(); i != roles.end(); ++i)
		{
			Role* temprole = *i;
			ntf.m_Data.add_roleid(temprole->GetID());
			ntf.m_Data.add_name(temprole->GetName());
			ntf.m_Data.add_damage((float)temprole->GetStatistics().GetTotalDamage());
		}
	}else if(scene->GetResWarHandler())
	{
		std::vector<Role*> roles;
		scene->GetResWarHandler()->GetSameGroupRoles(role, roles);
		for (auto i = roles.begin(); i != roles.end(); ++i)
		{
			Role* temprole = *i;
			ntf.m_Data.add_roleid(temprole->GetID());
			ntf.m_Data.add_name(temprole->GetName());
			ntf.m_Data.add_damage((float)temprole->GetStatistics().GetTotalDamage());
		}
	}
	role->Send(ntf);
}
