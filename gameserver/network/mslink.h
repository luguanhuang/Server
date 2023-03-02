#ifndef __MSLINK_H__
#define __MSLINK_H__

#include "baselink.h"


class MSLink : public BaseLink
{
	MSLink();
	~MSLink();
	DECLARE_SINGLETON(MSLink)
public:
	virtual void OnConnectSucceed(UINT32 dwConnID);
	virtual void OnConnectFailed();
	virtual void OnPassiveConnect(UINT32 dwConnID) {}
	virtual void OnClose(UINT32 dwConnID, int err);
	virtual const std::string &GetName();

	//Send to ms
	bool SendTo(const CProtocol& roPtc);
	bool SendTo(const CRpc &roRpc);

	bool SendTo(UINT64 roleId, const CProtocol& roPtc);
	bool SendTo(UINT64 roleId, CRpc &roRpc);

	UINT32	GetConnID() const { return m_dwConnID; }
	void	SetConnID(UINT32 dwConnID) { m_dwConnID = dwConnID; }
	bool	IsRegistered() const { return m_bRegistered; }
	void	SetRegistered(bool bVal) { m_bRegistered = bVal; }
	void	CloseMsLink();
	void    ReqGuildSimpleData();

	void	ReqDragonGuildSimpleData();

	UINT32	GetLinkTimes() const { return m_dwLinkTimes; }
	void	AddLinkTimes()		 { m_dwLinkTimes++; }
private:
	UINT32	m_dwConnID;
	UINT32	m_dwLinkTimes;
	bool	m_bRegistered;
};

#endif