#include "pch.h"
#include "mslink.h"
#include "network.h"
#include "netproc.h"
#include "config.h"
#include "crpc.h"
#include "db/mysqlmgr.h"


INSTANCE_SINGLETON(MSLink)


MSLink::MSLink()
:m_dwConnID(INVALID_CONN_ID)
{
}

MSLink::~MSLink()
{

}

void MSLink::OnPassiveConnect(UINT32 dwConnID)
{
	SetConnID(dwConnID);
	LogInfo("ms link connected");
	CMysqlMgr::Instance()->SetMSSaveDBEnd(false);
}

void MSLink::OnClose(UINT32 dwConnID, int err)
{
	SetConnID(INVALID_CONN_ID);
	LogInfo("ms link was disconnected");
	CMysqlMgr::Instance()->SetMSSaveDBEnd(true);
}

const std::string & MSLink::GetName()
{
	static const std::string LinkName = "mslink";
	return LinkName;
}