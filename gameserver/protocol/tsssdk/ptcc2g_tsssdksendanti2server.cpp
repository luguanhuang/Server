#include "pch.h"
#include "tsssdk/ptcc2g_tsssdksendanti2server.h"
#include "unit/rolemanager.h"
#include "tsssdk/TssHandler.h"

// generate by ProtoGen at date: 2016/6/22 13:22:34

void PtcC2G_TssSdkSendAnti2Server::Process(UINT32 dwConnID)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		return;
	}
	TssHandler::Instance()->on_recv_anti_data(pRole, (unsigned char*)m_Data.anti_data().c_str(), m_Data.anti_data().length());
}
