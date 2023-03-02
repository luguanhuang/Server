#include "pch.h"
#include "rslink.h"
#include "netproc.h"
#include "protocol.h"
#include "unit/rolemanager.h"
#include "dblink.h"
#include "config.h"
#include "foreach.h"
#include "reconnetmgr.h"
#include "common/rpcg2r_registergs2router.h"
#include "unit/rolemanager.h"

INSTANCE_SINGLETON(RSLink)


RSLink::RSLink()
{
}

RSLink::~RSLink()
{ 
}

void RSLink::OnConnectSucceed(UINT32 dwConnID)
{
	LogInfo("connect to router OK! connId: %u", dwConnID);

	RpcG2R_RegisterGs2Router* rpc = RpcG2R_RegisterGs2Router::CreateRpc();
	rpc->m_oArg.set_line(GSConfig::Instance()->GetLine());
	BaseLink::Send(dwConnID, *rpc);
}

void RSLink::OnConnectFailed()
{
	LogError("connect to router Failed!");
}

const std::string & RSLink::GetName()
{
	static const std::string LinkName = "rslink";
	return LinkName;
}

void RSLink::OnLineClosed(UINT32 dwLine)
{
	LogInfo("Router server line [%u] disconnected!", dwLine);
	auto it = m_line2Conn.find(dwLine);
	if (it != m_line2Conn.end())
	{
		RoleManager::Instance()->OnRouterClose(dwLine, it->second);
		m_line2Conn.erase(it);
	}
}

void RSLink::OnLineConnected(UINT32 dwLine)
{
	LogInfo("Router server line [%u] connected", dwLine);
}

KKSG::ErrorCode RSLink::AddRouterServer(UINT32 connId, UINT32 line)
{
	if (IsLineConnected(line))
	{
		LogError("Router server line [%u] has been registered", line);
		return KKSG::ERR_FAILED;
	}

	RegisterLine(line, connId);
	m_line2Conn[line] = connId;
	return KKSG::ERR_SUCCESS;
}

void RSLink::AddToConnect(UINT32 line, const std::string& ip , UINT32 port)
{
	if (m_allLines.find(line) != m_allLines.end())
	{
		LogInfo("router line [%u] is in connect set", line);
		return;
	}
	m_allLines.insert(line);

	PeerInfo peer;
	peer.m_action = NA_CONNECT;
	peer.m_state = PS_CONNECT_FAILED;
	peer.m_address.m_ip = ip;
	peer.m_address.m_port = port;
	peer.m_handler = GetName();
	peer.m_dwSendBuffSize = 1024 * 1024 * 2;
	peer.m_dwRecvBuffSize = 1024 * 1024 * 2;
	ReconnectManager::Instance()->AddPeerInfo(peer);

	SSInfo << "Begin to connect router server, ip: " << ip << ", port: " << port << END;
}


