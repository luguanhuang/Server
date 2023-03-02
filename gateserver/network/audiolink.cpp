#include "pch.h"
#include "gslink.h"
#include "network.h"
#include "netproc.h"
#include "clientlink.h"
#include "config.h"
#include "protocol.h"
#include "crpc.h"
#include "mslink.h"
#include "audiolink.h"
#include "protocolcoder.h"


INSTANCE_SINGLETON(AudioLink)


AudioLink::AudioLink()
:m_dwConnID(INVALID_CONN_ID)
,m_bRegistered(false)
{
	SetPrintProtocol(false);
	AddPrintIgnoreProtocol(30732); 
	AddPrintIgnoreProtocol(33850); 

	SetForwardInterface(this);
}

AudioLink::~AudioLink()
{
}

void AudioLink::OnConnectSucceed(UINT32 dwConnID)
{
	SetConnID(dwConnID);
	SetRegistered(true);
}

void AudioLink::OnConnectFailed()
{
	LogError("connect to audio failed!");
}

void AudioLink::OnClose(UINT32 dwConnID, int err)
{
	SetRegistered(false);
	SetConnID(INVALID_CONN_ID);
	LogInfo("ms link was disconnected");
}

bool AudioLink::SendTo(const CProtocol& roPtc)
{
	if(GetConnID() == INVALID_CONN_ID)
	{
		return false;
	}

	return Send(GetConnID(), roPtc);
}

bool AudioLink::SendTo(const CRpc &roRpc)
{
	if(GetConnID() == INVALID_CONN_ID)
	{
		return false;
	}

	return Send(GetConnID(), const_cast<CRpc&>(roRpc));
}

const std::string& AudioLink::GetName()
{
	static const std::string LinkName = "audiolink";
	return LinkName;
}

UINT32 AudioLink::GetForwardConnID(UINT32 dwConnID, const RpcHead &head)
{
	if(IsRegistered())
	{
		return m_dwConnID;
	}
	else
	{
		CClientLink::Instance()->RpcNullReply(dwConnID, head.m_dwType, head.m_dwTagID);
		return INVALID_CONN_ID;
	}

}

UINT32 AudioLink::GetForwardConnID(UINT32 dwConnID, const ProtocolHead &head)
{
	return m_dwConnID;
}

void AudioLink::GetNewHeadInfo(const ProtocolHead& srcHead, const char* pcData, UINT32 dwDataLen, HeadInfo& newHead, DataBlob& newData)
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

void AudioLink::GetNewHeadInfo(const RpcHead& srcHead, const char* pcData, UINT32 dwDataLen, HeadInfo& newHead, DataBlob& newData)
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