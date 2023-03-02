#include "pch.h"
#include "cslink.h"
#include "config.h"
#include "util/XRandom.h"

INSTANCE_SINGLETON(CSLink);


CSLink::CSLink()
{
	m_usingCenterServerConnID = INVALID_CONN_ID;
}

CSLink::~CSLink()
{

}

void CSLink::OnPassiveConnect(UINT32 dwConnID)
{
	LogInfo("A CS Connected[%u]", dwConnID);
	AddConnId(dwConnID);
}

void CSLink::OnClose(UINT32 dwConnID, int err)
{
	LogInfo("A CS DisConnected[%u]", dwConnID);
	RemoveConnId(dwConnID);
}

void CSLink::AddConnId(UINT32 connId)
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
		RandUsingCenterSever();
	}
}

void CSLink::RemoveConnId(UINT32 connId)
{
	for (auto it = m_connIds.begin(); it != m_connIds.end(); ++it)
	{
		if (*it == connId)
		{
			m_connIds.erase(it);
			break;
		}
	}

	RandUsingCenterSever();
}

const std::string& CSLink::GetName()
{
	static const std::string LinkName = "cslink";
	return LinkName;
}


void CSLink::RandUsingCenterSever()
{
	if (m_connIds.empty())
	{
		m_usingCenterServerConnID = INVALID_CONN_ID;
		LogError("not using centerserver.");
		return;
	}

	int rand = XRandom::randInt(0, m_connIds.size());

	m_usingCenterServerConnID = m_connIds[rand];
	LogInfo("using centerserver:%u", m_usingCenterServerConnID);
}


UINT32 CSLink::GetUsingCenterServerID()
{
	return m_usingCenterServerConnID;
}
