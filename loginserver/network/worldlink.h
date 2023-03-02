#ifndef __WORLD_LINK_H__
#define __WORLD_LINK_H__

#include "baselink.h"
#include "pb/enum.pb.h"

class CProtocol;
class CRpc;

class WorldLink : public ListenLink
{
	WorldLink();
	~WorldLink();
	DECLARE_SINGLETON(WorldLink);

public:

	virtual void OnPassiveConnect(UINT32 dwConnID);
	virtual void OnClose(UINT32 dwConnID, int err);
	virtual const std::string &GetName();

	void SendToAll(const CProtocol& ptc);

private:
	void AddConnId(UINT32 connId);
	void RemoveConnId(UINT32 connId);

private:
	std::vector<UINT32> m_connIds;
};

#endif
