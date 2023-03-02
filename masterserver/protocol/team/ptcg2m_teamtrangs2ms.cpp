#include "pch.h"
#include "team/ptcg2m_teamtrangs2ms.h"
#include "define/teamdef.h"
#include "team/teamtranshandler.h"
#include "network/teamlink.h"
#include "team/ptcm2h_teamtranms2ts.h"

// generate by ProtoGen at date: 2017/7/4 11:07:54

void PtcG2M_TeamTranGs2Ms::Process(UINT32 dwConnID)
{
	if(!TeamLink::Instance()->IsConnected())
	{
		SSError << " ts is disconnected " << END;
		return ;
	}
	//默认转发
	PtcM2H_TeamTranMs2Ts ptc;
	ptc.m_Data = m_Data;
	TeamLink::Instance()->SendTo(ptc);
}
