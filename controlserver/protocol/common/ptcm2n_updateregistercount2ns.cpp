#include "pch.h"
#include "common/ptcm2n_updateregistercount2ns.h"
#include "account/accountsessionmgr.h"

// generate by ProtoGen at date: 2016/12/23 11:14:20

void PtcM2N_UpdateRegisterCount2Ns::Process(UINT32 dwConnID)
{
	CAccountSessionMgr::Instance()->SetRegisterAccount(m_Data.register_account());
}
