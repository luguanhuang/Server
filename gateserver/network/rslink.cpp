#include "pch.h"
#include "rslink.h"
#include "config.h"
#include "common/rpct2r_registergt2router.h"
#include "reconnetmgr.h"

INSTANCE_SINGLETON(RSLink)

RSLink::RSLink()
{
	m_connId = INVALID_CONN_ID;
	m_routerPort = 0;
}

RSLink::~RSLink()
{
}

void RSLink::OnConnectSucceed(UINT32 dwConnID)
{
	m_connId = dwConnID;
	RpcT2R_RegisterGt2Router* rpc = RpcT2R_RegisterGt2Router::CreateRpc();
	rpc->m_oArg.set_server_id(GateConfig::Instance()->GetServerID());
	rpc->m_oArg.set_line(GateConfig::Instance()->GetLine());
	SendTo(*rpc);
}

void RSLink::OnConnectFailed()
{
	LogError("connected router failed!");
}

void RSLink::OnClose(UINT32 dwConnID, int err)
{
	LogInfo("router link was disconnected");
	m_connId = INVALID_CONN_ID;
}

const std::string& RSLink::GetName()
{
	static const std::string LinkName = "rslink";
	return LinkName;
}

bool RSLink::SendTo(const CProtocol& ptc)
{
	if (m_connId == INVALID_CONN_ID)
	{
		return false;
	}
	return Send(m_connId, ptc);
}

bool RSLink::SendTo(CRpc& rpc)
{
	if (m_connId == INVALID_CONN_ID)
	{
		return false;
	}
	return Send(m_connId, rpc);
}

void RSLink::SetRouterIpPort(const std::string& ip, UINT32 port)
{
	if (ip.empty() || port == 0)
	{
		return;
	}
	if (m_routerIp == ip && port == m_routerPort)
	{
		return;
	}

	if (!m_routerIp.empty())
	{
		// 通知ReconnectMgr断开老的连接
		PeerInfo peer;
		peer.m_action = NA_CONNECT;
		peer.m_state = PS_CONNECT_FAILED;
		peer.m_address.m_ip = m_routerIp;
		peer.m_address.m_port = m_routerPort;
		peer.m_handler = GetName();
		ReconnectManager::Instance()->RemovePeerInfo(peer);
	}

	// 如果之前的Router还连接的话，需要断开, 在正常情况下不会出现
	if (m_connId != INVALID_CONN_ID)
	{
		LogError("Router Is Ok, But Use a new router, error");
		CNetProcessor::Instance()->Close(m_connId);
	}

	m_routerIp = ip;
	m_routerPort = port;
	LogInfo("router ip: %s, port: %u", m_routerIp.c_str(), m_routerPort);

	// 加到ReconnectMgr里去
	PeerInfo peer;
	peer.m_action = NA_CONNECT;
	peer.m_state = PS_CONNECT_FAILED;
	peer.m_address.m_ip = m_routerIp;
	peer.m_address.m_port = m_routerPort;
	peer.m_handler = GetName();
	peer.m_dwSendBuffSize = 1024 * 1024 * 2;
	peer.m_dwRecvBuffSize = 1024 * 1024 * 2;
	ReconnectManager::Instance()->AddPeerInfo(peer);
}

