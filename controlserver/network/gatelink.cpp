#include "pch.h"
#include "gatelink.h"
#include "dblink.h"
#include "netproc.h"
#include "protocol.h"
#include "config.h"
#include "gslink.h"
#include "common/ptcn2g_newgateconnected.h"
#include "account/accountsessionmgr.h"


INSTANCE_SINGLETON(GateLink)


GateLink::GateLink()
{
}

GateLink::~GateLink()
{ 
}

const std::string& GateLink::GetName()
{
	static const std::string LinkName = "gatelink";
	return LinkName;
}

void GateLink::OnLineClosed(UINT32 dwLine)
{
	m_oGtInfoMap.erase(dwLine);
	LogInfo("Gate server line [%u] disconnected!", dwLine);
	CAccountSessionMgr::Instance()->KickoutPlayerOnGateLine(dwLine);
}

void GateLink::OnLineConnected(UINT32 dwLine)
{
	LogInfo("Gate server line [%u] connected", dwLine);
}

bool GateLink::SendToSession(const CProtocol &roPtc)
{
	UINT32 dwLine = (UINT32)((roPtc.m_sessionID >> SessionGateIDShift) & 0xFF);
	return SendToLine(dwLine, roPtc);
}

KKSG::ErrorCode GateLink::AddGateServer(UINT32 dwConnID, UINT32 dwServerID, UINT32 dwLine, const std::string& strIP, UINT32 dwPort)
{
	if(dwServerID != ControlConfig::Instance()->GetServerID())
	{
		LogError("Gate server line [%u] 's server id [%u] error", dwLine, dwServerID);
		return KKSG::ERR_FAILED;
	}

	if(IsLineConnected(dwLine))
	{
		LogError("Gate server line [%u] has been registered", dwLine);
		return KKSG::ERR_FAILED;
	}

	GateServerInfo oInfo;
	oInfo.dwLine = dwLine;
	oInfo.listenIP = strIP;
	oInfo.listenPort = dwPort;
	m_oGtInfoMap[dwLine] = oInfo;


	RegisterLine(dwLine, dwConnID);

	PtcN2G_NewGateConnected oPtc;
	oPtc.m_Data.set_line(dwLine);
	oPtc.m_Data.mutable_addr()->set_ip(strIP);
	oPtc.m_Data.mutable_addr()->set_port(dwPort);
	GSLink::Instance()->SendToAllLine(oPtc);

	return KKSG::ERR_SUCCESS;
}