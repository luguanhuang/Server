#ifndef _ClientLink_H__
#define _ClientLink_H__

#include "baselink.h"

class CProtocol;

#pragma pack (4)
struct ClientProtocolHead
{
	UINT32 dwLen;
	UINT32 dwType;
	UINT32 dwFlag;
};

struct ClientRpcHead
{
	UINT32 dwLen;
	UINT32 dwType;
	UINT32 dwFlag;
	UINT32 dwTagID;
};
#pragma pack ()

class CClientLink : public ListenLink, public ForwardInterface
{
	CClientLink();
	virtual ~CClientLink();
	DECLARE_SINGLETON(CClientLink)

public:

	virtual void OnPassiveConnect(UINT32 dwConnID);
	virtual void OnClose(UINT32 dwConnID, int err);
	virtual const std::string & GetName();
	virtual UINT32 MinSize();

	virtual UINT32 GetForwardConnID(UINT32 dwConnID, const ProtocolHead &head);
	virtual UINT32 GetForwardConnID(UINT32 dwConnID, const RpcHead &head);

	virtual bool IsChangeHead() { return true; }
	virtual void GetNewHeadInfo(const ProtocolHead& srcHead, const char* pcData, UINT32 dwDataLen, HeadInfo& newHead, DataBlob& newData);
	virtual void GetNewHeadInfo(const RpcHead& srcHead, const char* pcData, UINT32 dwDataLen, HeadInfo& newHead, DataBlob& newData);

	void Close(UINT32 dwConnID);

	virtual bool Send(UINT32 dwConnID, const CProtocol& roPtc);
	virtual bool Send(UINT32 dwConnID, CRpc &roRpc);

	void RpcNullReply(UINT32 dwConnID, UINT32 dwRpcType, UINT32 dwRpcTagID);

	UINT64 GetRecvPtcNum() { return m_recvPtcNum; }	
	UINT64 GetRecvRpcNum() { return m_recvRpcNum; }
	UINT64 GetSendPtcNum() { return m_sendPtcNum; }
	UINT64 GetSendRpcNum() { return m_sendRpcNum; }

	void ClearStatNum()
	{
		m_recvPtcNum = 0; 
		m_recvRpcNum = 0;
		m_sendPtcNum = 0;
		m_sendRpcNum = 0;
	}

	virtual bool UseKeepAlive() const { return false; }
private:

	virtual HeadStruct DecodeHead(UINT32 dwConnID, const char* pcData, UINT32 dwLen);

	ClientRpcHead RpcChangeHead;
	ClientProtocolHead PtcChangeHead;
	UINT64 m_recvPtcNum;
	UINT64 m_recvRpcNum;
	UINT64 m_sendPtcNum;
	UINT64 m_sendRpcNum;
};

#endif
