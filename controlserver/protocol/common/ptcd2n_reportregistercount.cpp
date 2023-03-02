#include "pch.h"
#include "common/ptcd2n_reportregistercount.h"
#include "account/accountsessionmgr.h"

// generate by ProtoGen at date: 2017/3/7 17:25:53

void PtcD2N_ReportRegisterCount::Process(UINT32 dwConnID)
{
	CAccountSessionMgr::Instance()->SetRegisterAccount(m_Data.register_account());
}
