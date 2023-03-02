#include "pch.h"
#include "worldlink.h"
#include "network.h"
#include "netproc.h"
#include "config.h"
#include "gatemgr/gatemgr.h"

INSTANCE_SINGLETON(WorldLink);


WorldLink::WorldLink()
{
}

WorldLink::~WorldLink()
{

}

void WorldLink::OnPassiveConnect(UINT32 dwConnID)
{
	LogInfo("A World Connected[%u]", dwConnID);
	AddConnId(dwConnID);

	GateMgr::Instance()->ServerInfoNotifyToWorld(dwConnID);
}

void WorldLink::OnClose(UINT32 dwConnID, int err)
{
	LogInfo("A World DisConnected[%u]", dwConnID);
	RemoveConnId(dwConnID);
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
}

const std::string& WorldLink::GetName()
{
	static const std::string LinkName = "worldlink";
	return LinkName;
}

void WorldLink::SendToAll(const CProtocol& ptc)
{
	for (auto it = m_connIds.begin(); it != m_connIds.end(); ++it)
	{
		Send(*it, ptc);
	}
}

