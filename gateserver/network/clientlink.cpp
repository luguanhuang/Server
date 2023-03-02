#include "pch.h"
#include "clientlink.h"
#include "netproc.h"
#include "config.h"
#include "timeutil.h"
#include "session/sessionmanager.h"
#include "crpc.h"
#include "protocol.h"
#include "LogInit.h"
#include "forward.h"
#include "gslink.h"
#include "serverstateinfo.h"
#include "protocolstat.h"
#include "protocolcoder.h"


INSTANCE_SINGLETON(CClientLink)

CClientLink::CClientLink()
:m_recvPtcNum(0)
,m_recvRpcNum(0)
,m_sendPtcNum(0)
,m_sendRpcNum(0)
{ 
	SetPrintProtocol(false);
	AddPrintIgnoreProtocol(30732); // 移动
	AddPrintIgnoreProtocol(33850); 

	SetForwardInterface(this);
}

CClientLink::~CClientLink()
{ 
}

void CClientLink::OnPassiveConnect(UINT32 dwConnID)
{
	//if (!GSLink::Instance()->IsAnyGSConnected())
	//{
	//	LogWarn("No gameserver is connected to gateserver, disconnect client!");
	//	Close(dwConnID);
	//	return;
	//}

	SessionManager::Instance()->OnNewConnection(dwConnID);
}

void CClientLink::OnClose(UINT32 dwConnID, int err)
{
	// 客户端正常关闭socket
	if (err == 0)
	{
	}

	SessionManager::Instance()->OnClose(dwConnID);
}

const std::string & CClientLink::GetName()
{
	static const std::string LinkName = "clientlink";
	return LinkName;
}

void CClientLink::Close(UINT32 dwConnID)
{
	CNetProcessor::Instance()->Close(dwConnID);
}

bool CClientLink::Send(UINT32 dwConnID, const CProtocol& roPtc)
{
	MemoryStream *stream = MemoryStream::Get();
	// 发送给客户端的包头，不包含Session字段
	const int HeadSize = sizeof(ClientProtocolHead);
	stream->Seek(HeadSize);

	int len = roPtc.Serialize(stream->GetWritePosition(), stream->GetSpace());
	if (len < 0)
	{
		LogError("serialize protocol [%u] failed.", roPtc.m_dwType);
		return false;
	}

	stream->Advance(len);

	ClientProtocolHead head;
	head.dwLen = stream->GetLength() - sizeof(UINT32);
	head.dwType = roPtc.m_dwType;
	head.dwFlag = PTC_TYPE_FLAG;

	stream->Seek(0);
	stream->Write((char *)&head, sizeof(head));

	return CNetProcessor::Instance()->Send(dwConnID, stream->GetBuffer(), stream->GetLength());
}

bool CClientLink::Send(UINT32 dwConnID, CRpc &roRpc)
{
	MemoryStream *stream = MemoryStream::Get();

	const int HeadSize = sizeof(ClientRpcHead);
	stream->Seek(HeadSize);

	int len = roRpc.Serialize(stream->GetWritePosition(), stream->GetSpace());
	if (len < 0)
	{
		LogError("serialize rpc [%u] failed.", roRpc.m_dwType);
		return false;
	}

	stream->Advance(len);
	roRpc.OnSend();

	ClientRpcHead head;
	head.dwLen = stream->GetLength() - sizeof(UINT32);
	head.dwType = roRpc.m_dwType;
	head.dwTagID = roRpc.m_dwTag;
	head.dwFlag = (roRpc.GetEndType() == RPC_CLIENT) ? RPC_TYPE_REQUEST : RPC_TYPE_REPLY;

	stream->Seek(0);
	stream->Write((char *)&head, sizeof(head));

	return CNetProcessor::Instance()->Send(dwConnID, stream->GetBuffer(), stream->GetLength());
}

void CClientLink::RpcNullReply(UINT32 dwConnID, UINT32 dwRpcType, UINT32 dwRpcTagID)
{
	MemoryStream *stream = MemoryStream::Get();

	const int HeadSize = sizeof(ClientRpcHead);
	stream->Seek(0);

	ClientRpcHead head;
	head.dwLen = HeadSize - sizeof(UINT32);
	head.dwType = dwRpcType;
	head.dwTagID = dwRpcTagID;
	head.dwFlag = (RPC_TYPE_REPLY | RPC_TYPE_NULL_REPLY);

	stream->Write((char *)&head, sizeof(head));
	CNetProcessor::Instance()->Send(dwConnID, stream->GetBuffer(), stream->GetLength());
}


HeadStruct CClientLink::DecodeHead(UINT32 dwConnID, const char* pcData, UINT32 dwLen)
{
	HeadStruct RetInfo;

	ClientProtocolHead head;
	memcpy(&head, pcData, sizeof(head));

	if ((head.dwFlag & RPC_TYPE_FLAG))
	{
		ClientRpcHead rpcHead;
		memcpy(&rpcHead, pcData, sizeof(ClientRpcHead));

		RetInfo.m_IsPtcHead = false;
		RetInfo.m_dwHeadSize = sizeof(ClientRpcHead);
		RetInfo.m_oRpcHead.m_dwType = rpcHead.dwType;
		RetInfo.m_oRpcHead.m_dwLen = rpcHead.dwLen;
		RetInfo.m_oRpcHead.m_dwFlag = rpcHead.dwFlag;
		RetInfo.m_oRpcHead.m_dwTagID = rpcHead.dwTagID;
		RetInfo.m_oRpcHead.m_dwSessionID = SessionManager::Instance()->SessionConvert(dwConnID, rpcHead.dwType);
		m_recvRpcNum++;
	}
	else
	{
		RetInfo.m_IsPtcHead = true;
		RetInfo.m_dwHeadSize = sizeof(ClientProtocolHead);
		RetInfo.m_oPtcHead.m_dwType = head.dwType;
		RetInfo.m_oPtcHead.m_dwLen = head.dwLen;
		RetInfo.m_oPtcHead.m_dwFlag = head.dwFlag;
		RetInfo.m_oPtcHead.m_dwSessionID = SessionManager::Instance()->SessionConvert(dwConnID, head.dwType);
		m_recvPtcNum++;
	}

	return RetInfo;
}

UINT32 CClientLink::MinSize()
{
	return sizeof(ClientProtocolHead);
}

UINT32 CClientLink::GetForwardConnID(UINT32 dwConnID, const ProtocolHead &head)
{
//	SSDebug << "protocol type: " << head.m_dwType << END;
	m_sendPtcNum++;
	return SessionManager::Instance()->GetSessionConnID(head.m_dwSessionID);
}

UINT32 CClientLink::GetForwardConnID(UINT32 dwConnID, const RpcHead &head)
{
//	SSDebug << "rpc type: " << head.m_dwType << END;
	m_sendRpcNum++;
	CProtocolStat::Instance()->OnReply(head.m_dwSessionID, head.m_dwType);
	return SessionManager::Instance()->GetSessionConnID(head.m_dwSessionID);
}

void CClientLink::GetNewHeadInfo(const ProtocolHead& srcHead, const char* pcData, UINT32 dwDataLen, HeadInfo& newHead, DataBlob& newData)
{
	// dwLen = 包体长度 + 包头长度 - 4 字节
	PtcChangeHead.dwLen = dwDataLen + sizeof(PtcChangeHead) - sizeof(UINT32);
	PtcChangeHead.dwType = srcHead.m_dwType;
	PtcChangeHead.dwFlag = srcHead.m_dwFlag;

	newHead.m_ptr = (char *)&PtcChangeHead;
	newHead.m_size = sizeof(PtcChangeHead);

	CProtocolCoder::Instance()->Encode(pcData, dwDataLen, newData, PtcChangeHead.dwType, PtcChangeHead.dwFlag);
	if(!newData.IsNull())
	{
		PtcChangeHead.dwLen = newData.m_size + sizeof(PtcChangeHead) - sizeof(UINT32);
	}
}

void CClientLink::GetNewHeadInfo(const RpcHead& srcHead, const char* pcData, UINT32 dwDataLen, HeadInfo& newHead, DataBlob& newData)
{
	// dwLen = 包体长度 + 包头长度 - 4 字节
	RpcChangeHead.dwLen = dwDataLen + sizeof(RpcChangeHead) - sizeof(UINT32);
	RpcChangeHead.dwFlag = srcHead.m_dwFlag;
	RpcChangeHead.dwTagID = srcHead.m_dwTagID;
	RpcChangeHead.dwType = srcHead.m_dwType;

	newHead.m_ptr = (char *)&RpcChangeHead;
	newHead.m_size = sizeof(RpcChangeHead);

	CProtocolCoder::Instance()->Encode(pcData, dwDataLen, newData, RpcChangeHead.dwType, RpcChangeHead.dwFlag);
	if(!newData.IsNull())
	{
		RpcChangeHead.dwLen = newData.m_size + sizeof(RpcChangeHead) - sizeof(UINT32);
	}
}

