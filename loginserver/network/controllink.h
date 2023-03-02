#ifndef __CONTROL_SERVER_LINK_H__
#define __CONTROL_SERVER_LINK_H__

#include "baselink.h"
#include "pb/enum.pb.h"
#include "network/mslink.h"

class CProtocol;
class CRpc;

class ControlLink : public ListenLink
{
	ControlLink();
	~ControlLink();
	DECLARE_SINGLETON(ControlLink)

public:

	virtual void OnPassiveConnect(UINT32 dwConnID);
	virtual void OnClose(UINT32 dwConnID, int err);
	virtual const std::string &GetName();

	KKSG::ErrorCode RegisterNs(UINT32 dwConnID, UINT32 dwZoneID, UINT32 dwServerID);
	void Brocast(const CProtocol& ptc);
	bool SendToServer(UINT32 serverID, const CProtocol& ptc);
	bool SendToServer(UINT32 serverID, CRpc& rpc);

private:
	std::set<UINT32> m_connIDs;
	std::map<UINT32, UINT32> m_server2ConnID;
};

#endif