#include "pch.h"
#include "role/ptcm2g_roleeventsyncgs.h"
#include "unit/rolemanager.h"
#include "unit/role.h"

// generate by ProtoGen at date: 2016/10/14 16:11:45

void PtcM2G_RoleEventSyncGS::Process(UINT32 dwConnID)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(m_Data.roleid());
	if(NULL == pRole)
	{
		LogWarn("roleid:%llu can't find in gs", m_Data.roleid());
		return;
	}
	switch(m_Data.type())
	{
	case KKSG::OnSendFriendGift:
		{
			pRole->OnSendFriendGift();
			break;
		}
	default:
		{
			break;
		}
	}
}
