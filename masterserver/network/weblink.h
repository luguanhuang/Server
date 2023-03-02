#ifndef __WEBLINK_H__
#define __WEBLINK_H__

#include "baselink.h"

class CRpc;
class CProtocol;

class WebLink : public BaseLink
{
	WebLink();
	~WebLink();
	DECLARE_SINGLETON(WebLink)
public:

	virtual void OnConnectSucceed(UINT32 dwConnID);
	virtual void OnConnectFailed();
	virtual void OnPassiveConnect(UINT32 dwConnID);
	virtual void OnClose(UINT32 dwConnID, int err);
	virtual const std::string &GetName();

	void Close();
	bool Send(const CProtocol& roPtc);
	bool Send(CRpc& rpc);

private:

	UINT32 m_dwConnID;
};

#endif