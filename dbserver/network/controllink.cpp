#include "pch.h"
#include "controllink.h"
#include "network.h"
#include "netproc.h"
#include "config.h"
#include "crpc.h"


INSTANCE_SINGLETON(ControlLink)


ControlLink::ControlLink()
:m_dwConnID(INVALID_CONN_ID)
{
}

ControlLink::~ControlLink()
{

}

void ControlLink::OnPassiveConnect(UINT32 dwConnID)
{
	SetConnID(dwConnID);
	LogInfo("ns link connected");
}

void ControlLink::OnClose(UINT32 dwConnID, int err)
{
	SetConnID(INVALID_CONN_ID);
	LogInfo("ns link was disconnected");
}

const std::string & ControlLink::GetName()
{
	static const std::string LinkName = "ctrllink";
	return LinkName;
}

bool ControlLink::SendTo(const CProtocol& roPtc)
{
	if (m_dwConnID == INVALID_CONN_ID)
	{
		return false;
	}

	return Send(m_dwConnID, roPtc);
}

bool ControlLink::SendTo(CRpc &roRpc)
{
	if (m_dwConnID == INVALID_CONN_ID)
	{
		return false;
	}

	return Send(m_dwConnID, roRpc);
}