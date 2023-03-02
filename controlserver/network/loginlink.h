#ifndef __LOGIN_LINK_H__
#define __LOGIN_LINK_H__

#include "baselink.h"
#include "pb/enum.pb.h"

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


	//为了兼容，dwLoginID传0取第一个LoginServer
	bool SendTo(UINT32 dwLoginID, const CProtocol& roPtc);
	bool SendTo(UINT32 dwLoginID, CRpc& roRpc);
	void GetAllLogins(std::vector<UINT32>& logins);


	UINT32	GetLoginConnID(UINT32 dwLoginID);
	KKSG::ErrorCode AddLoginServer(UINT32 dwConnID, UINT32 dwLoginID);

	bool IsConnected() const { return !m_oLoginSvrMap.empty(); }
private:
	typedef std::map<UINT32, UINT32>	CLoginServerMap;	//ServerID -> ConnID
	CLoginServerMap	m_oLoginSvrMap;
};

#endif