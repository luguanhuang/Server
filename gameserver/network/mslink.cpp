#include "pch.h"
#include "mslink.h"
#include "network.h"
#include "netproc.h"
#include "config.h"
#include "crpc.h"
#include "common/rpcg2m_registergs2ms.h"
#include "guild/ptcg2m_reqguildsimpledata.h"
#include "network/rslink.h"
#include "gamelogic/teammgr.h"
#include "servermgr/servermgr.h"
#include "dragonguild/ptcg2m_reqdragonguildsimpledata.h"

INSTANCE_SINGLETON(MSLink)


MSLink::MSLink()
:m_dwConnID(INVALID_CONN_ID)
,m_dwLinkTimes(0)
,m_bRegistered(false)
{
}

MSLink::~MSLink()
{
}

void MSLink::OnConnectSucceed(UINT32 dwConnID)
{
	SetConnID(dwConnID);

	RpcG2M_RegisterGs2Ms* rpc = RpcG2M_RegisterGs2Ms::CreateRpc();
	rpc->m_oArg.set_serverid(GSConfig::Instance()->GetServerID());
	rpc->m_oArg.set_line(GSConfig::Instance()->GetLine());
	SendTo(*rpc);
	ReqGuildSimpleData();  // 成功后 请求公会基础数据
	ReqDragonGuildSimpleData();
}

void MSLink::OnConnectFailed()
{
	LogError("connect to ms failed!");
}

void MSLink::OnClose(UINT32 dwConnID, int err)
{
	SetRegistered(false);
	SetConnID(INVALID_CONN_ID);
	LogInfo("ms link was disconnected");

	TeamCopyMgr::Instance()->OnMsClose();
}

const std::string & MSLink::GetName()
{
	static const std::string LinkName = "mslink";
	return LinkName;
}

bool MSLink::SendTo(const CProtocol& roPtc)
{
	if(m_dwConnID == INVALID_CONN_ID)
	{
		SSWarn << "Ms not connected, protocol type: " << CProtocol::GetProtoName(roPtc.m_dwType) << END;
		return false;
	}
	return Send(GetConnID(), roPtc);
}

bool MSLink::SendTo(const CRpc &roRpc)
{
	if(m_dwConnID == INVALID_CONN_ID)
	{
		SSWarn << "Ms not connected, rpc type: " << CProtocol::GetProtoName(roRpc.m_dwType) << END;
		return false;
	}

	return Send(GetConnID(), const_cast<CRpc&>(roRpc));
}

bool MSLink::SendTo(UINT64 roleId, const CProtocol& roPtc)
{
	if (GSConfig::Instance()->IsCrossGS())
	{
		return ServerMgr::Instance()->SendByRoleId(roleId, roPtc);
	}
	return SendTo(roPtc);
}

bool MSLink::SendTo(UINT64 roleId, CRpc &roRpc)
{
	if (GSConfig::Instance()->IsCrossGS())
	{
		return ServerMgr::Instance()->SendByRoleId(roleId, roRpc);
	}
	return SendTo(roRpc);
}

void MSLink::CloseMsLink()
{
	CNetProcessor::Instance()->Close(m_dwConnID);
	LogInfo("ms link was closed");
}

void MSLink::ReqGuildSimpleData()
{
	PtcG2M_ReqGuildSimpleData msg;
	msg.m_Data.set_gslineid(GSConfig::Instance()->GetLine());
	SendTo(msg);
}

void MSLink::ReqDragonGuildSimpleData()
{
	PtcG2M_reqdragonguildsimpledata msg;
	msg.m_Data.set_gsline(GSConfig::Instance()->GetLine());
	SendTo(msg);
}