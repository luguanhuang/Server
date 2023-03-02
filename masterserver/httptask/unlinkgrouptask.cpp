#include "pch.h"
#include "unlinkgrouptask.h"
#include "HashUtil.h"
#include "timeutil.h"
#include "appid.h"
#include "config.h"
#include "util.h"
#include "guild/guildbindgrouphelper.h"

static std::string InterFace = "/relation/unlink_groupv2?";

UnlinkGroupTask::UnlinkGroupTask()
{
	m_strUrl = MSConfig::Instance()->GetMsdkUrl();
}

UnlinkGroupTask::~UnlinkGroupTask()
{

}

const std::string UnlinkGroupTask::GetUrl()
{
	AddParam("appid", m_strAppID);
	AddParam("openid", m_openId);
	AddParam("accessToken", m_token);
	AddParam("guild_id", ToString(m_guildId));
	AddParam("zone_id", ToString(m_serverId));
	AddParam("guild_name", m_guildName);
	BuildPostData();

	UINT32 now = TimeUtil::GetTime();
	std::ostringstream oss;
	oss << m_strUrl << InterFace << GetUri();
//	SSDebug << oss.str() << END;
	return oss.str();
}

void UnlinkGroupTask::OnResult(const std::string &response)
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
		SSError << "return failed,res=" << response << END;
		OnFailed();
		return;
	}
	rapidjson::Value& dataJson = doc["data"];
	std::string groupCode = JsonUtil::GetJsonString(dataJson, "gc");
	BindGroupHelper::OnUnlinkGroupSuccess(this, groupCode);
}

void UnlinkGroupTask::OnFailed()
{
	BindGroupHelper::OnUnlinkGroupFailed(this, m_retCode);
}
