#include "pch.h"
#include "scene/ptcm2g_enterscenefromms.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "scene/sceneswitch.h"

// generate by ProtoGen at date: 2017/4/16 16:59:59

void PtcM2G_EnterSceneFromMs::Process(UINT32 dwConnID)
{
	Role* role = RoleManager::Instance()->FindByRoleID(m_Data.roleid());
	if (!role || !role->GetCurrScene())
	{
		return;
	}
	KKSG::SceneSwitchData oData;
	CSceneSwitch::EnterScene(role, m_Data.mapid(), m_Data.sceneid(), oData, m_Data.iscross(), m_Data.gsline());
}
