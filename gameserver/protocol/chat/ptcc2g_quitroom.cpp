#include "pch.h"
#include "chat/ptcc2g_quitroom.h"
#include "unit/rolemanager.h"
#include "apollovoice/ApolloVoiceMgr.h"

// generate by ProtoGen at date: 2016/8/9 15:08:03

void PtcC2G_QuitRoom::Process(UINT32 dwConnID)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}
	SSInfo << "Handle C2G Message, CMD=PtcC2G_QuitRoom, RoleID:" << pRole->GetID() << ", Name:" << pRole->GetName() << END; 
	ApolloVoiceMgr::Instance()->HandleExitRoom( pRole,  m_Data.roomid(), m_Data.memberid(), "ClientRequest" );
}
