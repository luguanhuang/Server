#include "pch.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "scene/scenecreate.h"
#include "scene/sceneswitch.h"
#include "scene/ptck2m_changesceneworldreq.h"

// generate by ProtoGen at date: 2017/4/21 15:17:15

void PtcK2M_ChangeSceneWorldReq::Process(UINT32 dwConnID)
{
	for (int i = 0; i < m_Data.roleid_size(); ++i)
	{
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(m_Data.roleid(i));
		if (NULL == pRole)
		{
			SSWarn<<"custombattle match ok, bu role not online, roleId:"<<m_Data.roleid(i)<<END;
			continue;
		}
		KKSG::SceneSwitchData data;
		CSceneSwitch::EnterScene(pRole, m_Data.sceneuid(), data, m_Data.gsline(), true, m_Data.type());

		SSInfo<<"world change scene, role:"<<pRole->GetID()<<" map id:"<<m_Data.type()<<" scene uid:"<<m_Data.sceneuid()<<" gsline:"<<m_Data.gsline()<<" iscross:"<<true<<END;
	}
}
