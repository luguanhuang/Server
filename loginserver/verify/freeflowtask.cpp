#include "pch.h"
#include "freeflowtask.h"
#include <rapidjson/document.h>
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

using namespace rapidjson;

FreeFlowTask::FreeFlowTask()
:m_ip(0)
,m_dwRpcID(0)
,m_loginType(KKSG::LOGIN_QQ_PF)
,m_isFree(false)
,m_ccType(CCType_Normal)
{
	m_qwBeginTime = TimeUtil::GetMilliSecond();
	SetType(true);
}

FreeFlowTask::~FreeFlowTask()
{
}

const std::string FreeFlowTask::GetUrl()
{
	UINT32 now = TimeUtil::GetTime();
	std::ostringstream oss;
	if (m_loginType == KKSG::LOGIN_QQ_PF)
	{
		oss << LoginConfig::Instance()->GetMSDKUrl() << "/freeflow/getinfo/?timestamp=" << now
			<< "&appid=" << MobileQQ_App_ID 
			<< "&sig=" << GetSign(now, MobileQQ_App_Key) 
			<< "&openid=" << m_openid
			<< "&encode=1";
	}
	else if (m_loginType == KKSG::LGOIN_WECHAT_PF)
	{
		oss << LoginConfig::Instance()->GetMSDKUrl() << "/freeflow/getinfo/?timestamp=" << now
			<< "&appid=" << WeChat_App_ID << "&sig=" << GetSign(now, WeChat_App_Key) << "&openid=" << m_openid << "&encode=1";
	}
	else if (m_loginType == KKSG::LOGIN_IOS_GUEST)
	{
		oss << LoginConfig::Instance()->GetMSDKUrl() << "/freeflow/getinfo/?timestamp=" << now
			<< "&appid=" << "G_" << MobileQQ_App_ID << "&sig=" << GetSign(now, MobileQQ_App_Key) << "&openid=" << m_openid << "&encode=1";
	}
	m_debugUrl = oss.str();

	return m_debugUrl;
}

std::string FreeFlowTask::GetSign(UINT32 now, const std::string& key)
{
	std::ostringstream oss;
	oss << key << now;
	return Md5Hash(oss.str());
}

void FreeFlowTask::BuildPostData()
{
}

void FreeFlowTask::OnResult(const std::string &response)
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
		LogWarn("failed, code[%d], msg: %s, url: %s", ret, response.c_str(), m_debugUrl.c_str());
		OnFailed();
		return;
	}
	if (!doc.HasMember("freeFlowInfo"))
	{
		LogWarn("No freeFlowInfo");
		OnFailed();
		return;
	}
	const rapidjson::Value& freeFlowInfo = doc["freeFlowInfo"];

	if (freeFlowInfo.HasMember("isFree"))
	{
		int free = freeFlowInfo["isFree"].GetInt();		
		m_isFree = (free != 0) ? true : false;
	}

	if (freeFlowInfo.HasMember("ccType"))
	{
		m_ccType = freeFlowInfo["ccType"].GetInt();
	}
	LogInfo("openid[%s] ccType[%u] isFree[%u]", m_openid.c_str(), m_ccType, m_isFree);
	
	UINT64 qwEndTime = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = qwEndTime >= m_qwBeginTime ? qwEndTime - m_qwBeginTime : 0;
	if(qwTimeUsed > 300)
	{
		LogWarn("FreeFlow web task finish use time %llu ms", qwTimeUsed);
	}

	LoginInfo& info = CLoginControl::Instance()->GetLoginInfo(m_openid);
	info.SetFreeFlow(m_isFree);
	info.SetCCType(m_ccType);
	info.SetFinish(LoginFlowTask_FreeFlow, TaskStatus_Success);
}

void FreeFlowTask::OnFailed()
{
	UINT64 qwEndTime = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = qwEndTime >= m_qwBeginTime ? qwEndTime - m_qwBeginTime : 0;
	LogWarn("FreeFlow web task failed, use time %llu ms", qwTimeUsed);

	LoginInfo& info = CLoginControl::Instance()->GetLoginInfo(m_openid);
	info.SetFinish(LoginFlowTask_FreeFlow, TaskStatus_Failed);
}
