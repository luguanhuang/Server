#include "pch.h"
#include "controllink.h"
#include "network.h"
#include "netproc.h"
#include "config.h"
#include "gatemgr/gatemgr.h"

INSTANCE_SINGLETON(ControlLink);


ControlLink::ControlLink()
{
}

ControlLink::~ControlLink()
{

}

void ControlLink::OnPassiveConnect(UINT32 dwConnID)
{
}

void ControlLink::OnClose(UINT32 dwConnID, int err)
{
	for (auto it = m_server2ConnID.begin(); it != m_server2ConnID.end(); )
	{
		if (it->second == dwConnID)
		{
			LogInfo("Ns server=[%u] closed", it->first);
			m_server2ConnID.erase(it++);
		}
		else
		{
			it++;
		}
	}
	m_connIDs.erase(dwConnID);
}

const std::string& ControlLink::GetName()
{
	static const std::string LinkName = "ctrllink";
	return LinkName;
}


KKSG::ErrorCode ControlLink::RegisterNs(UINT32 dwConnID, UINT32 dwZoneID, UINT32 dwServerID)
{
	auto it = m_server2ConnID.find(dwServerID);
	if (it != m_server2ConnID.end())
	{
		LogError("Ns server=[%u] already register", dwServerID);
	}
	m_server2ConnID[dwServerID] = dwConnID;
	m_connIDs.insert(dwConnID);
	LogInfo("Ns server=[%u] registered success", dwServerID);
	return KKSG::ERR_SUCCESS;
}

void ControlLink::Brocast(const CProtocol& ptc)
{
	for (auto it = m_connIDs.begin(); it != m_connIDs.end(); ++it)
	{
		Send(*it, ptc);
	}
}

bool ControlLink::SendToServer(UINT32 serverID, const CProtocol& ptc)
{
	auto it = m_server2ConnID.find(serverID);
	if (it == m_server2ConnID.end())
	{
		return false;
	}
	return Send(it->second, ptc);
}

bool ControlLink::SendToServer(UINT32 serverID, CRpc& rpc)
{
	auto it = m_server2ConnID.find(serverID);
	if (it == m_server2ConnID.end())
	{
		return false;
	}
	return Send(it->second, rpc);
}