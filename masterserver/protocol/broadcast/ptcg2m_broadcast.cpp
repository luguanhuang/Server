#include "pch.h"
#include "broadcast/ptcg2m_broadcast.h"
#include "broadcast/ptcm2t_broadcastm2t.h"
#include "network/gatelink.h"

// generate by ProtoGen at date: 2016/10/11 14:12:22

void PtcG2M_BroadCast::Process(UINT32 dwConnID)
{
	PtcM2T_BroadCastM2T ntf;
	ntf.m_Data.mutable_data()->CopyFrom(m_Data);
	switch(m_Data.broadcasttype())
	{
	case KKSG::BroadCastToAll:
		GateLink::Instance()->SendToAllLine(ntf);
		break;
	default:
		break;
	}
	
}
