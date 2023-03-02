#include "pch.h"
#include "chat/ptcc2g_joinroom.h"
#include "unit/rolemanager.h"
#include "apollovoice/ApolloVoiceMgr.h"

// generate by ProtoGen at date: 2016/8/9 15:06:01

void PtcC2G_JoinRoom::Process(UINT32 dwConnID)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if(!pRole)
	{
		SSError << "Cannot Find Role, SessionID:" << m_sessionID << END; 
		return; 
	}
	SSWarn << "Handle C2G Message, cmd=PtcC2G_JoinRoom, RoleID:" << pRole->GetID() << ", Name:" << pRole->GetName() << ", ClientIP:" << pRole->GetClientConf().ip() << END;
	ApolloVoiceMgr::Instance()->HandleJoinRoom(pRole);
}
