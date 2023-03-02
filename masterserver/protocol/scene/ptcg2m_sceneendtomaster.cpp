#include "pch.h"
#include "scene/ptcg2m_sceneendtomaster.h"
#include "commondef.h"
#include "network/gslink.h"
#include "friend/friendconfig.h"
#include "idutil.h"

// generate by ProtoGen at date: 2016/7/28 14:32:53

void PtcG2M_SceneEndToMaster::Process(UINT32 dwConnID)
{
	// friend degree
	std::vector<UINT64> roleids;
	for (int i = 0; i < m_Data.roles_size(); ++i)
	{
		UINT64 roleID = m_Data.roles(i);
		UINT32 serverID = GetServerIDByRoleID(roleID);
		if (MSConfig::Instance()->HasServerId(serverID))
		{
			roleids.push_back(roleID);
		}
	}
	UINT32 type = FriendConfig::Instance()->SceneToType(m_Data.scenetype(), m_Data.sceneid());
	FriendConfig::Instance()->AddDegree(type, roleids);	


	/*wyxflag 和战友通关龙穴*/

}
