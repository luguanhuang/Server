#ifndef __FM_LINK_H__
#define __FM_LINK_H__

#include "baselink.h"

class CRpc;
class CProtocol;


class FMLink : public BaseLink
{
	FMLink();
	~FMLink();
	DECLARE_SINGLETON(FMLink)
public:
	virtual void OnConnectSucceed(UINT32 dwConnID);
	virtual void OnConnectFailed();
	virtual void OnPassiveConnect(UINT32 dwConnID);
	void RemoveConnId(UINT32 connId);
	virtual void OnClose(UINT32 dwConnID, int err);
	bool IsConnected();
	virtual const std::string &GetName();
	void AddFmServer(UINT32 dwConnID, UINT32 dwLoginID);
	bool SendTo(const CProtocol& roPtc);
	bool SendTo(CRpc& roRpc);
	void SetMasterFmConnId(UINT32 connId);
private:
	UINT32 m_masterFmId;
	std::vector<UINT32> m_connIds;
};

#endif
