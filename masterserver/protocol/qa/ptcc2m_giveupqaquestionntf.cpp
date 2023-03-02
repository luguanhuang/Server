#include "pch.h"
#include "qa/ptcc2m_giveupqaquestionntf.h"
#include "qa/qamgr.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "qa/ptcm2g_giveanserqarewardntf.h"
#include "network/gslink.h"

// generate by ProtoGen at date: 2016/10/8 23:11:39

void PtcC2M_GiveUpQAQuestionNtf::Process(UINT32 dwConnID)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);	
	if(NULL == role)
	{
		return ;
	}
	QARoom* room = QAMgr::Instance()->GetRoomByRoleId(role->GetID());
	if (!room)
	{
		SSError << "Get Room Failed, RoleID:" << role->GetID() << END;
		return; 
	}
	room->GiveUp();
}
