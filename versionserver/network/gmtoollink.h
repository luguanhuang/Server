#ifndef __GM_TOOL_LINK_H__
#define __GM_TOOL_LINK_H__

#include "baselink.h"

class CProtocol;
class CRpc;

class GMToolLink : public ListenLink
{
	GMToolLink();
	~GMToolLink();
	DECLARE_SINGLETON(GMToolLink)

public:
	virtual void OnPassiveConnect(UINT32 dwConnID);
	virtual void OnClose(UINT32 dwConnID, int err);
	virtual const std::string &GetName();
	virtual bool UseKeepAlive() const { return false; }

	UINT32	GetConnID() const { return m_dwConnID; }
	void	SetConnID(UINT32 dwConnID) { m_dwConnID = dwConnID; }
private:
	UINT32 m_dwConnID;
};

#endif