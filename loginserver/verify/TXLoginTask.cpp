#include "pch.h"
#include <rapidjson/document.h>
#include "TXLoginTask.h"
#include "crpc.h"
#include "tokenverifymgr.h"
#include "util.h"
#include "gatemgr/gatemgr.h"
#include "HashUtil.h"
#include "timeutil.h"
#include "iplist/whitelistmgr.h"
#include "verify/logincontrol.h"
#include "config.h"
#include "appid.h"
#include "util/jsonutil.h"
#include "verify/DBGetSelfServerTask.h"
#include "verify/freeflowtask.h"
#include "verify/TXPlatFriendTask.h"

using namespace rapidjson;

TXLoginTask::TXLoginTask()
:m_ip(0)
,m_dwRpcID(0)
,m_loginType(KKSG::LOGIN_QQ_PF)
{
	m_qwBeginTime = TimeUtil::GetMilliSecond();
	SetType(true);
}

TXLoginTask::~TXLoginTask()
{
}

const std::string TXLoginTask::GetUrl()
{
	UINT32 now = TimeUtil::GetTime();
	std::ostringstream oss;
	if (m_loginType == KKSG::LOGIN_QQ_PF)
	{
		oss << LoginConfig::Instance()->GetMSDKUrl() << "/auth/verify_login/?timestamp=" << now
			<< "&appid=" << MobileQQ_App_ID 
			<< "&sig=" << GetSign(now, MobileQQ_App_Key) 
			<< "&openid=" << m_openid
			<< "&encode=1";
	}
	else if (m_loginType == KKSG::LGOIN_WECHAT_PF)
	{
		oss << LoginConfig::Instance()->GetMSDKUrl() << "/auth/check_token/?timestamp=" << now
			<< "&appid=" << WeChat_App_ID << "&sig=" << GetSign(now, WeChat_App_Key) << "&openid=" << m_openid << "&encode=1";
	}
	else if (m_loginType == KKSG::LOGIN_IOS_GUEST)
	{
		oss << LoginConfig::Instance()->GetMSDKUrl() << "/auth/guest_check_token/?timestamp=" << now
			<< "&appid=" << "G_" << MobileQQ_App_ID << "&sig=" << GetSign(now, MobileQQ_App_Key) << "&openid=" << m_openid << "&encode=1";
	}

	return oss.str();
}

std::string TXLoginTask::GetSign(UINT32 now, const std::string& key)
{
	std::ostringstream oss;
	oss << key << now;
	return Md5Hash(oss.str());
}

void TXLoginTask::BuildPostData()
{
	JsonBuilder jb;	
	if (m_loginType == KKSG::LOGIN_QQ_PF)
	{
		jb.AddMember_Int("appid", convert<int>(MobileQQ_App_ID));
		jb.AddMember_String("openid", m_openid);
		jb.AddMember_String("openkey", m_token);
		char IPBuf[64] = {0};
		unsigned char *p = (unsigned char *)&m_ip;
		sprintf(IPBuf, "%u.%u.%u.%u", p[0], p[1], p[2], p[3]);
		jb.AddMember_String("userip", IPBuf);
	}
	else if (m_loginType == KKSG::LGOIN_WECHAT_PF)
	{
		jb.AddMember_String("accessToken", m_token);
		jb.AddMember_String("openid", m_openid);
	}
	else if (m_loginType == KKSG::LOGIN_IOS_GUEST)
	{
		jb.AddMember_String("accessToken", m_token);
		jb.AddMember_String("guestid", m_openid);
	}

	SSDebug << jb.GetString() << END;
	SetHttpPostString(jb.GetString());
}

void TXLoginTask::OnResult(const std::string &response)
{
	LogDebug("get tencent response result: %s", response.c_str());

	Document doc;
	doc.Parse(response.c_str());
	if (doc.HasParseError())
	{
		LogWarn("parser json failed, %s", response.c_str());
		OnFailed();
		return;
	}

	if (!doc.HasMember("ret"))
	{
		LogWarn("tx response no ret");
		OnFailed();
		return;
	}
	int ret = doc["ret"].GetInt();
	if (ret != 0)
	{
		LogWarn("tx login failed, code[%d]", ret);
		OnFailed();
		return;
	}
	
	UINT64 qwEndTime = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = qwEndTime >= m_qwBeginTime ? qwEndTime - m_qwBeginTime : 0;
	if(qwTimeUsed > 300)
	{
		LogWarn("TXAuth web task finish use time %llu ms", qwTimeUsed);
	}

	if (m_loginType == KKSG::LOGIN_QQ_PF || m_loginType == KKSG::LGOIN_WECHAT_PF)
	{
		TXPlatFriendTask* pPlatTask = new TXPlatFriendTask();
		pPlatTask->SetIP(m_ip);
		pPlatTask->SetRpcID(m_dwRpcID);
		pPlatTask->SetOpenId(m_openid);
		pPlatTask->SetLoginType(m_loginType);
		pPlatTask->SetToken(m_token);
		pPlatTask->BuildPostData();
		TokenVerifyMgr::Instance()->AddPFVerifyTask(pPlatTask);
	}
	else
	{
		LoginInfo& info = CLoginControl::Instance()->GetLoginInfo(m_openid);
		info.SetFinish(LoginFlowTask_PlatFriend, TaskStatus_Success);
	}

	if (LoginConfig::Instance()->UseFreeFlow())
	{
		FreeFlowTask* pTask = new FreeFlowTask();
		pTask->SetIP(m_ip);
		pTask->SetRpcID(m_dwRpcID);
		pTask->SetOpenId(m_openid);
		pTask->SetLoginType(m_loginType);
		TokenVerifyMgr::Instance()->AddPFVerifyTask(pTask);
	}
	else
	{
		LoginInfo& info = CLoginControl::Instance()->GetLoginInfo(m_openid);
		info.SetFreeFlow(false);
		info.SetFinish(LoginFlowTask_FreeFlow, TaskStatus_Success);
	}

	DBGetSelfServerTask* pSelfServerTask = new DBGetSelfServerTask(m_dwRpcID, m_openid, "", m_ip);
	//pTask->SetFreeFlow(false);
	TokenVerifyMgr::Instance()->AddDBTask(pSelfServerTask);
}

void TXLoginTask::OnFailed()
{
	UINT64 qwEndTime = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = qwEndTime >= m_qwBeginTime ? qwEndTime - m_qwBeginTime : 0;
	LogWarn("TXAuth web task failed, use time %llu ms", qwTimeUsed);

	CUserData oUserData;
	oUserData.m_dwUserData = 0;
	oUserData.m_pUserPtr = NULL;
	CLoginControl::Instance()->EndLogin(m_openid, false, "Tx Auth Failed");
	CRpc::ReplyDelayRpc(m_dwRpcID, oUserData);
}