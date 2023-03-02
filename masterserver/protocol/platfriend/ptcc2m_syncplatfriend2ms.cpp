#include "pch.h"
#include "platfriend/ptcc2m_syncplatfriend2ms.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "platforminfo/platformfriendmgr.h"

// generate by ProtoGen at date: 2016/10/26 11:34:42

void PtcC2M_SyncPlatFriend2MS::Process(UINT32 dwConnID)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (!role)
	{
		return;
	}

	CPlatformFriendMgr::Instance()->SyncPlatFriendListFromClient(role, m_Data);
}
