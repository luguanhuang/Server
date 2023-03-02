#include "pch.h"
#include "controllink.h"
#include "network.h"
#include "netproc.h"
#include "config.h"
#include "crpc.h"
#include "rslink.h"
#include "common/rpcm2n_registerms2ns.h"

INSTANCE_SINGLETON(ControlLink)


ControlLink::ControlLink()
:m_dwConnID(INVALID_CONN_ID)
{
}

ControlLink::~ControlLink()
{
}

void ControlLink::OnConnectSucceed(UINT32 dwConnID)
{
	if(m_dwConnID != INVALID_CONN_ID)
	{
		LogError("more than one ns is connected!");
		return;
	}

	SetConnID(dwConnID);
	LogInfo("connect to ns success");

	RSLink::Instance()->NotifyRouter2Ctrl();
	RpcM2N_registerms2ns* rpc = RpcM2N_registerms2ns::CreateRpc();
	SendTo(*rpc);
}

void ControlLink::OnConnectFailed()
{
	LogError("connect to ns failed!");
}

void ControlLink::OnClose(UINT32 dwConnID, int err)
{
	SetConnID(INVALID_CONN_ID);
	LogInfo("ns link was disconnected");
}

bool ControlLink::SendTo(const CProtocol& roPtc)
{
	if (m_dwConnID == INVALID_CONN_ID)
	{
		return false;
	}

	return Send(m_dwConnID, roPtc);
}

bool ControlLink::SendTo(CRpc &roRpc)
{
	if (m_dwConnID == INVALID_CONN_ID)
	{
		return false;
	}

	return Send(m_dwConnID, roRpc);
}

const std::string& ControlLink::GetName()
{
	static const std::string LinkName = "ctrllink";
	return LinkName;
}
