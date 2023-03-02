#include "pch.h"
#include <rapidjson/document.h>
#include "TXPlatFriendTask.h"
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
#include "logincontrol.h"
#include "platfriend/rpci2s_reqplatusersinfoi2s.h"
#include "network/cslink.h"

using namespace rapidjson;

TXPlatFriendTask::TXPlatFriendTask()
:m_ip(0)
,m_dwRpcID(0)
,m_loginType(KKSG::LOGIN_QQ_PF)
{
	m_qwBeginTime = TimeUtil::GetMilliSecond();
	SetType(true);
}

TXPlatFriendTask::~TXPlatFriendTask()
{
}

const std::string TXPlatFriendTask::GetUrl()
{
	UINT32 now = TimeUtil::GetTime();
	std::ostringstream oss;
	if (m_loginType == KKSG::LOGIN_QQ_PF)
	{
		oss << LoginConfig::Instance()->GetMSDKUrl() << "/relation/qqfriends_detail/?timestamp=" << now
			<< "&appid=" << MobileQQ_App_ID 
			<< "&sig=" << GetSign(now, MobileQQ_App_Key) 
			<< "&openid=" << m_openid
			<< "&encode=1";
	}
	else if (m_loginType == KKSG::LGOIN_WECHAT_PF)
	{
		oss << LoginConfig::Instance()->GetMSDKUrl() << "/relation/wxfriends/?timestamp=" << now
			<< "&appid=" << WeChat_App_ID << "&sig=" << GetSign(now, WeChat_App_Key) << "&openid=" << m_openid << "&encode=1";
	}

	return oss.str();
}

std::string TXPlatFriendTask::GetSign(UINT32 now, const std::string& key)
{
	std::ostringstream oss;
	oss << key << now;
	return Md5Hash(oss.str());
}

void TXPlatFriendTask::BuildPostData()
{
	JsonBuilder jb;	
	if (m_loginType == KKSG::LOGIN_QQ_PF)
	{
		jb.AddMember_Int("appid", convert<int>(MobileQQ_App_ID));
		jb.AddMember_String("openid", m_openid);
		jb.AddMember_String("accessToken", m_token);
		jb.AddMember_Int("flag", 1);
	}
	else if (m_loginType == KKSG::LGOIN_WECHAT_PF)
	{
		jb.AddMember_Int("appid", convert<int>(WeChat_App_ID));
		jb.AddMember_String("openid", m_openid);
		jb.AddMember_String("accessToken", m_token);
	}
	//else if (m_loginType == KKSG::LOGIN_IOS_GUEST)
	//{
	//	jb.AddMember_Int("appid", convert<int>(MobileQQ_App_ID));
	//	jb.AddMember_String("openid", m_openid);
	//	jb.AddMember_String("accessToken", m_token);
	//	jb.AddMember_Int("flag", 1);
	//}

	SSDebug << jb.GetString() << END;
	SetHttpPostString(jb.GetString());
}

void TXPlatFriendTask::OnResult(const std::string &response)
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
		LogWarn("tx query plat friend failed, code[%d], %s", ret, response.c_str());
		OnFailed();
		return;
	}

	std::set<std::string> friendOpenIDs;
	if (m_loginType == KKSG::LOGIN_QQ_PF)
	{
		if (!doc.HasMember("lists"))
		{
			LogWarn("tx response no list");
			OnFailed();
			return;
		}
		rapidjson::Value& valueArray = doc["lists"];
		if (!valueArray.IsArray())
		{
			LogWarn("tx response list not array");
			OnFailed();
			return;
		}
		for (int i = 0; i < valueArray.Capacity(); ++i)
		{
			rapidjson::Value& first = valueArray[i];
			if (first.HasMember("openid"))
			{
				friendOpenIDs.insert(first["openid"].GetString());
			}
			if (friendOpenIDs.size() >= MAX_FRIEND_QUERYNUM)
			{
				break;
			}
		}
	}
	else if(m_loginType == KKSG::LGOIN_WECHAT_PF)
	{
		if (!doc.HasMember("openids"))
		{
			LogWarn("tx response no openids");
			OnFailed();
			return;
		}
		rapidjson::Value& valueArray = doc["openids"];
		if (!valueArray.IsArray())
		{
			LogWarn("tx response openids not array");
			OnFailed();
			return;
		}
		for (int i = 0; i < valueArray.Capacity(); ++i)
		{
			rapidjson::Value& first = valueArray[i];
			friendOpenIDs.insert(first.GetString());
			if (friendOpenIDs.size() >= MAX_FRIEND_QUERYNUM)
			{
				break;
			}
		}
		friendOpenIDs.erase(m_openid);
	}
	
	UINT64 qwEndTime = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = qwEndTime >= m_qwBeginTime ? qwEndTime - m_qwBeginTime : 0;
	if(qwTimeUsed > 300)
	{
		LogWarn("TXPlatFriend web task finish use time %llu ms", qwTimeUsed);
	}

	if (friendOpenIDs.empty())
	{
		CLoginControl::Instance()->GetLoginInfo(m_openid).SetFinish(LoginFlowTask_PlatFriend, TaskStatus_Success);
	}
	else
	{
		UINT32 connID = CSLink::Instance()->GetUsingCenterServerID();
		if (connID == INVALID_CONN_ID)
		{
			CLoginControl::Instance()->GetLoginInfo(m_openid).SetFinish(LoginFlowTask_PlatFriend, TaskStatus_Success);
		}
		else
		{
			RpcI2S_ReqPlatUsersInfoI2S* rpc = RpcI2S_ReqPlatUsersInfoI2S::CreateRpc();
			rpc->m_oArg.set_sourceopenid(m_openid.c_str());
			for (auto iter = friendOpenIDs.begin(); iter != friendOpenIDs.end(); ++iter)
			{
				rpc->m_oArg.add_openid(*iter);
			}

			CSLink::Instance()->Send(connID, *rpc);
		}
	}
}

void TXPlatFriendTask::OnFailed()
{
	UINT64 qwEndTime = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = qwEndTime >= m_qwBeginTime ? qwEndTime - m_qwBeginTime : 0;
	LogWarn("TXPlatFriend web task failed,%s use time %llu ms", m_openid.c_str(), qwTimeUsed);

	CLoginControl::Instance()->GetLoginInfo(m_openid).SetFinish(LoginFlowTask_PlatFriend, TaskStatus_Failed);
}
