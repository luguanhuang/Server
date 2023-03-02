#include "pch.h"
#include "queryqqfriendsviptask.h"

#include <rapidjson/document.h>
#include "crpc.h"
#include "util.h"
#include "HashUtil.h"
#include "timeutil.h"
#include "config.h"
#include "appid.h"
#include "util/jsonutil.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/qqviprecord.h"
#include "gamelogic/rolesummarymgr.h"

QueryQQFriendsVipTask::QueryQQFriendsVipTask()
:m_dwRpcID(0)
{
	SetType(true);
}

QueryQQFriendsVipTask::~QueryQQFriendsVipTask()
{
}

const std::string QueryQQFriendsVipTask::GetUrl()
{
	UINT32 now = TimeUtil::GetTime();
	std::ostringstream oss;
	oss << GSConfig::Instance()->GetSdkUrl() << "/relation/qqfriends_vip/?timestamp=" << now
			<< "&appid=" << MobileQQ_App_ID 
			<< "&sig=" << GetSign(now, MobileQQ_App_Key) 
			<< "&openid=" << m_openid
			<< "&encode=1";
	// TEST
	//SSDebug << oss.str() << END;
	return oss.str();
}

std::string QueryQQFriendsVipTask::GetSign(UINT32 now, const std::string& key)
{
	std::ostringstream oss;
	oss << key << now;
	return Md5Hash(oss.str());
}

void QueryQQFriendsVipTask::BuildPostData()
{
	JsonBuilder jb;	
	jb.AddMember_String("appid", MobileQQ_App_ID);
	jb.AddMember_String("openid", m_openid);
	jb.AddMember_String("accessToken", m_token);

	rapidjson::Value fOpenIds(rapidjson::kArrayType);
	for (size_t i = 0; i < m_friendOpenIds.size(); i++)
	{
		rapidjson::Value v(rapidjson::kStringType);
		v.SetString(m_friendOpenIds[i], jb.Allocator());
		fOpenIds.PushBack(v, jb.Allocator());
	}
	jb.AddMember_Value("fopenids", fOpenIds);

	jb.AddMember_String("flags", "qq_vip,qq_svip");
	jb.AddMember_String("pf", m_pf);
	jb.AddMember_String("userip", "127.0.0.1");		// TODO

	SSDebug << jb.GetString() << END;
	SetHttpPostString(jb.GetString());
}

int QueryQQFriendsVipTask::GetIntFromString(const rapidjson::Value& json, const char* key)
{
	if(json.IsNull()) return 0;

	const rapidjson::Value& member = JsonUtil::GetJsonValue(json, key);
	if(member.IsNull()) return 0;

	if(!member.IsString()) return 0;

	return convert<int>(member.GetString());
}

void QueryQQFriendsVipTask::OnResult(const std::string &response)
{
	LogDebug("get tencent response result: %s", response.c_str());

	rapidjson::Document doc;
	doc.Parse(response.c_str());
	if (doc.HasParseError())
	{
		LogError("parser json failed, %s", response.c_str());
		OnFailed();
		return;
	}

	if (!doc.HasMember("ret"))
	{
		OnFailed();
		return;
	}
	int ret = doc["ret"].GetInt();
	if (ret != 0)
	{
		LogWarn("failed, code[%d]", ret);
		OnFailed();
		return;
	}
	if (!doc.HasMember("lists"))
	{
		LogWarn("no lists");
		OnFailed();
		return;
	}
	rapidjson::Value& array = doc["lists"];
	if (!array.IsArray())
	{
		LogWarn("lists not array");
		OnFailed();
		return;
	}

	std::vector<KKSG::FriendVipInfo> infos;
	for (auto it = array.Begin(); it != array.End(); ++it)
	{
		KKSG::FriendVipInfo info;
		info.set_is_vip(JsonUtil::GetJsonInt(*it, "is_qq_vip") == 1 ? true : false);
		info.set_is_svip(JsonUtil::GetJsonInt(*it, "is_qq_svip") == 1 ? true : false);
		info.set_openid(JsonUtil::GetJsonString(*it, "openid"));
		infos.push_back(info);
	}

	if (m_dwRpcID)
	{
		CUserData oUserData;
		oUserData.m_dwUserData = 0;
		oUserData.m_pUserPtr = (void*)&infos;
		CRpc::ReplyDelayRpc(m_dwRpcID, oUserData);
	}
}

void QueryQQFriendsVipTask::OnFailed()
{
	SSWarn << "query qq friend vip task failed, openId = " << m_openid << ", token: " << m_token << END;
	if (m_dwRpcID)
	{
		CUserData oUserData;
		oUserData.m_dwUserData = 0;
		oUserData.m_pUserPtr = NULL;
		CRpc::ReplyDelayRpc(m_dwRpcID, oUserData);
	}
}