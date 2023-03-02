#include "pch.h"
#include "cslink.h"
#include "network.h"
#include "config.h"
#include "crpc.h"
#include "common/rpcm2s_registerms2cs.h"
#include "util/XRandom.h"
#include "role/rolemanager.h"


INSTANCE_SINGLETON(CSLink)


CSLink::CSLink()
{
	m_usingCenterServerConnID = INVALID_CONN_ID;
}

CSLink::~CSLink()
{

}

void CSLink::OnConnectSucceed(UINT32 dwConnID)
{

	RpcM2S_RegisterMS2CS* rpc = RpcM2S_RegisterMS2CS::CreateRpc();
	rpc->m_oArg.set_platformid(MSConfig::Instance()->GetZoneID());
	rpc->m_oArg.set_serverid(MSConfig::Instance()->GetServerID());
	rpc->m_oArg.set_platformtype(MSConfig::Instance()->GetPlatType());
	rpc->m_oArg.set_apptype(MSConfig::Instance()->GetAppType());
	rpc->m_oArg.set_servername(MSConfig::Instance()->GetServerName());
	
	for (auto iter = CRoleManager::Instance()->Begin(); iter != CRoleManager::Instance()->End(); ++iter)
	{
		KKSG::AccountRole* temp = rpc->m_oArg.add_onlineaccounts();
		temp->set_account((*iter)->GetAccount());
		temp->set_roleid((*iter)->GetID());
	}
	_SendTo(dwConnID, *rpc);

	LogInfo("connect to center server ok!, account size:%u", rpc->m_oArg.onlineaccounts_size());
}

void CSLink::OnConnectFailed()
{
	LogError("connect to center server failed!");
}

void CSLink::OnPassiveConnect(UINT32 dwConnID)
{
	assert(false);
}

void CSLink::OnClose(UINT32 dwConnID, int err)
{
	LogError("center server link closed, error code: %d", err);

	RemoveCenterServer(dwConnID);
}

bool CSLink::SendTo(const CProtocol& roPtc)
{
	if (m_usingCenterServerConnID == INVALID_CONN_ID)
	{
		LogError("not using centerserver");
		return false;
	}

	return Send(m_usingCenterServerConnID, roPtc);
}

bool CSLink::SendTo(CRpc &roRpc)
{
	if (m_usingCenterServerConnID == INVALID_CONN_ID)
	{
		LogError("not using centerserver");
		return false;
	}

	return Send(m_usingCenterServerConnID, roRpc);
}

bool CSLink::SendToAll(const CProtocol& roPtc)
{
	for (auto iter = m_oCenterSvrList.begin(); iter != m_oCenterSvrList.end(); ++iter)
	{
		_SendTo(iter->first, roPtc);
	}

	return true;
}

void CSLink::AddCenterServer(UINT32 dwConnID, UINT32 dwCenterServerID)
{
	for (auto iter = m_oCenterSvrList.begin(); iter != m_oCenterSvrList.end(); ++iter)
	{
		if (iter->second.m_dwServerID == dwCenterServerID)
		{
			LogError("duplicate center id:%u", dwCenterServerID);
			return;
		}
	}

	CenterServerInfo info;
	info.m_dwConnID = dwConnID;
	info.m_dwServerID = dwCenterServerID;
	m_oCenterSvrList.insert(std::make_pair(dwConnID, info));

	RandUsingCenterSever();

	LogInfo("register cs success centerserver id:%u", dwCenterServerID);
}

void CSLink::RemoveCenterServer(UINT32 dwConnID)
{
	auto find = m_oCenterSvrList.find(dwConnID);
	if (find == m_oCenterSvrList.end())
	{
		LogWarn("can't find server, conn:%u", dwConnID);
		return;
	}

	m_oCenterSvrList.erase(dwConnID);

	RandUsingCenterSever();
}

void CSLink::RandUsingCenterSever()
{
	int rand = XRandom::randInt(0, m_oCenterSvrList.size());

	int i = 0;
	for (auto iter = m_oCenterSvrList.begin(); iter != m_oCenterSvrList.end(); ++iter, ++i)
	{
		if (rand == i)
		{
			m_usingCenterServerConnID = iter->first;
			LogInfo("using centerserver:%u", iter->second.m_dwServerID);
			return;
		}
	}
	
	m_usingCenterServerConnID = INVALID_CONN_ID;
	LogError("not using centerserver.");
}

UINT32 CSLink::GetUsingCenterServerID()
{
	return m_usingCenterServerConnID;
}

void CSLink::Close(UINT32 dwConnID)
{
	LogError("close :%u, %s", dwConnID, __FUNCTION__);

	CNetProcessor::Instance()->Close(dwConnID);

	RemoveCenterServer(dwConnID);
}

bool CSLink::_SendTo(UINT32 dwConnID, const CProtocol& roPtc)
{
	return Send(dwConnID, roPtc);
}

bool CSLink::_SendTo(UINT32 dwConnID, CRpc& roRpc)
{
	return Send(dwConnID, roRpc);
}

const std::string& CSLink::GetName()
{
	static const std::string LinkName = "centerlink";
	return LinkName;
}
