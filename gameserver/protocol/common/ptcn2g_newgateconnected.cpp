#include "pch.h"
#include "common/ptcn2g_newgateconnected.h"
#include "reconnetmgr.h"
#include "network/gatelink.h"

// generate by ProtoGen at date: 2016/11/2 10:27:44

void PtcN2G_NewGateConnected::Process(UINT32 dwConnID)
{
	if(GatetLink::Instance()->IsLineConnected(m_Data.line()))
	{
		return;
	}
	if(GatetLink::Instance()->IsInSet(m_Data.line()))
	{
		return;
	}

	PeerInfo peer;
	peer.m_action = NA_CONNECT;
	peer.m_state = PS_CONNECT_FAILED;
	peer.m_address.m_ip = m_Data.addr().ip();
	peer.m_address.m_port = m_Data.addr().port();
	peer.m_handler = GatetLink::Instance()->GetName();
	ReconnectManager::Instance()->AddPeerInfo(peer);
	GatetLink::Instance()->AddToSet(m_Data.line());
}
