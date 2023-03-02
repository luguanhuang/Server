#ifndef __CONTROL_LINK_H__
#define __CONTROL_LINK_H__

#include "baselink.h"


class ControlLink : public BaseLink
{
	ControlLink();
	~ControlLink();
	DECLARE_SINGLETON(ControlLink)
public:
	virtual void OnConnectSucceed(UINT32 dwConnID) {}
	virtual void OnConnectFailed() {}
	virtual void OnPassiveConnect(UINT32 dwConnID);
	virtual void OnClose(UINT32 dwConnID, int err);
	virtual const std::string &GetName();

	//Send to ns
	bool SendTo(const CProtocol& roPtc);
	bool SendTo(CRpc &roRpc);

	UINT32	GetConnID() const { return m_dwConnID; }
	void	SetConnID(UINT32 dwConnID) { m_dwConnID = dwConnID; }
private:
	UINT32	m_dwConnID;
};

#endif