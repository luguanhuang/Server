#include "pch.h"
#include "queryqqviptask.h"

#include <rapidjson/document.h>
#include "crpc.h"
#include "util.h"
#include "HashUtil.h"
#include "timeutil.h"
#include "config.h"
#include "appid.h"
#include "util/jsonutil.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/qqviprecord.h"

#define QQ_VIP 0x01
#define QQ_SUPER_VIP 0x10
#define VIP_XINYUE 0x40

QueryQQVipTask::QueryQQVipTask()
:m_dwRpcID(0)
,m_roleId(0)
{
	SetType(true);
}

QueryQQVipTask::~QueryQQVipTask()
{
}

const std::string QueryQQVipTask::GetUrl()
{
	UINT32 now = TimeUtil::GetTime();
	std::ostringstream oss;
	oss << GSConfig::Instance()->GetSdkUrl() << "/profile/query_vip/?timestamp=" << now
			<< "&appid=" << MobileQQ_App_ID 
			<< "&sig=" << GetSign(now, MobileQQ_App_Key) 
			<< "&openid=" << m_openid
			<< "&encode=1";

	return oss.str();
}

std::string QueryQQVipTask::GetSign(UINT32 now, const std::string& key)
{
	std::ostringstream oss;
	oss << key << now;
	return Md5Hash(oss.str());
}

void QueryQQVipTask::BuildPostData()
{
	JsonBuilder jb;	
	jb.AddMember_String("appid", MobileQQ_App_ID);
	jb.AddMember_String("openid", m_openid);
	jb.AddMember_String("accessToken", m_token);
	jb.AddMember_Int("vip", VIP_XINYUE);

	SSDebug << jb.GetString() << END;
	SetHttpPostString(jb.GetString());
}

void QueryQQVipTask::OnResult(const std::string &response)
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
		OnFailed();
		return;
	}
	rapidjson::Value& array = doc["lists"];
	if (!array.IsArray())
	{
		OnFailed();
		return;
	}
	Role* pRole = RoleManager::Instance()->FindByRoleID(m_roleId);
	if (pRole == NULL)
	{
		return;
	}

	//获取超级会员的时候，struct VIP中，只有isvip和flag参数有效
	for (rapidjson::SizeType i = 0; i < array.Size(); ++i)
	{
		const rapidjson::Value& p = array[i];
		int flag = JsonUtil::GetJsonInt(p, "flag");
		int isvip = JsonUtil::GetJsonInt(p, "isvip");

		if (flag == VIP_XINYUE)
		{
			pRole->Get<QQVipRecord>()->SetIsXinYueVip(isvip != 0);
		}
	}
}

void QueryQQVipTask::OnFailed()
{
	SSWarn << "query qq vip task failed, openId = " << m_openid << ", token: " << m_token << END;
	/*
	if (m_dwRpcID)
	{
		CUserData oUserData;
		oUserData.m_dwUserData = 0;
		oUserData.m_pUserPtr = NULL;
		CRpc::ReplyDelayRpc(m_dwRpcID, oUserData);
	}
	*/
}