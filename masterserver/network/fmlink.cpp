#include "pch.h"
#include "fmlink.h"
#include "network.h"
#include "config.h"
#include "crpc.h"
#include "chat/fmmgr.h"
#include "common/ptcm2f_registfmserver.h"
#include "util/XRandom.h"
#include "common/rpcm2f_registerms2fm.h"

INSTANCE_SINGLETON(FMLink)


FMLink::FMLink(){}

FMLink::~FMLink(){}

void FMLink::OnConnectSucceed(UINT32 dwConnID)
{
	RpcM2F_RegisterMs2Fm* rpc = RpcM2F_RegisterMs2Fm::CreateRpc();
	rpc->m_oArg.set_zoneid(MSConfig::Instance()->GetZoneID());
	rpc->m_oArg.set_serverid(MSConfig::Instance()->GetServerID());
	Send(dwConnID, *rpc);
}

void FMLink::OnConnectFailed()
{
	LogError("connect to fm server failed!");
}

void FMLink::OnPassiveConnect(UINT32 dwConnID)
{
	assert(false);
}


void FMLink::RemoveConnId(UINT32 connId)
{
	for (auto it = m_connIds.begin(); it != m_connIds.end(); ++it)
	{
		if (*it == connId)
		{
			m_connIds.erase(it);
			break;
		}
	}
	if (m_masterFmId == connId)
	{
		m_masterFmId = INVALID_CONN_ID;
		FMMgr::Instance()->CloseRoom();
	}
}

void FMLink::OnClose(UINT32 dwConnID, int err)
{
	LogInfo("world link was disconnected[%u]", dwConnID);
	RemoveConnId(dwConnID);
}

bool FMLink::IsConnected()
{
	return m_masterFmId != INVALID_CONN_ID;
}

bool FMLink::SendTo(const CProtocol& roPtc)
{
	if (m_masterFmId == INVALID_CONN_ID)
	{
		return false;
	}
	return Send(m_masterFmId, roPtc);
}

bool FMLink::SendTo(CRpc &roRpc)
{
	if (m_masterFmId == INVALID_CONN_ID)
	{
		return false;
	}
	return Send(m_masterFmId, roRpc);
}


void FMLink::SetMasterFmConnId(UINT32 connId)
{
	m_masterFmId = connId;
	LogInfo("master fm connid[%u]", connId);
}

const std::string& FMLink::GetName()
{
	static const std::string LinkName = "fmlink";
	return LinkName;
}

void FMLink::AddFmServer(UINT32 dwConnID, UINT32 dwLoginID)
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