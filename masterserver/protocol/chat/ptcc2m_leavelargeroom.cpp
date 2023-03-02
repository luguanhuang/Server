#include "pch.h"
#include "chat/ptcc2m_leavelargeroom.h"
#include "chat/fmmgr.h"
#include "role/rolemanager.h"
#include "role/role.h"

// generate by ProtoGen at date: 2016/11/24 20:16:48

void PtcC2M_LeaveLargeRoom::Process(UINT32 dwConnID)
{
	CRole* poRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(NULL == poRole)
	{
		SSError << "Cannot Find Role, SessionID:" << m_sessionID << END;
		return;
	}
	FMMgr::Instance()->DelRole(poRole->GetID());
	FMMgr::Instance()->LeaveRoom(poRole->GetAccount(), poRole->GetID());
}
