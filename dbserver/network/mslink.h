#ifndef __MSLINK_H__
#define __MSLINK_H__

#include "baselink.h"


class MSLink : public BaseLink
{
	MSLink();
	~MSLink();
	DECLARE_SINGLETON(MSLink)
public:
	virtual void OnConnectSucceed(UINT32 dwConnID) {}
	virtual void OnConnectFailed() {}
	virtual void OnPassiveConnect(UINT32 dwConnID);
	virtual void OnClose(UINT32 dwConnID, int err);
	virtual const std::string &GetName();

	//Send to ms
	bool SendTo(const CProtocol& roPtc);
	bool SendTo(const CRpc &roRpc);

	UINT32	GetConnID() const { return m_dwConnID; }
	void	SetConnID(UINT32 dwConnID) { m_dwConnID = dwConnID; }
private:
	UINT32	m_dwConnID;
};

#endif