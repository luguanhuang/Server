#ifndef __H_Login_Control_H__
#define __H_Login_Control_H__

#include "tokenverifymgr.h"

enum LoginFlowTaskType
{
	LoginFlowTask_Begin = 0,
	LoginFlowTask_TencentTokenAuth = 0,
	LoginFlowTask_FreeFlow,
	LoginFlowTask_PlatFriend,
	LoginFlowTask_GetSelfServer,
	LoginFlowTask_End,
};

enum LoginTaskStatus
{
	TaskStatus_NULL,
	TaskStatus_Success,
	TaskStatus_Failed,
};

struct LoginInfo
{
	LoginInfo()
	{
		ip = 0;
		delayRpcID = 0;
		loginType = KKSG::LOGIN_QQ_PF;
		requestTime = 0;
		for (int i = LoginFlowTask_FreeFlow; i < LoginFlowTask_End; ++i)
		{
			loginTaskFlowStatus[i] = 0;
		}
	}

	void Init();
	bool CheckAllTaskFinish();
	void SetFreeFlow(bool free);
	void SetCCType(int ccType);
	void SetFinish(LoginFlowTaskType type, LoginTaskStatus status);


	std::string openID;
	UINT32 ip;
	UINT32 delayRpcID;
	KKSG::LoginType loginType;
	std::string tencentAuthToken;
	UserGateInfo userGateInfo;
	UINT32 requestTime;

	std::map<UINT32, UINT32> loginTaskFlowStatus;
};

class CLoginControl
{
	CLoginControl();
	virtual ~CLoginControl();
	DECLARE_SINGLETON(CLoginControl)

public:
	bool Init();
	void Uninit();


	bool CheckLogin(const std::string& strAccount);
	void BeginLogin(LoginInfo& info);
	void EndLogin(const std::string& strAccount, bool success = true, const std::string& reason = std::string(""));
	LoginInfo& GetLoginInfo(const std::string strAccount);

	UINT32 GetLoginNum() { return m_loginNum; }
	void ClearLoginNum() { m_loginNum = 0; }

private:
	typedef std::unordered_map<std::string, LoginInfo>			CLoginRequestMap;
	CLoginRequestMap	m_oMap;
	UINT32 m_loginNum;
};


#endif 