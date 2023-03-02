#include "pch.h"
#include "dblink.h"
#include "network.h"
#include "netproc.h"
#include "config.h"
#include "crpc.h"
#include "common/ptcg2d_registgameserver.h"
#include "common/ptcg2t_opengate.h"
#include "gamelogic/rolesummarymgr.h"
#include "gatelink.h"
#include "guild/guildmgr.h"

INSTANCE_SINGLETON(DBLink)

DBLink::DBLink()
{
	AddPrintIgnoreProtocol(51949);
	
	SetForwardInterface(this);

	m_dwConnID = INVALID_CONN_ID;
}

DBLink::~DBLink()
{

}

void DBLink::OnConnectSucceed(UINT32 dwConnID)
{
	LogInfo("connect to db ok!");

	if (m_dwConnID != INVALID_CONN_ID)
	{
		LogError("more than one db is connected!");
	}

	m_dwConnID = dwConnID;

	PtcG2D_RegistGameServer oRegistPtc;
	oRegistPtc.m_Data.set_serverid(GSConfig::Instance()->GetServerID());
	oRegistPtc.m_Data.set_servername(GSConfig::Instance()->GetServerName());
	
	BaseLink::Send(dwConnID, oRegistPtc);
}

void DBLink::OnConnectFailed()
{
	LogError("connect to db failed!");
	m_dwConnID = INVALID_CONN_ID;
}

void DBLink::OnPassiveConnect(UINT32 dwConnID)
{
	// GS主动连接DB，不应该进这个函数
	LogError("%s", __FUNCTION__);
	assert(false);
}

void DBLink::OnClose(UINT32 dwConnID, int err)
{
	LogError("%s", __FUNCTION__);

	if (m_dwConnID != INVALID_CONN_ID && m_dwConnID != dwConnID)
	{
		LogError("closed connid %d is not matched %d!", dwConnID, m_dwConnID);
	}

	m_dwConnID = INVALID_CONN_ID;
}


void DBLink::Close()
{
	LogError("%s", __FUNCTION__);

	if (m_dwConnID == INVALID_CONN_ID)
	{
		LogError("%s connID is invalid!", __FUNCTION__);
		return;
	}

	CNetProcessor::Instance()->Close(m_dwConnID);
	m_dwConnID = INVALID_CONN_ID;
}

bool DBLink::Send(const CProtocol& roPtc)
{
	if (m_dwConnID == INVALID_CONN_ID)
	{
		LogError("%s connID is invalid!", __FUNCTION__);
		return false;
	}

	return BaseLink::Send(m_dwConnID, roPtc);
}

bool DBLink::Send(CRpc &roRpc)
{
	if (m_dwConnID == INVALID_CONN_ID)
	{
		LogError("%s connID is invalid!", __FUNCTION__);
		return false;
	}

	return BaseLink::Send(m_dwConnID, roRpc);
}

bool DBLink::IsConnected()
{
	return m_dwConnID != INVALID_CONN_ID;
}

const std::string & DBLink::GetName()
{
	static const std::string LinkName = "dblink";
	return LinkName;
}
