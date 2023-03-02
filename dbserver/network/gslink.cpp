#include "pch.h"
#include "gslink.h"
#include "network.h"
#include "netproc.h"

INSTANCE_SINGLETON(GSLink);


GSLink::GSLink()
{
}

GSLink::~GSLink()
{

}

void GSLink::OnPassiveConnect(UINT32 dwConnID)
{
	LogInfo("a gameserver is connected to db!");
}

void GSLink::OnClose(UINT32 dwConnID, int err)
{
	bool IsFindConnID = false;
	for (auto i = m_gsConnections.begin(); i != m_gsConnections.end(); ++i)
	{
		if (i->m_dwConnID == dwConnID)
		{
			LogInfo("remove gameserver [%d:%s]", i->m_dwServerID, i->m_serverName.c_str());
			m_gsConnections.erase(i);
			IsFindConnID = true;
			break;
		}
	}

	if (!IsFindConnID)
	{
		LogError("error: OnClose Not found connID!", dwConnID);
	}
}


void GSLink::Close(UINT32 dwConnID)
{
	CNetProcessor::Instance()->Close(dwConnID);
}

const std::string& GSLink::GetName()
{
	static const std::string GSLinkName = "gslink";
	return GSLinkName;
}

GameServerInfo * GSLink::GetServerInfo(UINT32 dwConnID)
{
	for (auto i = m_gsConnections.begin(); i != m_gsConnections.end(); ++i)
	{
		if (i->m_dwConnID == dwConnID)
		{
			return &*i;
		}
	}

	return NULL;
}

GameServerInfo * GSLink::GetServerInfoByServerID(UINT32 dwServerID)
{
	for (auto i = m_gsConnections.begin(); i != m_gsConnections.end(); ++i)
	{
		if (i->m_dwServerID == dwServerID)
		{
			return &*i;
		}
	}

	return NULL;
}

void GSLink::AddServerInfo(UINT32 dwConnID, UINT32 dwServerID, const std::string &serverName)
{
	GameServerInfo oInfo;
	oInfo.m_dwConnID = dwConnID;
	oInfo.m_serverName = serverName;
	oInfo.m_dwServerID = dwServerID;

	m_gsConnections.push_back(oInfo);
	LogInfo("add gameserver [%d:%s]", dwServerID, serverName.c_str());
}

void GSLink::ShowGSLinkInfo()
{
	LogDebug("Begin Show GSLink Info, ListSize:%u", m_gsConnections.size());
	for( auto i=this->m_gsConnections.begin(); i!= this->m_gsConnections.end(); i++ )
	{
		LogDebug("GameServer Link Info, ConnID:%u, GSID:%u, GSName:%s", (*i).m_dwConnID, (*i).m_dwServerID, (*i).m_serverName.c_str());
	}
	LogDebug("Finish Show GSLink Info, ListSize:%u", m_gsConnections.size());
}
