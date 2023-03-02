#include "pch.h"
#include "logincontrol.h"
#include "timeutil.h"
#include "config.h"
#include "crpc.h"

INSTANCE_SINGLETON(CLoginControl)


void LoginInfo::Init()
{
	if (loginType == KKSG::LOGIN_PASSWORD)
	{
		for (int i = LoginFlowTask_FreeFlow; i < LoginFlowTask_End; ++i)
		{
			if (i != LoginFlowTask_GetSelfServer)
			{
				loginTaskFlowStatus[i] = TaskStatus_Success;
			}
		}
	}
}

bool LoginInfo::CheckAllTaskFinish()
{
	bool allfinish = true;
	for (auto iter = loginTaskFlowStatus.begin(); iter != loginTaskFlowStatus.end(); ++iter)
	{
		if (!iter->second)
		{
			allfinish = false;
			break;
		}
	}

	if (allfinish)
	{
		if (loginTaskFlowStatus[LoginFlowTask_GetSelfServer] == TaskStatus_Failed)
		{
			CUserData oUserData;
			oUserData.m_dwUserData = 0;
			oUserData.m_pUserPtr = NULL;
			CLoginControl::Instance()->EndLogin(openID, false, "DB query failed");
			CRpc::ReplyDelayRpc(delayRpcID, oUserData);
		}
		else
		{
			CUserData userdata;
			userdata.m_dwUserData = 1;
			userdata.m_pUserPtr = (void*)&userGateInfo;
			CRpc::ReplyDelayRpc(delayRpcID, userdata);
			CLoginControl::Instance()->EndLogin(openID);
		}
	}

	return allfinish;
}

void LoginInfo::SetFreeFlow(bool free)
{
	userGateInfo.isFreeFlow = free;
}



void LoginInfo::SetCCType(int ccType)
{
	userGateInfo.ccType = ccType;
}

void LoginInfo::SetFinish(LoginFlowTaskType type, LoginTaskStatus status)
{
	loginTaskFlowStatus[type] = status;

	CheckAllTaskFinish();
}

CLoginControl::CLoginControl()
:m_loginNum(0)
{
}

CLoginControl::~CLoginControl()
{
}


bool CLoginControl::Init()
{
	return true;
}

void CLoginControl::Uninit()
{
}

bool CLoginControl::CheckLogin(const std::string& strAccount)
{
	if(m_oMap.size() >= LoginConfig::Instance()->GetMaxLoginConcurrent())
	{
		SSWarn << "Concurrent login num: " << m_oMap.size() << " over max" << END;
		return false;
	}

	if(m_oMap.find(strAccount) != m_oMap.end())
	{
		LogWarn("Account[%s] repeat login", strAccount.c_str());
		return false;
	}

	return true;
}

void CLoginControl::BeginLogin(LoginInfo& info)
{
	info.requestTime = TimeUtil::GetTime();
	m_oMap.insert(std::make_pair(info.openID, info));
	m_loginNum++;
}

void CLoginControl::EndLogin(const std::string& strAccount, bool success, const std::string& reason)
{
	CLoginRequestMap::iterator it = m_oMap.find(strAccount);
	if(it == m_oMap.end())
	{
		return;
	}

	UINT32 dwNow = TimeUtil::GetTime();
	UINT32 dwUsed = dwNow >= it->second.requestTime ? dwNow - it->second.requestTime : 0; 
	if(dwUsed >= 4)
	{
		LogWarn("Account %s login finished, total use time %u sec", strAccount.c_str(), dwUsed);
	}
	if (!success)
	{
		LogWarn("Account %s login failed, reason: %s", strAccount.c_str(), reason.c_str());
	}
	m_oMap.erase(strAccount);
}

LoginInfo& CLoginControl::GetLoginInfo(const std::string strAccount)
{
	return m_oMap[strAccount];
}
