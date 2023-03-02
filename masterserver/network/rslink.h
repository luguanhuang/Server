#ifndef _MS_RSLINK_H_
#define _MS_RSLINK_H_

#include "baselink.h"

class RSLink : public BaseLink
{
	RSLink();
	~RSLink();
	DECLARE_SINGLETON(RSLink)
public:
	virtual void OnConnectSucceed(UINT32 dwConnID);
	virtual void OnConnectFailed();
	virtual void OnPassiveConnect(UINT32 dwConnID) {}
	virtual void OnClose(UINT32 dwConnID, int err);
	virtual const std::string &GetName();

	bool SendTo(const CProtocol& ptc);
	bool SendTo(CRpc& rpc);

	UINT32 GetConnId()
	{
		return m_connId;
	}


	void QueryRouterFromWorld();
	void OnQueryRouterReply(const KKSG::RouterListenInfo & info);

	void NotifyRouter2Gate(UINT32 line = 0);
	void NotifyRouter2DB();
	void NotifyRouter2Ctrl();

private:
	void SetRouterIpPort(const std::string& ip, UINT32 port);

private:
	UINT32 m_connId;
	std::string m_routerIp;
	UINT32 m_routerPort;
	UINT32 m_reconnectCount;

	UINT32 m_lastQueryTime;
	KKSG::RouterListenInfo m_routerInfo;
};

#endif
