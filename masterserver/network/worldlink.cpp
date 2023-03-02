#include "pch.h"
#include "worldlink.h"
#include "common/rpcm2k_registerms2world.h"
#include "config.h"
#include "pvp/pvpmgr.h"
#include "cross/crosszonestatus.h"

INSTANCE_SINGLETON(WorldLink)

WorldLink::WorldLink()
:m_masterWorldId(INVALID_CONN_ID)
{
}

WorldLink::~WorldLink()
{
}

void WorldLink::OnConnectSucceed(UINT32 dwConnID)
{
	LogInfo("world link connected[%u]", dwConnID);
	AddConnId(dwConnID);

	RpcM2K_RegisterMs2World* rpc = RpcM2K_RegisterMs2World::CreateRpc();
	rpc->m_oArg.set_server_id(MSConfig::Instance()->GetServerID());
	Send(dwConnID, *rpc);
}

void WorldLink::OnConnectFailed()
{
	if (!IsConnected())
	{
		LogError("connected world failed!");
	}
}

void WorldLink::OnClose(UINT32 dwConnID, int err)
{
	LogInfo("world link was disconnected[%u]", dwConnID);
	RemoveConnId(dwConnID);

	if (IsConnected() == false)
	{
		CrossZoneStatus::Instance()->SetWorldStatus(false);
	}
}

void WorldLink::SetMasterWorldConnId(UINT32 connId)
{
	m_masterWorldId = connId;
	LogInfo("master world connid[%u]", connId);
}

void WorldLink::AddConnId(UINT32 connId)
{
	bool found = false;
	for (size_t i = 0; i < m_connIds.size(); ++i)
	{
		if (m_connIds[i] == connId)
		{
			found = true;
			break;
		}
	}
	if (found == false)
	{
		m_connIds.push_back(connId);
	}
}

void WorldLink::RemoveConnId(UINT32 connId)
{
	for (auto it = m_connIds.begin(); it != m_connIds.end(); ++it)
	{
		if (*it == connId)
		{
			m_connIds.erase(it);
			break;
		}
	}

	if (m_masterWorldId == connId)
	{
		m_masterWorldId = INVALID_CONN_ID;
	}
}

const std::string& WorldLink::GetName()
{
	static const std::string LinkName = "worldlink";
	return LinkName;
}

bool WorldLink::SendTo(const CProtocol& ptc)
{
	if (m_masterWorldId == INVALID_CONN_ID)
	{
		return false;
	}
	return Send(m_masterWorldId, ptc);
}

bool WorldLink::SendTo(CRpc& rpc)
{
	if (m_masterWorldId == INVALID_CONN_ID)
	{
		return false;
	}
	return Send(m_masterWorldId, rpc);
}

bool WorldLink::IsConnected()
{
	return m_masterWorldId != INVALID_CONN_ID;
}

bool WorldLink::SendToAllConn(const CProtocol& ptc)
{
	bool ret = true;
	for (size_t i = 0; i < m_connIds.size(); ++i)
	{
		if (!Send(m_connIds[i], ptc))
		{
			ret = false;
		}
	}
	return ret;
}
