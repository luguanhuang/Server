#include "pch.h"
#include "chat/ptcc2m_handlemicphone.h"
#include "chat/ptcm2f_fmshowntf.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "network/fmlink.h"
// generate by ProtoGen at date: 2016/11/29 16:36:19

void PtcC2M_HandleMicphone::Process(UINT32 dwConnID)
{
	CRole* poRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(NULL == poRole)
	{
		SSError << "Cannot Find Role, SessionID:" << m_sessionID << END;
		return;
	}
	PtcM2F_FMShowNtf ntf;
	if (m_Data.param())
		ntf.m_Data.set_roleid(poRole->GetID());
	else
		ntf.m_Data.set_roleid(0);
	ntf.m_Data.set_data(poRole->GetAccount());
	FMLink::Instance()->SendTo(ntf);
}
