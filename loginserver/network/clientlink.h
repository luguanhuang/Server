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

class CClientLink : public ListenLink //, public ForwardInterface
{
	CClientLink();
	virtual ~CClientLink();
	DECLARE_SINGLETON(CClientLink)

public:

	virtual void OnPassiveConnect(UINT32 dwConnID);
	virtual void OnClose(UINT32 dwConnID, int err);
	virtual const std::string & GetName();
	virtual UINT32 MinSize();

	//virtual UINT32 GetForwardConnID(UINT32 dwConnID, const ProtocolHead &head);
	//virtual UINT32 GetForwardConnID(UINT32 dwConnID, const RpcHead &head);

	//virtual bool IsChangeHead() { return true; }
	//virtual HeadInfo GetNewHeadInfo(const ProtocolHead &head, UINT32 dataLen);
	//virtual HeadInfo GetNewHeadInfo(const RpcHead &head, UINT32 dataLen);

	void Close(UINT32 dwConnID);

	virtual bool Send(UINT32 dwConnID, const CProtocol& roPtc);
	virtual bool Send(UINT32 dwConnID, CRpc &roRpc);

	virtual bool UseKeepAlive() const { return false; }
private:

	virtual HeadStruct DecodeHead(UINT32 dwConnID, const char* pcData, UINT32 dwLen);

	ClientRpcHead RpcChangeHead;
	ClientProtocolHead PtcChangeHead;
};

#endif
