#include "pch.h"
#include "role/ptcc2g_picurlntf.h"
#include "unit/rolemanager.h"
#include "unit/role.h"

// generate by ProtoGen at date: 2016/12/9 21:15:17

void PtcC2G_PicUrlNtf::Process(UINT32 dwConnID)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole)
	{
		pRole->SetPicUrl(m_Data.url());
	}
}
