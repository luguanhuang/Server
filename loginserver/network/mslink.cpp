#include "pch.h"
#include "mslink.h"
#include "config.h"

INSTANCE_SINGLETON(MsLink);


MsLink::MsLink()
{
}

MsLink::~MsLink()
{

}

void MsLink::OnPassiveConnect(UINT32 dwConnID)
{
}

void MsLink::OnClose(UINT32 dwConnID, int err)
{
	for (auto it = m_server2ConnID.begin(); it != m_server2ConnID.end(); )
	{
		if (it->second == dwConnID)
		{
			LogInfo("Ms server=[%u] closed", it->first);
			m_server2ConnID.erase(it++);
		}
		else
		{
			it++;
		}
	}
	m_connIDs.erase(dwConnID);
}

const std::string& MsLink::GetName()
{
	static const std::string LinkName = "mslink";
	return LinkName;
}

KKSG::ErrorCode MsLink::RegisterMs(UINT32 dwConnID, UINT32 dwZoneID, UINT32 dwServerID)
{
	auto it = m_server2ConnID.find(dwServerID);
	if (it != m_server2ConnID.end())
	{
		LogError("Ms server=[%u] already register", dwServerID);
	}
	m_server2ConnID[dwServerID] = dwConnID;
	m_connIDs.insert(dwConnID);
	LogInfo("Ms server=[%u] registered success", dwServerID);
	return KKSG::ERR_SUCCESS;
}

bool MsLink::IsServerConnected(UINT32 serverID)
{
	return m_server2ConnID.find(serverID) != m_server2ConnID.end();
}

void MsLink::Brocast(const CProtocol& ptc)
{
	for (auto it = m_connIDs.begin(); it != m_connIDs.end(); ++it)
	{
		Send(*it, ptc);
	}
}

bool MsLink::SendToServer(UINT32 serverID, const CProtocol& ptc)
{
	auto it = m_server2ConnID.find(serverID);
	if (it == m_server2ConnID.end())
	{
		return false;
	}
	return Send(it->second, ptc);
}

bool MsLink::SendToServer(UINT32 serverID, CRpc& rpc)
{
	auto it = m_server2ConnID.find(serverID);
	if (it == m_server2ConnID.end())
	{
		return false;
	}
	return Send(it->second, rpc);
}

