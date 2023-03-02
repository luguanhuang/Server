#ifndef _Login_Control_H__
#define _Login_Control_H__

#include "timer.h"
#include "pb/project.pb.h"
#include "logoutlistener.h"

#define LOGIN_QUEUED_SIZE	100

enum ELoginState
{
	LOGIN_STATE_INVALID,
	LOGIN_STATE_VERIFY_TOKEN,
	LOGIN_STATE_KICKOUT_OLD_ACCOUNT,
	LOGIN_STATE_LOADING_ACCOUNT_DATA,
	LOGIN_STATE_OK,
};

namespace KKSG
{
	class LoginRequestNewArg;
	class LoginRequestNewRes;
}

class CLoginRequest
{
public:
	std::string	m_strAccount;		//¾ÍÊÇopenid
	std::string	m_strToken;
	UINT64		m_qwSessionID;
	UINT32		m_dwRpcDelayed;

	UINT64		m_qwBeginTime;
	UINT64		m_qwEndTime;
	ELoginState	m_nState;
	UINT32		m_dwLoginServerId;
	UINT32		m_dwServerId;
	UINT32		m_sceneTemplateId;
	UINT32		m_sceneTime;
	bool		m_accountBackFlow;


	CLoginRequest()
		:m_qwSessionID(INVALID_SESSION_ID)
		,m_dwRpcDelayed(0)
		,m_qwBeginTime(0)
		,m_qwEndTime(0)
		,m_nState(LOGIN_STATE_INVALID)
		,m_dwLoginServerId(0)
		,m_dwServerId(0)
		,m_sceneTemplateId(0)
		,m_sceneTime(0)
		,m_accountBackFlow(false)
	{
	}
	~CLoginRequest()
	{
	}
	bool IsValid() const;
};


class CLoginControl : public ILogoutListener
{
	CLoginControl();
	virtual ~CLoginControl();
	DECLARE_SINGLETON(CLoginControl)

public:
	bool Init();
	void Uninit();


	KKSG::ErrorCode BeginLogin(const KKSG::LoginRequestNewArg& roArg, KKSG::LoginRequestNewRes& roRes, /* const std::string& strAccount, const std::string& strToken,*/ UINT64 qwSessionID);
	void CancelLogin(UINT64 qwSessionID);
	void CancelLogin(const std::string& strAccount);

	CLoginRequest* GetLoginReq(UINT64 qwSessionID);
	CLoginRequest* GetLoginReq(const std::string& strAccount);
	UINT32 GetLoginReqNum() const { return m_oAccount2ReqMap.size(); }

	KKSG::ErrorCode StartTokenVerify(CLoginRequest* poReq);
	void OnTokenVerfiyCompleted(UINT64 qwSessionID, const std::string& strUserID, KKSG::ErrorCode nErrCode, const KKSG::LoginVerifyRes* roRes);

	void OnSessionTerminated(UINT64 qwSessionID);
	virtual void OnLogoutFinished(const std::string& strAccount) {}


	void LockName(const std::string& strName);
	void UnlockName(const std::string& strName);
	bool IsNameLocked(const std::string& strName) const;
private:
	typedef std::unordered_map<std::string, CLoginRequest*>	CAccount2RequestMap;
	typedef std::map<UINT64, CLoginRequest>					CSession2RequestMap;
	typedef std::list<CLoginRequest*>						CLoginQueue;

	CAccount2RequestMap		m_oAccount2ReqMap;
	CSession2RequestMap		m_oSession2ReqMap;
	CLoginQueue				m_oLoginQueue;

	std::unordered_set<std::string>	m_oNameLocked;
};


#endif