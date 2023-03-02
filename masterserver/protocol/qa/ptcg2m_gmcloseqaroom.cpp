#include "pch.h"
#include "qa/ptcg2m_gmcloseqaroom.h"
#include "qa/rpcg2m_createguildqaroom.h"
#include "qa/qamgr.h"
#include "role/rolemanager.h"
#include "role/role.h"

// generate by ProtoGen at date: 2016/10/9 19:47:35

void PtcG2M_GMCloseQARoom::Process(UINT32 dwConnID)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);	
	if(NULL == role)
	{
		SSWarn << "Role Not Found" << END;
		return ;
	} 
	SSInfo << "GM Close Room, roldId: " << role->GetID() << END;
	QARoom* room = QAMgr::Instance()->GetRoomByRoleId(role->GetID());
	if (!room)
	{
		return; 
	}
	room->SetRoomOver(false);
}
