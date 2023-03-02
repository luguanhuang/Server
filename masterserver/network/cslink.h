#ifndef __CS_LINK_H__
#define __CS_LINK_H__

#include "baselink.h"

class CRpc;
class CProtocol;


class CSLink : public BaseLink
{
	CSLink();
	~CSLink();
	DECLARE_SINGLETON(CSLink)
public:
	virtual void OnConnectSucceed(UINT32 dwConnID);
	virtual void OnConnectFailed();
	virtual void OnPassiveConnect(UINT32 dwConnID);
	virtual void OnClose(UINT32 dwConnID, int err);
	virtual const std::string &GetName();


	bool SendToAll(const CProtocol& roPtc);
	bool SendTo(const CProtocol& roPtc);
	bool SendTo(CRpc& roRpc);

	void AddCenterServer(UINT32 dwConnID, UINT32 dwCenterServerID);
	void RemoveCenterServer(UINT32 dwConnID);

	void RandUsingCenterSever();
	UINT32 GetUsingCenterServerID();

	void Close(UINT32 dwConnID);

private:
	bool _SendTo(UINT32 dwConnID, const CProtocol& roPtc);
	bool _SendTo(UINT32 dwConnID, CRpc& roRpc);
private:
	struct CenterServerInfo
	{
		UINT32	m_dwConnID;
		UINT32	m_dwServerID;

		CenterServerInfo()
			:m_dwConnID(INVALID_CONN_ID)
			,m_dwServerID(0)
		{
		}
	};

	typedef std::map<UINT32, CenterServerInfo> CCenterServerList;//connid map server
	CCenterServerList m_oCenterSvrList;

	UINT32 m_usingCenterServerConnID;
};

#endif
