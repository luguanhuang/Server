#include "pch.h"
#include "queryplatformsharechestid.h"
#include "config.h"
#include "globalconfig.h"
#include "appid.h"
#include "HashUtil.h"
#include "timeutil.h"

QueryPlatformShareChestid::QueryPlatformShareChestid()
	:rpc_id_(0)
{
	SetType(true);
	SetTimeOut(3000);
}

QueryPlatformShareChestid::~QueryPlatformShareChestid()
{

}

const std::string QueryPlatformShareChestid::GetUrl()
{
	UINT32 now = TimeUtil::GetTime();
	std::ostringstream oss;
	oss << GSConfig::Instance()->GetSdkUrl() << "/relation/qq_gain_chest?timestamp=" << now
		<< "&appid=" << MobileQQ_App_ID 
		<< "&sig=" << GetSign(now, MobileQQ_App_Key) 
		<< "&openid=" << open_id_
		<< "&encode=1";
		

	return oss.str();
}

void QueryPlatformShareChestid::OnResult(const std::string &response)
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
		LogError("parser json failed, %s", response.c_str());
		OnFailed();
		return;
	}
	int ret = doc["ret"].GetInt();
	if (ret != 0)
	{		
		LogWarn(" parser json failed, %s", response.c_str());
		OnFailed();
		return;
	}

	if (!doc.HasMember("boxid"))
	{
		LogWarn("parser json failed, %s", response.c_str());
		OnFailed();
		return;
	}
	string boxid = doc["boxid"].GetString();

	KKSG::GetPlatformShareChestRes res;
	res.set_errorcode(KKSG::ErrorCode(ret));
	res.set_boxid(boxid);
	if (rpc_id_ > 0)
	{
		CUserData oUserData;
		oUserData.m_dwUserData = 0;
		oUserData.m_pUserPtr = (void*)(&res);
		CRpc::ReplyDelayRpc(rpc_id_, oUserData);
	}

}

void QueryPlatformShareChestid::OnFailed()
{
	SSWarn << "query QueryPlatformShareChestid failed, openId = " << open_id_ <<  END; 

	KKSG::GetPlatformShareChestRes res;
	res.set_errorcode(KKSG::ERR_PLATSHARE_FAILED);
	if (rpc_id_ > 0)
	{
		CUserData oUserData;
		oUserData.m_dwUserData = 0;
		oUserData.m_pUserPtr = &res;
		CRpc::ReplyDelayRpc(rpc_id_, oUserData);
	}
}

std::string QueryPlatformShareChestid::GetSign(UINT32 now, const std::string& key)
{
	std::ostringstream oss;
	oss << key << now;
	return Md5Hash(oss.str());
}


void QueryPlatformShareChestid::BuildPostData()
{
	JsonBuilder jb;	
	jb.AddMember_String("appid", MobileQQ_App_ID);
	jb.AddMember_String("accessToken", token_);
	jb.AddMember_String("openid", open_id_);
	jb.AddMember_String("pf", pf_);
	jb.AddMember_Int("actid", act_id_);
	jb.AddMember_Int("num", num_);
	jb.AddMember_Int("peoplenum", people_num_);
	jb.AddMember_Int("type", 0);

	SSDebug << jb.GetString() << END;
	SetHttpPostString(jb.GetString());
}
