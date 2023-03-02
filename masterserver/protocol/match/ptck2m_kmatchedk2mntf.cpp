#include "pch.h"
#include "match/ptck2m_kmatchedk2mntf.h"
#include "table/expeditionconfigmgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "other/matchhandler.h"
#include "scene/sceneswitch.h"
#include "match/matchmgr.h"

// generate by ProtoGen at date: 2017/5/5 21:21:03

void PtcK2M_KMatchedk2mNtf::Process(UINT32 dwConnID)
{
	UINT32 mapID = m_Data.mapid();//MatchMgr::Instance()->GetMapID(m_Data.type(), m_Data.kunit());
	MatchMgr::Instance()->MatchDone(m_Data.type(), m_Data.kunit());

	if(0 == mapID)
	{
		SSError << " mapid = 0 match type = " <<  m_Data.type() << END;
	}

	KKSG::SceneSwitchData data;
	for(int i = 0; i < m_Data.kunit().roles_size(); ++i)
	{
		UINT64 roleID = m_Data.kunit().roles(i).roleid();
		UINT32 serverID = m_Data.kunit().roles(i).serverid();
		SSInfo << " role = " << roleID << " server = " << serverID << END;
		if(serverID != MSConfig::Instance()->GetServerID())//其他服玩家
		{
			continue;
		}
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleID);
		if(pRole)
		{
			CSceneSwitch::EnterScene(pRole, m_Data.sceneid(), data, m_Data.gsline(), true, mapID);
		}
		else
		{
			SSWarn << " find role failed role = " << roleID << END;
		}
	}

}
