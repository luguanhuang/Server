#include "pch.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "scene/scene.h"
#include "role/ptcc2g_queryrolestatereq.h"
#include "role/ptcg2c_queryrolestateack.h"

// generate by ProtoGen at date: 2016/12/28 11:10:02

void PtcC2G_QueryRoleStateReq::Process(UINT32 dwConnID)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		SSWarn<<"role is NULL, sessionid:"<<m_sessionID<<END;
		return;
	}
	Scene* scene = role->GetCurrScene();
	if (NULL == scene)
	{
		SSWarn<<"scene is NULL, roleid:"<<role->GetID()<<END;
		return;
	}

	const std::map<UINT64, Role*>& waitingroles = scene->GetWaitingRoles();

	PtcG2C_QueryRoleStateAck ack;
	for (int i = 0; i < m_Data.roleids_size(); ++i)
	{
		UINT64 roleid = m_Data.roleids(i);
		ack.m_Data.add_roleids(roleid);
		// in 
		if (NULL != scene->IsInScene(roleid))
		{
			ack.m_Data.add_state(1);
			break;
		}
		// loading
		else if (waitingroles.end() != waitingroles.find(roleid))
		{
			ack.m_Data.add_state(2);
			break;
		}
		else
		{
			ack.m_Data.add_state(3);
			break;
		}
	}
	role->Send(ack);
}
