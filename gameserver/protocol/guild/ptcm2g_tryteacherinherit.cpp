#include "pch.h"
#include "guild/ptcm2g_tryteacherinherit.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "guild/guildrecord.h"
#include "guild/guildinherit.h"
#include "scene/scene.h"
#include "scene/scenemanager.h"
#include "table/globalconfig.h"

// generate by ProtoGen at date: 2016/12/22 11:32:57

void PtcM2G_TryTeacherInherit::Process(UINT32 dwConnID)
{
	Role* pRoleOne = RoleManager::Instance()->FindByRoleID(m_Data.guildidone());
	Role* pRoleTwo = RoleManager::Instance()->FindByRoleID(m_Data.guildidtwo());
	if (pRoleOne == NULL || pRoleTwo == NULL)
	{
		return;
	}
		
	/*if (pRoleOne->Get<CGuildRecord>()->GetTeacherInheritTime() >= GetGlobalConfig().TeacherInheritTime 
		|| pRoleTwo->Get<CGuildRecord>()->GetTeacherInheritTime() >= GetGlobalConfig().TeacherInheritTime)
	{
		return ;
	}*/

	/*
	pRoleOne->Get<CGuildRecord>()->AddTeacherInheritTime();
	pRoleTwo->Get<CGuildRecord>()->AddTeacherInheritTime();
	*/
		
	if (NULL == pRoleOne->GetCurrScene())
	{
		return;
	}
	if (pRoleOne->GetCurrScene()->GetSceneTemplateID() == HALL_MAP_ID)
	{
		CGuildInherit::Instance()->Add(pRoleOne, CGuildInherit::TEACHER_INHERIT);
	}
	else
	{
		SceneManager::Instance()->ChangeScene(pRoleOne, HALL_MAP_ID);
	}
	if (NULL == pRoleTwo->GetCurrScene())
	{
		return;
	}
	if (pRoleTwo->GetCurrScene()->GetSceneTemplateID() == HALL_MAP_ID)
	{
		CGuildInherit::Instance()->Add(pRoleTwo, CGuildInherit::TEACHER_INHERIT);
	}
	else
	{
		SceneManager::Instance()->ChangeScene(pRoleTwo, HALL_MAP_ID);
	}

	CGuildInherit::Instance()->Push(pRoleOne->GetID(), pRoleTwo->GetID(), CGuildInherit::TEACHER_INHERIT);
	
}
