#ifndef __TEAM_LINK_H__
#define __TEAM_LINK_H__

#include "baselink.h"

class CRpc;
class CProtocol;

class TeamLink : public BaseLink
{
	TeamLink();
	~TeamLink();
	DECLARE_SINGLETON(TeamLink)
public:
	virtual void OnConnectSucceed(UINT32 dwConnID);
	virtual void OnConnectFailed();
	virtual void OnPassiveConnect(UINT32 dwConnID){}
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