#include "pch.h"
#include "dblink.h"
#include "network.h"
#include "netproc.h"
#include "config.h"
#include "crpc.h"


INSTANCE_SINGLETON(DBLink)


DBLink::DBLink()
:m_dwConnID(INVALID_CONN_ID)
{
}

DBLink::~DBLink()
{
}

void DBLink::OnConnectSucceed(UINT32 dwConnID)
{
	if(m_dwConnID != INVALID_CONN_ID)
	{
		LogError("more than one db is connected!");
		return;
	}

	SetConnID(dwConnID);
	LogInfo("connect to db success");
}

void DBLink::OnConnectFailed()
{
	LogError("connect to db failed!");
}

void DBLink::OnClose(UINT32 dwConnID, int err)
{
	SetConnID(INVALID_CONN_ID);
	LogInfo("db link was disconnected");
}

bool DBLink::SendTo(const CProtocol& roPtc)
{
	if (m_dwConnID == INVALID_CONN_ID)
	{
		return false;
	}

	return Send(m_dwConnID, roPtc);
}

bool DBLink::SendTo(CRpc &roRpc)
{
	if (m_dwConnID == INVALID_CONN_ID)
	{
		return false;
	}

	return Send(m_dwConnID, roRpc);
}

const std::string& DBLink::GetName()
{
	static const std::string LinkName = "dblink";
	return LinkName;
}
