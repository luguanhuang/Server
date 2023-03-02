#include "pch.h"
#include "pay/ptcm2g_consumerebatentf.h"
#include "gamelogic/payconsumemgr.h"

// generate by ProtoGen at date: 2017/9/25 19:28:01

void PtcM2G_ConsumeRebateNtf::Process(UINT32 dwConnID)
{
	CPayConsumeMgr::Instance()->SetData(m_Data);
}
