#include "pch.h"
#include "createbindtask.h"
#include "HashUtil.h"
#include "timeutil.h"
#include "appid.h"
#include "config.h"
#include "util.h"
#include "guild/guildbindgrouphelper.h"

static std::string InterFace = "/relation/createbind_groupv2?";

CreateBindTask::CreateBindTask()
{
	m_strUrl = MSConfig::Instance()->GetMsdkUrl();
}

CreateBindTask::~CreateBindTask()
{

}

const std::string CreateBindTask::GetUrl()
{
	AddParam("appid", m_strAppID);
	AddParam("openid", m_openId);
	AddParam("accessToken", m_token);
	AddParam("guild_id", ToString(m_guildId));
	AddParam("zone_id", ToString(m_serverId));
	AddParam("guild_name", m_guildName);
	AddParam("platid", m_platType == KKSG::PLAT_IOS ? "2" : "1");
	AddParam("roleid", ToString(m_roleId));
	AddParam("partition", ToString(m_serverId));
	BuildPostData();

	UINT32 now = TimeUtil::GetTime();
	std::ostringstream oss;
	oss << m_strUrl << InterFace << GetUri();
	//GetParamJsonString();
	//SSWarn << oss.str() << END;
	return oss.str();
}

void CreateBindTask::OnResult(const std::string &response)
{
	SSDebug << "Result: " << response << END;
	rapidjson::Document doc;
	doc.Parse(response.c_str());
	if (doc.HasParseError() || GetRetCode(doc) != 0)
	{
		OnFailed();
		SSError << "parse error,res=" << response << END;
		return;
	}
	if (!doc.HasMember("data"))
	{
		SSError << "no data,res=" << response << END;
		OnFailed();
		return;
	}
	rapidjson::Value& dataJson = doc["data"];
	std::string groupCode = JsonUtil::GetJsonString(dataJson, "gc");
	std::string groupName = JsonUtil::GetJsonString(dataJson, "group_name", false);

	BindGroupHelper::OnCreateBindSuccess(this, groupCode, groupName);
}

void CreateBindTask::OnFailed()
{
	BindGroupHelper::OnCreateBindFailed(this, m_retCode);
}
