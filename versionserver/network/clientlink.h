#ifndef _ClientLink_H__
#define _ClientLink_H__

#include "baselink.h"

class CProtocol;

#pragma pack (4)
struct ClientProtocolHead
{
	UINT32 dwLen;
};

//struct ClientRpcHead
//{
//	UINT32 dwLen;
//	UINT32 dwType;
//	UINT32 dwFlag;
//	UINT32 dwTagID;
//};
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

	void Close(UINT32 dwConnID);

	virtual bool UseKeepAlive() const { return false; }
private:
	bool Send(UINT32 dwConnID);
	void SuffixWrite(MemoryStream* stream,  const std::string& info, int& offset);

private:
	//ClientRpcHead RpcChangeHead;
	ClientProtocolHead PtcChangeHead;
};

#endif
