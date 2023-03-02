#include "pch.h"
#include "loginlink.h"
#include "network.h"
#include "config.h"
#include "crpc.h"
#include "common/rpcn2i_registerns2login.h"


INSTANCE_SINGLETON(LoginLink)


LoginLink::LoginLink()
{
}

LoginLink::~LoginLink()
{

}

void LoginLink::OnConnectSucceed(UINT32 dwConnID)
{
	RpcN2I_RegisterNs2Login* rpc = RpcN2I_RegisterNs2Login::CreateRpc();
	rpc->m_oArg.set_zoneid(ControlConfig::Instance()->GetZoneID());
	rpc->m_oArg.set_serverid(ControlConfig::Instance()->GetServerID());
	auto serverIds = ControlConfig::Instance()->GetServerIds();
	for (auto it = serverIds.begin(); it != serverIds.end(); ++it)
	{
		rpc->m_oArg.add_serverids(*it);
	}

	Send(dwConnID, *rpc);
}

void LoginLink::OnConnectFailed()
{
	LogError("connect to login server failed!");
}

void LoginLink::OnPassiveConnect(UINT32 dwConnID)
{
	assert(false);
}

void LoginLink::OnClose(UINT32 dwConnID, int err)
{
	UINT32 dwServerID = 0;
	for(CLoginServerMap::iterator it = m_oLoginSvrMap.begin(); it != m_oLoginSvrMap.end(); ++it)
	{
		if(it->second == dwConnID)
		{
			dwServerID = it->first;
			m_oLoginSvrMap.erase(it);
			break;
		}
	}

	LogError("login server [%u] link closed, error code: %d", dwServerID, err);
}

bool LoginLink::SendTo(UINT32 dwLoginID, const CProtocol& roPtc)
{
	UINT32 dwConnID = GetLoginConnID(dwLoginID);
	if (dwConnID == INVALID_CONN_ID)
	{
		LogError("Login server [%u] 's conn id is invalid", dwLoginID);
		return false;
	}

	return Send(dwConnID, roPtc);
}

bool LoginLink::SendTo(UINT32 dwLoginID, CRpc &roRpc)
{
	UINT32 dwConnID = GetLoginConnID(dwLoginID);
	if (dwConnID == INVALID_CONN_ID)
	{
		LogError("Login server [%u] 's conn id is invalid", dwLoginID);
		return false;
	}

	return Send(dwConnID, roRpc);
}

void LoginLink::GetAllLogins(std::vector<UINT32>& logins)
{
	for (auto it = m_oLoginSvrMap.begin(); it != m_oLoginSvrMap.end(); it++)
	{
		logins.push_back(it->first);
	}
}

const std::string& LoginLink::GetName()
{
	static const std::string LinkName = "loginlink";
	return LinkName;
}

UINT32 LoginLink::GetLoginConnID(UINT32 dwLoginID)
{
	//ÁÙÊ±
	if(dwLoginID == 0)
	{
		return m_oLoginSvrMap.empty() ? INVALID_CONN_ID : m_oLoginSvrMap.begin()->second;
	}

	CLoginServerMap::iterator it = m_oLoginSvrMap.find(dwLoginID);
	return it == m_oLoginSvrMap.end() ? INVALID_CONN_ID : it->second;
}

KKSG::ErrorCode LoginLink::AddLoginServer(UINT32 dwConnID, UINT32 dwLoginID)
{
	if(GetLoginConnID(dwLoginID) != INVALID_CONN_ID)
	{
		LogError("Login server [%u] has been registered", dwLoginID);
		return ERR_FAILED;
	}

	m_oLoginSvrMap[dwLoginID] = dwConnID;
	return ERR_SUCCESS;
}
