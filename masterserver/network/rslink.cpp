#include "pch.h"
#include "rslink.h"
#include "config.h"
#include "common/rpcm2r_registerms2router.h"
#include "live/livemanager.h"
#include "common/rpcm2k_queryrouter.h"
#include "network/worldlink.h"
#include "network/gatelink.h"
#include "network/dblink.h"
#include "network/controllink.h"
#include "timeutil.h"
#include "common/ptcm2t_notifyrouterinfo2gate.h"
#include "common/ptcm2d_notifyrouterinfo2db.h"
#include "common/ptcm2n_notifyrouterinfo2ctrl.h"
#include "reconnetmgr.h"

INSTANCE_SINGLETON(RSLink)

RSLink::RSLink()
:m_routerPort(0)
,m_reconnectCount(0)
,m_lastQueryTime(0)
{
	m_connId = INVALID_CONN_ID;
}

RSLink::~RSLink()
{
}

void RSLink::OnConnectSucceed(UINT32 dwConnID)
{
	m_connId = dwConnID;
	RpcM2R_RegisterMs2Router* rpc = RpcM2R_RegisterMs2Router::CreateRpc();
	rpc->m_oArg.set_server_id(MSConfig::Instance()->GetServerID());
	SendTo(*rpc);
	m_reconnectCount = 0;
}

void RSLink::OnConnectFailed()
{
	LogError("connected router failed!");
	m_reconnectCount++;

	if (m_reconnectCount > 10)
	{
		QueryRouterFromWorld();
	}
}

void RSLink::OnClose(UINT32 dwConnID, int err)
{
	LogInfo("router link was disconnected");
	m_connId = INVALID_CONN_ID;

	LiveManager::Instance()->OnCrossGSClose(0, true);
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

void RSLink::QueryRouterFromWorld()
{
	UINT32 now = TimeUtil::GetTime();
	if (now < (m_lastQueryTime + 3))
	{
		return;
	}
	m_lastQueryTime = now;
	if (m_connId != INVALID_CONN_ID)
	{
		return;
	}

	RpcM2K_QueryRouter* rpc = RpcM2K_QueryRouter::CreateRpc();
	rpc->m_oArg.set_serverid(MSConfig::Instance()->GetServerID());
	WorldLink::Instance()->SendTo(*rpc);
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
	ReconnectManager::Instance()->AddPeerInfo(peer);
}

void RSLink::OnQueryRouterReply(const KKSG::RouterListenInfo & info)
{
	SetRouterIpPort(info.ms().ip(), info.ms().port());
	m_routerInfo.CopyFrom(info);

	NotifyRouter2Gate();
	NotifyRouter2DB();
	NotifyRouter2Ctrl();
}

void RSLink::NotifyRouter2Gate(UINT32 line)
{
	if (!m_routerInfo.has_gate())
	{
		return;
	}

	PtcM2T_NotifyRouterInfo2Gate ntf;
	ntf.m_Data.CopyFrom(m_routerInfo);
	if (line == 0)
	{
		GateLink::Instance()->SendToAllLine(ntf);
	}
	else
	{
		GateLink::Instance()->SendToLine(line, ntf);
	}
}

void RSLink::NotifyRouter2DB()
{
	if (!m_routerInfo.has_db())
	{
		return;
	}
	PtcM2D_NotifyRouterInfo2DB ntf;
	ntf.m_Data.CopyFrom(m_routerInfo);
	DBLink::Instance()->SendTo(ntf);
}

void RSLink::NotifyRouter2Ctrl()
{
	if (!m_routerInfo.has_ctrl())
	{
		return;
	}
	PtcM2N_NotifyRouterInfo2Ctrl ntf;
	ntf.m_Data.CopyFrom(m_routerInfo);
	ControlLink::Instance()->SendTo(ntf);
}
