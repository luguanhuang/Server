#include "pch.h"
#include "controllink.h"
#include "network.h"
#include "netproc.h"
#include "config.h"
#include "crpc.h"
#include "protocol.h"
#include "common/rpcg2n_registergs2ns.h"


INSTANCE_SINGLETON(ControlLink)


ControlLink::ControlLink()
:m_dwConnID(INVALID_CONN_ID)
,m_bRegistered(false)
{
}

ControlLink::~ControlLink()
{
}

void ControlLink::OnConnectSucceed(UINT32 dwConnID)
{
	SetConnID(dwConnID);

	RpcG2N_RegisterGs2Ns* rpc = RpcG2N_RegisterGs2Ns::CreateRpc();
	rpc->m_oArg.set_serverid(GSConfig::Instance()->GetServerID());
	rpc->m_oArg.set_line(GSConfig::Instance()->GetLine());
	SendTo(*rpc);
}

void ControlLink::OnConnectFailed()
{
	LogError("connect to ns failed!");
}

void ControlLink::OnClose(UINT32 dwConnID, int err)
{
	SetRegistered(false);
	SetConnID(INVALID_CONN_ID);
	LogInfo("ns link was disconnected, gameserver exit now");

	LogMgr::Instance()->Flush();
	exit(0);
}

const std::string & ControlLink::GetName()
{
	static const std::string LinkName = "ctrllink";
	return LinkName;
}

bool ControlLink::SendTo(const CProtocol& roPtc)
{
	if(m_dwConnID == INVALID_CONN_ID)
	{
		SSWarn << "Ns not connected, protocol type: " << roPtc.m_dwType << END;
		return false;
	}
	return Send(GetConnID(), roPtc);
}

bool ControlLink::SendTo(const CRpc &roRpc)
{
	if(m_dwConnID == INVALID_CONN_ID)
	{
		SSWarn << "Ns not connected, rpc type: " << roRpc.m_dwType << END;
		return false;
	}

	return Send(GetConnID(), const_cast<CRpc&>(roRpc));
}

void ControlLink::CloseNsLink()
{
	if(m_dwConnID == INVALID_CONN_ID)
	{
		return;
	}

	CNetProcessor::Instance()->Close(m_dwConnID);
}