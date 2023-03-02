#include "pch.h"
#include "weblink.h"
#include "network.h"
#include "netproc.h"
#include "config.h"
#include "crpc.h"
#include "utility/loghelper.h"
#include "common/ptcg2w_registgameservertowebserver.h"


INSTANCE_SINGLETON(WebLink)

WebLink::WebLink()
{
	m_dwConnID = INVALID_CONN_ID;
}

WebLink::~WebLink()
{

}

void WebLink::OnConnectSucceed(UINT32 dwConnID)
{
	LogInfo("connect to web server ok!");

	if (m_dwConnID != INVALID_CONN_ID)
	{
		LogError("more than one web server is connected!");
	}

	m_dwConnID = dwConnID;

	PtcG2W_RegistGameServerToWebServer oRegistPtc;
	oRegistPtc.m_Data.set_serverid(GSConfig::Instance()->GetServerID());
	oRegistPtc.m_Data.set_servername(GSConfig::Instance()->GetServerName());
	BaseLink::Send(dwConnID, oRegistPtc);
}

void WebLink::OnConnectFailed()
{
	LogError("connect to web server failed!");
	m_dwConnID = INVALID_CONN_ID;
}

void WebLink::OnPassiveConnect(UINT32 dwConnID)
{
	// GS主动连接DB，不应该进这个函数
	LogError("%s", __FUNCTION__);
	assert(false);
}

void WebLink::OnClose(UINT32 dwConnID, int err)
{
	LogError("%s", __FUNCTION__);

	m_dwConnID = INVALID_CONN_ID;
}


void WebLink::Close()
{
	LogError("%s", __FUNCTION__);

	if (m_dwConnID == INVALID_CONN_ID)
	{
		LogError("weblink %s connID is invalid!", __FUNCTION__);
		return;
	}

	CNetProcessor::Instance()->Close(m_dwConnID);
	m_dwConnID = INVALID_CONN_ID;
}

bool WebLink::Send(const CProtocol& roPtc)
{
	if (m_dwConnID == INVALID_CONN_ID)
	{
		LogError("weblink %s connID is invalid!", __FUNCTION__);
		return false;
	}

	return BaseLink::Send(m_dwConnID, roPtc);
}

bool WebLink::Send(CRpc& rpc)
{
	if (m_dwConnID == INVALID_CONN_ID)
	{
		LogError("weblink %s connID is invalid!", __FUNCTION__);
		return false;
	}

	return BaseLink::Send(m_dwConnID, rpc);
}

const std::string & WebLink::GetName()
{
	static const std::string LinkName = "weblink";
	return LinkName;
}

