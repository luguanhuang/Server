#include "pch.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "friend/ptcm2g_updatefriendongs.h"
#include "unit/roleview.h"
#include "spactivity/spactivityhandler.h"

// generate by ProtoGen at date: 2016/7/30 20:53:11

void PtcM2G_UpdateFriendOnGS::Process(UINT32 dwConnID)
{
	Role* role = RoleManager::Instance()->FindByRoleID(m_Data.roleid());
	if (NULL == role)
	{
		SSWarn<<"role is not in this gs, roleid:"<<m_Data.roleid()<<END;
		return;
	}
	if (m_Data.isadd())	
	{
		int count = 0;
		for (int i = 0; i < m_Data.friendid_size(); ++i)
		{
			role->AddFriend(m_Data.friendid(i));	
			role->GetRoleView()->Add(m_Data.friendid(i), View_Friend);
		}
	}
	else
	{
		for (int i = 0; i < m_Data.friendid_size(); ++i)
		{
			role->DelFriend(m_Data.friendid(i));	
			role->GetRoleView()->Del(m_Data.friendid(i), View_Friend);
		}
	}
	SpActivityHandler::GlobalHandler.OnFriendChange(role);
}
