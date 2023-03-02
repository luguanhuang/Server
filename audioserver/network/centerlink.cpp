#include "pch.h"
#include "centerlink.h"
#include "network.h"
#include "config.h"
#include "crpc.h"
#include "common/rpca2s_registeraudiotocenter.h"
#include "util/XRandom.h"

INSTANCE_SINGLETON(CenterLink)


CenterLink::CenterLink(){}

CenterLink::~CenterLink(){}

void CenterLink::OnConnectSucceed(UINT32 dwConnID)
{
	RpcA2S_RegisterAudioToCenter* rpc = RpcA2S_RegisterAudioToCenter::CreateRpc();
	rpc->m_oArg.set_zoneid(AudioConfig::Instance()->GetZoneID());
	rpc->m_oArg.set_serverid(AudioConfig::Instance()->GetServerID());
	Send(dwConnID, *rpc);
}

void CenterLink::OnConnectFailed()
{
	LogError("connect to center server failed!");
}

void CenterLink::OnPassiveConnect(UINT32 dwConnID)
{
	assert(false);
}


void CenterLink::RemoveConnId(UINT32 connId)
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

void CenterLink::OnClose(UINT32 dwConnID, int err)
{
	LogInfo("world link was disconnected[%u]", dwConnID);
	RemoveConnId(dwConnID);
}

bool CenterLink::IsConnected()
{
	return m_connIds.empty()==false;
}

bool CenterLink::SendTo(const CProtocol& roPtc)
{
	if (m_connIds.empty()==true)
	{
		return false;
	}
	UINT32 nIdx = XRandom::randInt(0,m_connIds.size());
	return Send(m_connIds[nIdx], roPtc);
}

bool CenterLink::SendTo(CRpc &roRpc)
{
	if (m_connIds.empty()==true)
	{
		return false;
	}
	UINT32 nIdx = XRandom::randInt(0,m_connIds.size());
	return Send(m_connIds[nIdx], roRpc);
}

const std::string& CenterLink::GetName()
{
	static const std::string LinkName = "centerlink";
	return LinkName;
}

void CenterLink::AddCSServer(UINT32 dwConnID)
{
	bool found = false;
	for (size_t i = 0; i < m_connIds.size(); ++i)
	{
		if (m_connIds[i] == dwConnID)
		{
			found = true;
			break;
		}
	}
	if (found == false)
	{
		m_connIds.push_back(dwConnID);
	}
}