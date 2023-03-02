#ifndef __DBLINK_H__
#define __DBLINK_H__

#include "baselink.h"

class CRpc;
class CProtocol;

class DBLink : public BaseLink
{
	DBLink();
	~DBLink();
	DECLARE_SINGLETON(DBLink)
public:
	virtual void OnConnectSucceed(UINT32 dwConnID);
	virtual void OnConnectFailed();
	virtual void OnPassiveConnect(UINT32 dwConnID) {}
	virtual void OnClose(UINT32 dwConnID, int err);
	virtual const std::string& GetName();


	bool SendTo(const CProtocol& roPtc);
	bool SendTo(CRpc &roRpc);


	UINT32	GetConnID() const { return m_dwConnID; }
	void	SetConnID(UINT32 dwConnID) { m_dwConnID = dwConnID; }
	bool	IsConnected() const { return m_dwConnID != INVALID_CONN_ID; }
private:
	UINT32 m_dwConnID;
};

#endif