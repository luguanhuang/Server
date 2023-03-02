#include "pch.h"
#include "rslink.h"
#include "config.h"
#include "common/rpcn2r_registerctrl2router.h"
#include "account/accountsessionmgr.h"
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

	RpcN2R_RegisterCtrl2Router* rpc = RpcN2R_RegisterCtrl2Router::CreateRpc();
	rpc->m_oArg.set_serverid(ControlConfig::Instance()->GetServerID());
	auto& serverIds = ControlConfig::Instance()->GetServerIds();
	for (auto it = serverIds.begin(); it != serverIds.end(); ++it)
	{
		rpc->m_oArg.add_serverids(*it);
	}
	SendTo(*rpc);
	LogInfo("connect router ok!");
}

void RSLink::OnConnectFailed()
{
	LogError("connected router failed!");
}

void RSLink::OnClose(UINT32 dwConnID, int err)
{
	LogInfo("router link was disconnected");
	m_connId = INVALID_CONN_ID;
	CAccountSessionMgr::Instance()->KickoutAllCrossPlayer();
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
		SSError << "Not connected router, can't send" << END;
		return false;
	}
	return Send(m_connId, ptc);
}

bool RSLink::SendTo(CRpc& rpc)
{
	if (m_connId == INVALID_CONN_ID)
	{
		SSError << "Not connected router, can't send" << END;
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
		// ֪ͨReconnectMgr�Ͽ��ϵ�����
		PeerInfo peer;
		peer.m_action = NA_CONNECT;
		peer.m_state = PS_CONNECT_FAILED;
		peer.m_address.m_ip = m_routerIp;
		peer.m_address.m_port = m_routerPort;
		peer.m_handler = GetName();
		ReconnectManager::Instance()->RemovePeerInfo(peer);
	}

	// ���֮ǰ��Router�����ӵĻ�����Ҫ�Ͽ�, ����������²������
	if (m_connId != INVALID_CONN_ID)
	{
		LogError("Router Is Ok, But Use a new router, error");
		CNetProcessor::Instance()->Close(m_connId);
	}

	m_routerIp = ip;
	m_routerPort = port;
	LogInfo("router ip: %s, port: %u", m_routerIp.c_str(), m_routerPort);

	// �ӵ�ReconnectMgr��ȥ
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

