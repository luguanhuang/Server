#include "pch.h"
#include "gslink.h"
#include "network.h"
#include "netproc.h"
#include "clientlink.h"
#include "config.h"
#include "protocol.h"
#include "crpc.h"
#include "mslink.h"
#include "common/rpct2m_registergt2ms.h"
#include "protocolcoder.h"
#include "session/sessionmanager.h"
#include "protocolban/crossprotocolbanmgr.h"


INSTANCE_SINGLETON(MSLink)


MSLink::MSLink()
:m_dwConnID(INVALID_CONN_ID)
,m_bRegistered(false)
{
	SetPrintProtocol(false);
	AddPrintIgnoreProtocol(30732); 
	AddPrintIgnoreProtocol(33850); 

	SetForwardInterface(this);
}

MSLink::~MSLink()
{
}

void MSLink::OnConnectSucceed(UINT32 dwConnID)
{
	SetConnID(dwConnID);

	RpcT2M_RegisterGt2Ms* rpc = RpcT2M_RegisterGt2Ms::CreateRpc();
	rpc->m_oArg.set_serverid(GateConfig::Instance()->GetServerID());
	rpc->m_oArg.set_line(GateConfig::Instance()->GetLine());
	SendTo(*rpc);
}

void MSLink::OnConnectFailed()
{
	LogError("connect to ms failed!");
}

void MSLink::OnClose(UINT32 dwConnID, int err)
{
	SetRegistered(false);
	SetConnID(INVALID_CONN_ID);
	LogInfo("ms link was disconnected");
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

const std::string& MSLink::GetName()
{
	static const std::string LinkName = "mslink";
	return LinkName;
}

UINT32 MSLink::GetForwardConnID(UINT32 dwConnID, const RpcHead &head)
{
	if(IsRegistered())
	{
		SessionInfo* poSession = SessionManager::Instance()->FindByConnID(dwConnID);
		if(poSession == NULL)
		{
			return m_dwConnID;
		}
		if (poSession->m_crossGs && CrossProtocolBanMgr::Instance()->IsBan(head.m_dwType))
		{
			CClientLink::Instance()->RpcNullReply(dwConnID, head.m_dwType, head.m_dwTagID);
			return INVALID_CONN_ID;
		}
		return m_dwConnID;
	}
	else
	{
		CClientLink::Instance()->RpcNullReply(dwConnID, head.m_dwType, head.m_dwTagID);
		return INVALID_CONN_ID;
	}
}

UINT32 MSLink::GetForwardConnID(UINT32 dwConnID, const ProtocolHead &head)
{
	if(IsRegistered())
	{
		SessionInfo* poSession = SessionManager::Instance()->FindByConnID(dwConnID);
		if(poSession == NULL)
		{
			return m_dwConnID;
		}
		if (poSession->m_crossGs && CrossProtocolBanMgr::Instance()->IsBan(head.m_dwType))
		{
			return INVALID_CONN_ID;
		}
		return m_dwConnID;
	}
	return INVALID_CONN_ID;
}

void MSLink::GetNewHeadInfo(const ProtocolHead& srcHead, const char* pcData, UINT32 dwDataLen, HeadInfo& newHead, DataBlob& newData)
{
	PtcChangedHead = srcHead;
	PtcChangedHead.m_dwLen = dwDataLen + sizeof(PtcChangedHead) - sizeof(UINT32);

	newHead.m_ptr = (char *)&PtcChangedHead;
	newHead.m_size = sizeof(PtcChangedHead);

	//出错，不转发
	if(!CProtocolCoder::Instance()->Decode(pcData, dwDataLen, newData, PtcChangedHead.m_dwType, PtcChangedHead.m_dwFlag))
	{
		newHead.SetNull();
	}
	else
	{
		if(!newData.IsNull())
		{
			PtcChangedHead.m_dwLen = newData.m_size + sizeof(PtcChangedHead) - sizeof(UINT32);
		}
	}
}

void MSLink::GetNewHeadInfo(const RpcHead& srcHead, const char* pcData, UINT32 dwDataLen, HeadInfo& newHead, DataBlob& newData)
{
	RpcChangedHead = srcHead;
	RpcChangedHead.m_dwLen = dwDataLen + sizeof(RpcChangedHead) - sizeof(UINT32);

	newHead.m_ptr = (char *)&RpcChangedHead;
	newHead.m_size = sizeof(RpcChangedHead);

	//出错，不转发
	if(!CProtocolCoder::Instance()->Decode(pcData, dwDataLen, newData, RpcChangedHead.m_dwType, RpcChangedHead.m_dwFlag))
	{
		newHead.SetNull();
	}
	else
	{
		if(!newData.IsNull())
		{
			RpcChangedHead.m_dwLen = newData.m_size + sizeof(RpcChangedHead) - sizeof(UINT32);
		}
	}
}