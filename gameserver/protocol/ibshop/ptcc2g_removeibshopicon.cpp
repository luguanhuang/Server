#include "pch.h"
#include "ibshop/ptcc2g_removeibshopicon.h"
#include "unit/rolemanager.h"
#include "gamelogic/ibshopmgr.h"

// generate by ProtoGen at date: 2016/9/7 21:20:42

void PtcC2G_RemoveIBShopIcon::Process(UINT32 dwConnID)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSWarn<<"role is NULL, session id:"<<m_sessionID<<END;
		return;
	}
	IBShopMgr::Instance()->ClearIconFlag(pRole);
}
