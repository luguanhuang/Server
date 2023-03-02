#include "pch.h"
#include "gatelink.h"
#include "netproc.h"
#include "protocol.h"
#include "unit/rolemanager.h"
#include "common/ptcg2t_opengate.h"
#include "common/rpcg2t_registergs2gt.h"
#include "dblink.h"
#include "config.h"
#include "foreach.h"
#include "config.h"
#include "network/rslink.h"

INSTANCE_SINGLETON(GatetLink)


GatetLink::GatetLink()
{
}

GatetLink::~GatetLink()
{ 
}

void GatetLink::OnConnectSucceed(UINT32 dwConnID)
{
	RpcG2T_RegisterGs2Gt* rpc = RpcG2T_RegisterGs2Gt::CreateRpc();
	rpc->m_oArg.set_serverid(GSConfig::Instance()->GetServerID());
	rpc->m_oArg.set_line(GSConfig::Instance()->GetLine());
	BaseLink::Send(dwConnID, *rpc);

	LogInfo("connect to gate OK!");
}

void GatetLink::OnConnectFailed()
{
	LogError("connect to gate Failed!");
}

/*
void GatetLink::OnClose(UINT32 dwConnID, int err)
{	
	for (auto i = m_GateConnInfo.begin(); i != m_GateConnInfo.end(); ++i)
	{
		if (i->dwConnID == dwConnID)
		{
			LogInfo("gate [%s:%d] is disconnected!", i->serverName.c_str(), i->dwServerID);

			RoleManager::Instance()->KickPlayerFromGate(i->dwServerID);
			
			m_GateConnInfo.erase(i);
			return;
		}
	}

	LogWarn("ConnID : %d:%d disconnected, not found in gatelink", (dwConnID>>16), dwConnID&0xFFFF);
}*/

const std::string & GatetLink::GetName()
{
	static const std::string LinkName = "gatelink";
	return LinkName;
}

bool GatetLink::SendToSession(const CProtocol &roPtc)
{
	if (GSConfig::Instance()->IsCrossGS())
	{
		SSWarn << "Can't send to Gate in cross Gs" << END;
		return true;
	}
	UINT32 dwGateLine = (UINT32)((roPtc.m_sessionID >> SessionGateIDShift) & 0xFF);
	return SendToLine(dwGateLine, roPtc);
}

void GatetLink::OnLineClosed(UINT32 dwLine)
{
	LogInfo("Gate server line [%u] disconnected!", dwLine);
}

void GatetLink::OnLineConnected(UINT32 dwLine)
{
	LogInfo("Gate server line [%u] connected", dwLine);
}

KKSG::ErrorCode GatetLink::AddGateServer(UINT32 dwConnID, UINT32 dwServerID, UINT32 dwLine)
{
	if(dwServerID != GSConfig::Instance()->GetServerID())
	{
		LogError("Gate server line [%u] 's server id [%u] error", dwLine, dwServerID);
		return KKSG::ERR_FAILED;
	}

	if(IsLineConnected(dwLine))
	{
		LogError("Gate server line [%u] has been registered", dwLine);
		return KKSG::ERR_FAILED;
	}

	RegisterLine(dwLine, dwConnID);
	return KKSG::ERR_SUCCESS;

	/*if (DBLink::Instance()->IsConnected())
	{
		PtcG2T_OpenGate oPtc;
		oPtc.m_Data.set_flag(true);
		Send(dwConnID, oPtc);
	}*/
}

void GatetLink::BroadCast(const CProtocol& roPtc)
{
	SendToAllLine(roPtc);
}

void GatetLink::CloseAllLink()
{
	for(auto iter = m_oAllSet.begin(); iter != m_oAllSet.end(); ++iter)
	{
		CNetProcessor::Instance()->Close(GetConnID(*iter));
		LogInfo("Gate line [%u] closed", *iter);
	}
}
