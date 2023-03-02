#include "pch.h"
#include "queryqqviprichtask.h"

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

QueryQQVipRichTask::QueryQQVipRichTask()
:m_dwRpcID(0)
{
	SetType(true);
}

QueryQQVipRichTask::~QueryQQVipRichTask()
{
}

const std::string QueryQQVipRichTask::GetUrl()
{
	UINT32 now = TimeUtil::GetTime();
	std::ostringstream oss;
	oss << GSConfig::Instance()->GetSdkUrl() << "/relation/get_vip_rich_info/?timestamp=" << now
			<< "&appid=" << MobileQQ_App_ID 
			<< "&sig=" << GetSign(now, MobileQQ_App_Key) 
			<< "&openid=" << m_openid
			<< "&encode=1";

	return oss.str();
}

std::string QueryQQVipRichTask::GetSign(UINT32 now, const std::string& key)
{
	std::ostringstream oss;
	oss << key << now;
	return Md5Hash(oss.str());
}

void QueryQQVipRichTask::BuildPostData()
{
	JsonBuilder jb;	
	jb.AddMember_String("appid", MobileQQ_App_ID);
	jb.AddMember_String("openid", m_openid);
	jb.AddMember_String("accessToken", m_token);

	SSDebug << jb.GetString() << END;
	SetHttpPostString(jb.GetString());
}

int QueryQQVipRichTask::GetIntFromString(const rapidjson::Value& json, const char* key)
{
	if(json.IsNull()) return 0;

	const rapidjson::Value& member = JsonUtil::GetJsonValue(json, key);
	if(member.IsNull()) return 0;

	if(!member.IsString()) return 0;

	return convert<int>(member.GetString());
}

void QueryQQVipRichTask::OnResult(const std::string &response)
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

	int isQQVip = GetIntFromString(doc, "is_qq_vip");
	int qqVipStart = GetIntFromString(doc, "qq_vip_start");
	int qqVipEnd = GetIntFromString(doc, "qq_vip_end");
	int isQQYearVip = GetIntFromString(doc, "is_qq_year_vip");
	int qqYearVipStart = GetIntFromString(doc, "qq_year_vip_start");
	int qqYearVipEnd = GetIntFromString(doc, "qq_year_vip_end");

	int isSVip = GetIntFromString(doc, "is_svip");
	int qqSVipStart = GetIntFromString(doc, "qq_svip_start");
	int qqSVipEnd = GetIntFromString(doc, "qq_svip_end");

	SSDebug << "isQQVip: " << isQQVip << ", isSvip: " << isSVip << ", isYearVip: " << isQQYearVip << END;

	Role* role = RoleManager::Instance()->FindByRoleID(m_roleId);
	if (role == NULL)
	{
		LogWarn("query qq vip failed, can't find role[%llu]", m_roleId);
		OnFailed();
		return;
	}

	QQVipRecord* record = role->Get<QQVipRecord>();
	record->SetIsVip(isQQVip == 1 ? true : false);
	record->SetIsSvip(isSVip == 1 ? true : false);
	record->SetIsYearSvip(isQQYearVip == 1 ? true : false);
	record->SetVipTime(qqVipStart, qqVipEnd);
	record->SetSVipTime(qqSVipStart, qqSVipEnd);
	record->SetYearVipTime(qqYearVipStart, qqYearVipEnd);

	// 检查礼包发放
	record->CheckNewBieGift();

	RoleSummaryMgr::Instance()->UpdateRoleSummary(role, SUMMARY_QQVIP);

	if (m_dwRpcID)
	{
		CUserData oUserData;
		oUserData.m_dwUserData = 0;
		oUserData.m_pUserPtr = NULL;
		CRpc::ReplyDelayRpc(m_dwRpcID, oUserData);
	}
}

void QueryQQVipRichTask::OnFailed()
{
	SSWarn << "query qq viprich task failed, openId = " << m_openid << ", token: " << m_token << END;
	if (m_dwRpcID)
	{
		CUserData oUserData;
		oUserData.m_dwUserData = 0;
		oUserData.m_pUserPtr = NULL;
		CRpc::ReplyDelayRpc(m_dwRpcID, oUserData);
	}
}
