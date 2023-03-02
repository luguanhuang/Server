#include "pch.h"
#include "guild/ptcc2g_syndoingguildinherit.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "scene/scene.h"
#include "guild/ptcg2c_noticedoingguildinherit.h"
#include "foreach.h"

// generate by ProtoGen at date: 2016/11/18 21:07:38

void PtcC2G_SynDoingGuildInherit::Process(UINT32 dwConnID)
{
	Role* pRoleOne = RoleManager::Instance()->FindByRoleID(m_Data.roleone());
	Role* pRoleTwo = RoleManager::Instance()->FindByRoleID(m_Data.roletwo());

	if (pRoleOne == NULL || pRoleTwo == NULL)
	{
		return ;
	}

	Scene* pSceneOne = pRoleOne->GetCurrScene();
	Scene* pSceneTwo = pRoleTwo->GetCurrScene();
	if (pSceneOne == NULL || pSceneTwo == NULL || pSceneOne != pSceneTwo)
	{
		return ;
	}
	
	PtcG2C_NoticeDoingGuildInherit msg;
	msg.m_Data.set_roleone(m_Data.roleone());
	msg.m_Data.set_roletwo(m_Data.roletwo());
	msg.m_Data.set_type(m_Data.type());

	pSceneOne->Broadcast(msg);
}
