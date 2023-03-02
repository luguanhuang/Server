#include "pch.h"
#include "common/ptcc2n_updatestartuptypentf.h"
#include "role/rolemanager.h"
#include "account/accountsessionmgr.h"
#include "common/ptcn2g_updatestartuptypen2gntf.h"
#include "role/rolemanager.h"

// generate by ProtoGen at date: 2016/12/21 21:29:01

void PtcC2N_UpdateStartUpTypeNtf::Process(UINT32 dwConnID)
{
	CAccountSession* pSession = CAccountSessionMgr::Instance()->GetBySession(m_sessionID);	
	if (pSession == NULL)
	{
		return;
	}

	pSession->GetCliConf().set_starttype(m_Data.type());
	pSession->UpdateClientConfToShm();

	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		return;
	}

	PtcN2G_UpdateStartUpTypeN2GNtf ntf;
	ntf.m_sessionID = m_sessionID;
	ntf.m_Data.CopyFrom(m_Data);
	pRole->SendMsgToGS(ntf);
}
