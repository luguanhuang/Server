#include "pch.h"
#include "loglink.h"
#include "network.h"
#include "netproc.h"
#include "config.h"
#include "crpc.h"
#include "utility/loghelper.h"


INSTANCE_SINGLETON(LogLink)

LogLink::LogLink()
{
	m_dwConnID = INVALID_CONN_ID;
}

LogLink::~LogLink()
{

}

void LogLink::OnConnectSucceed(UINT32 dwConnID)
{
	LogInfo("connect to log server ok!");

	if (m_dwConnID != INVALID_CONN_ID)
	{
		LogError("more than one log server is connected!");
	}

	m_dwConnID = dwConnID;
}

void LogLink::OnConnectFailed()
{
	LogError("connect to log server failed!");
	m_dwConnID = INVALID_CONN_ID;
}

void LogLink::OnPassiveConnect(UINT32 dwConnID)
{
	// GS主动连接DB，不应该进这个函数
	LogError("%s", __FUNCTION__);
	assert(false);
}

void LogLink::OnClose(UINT32 dwConnID, int err)
{
	LogError("%s", __FUNCTION__);

	m_dwConnID = INVALID_CONN_ID;
}


void LogLink::Close()
{
	LogError("%s", __FUNCTION__);

	if (m_dwConnID == INVALID_CONN_ID)
	{
		LogError("loglink %s connID is invalid!", __FUNCTION__);
		return;
	}

	CNetProcessor::Instance()->Close(m_dwConnID);
	m_dwConnID = INVALID_CONN_ID;
}

bool LogLink::Send(const CProtocol& roPtc)
{
	if (m_dwConnID == INVALID_CONN_ID)
	{
		LogError("loglink %s connID is invalid!", __FUNCTION__);
		return false;
	}

	return BaseLink::Send(m_dwConnID, roPtc);
}

const std::string & LogLink::GetName()
{
	static const std::string LinkName = "loglink";
	return LinkName;
}

