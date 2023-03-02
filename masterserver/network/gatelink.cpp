#include "pch.h"
#include "gatelink.h"
#include "dblink.h"
#include "netproc.h"
#include "protocol.h"
#include "config.h"
#include "gslink.h"
#include "common/ptcg2t_opengate.h"


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
	LogInfo("Gate server line [%u] disconnected!", dwLine);
	//CAccountSessionMgr::Instance()->KickoutPlayerOnGateLine(dwLine);
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

KKSG::ErrorCode GateLink::AddGateServer(UINT32 dwConnID, UINT32 dwServerID, UINT32 dwLine)
{
	if(dwServerID != MSConfig::Instance()->GetServerID())
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
}