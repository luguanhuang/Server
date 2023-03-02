#ifndef __LOGIN_LINK_H__
#define __LOGIN_LINK_H__

#include "baselink.h"

class CRpc;
class CProtocol;


class LoginLink : public BaseLink
{
	LoginLink();
	~LoginLink();
	DECLARE_SINGLETON(LoginLink)
public:
	virtual void OnConnectSucceed(UINT32 dwConnID);
	virtual void OnConnectFailed();
	virtual void OnPassiveConnect(UINT32 dwConnID);
	virtual void OnClose(UINT32 dwConnID, int err);
	virtual const std::string &GetName();

	//随机选择一个
	bool SendTo(const CProtocol& roPtc);
	bool SendTo(CRpc& roRpc);

	KKSG::ErrorCode AddLoginServer(UINT32 dwConnID, UINT32 dwLoginID);
private:
	struct LoginInfo
	{
		UINT32	m_dwConnID;
		UINT32	m_dwServerID;

		LoginInfo()
			:m_dwConnID(INVALID_CONN_ID)
			,m_dwServerID(0)
		{
		}
	};

	typedef std::vector<LoginInfo>	CLoginServerList;
	CLoginServerList	m_oLoginSvrList;
};

#endif