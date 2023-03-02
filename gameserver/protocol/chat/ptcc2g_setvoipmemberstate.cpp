#include "pch.h"
#include "chat/ptcc2g_setvoipmemberstate.h"
#include "chat/ptcc2g_joinroom.h"
#include "unit/rolemanager.h"
#include "apollovoice/ApolloVoiceMgr.h"

// generate by ProtoGen at date: 2016/9/9 15:44:18

void PtcC2G_SetVoipMemberState::Process(UINT32 dwConnID)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (!pRole)
	{
		return;
	}
	ApolloVoiceMgr::Instance()->HandleSetState(pRole,m_Data.nstate());
}
