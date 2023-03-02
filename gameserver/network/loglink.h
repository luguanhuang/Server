#ifndef __LOGLINK_H__
#define __LOGLINK_H__

#include "baselink.h"

class CRpc;
class CProtocol;

class LogLink : public BaseLink
{
	LogLink();
	~LogLink();
	DECLARE_SINGLETON(LogLink)
public:

	virtual void OnConnectSucceed(UINT32 dwConnID);
	virtual void OnConnectFailed();
	virtual void OnPassiveConnect(UINT32 dwConnID);
	virtual void OnClose(UINT32 dwConnID, int err);
	virtual const std::string &GetName();

	void Close();
	bool Send(const CProtocol& roPtc);

private:

	UINT32 m_dwConnID;
};

#endif