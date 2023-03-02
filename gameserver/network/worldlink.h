#ifndef _GS_WORLD_LINK_H_
#define _GS_WORLD_LINK_H_

#include "baselink.h"

class WorldLink : public BaseLink
{
	WorldLink();
	~WorldLink();
	DECLARE_SINGLETON(WorldLink)
public:
	virtual void OnConnectSucceed(UINT32 dwConnID);
	virtual void OnConnectFailed();
	virtual void OnPassiveConnect(UINT32 dwConnID) {}
	virtual void OnClose(UINT32 dwConnID, int err);
	virtual const std::string &GetName();

	bool SendTo(const CProtocol& ptc);
	bool SendTo(CRpc& rpc);

	bool IsConnected();
	void SetMasterWorldConnId(UINT32 connId);

private:
	void AddConnId(UINT32 connId);
	void RemoveConnId(UINT32 connId);

private:
	std::vector<UINT32> m_connIds;
	UINT32 m_masterWorldId;
};

#endif
