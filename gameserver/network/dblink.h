#ifndef __DBLINK_H__
#define __DBLINK_H__

#include "baselink.h"

class CRpc;
class CProtocol;

class DBLink : public BaseLink, public ForwardInterface
{
	DBLink();
	~DBLink();
	DECLARE_SINGLETON(DBLink)
public:

	virtual void OnConnectSucceed(UINT32 dwConnID);
	virtual void OnConnectFailed();
	virtual void OnPassiveConnect(UINT32 dwConnID);
	virtual void OnClose(UINT32 dwConnID, int err);
	virtual const std::string &GetName();

	virtual UINT32 GetForwardConnID(UINT32 dwConnID, const ProtocolHead &head) { return m_dwConnID; }
	virtual UINT32 GetForwardConnID(UINT32 dwConnID, const RpcHead &head) { return m_dwConnID; }

	void Close();
	bool Send(const CProtocol& roPtc);
	bool Send(CRpc &roRpc);
	bool IsConnected();

private:

	UINT32 m_dwConnID;
};

#endif