#include "pch.h"
#include "common/ptcr2m_crossgsclosentf2ms.h"
#include "role/rolemanager.h"
#include "live/livemanager.h"

// generate by ProtoGen at date: 2016/11/28 17:37:50

void PtcR2M_CrossGsCloseNtf2Ms::Process(UINT32 dwConnID)
{
	LogInfo("cross_gs_%u close", m_Data.gsline());
	CRoleManager::Instance()->LogoutRoleOnGsLine(m_Data.gsline(), true);
	
	LiveManager::Instance()->OnCrossGSClose(m_Data.gsline());
}
