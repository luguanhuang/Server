#include "pch.h"
#include "loginlink.h"
#include "network.h"
#include "config.h"
#include "crpc.h"
#include "util/XRandom.h"
#include "common/rpcm2i_registerms2login.h"


INSTANCE_SINGLETON(LoginLink)


LoginLink::LoginLink()
{
}

LoginLink::~LoginLink()
{
}

void LoginLink::OnConnectSucceed(UINT32 dwConnID)
{
	LogInfo("connect to login server ok!");

	RpcM2I_RegisterMs2Login* rpc = RpcM2I_RegisterMs2Login::CreateRpc();
	rpc->m_oArg.set_zoneid(MSConfig::Instance()->GetZoneID());
	rpc->m_oArg.set_serverid(MSConfig::Instance()->GetServerID());
	auto serverIds = MSConfig::Instance()->GetServerIds();
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
	UINT32 i = 0;
	for(i = 0; i < m_oLoginSvrList.size(); ++i)
	{
		LoginInfo& roInfo = m_oLoginSvrList[i];
		if(roInfo.m_dwConnID == dwConnID)
		{
			LogError("login server [%u] link closed, error code: %d", roInfo.m_dwServerID, err);
			break;
		}
	}

	if(i < m_oLoginSvrList.size())
	{
		if(i != m_oLoginSvrList.size() - 1)
		{
			std::swap(m_oLoginSvrList[i], m_oLoginSvrList.back());
		}
		m_oLoginSvrList.pop_back();
	}
}

bool LoginLink::SendTo(const CProtocol& roPtc)
{
	if(m_oLoginSvrList.empty())
	{
		LogError("There is no any login server connected, send failed");
		return false;
	}

	//[Min, Max)
	UINT32 dwIndex = XRandom::randInt(0, m_oLoginSvrList.size());
	if(dwIndex >= m_oLoginSvrList.size())
	{
		dwIndex = 0;
	}

	return Send(m_oLoginSvrList[dwIndex].m_dwConnID, roPtc);
}

bool LoginLink::SendTo(CRpc &roRpc)
{
	if(m_oLoginSvrList.empty())
	{
		LogError("There is no any login server connected, send failed");
		return false;
	}

	//[Min, Max)
	UINT32 dwIndex = XRandom::randInt(0, m_oLoginSvrList.size());
	if(dwIndex >= m_oLoginSvrList.size())
	{
		dwIndex = 0;
	}

	return Send(m_oLoginSvrList[dwIndex].m_dwConnID, roRpc);
}

const std::string& LoginLink::GetName()
{
	static const std::string LinkName = "loginlink";
	return LinkName;
}

KKSG::ErrorCode LoginLink::AddLoginServer(UINT32 dwConnID, UINT32 dwLoginID)
{
	for(CLoginServerList::size_type i = 0; i < m_oLoginSvrList.size(); ++i)
	{
		LoginInfo& roInfo = m_oLoginSvrList[i];
		if(roInfo.m_dwServerID == dwLoginID)
		{
			LogError("Login server [%u] has been registered", dwLoginID);
			return KKSG::ERR_FAILED;
		}
	}

	LoginInfo oInfo;
	oInfo.m_dwConnID = dwConnID;
	oInfo.m_dwServerID = dwLoginID;
	m_oLoginSvrList.push_back(oInfo);
	return KKSG::ERR_SUCCESS;
}