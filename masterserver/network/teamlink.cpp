#include "pch.h"
#include "teamlink.h"
#include "network.h"
#include "netproc.h"
#include "config.h"
#include "crpc.h"
#include "common/rpcm2h_registerms2team.h"
#include "team/teammgr.h"
#include "pvp/pvpmgr.h"

INSTANCE_SINGLETON(TeamLink)


TeamLink::TeamLink()
:m_dwConnID(INVALID_CONN_ID)
{
}

TeamLink::~TeamLink()
{
}

void TeamLink::OnConnectSucceed(UINT32 dwConnID)
{
	if(m_dwConnID != INVALID_CONN_ID)
	{
		LogError("more than one ts is connected!");
		return;
	}

	SetConnID(dwConnID);
	LogInfo("connect to ts success");

	RpcM2H_RegisterMs2Team* rpc = RpcM2H_RegisterMs2Team::CreateRpc();
	rpc->m_oArg.set_serverid(MSConfig::Instance()->GetServerID());
	Send(GetConnID(), *rpc);
}

void TeamLink::OnConnectFailed()
{
	LogError("connect to ts failed!");
}

void TeamLink::OnClose(UINT32 dwConnID, int err)
{
	SetConnID(INVALID_CONN_ID);
	LogInfo("ts link was disconnected");

	TeamMgr::Instance()->OnTsDisconnected();
}

bool TeamLink::SendTo(const CProtocol& roPtc)
{
	if (m_dwConnID == INVALID_CONN_ID)
	{
		return false;
	}

	return Send(m_dwConnID, roPtc);
}

bool TeamLink::SendTo(CRpc &roRpc)
{
	if (m_dwConnID == INVALID_CONN_ID)
	{
		return false;
	}

	return Send(m_dwConnID, roRpc);
}

const std::string& TeamLink::GetName()
{
	static const std::string LinkName = "teamlink";
	return LinkName;
}
