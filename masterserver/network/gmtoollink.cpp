#include "pch.h"
#include "gmtoollink.h"

INSTANCE_SINGLETON(GMToolLink)


GMToolLink::GMToolLink()
:m_dwConnID(INVALID_CONN_ID)
{
}

GMToolLink::~GMToolLink()
{
}

void GMToolLink::OnPassiveConnect(UINT32 dwConnID)
{
	if(m_dwConnID != INVALID_CONN_ID)
	{
		LogError("more than one gmtool is connected!");
		return;
	}

	SetConnID(dwConnID);
	LogInfo("gmtool link connected");
}

void GMToolLink::OnClose(UINT32 dwConnID, int err)
{
	SetConnID(INVALID_CONN_ID);
	LogInfo("gmtool link was disconnected");
}

const std::string& GMToolLink::GetName()
{
	static const std::string name = "gmtoollink";
	return name;
}