#ifndef __CONTROL_LINK_H__
#define __CONTROL_LINK_H__

#include "baselink.h"


class ControlLink : public BaseLink
{
	ControlLink();
	~ControlLink();
	DECLARE_SINGLETON(ControlLink)
public:
	virtual void OnConnectSucceed(UINT32 dwConnID);
	virtual void OnConnectFailed();
	virtual void OnPassiveConnect(UINT32 dwConnID) {}
	virtual void OnClose(UINT32 dwConnID, int err);
	virtual const std::string &GetName();

	//Send to ms
	bool SendTo(const CProtocol& roPtc);
	bool SendTo(const CRpc &roRpc);

	UINT32	GetConnID() const { return m_dwConnID; }
	void	SetConnID(UINT32 dwConnID) { m_dwConnID = dwConnID; }
	bool	IsRegistered() const { return m_bRegistered; }
	void	SetRegistered(bool bVal) { m_bRegistered = bVal; }

	void	CloseNsLink();
private:
	UINT32	m_dwConnID;
	bool	m_bRegistered;
};

#endif