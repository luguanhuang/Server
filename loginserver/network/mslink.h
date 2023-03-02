#ifndef __MS_ERVER_LINK_H__
#define __MS_ERVER_LINK_H__

#include "baselink.h"
#include "pb/enum.pb.h"

class CProtocol;
class CRpc;

class MsLink : public ListenLink
{
	MsLink();
	~MsLink();
	DECLARE_SINGLETON(MsLink)

public:
	virtual void OnPassiveConnect(UINT32 dwConnID);
	virtual void OnClose(UINT32 dwConnID, int err);
	virtual const std::string &GetName();

	KKSG::ErrorCode RegisterMs(UINT32 dwConnID, UINT32 dwZoneID, UINT32 dwServerID);

	bool IsServerConnected(UINT32 severID);
	void Brocast(const CProtocol& ptc);
	bool SendToServer(UINT32 serverID, const CProtocol& ptc);
	bool SendToServer(UINT32 serverID, CRpc& rpc);

private:
	std::set<UINT32> m_connIDs;
	std::map<UINT32, UINT32> m_server2ConnID;
};

#endif