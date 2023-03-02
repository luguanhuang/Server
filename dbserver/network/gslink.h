#ifndef __GSLINK_H__
#define __GSLINK_H__

#include "baselink.h"

class CProtocol;
class CRpc;
struct GameServerInfo
{
	UINT32 m_dwConnID;
	UINT32 m_dwServerID;
	std::string m_serverName;
};


class GSLink : public ListenLink
{
	GSLink();
	~GSLink();
	DECLARE_SINGLETON(GSLink)

public:

	virtual void OnPassiveConnect(UINT32 dwConnID);
	virtual void OnClose(UINT32 dwConnID, int err);
	virtual const std::string &GetName();

	void AddServerInfo(UINT32 dwConnID, UINT32 dwServerID, const std::string &serverName);
	GameServerInfo *GetServerInfo(UINT32 dwConnID);
	GameServerInfo *GetServerInfoByServerID(UINT32 dwServerID);
	void Close(UINT32 dwConnID);
	const std::list<GameServerInfo>& getGSConnections(){return m_gsConnections;}
	void ShowGSLinkInfo();

private:

	std::list<GameServerInfo> m_gsConnections;
};

#endif