#ifndef __CENTER_SERVER_LINK_H__
#define __CENTER_SERVER_LINK_H__

#include "baselink.h"
#include "pb/enum.pb.h"

class CProtocol;
class CRpc;

class CSLink : public ListenLink
{
	CSLink();
	~CSLink();
	DECLARE_SINGLETON(CSLink)

public:

	virtual void OnPassiveConnect(UINT32 dwConnID);
	virtual void OnClose(UINT32 dwConnID, int err);
	void AddConnId(UINT32 connId);
	void RemoveConnId(UINT32 connId);
	virtual const std::string &GetName();

	void RandUsingCenterSever();
	UINT32 GetUsingCenterServerID();
private:
	std::vector<UINT32> m_connIds;
	UINT32 m_usingCenterServerConnID;
};

#endif
