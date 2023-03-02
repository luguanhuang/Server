#include "pch.h"
#include "mslink.h"
#include "network.h"
#include "netproc.h"
#include "config.h"
#include "crpc.h"
#include "common/ptcn2m_updatefunctionopen2ms.h"


INSTANCE_SINGLETON(MSLink)


MSLink::MSLink()
:m_dwConnID(INVALID_CONN_ID)
,m_hasVersion(false)
{
}

MSLink::~MSLink()
{

}

void MSLink::OnPassiveConnect(UINT32 dwConnID)
{
	SetConnID(dwConnID);
	LogInfo("ms link connected");
	/*
	PtcN2M_UpdateFunctionOpen2Ms ntf;
	ntf.m_Data.set_flag(FunctionOpenMgr::Instance()->GetOpenFlag());
	SendTo(ntf);
	*/
}

void MSLink::OnClose(UINT32 dwConnID, int err)
{
	SetConnID(INVALID_CONN_ID);
	LogInfo("ms link was disconnected");

	SetVersioned(false);
}

const std::string & MSLink::GetName()
{
	static const std::string LinkName = "mslink";
	return LinkName;
}


bool MSLink::SendTo(const CProtocol& roPtc)
{
	if(GetConnID() == INVALID_CONN_ID)
	{
		return false;
	}

	return Send(GetConnID(), roPtc);
}

bool MSLink::SendTo(const CRpc &roRpc)
{
	if(GetConnID() == INVALID_CONN_ID)
	{
		return false;
	}

	return Send(GetConnID(), const_cast<CRpc&>(roRpc));
}