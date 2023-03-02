#include "pch.h"
#include "gslink.h"
#include "network.h"
#include "netproc.h"
#include "clientlink.h"
#include "config.h"
#include "protocol.h"
#include "crpc.h"
#include "common/rpct2g_registergt2gs.h"
#include "session/sessionmanager.h"
#include "protocolstat.h"
#include "protocolcoder.h"
#include "rslink.h"
#include "protocolban/crossprotocolbanmgr.h"


INSTANCE_SINGLETON(GSLink)

GSLink::GSLink()
{
	SetPrintProtocol(false);
	AddPrintIgnoreProtocol(30732); 
	AddPrintIgnoreProtocol(33850); 

	m_GateServerMask = 0;

	SetForwardInterface(this);
}

GSLink::~GSLink()
{

}

void GSLink::OnPassiveConnect(UINT32 dwConnID)
{
	RpcT2G_RegisterGt2Gs* rpc = RpcT2G_RegisterGt2Gs::CreateRpc();
	rpc->m_oArg.set_serverid(GateConfig::Instance()->GetServerID());
	rpc->m_oArg.set_line(GateConfig::Instance()->GetLine());
	BaseLink::Send(dwConnID, *rpc);
}

void GSLink::OnClose(UINT32 dwConnID, int err)
{
	LineLink::OnClose(dwConnID, err);
	//SessionManager::Instance()->GSShutDownCloseAllSession(dwGSId);
}

const std::string & GSLink::GetName()
{
	static const std::string LinkName = "gslink";
	return LinkName;
}

SessionType GSLink::GetGateServerMask()
{
	if (m_GateServerMask == 0)
	{
		m_GateServerMask = GateConfig::Instance()->GetServerID();
		m_GateServerMask <<= SessionGateIDShift;
	}

	return m_GateServerMask;
}

UINT32 GSLink::GetForwardConnID(UINT32 dwConnID, const RpcHead &head)
{
	SessionInfo* poSession = SessionManager::Instance()->FindByConnID(dwConnID);
	if(poSession == NULL)
	{
		return INVALID_CONN_ID;
	}

	CProtocolStat::Instance()->OnRecv(poSession->m_sessionID, head.m_dwType, head.m_dwLen);
	if (poSession->m_crossGs)
	{
		if (CrossProtocolBanMgr::Instance()->IsBan(head.m_dwType))
		{
			CClientLink::Instance()->RpcNullReply(dwConnID, head.m_dwType, head.m_dwTagID);
			return INVALID_CONN_ID;
		}
		return RSLink::Instance()->GetConnId();
	}
	return GetConnID(poSession->m_dwGsLine);
}

UINT32 GSLink::GetForwardConnID(UINT32 dwConnID, const ProtocolHead &head)
{
	SessionInfo* poSession = SessionManager::Instance()->FindByConnID(dwConnID);
	if(poSession == NULL)
	{
		return INVALID_CONN_ID;
	}

	if (poSession->m_crossGs)
	{
		if (CrossProtocolBanMgr::Instance()->IsBan(head.m_dwType))
		{
			return INVALID_CONN_ID;
		}
		return RSLink::Instance()->GetConnId();
	}
	return GetConnID(poSession->m_dwGsLine);
}

void GSLink::GetNewHeadInfo(const ProtocolHead& srcHead, const char* pcData, UINT32 dwDataLen, HeadInfo& newHead, DataBlob& newData)
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

void GSLink::GetNewHeadInfo(const RpcHead& srcHead, const char* pcData, UINT32 dwDataLen, HeadInfo& newHead, DataBlob& newData)
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

void GSLink::OnLineClosed(UINT32 dwLine)
{
	LogInfo("Game server line [%u] disconnected!", dwLine);
}

void GSLink::OnLineConnected(UINT32 dwLine)
{
	LogInfo("Game server line [%u] connected", dwLine);
}

KKSG::ErrorCode GSLink::AddGameServer(UINT32 dwConnID, UINT32 dwServerID, UINT32 dwLine)
{
	if(dwServerID != GateConfig::Instance()->GetServerID())
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