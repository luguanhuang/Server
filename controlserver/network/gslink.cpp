#include "pch.h"
#include "gslink.h"
#include "config.h"
#include "network.h"
#include "netproc.h"
#include "scene/gsmanager.h"


INSTANCE_SINGLETON(GSLink);


GSLink::GSLink()
{
}

GSLink::~GSLink()
{
}

const std::string & GSLink::GetName()
{
	static const std::string GSLinkName = "gslink";
	return GSLinkName;
}

void GSLink::OnLineClosed(UINT32 dwLine)
{
	LogInfo("Game server line [%u] disconnected!", dwLine);

	CGsManager::Instance()->OnLineClosed(dwLine);
}

void GSLink::OnLineConnected(UINT32 dwLine)
{
	LogInfo("Game server line [%u] connected", dwLine);

	CGsManager::Instance()->OnLineConnected(dwLine);
}

KKSG::ErrorCode GSLink::AddGameServer(UINT32 dwConnID, UINT32 dwServerID, UINT32 dwLine)
{
	if(dwServerID != ControlConfig::Instance()->GetServerID())
	{
		LogError("Game server line [%u] 's server id [%u] error", dwLine, dwServerID);
		return KKSG::ERR_FAILED;
	}

	if(IsLineConnected(dwLine))
	{
		LogError("Game server line [%u] has been registered", dwLine);
		return KKSG::ERR_FAILED;
	}

	RegisterLine(dwLine, dwConnID);
	return KKSG::ERR_SUCCESS;
}