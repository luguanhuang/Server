#include "pch.h"
#include "gslink.h"
#include "network.h"
#include "netproc.h"
#include "clientlink.h"
#include "config.h"
#include "protocol.h"
#include "crpc.h"
#include "controllink.h"
#include "common/rpct2n_registergt2ns.h"
#include "protocolcoder.h"


INSTANCE_SINGLETON(ControlLink)


ControlLink::ControlLink()
:m_dwConnID(INVALID_CONN_ID)
,m_bRegistered(false)
{
	SetPrintProtocol(false);
	AddPrintIgnoreProtocol(30732); 
	AddPrintIgnoreProtocol(33850); 

	SetForwardInterface(this);
}

ControlLink::~ControlLink()
{
}

void ControlLink::OnConnectSucceed(UINT32 dwConnID)
{
	SetConnID(dwConnID);

	RpcT2N_RegisterGt2Ns* rpc = RpcT2N_RegisterGt2Ns::CreateRpc();
	rpc->m_oArg.set_serverid(GateConfig::Instance()->GetServerID());
	rpc->m_oArg.set_line(GateConfig::Instance()->GetLine());
	rpc->m_oArg.mutable_addr()->set_ip(GateConfig::Instance()->GetGsListenIP());
	rpc->m_oArg.mutable_addr()->set_port(GateConfig::Instance()->GetGsListenPort());
	SendTo(*rpc);
}

void ControlLink::OnConnectFailed()
{
	LogError("connect to ns failed!");
}

void ControlLink::OnClose(UINT32 dwConnID, int err)
{
	SetRegistered(false);
	SetConnID(INVALID_CONN_ID);
	LogInfo("ns link was disconnected");
}

bool ControlLink::SendTo(const CProtocol& roPtc)
{
	if(GetConnID() == INVALID_CONN_ID)
	{
		return false;
	}

	return Send(GetConnID(), roPtc);
}

bool ControlLink::SendTo(const CRpc &roRpc)
{
	if(GetConnID() == INVALID_CONN_ID)
	{
		return false;
	}

	return Send(GetConnID(), const_cast<CRpc&>(roRpc));
}

const std::string& ControlLink::GetName()
{
	static const std::string LinkName = "ctrllink";
	return LinkName;
}

UINT32 ControlLink::GetForwardConnID(UINT32 dwConnID, const RpcHead &head)
{
	return m_dwConnID;
}

UINT32 ControlLink::GetForwardConnID(UINT32 dwConnID, const ProtocolHead &head)
{
	return m_dwConnID;
}

void ControlLink::GetNewHeadInfo(const ProtocolHead& srcHead, const char* pcData, UINT32 dwDataLen, HeadInfo& newHead, DataBlob& newData)
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

void ControlLink::GetNewHeadInfo(const RpcHead& srcHead, const char* pcData, UINT32 dwDataLen, HeadInfo& newHead, DataBlob& newData)
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