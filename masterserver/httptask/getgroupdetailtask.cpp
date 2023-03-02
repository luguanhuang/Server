#include "pch.h"
#include "getgroupdetailtask.h"
#include "HashUtil.h"
#include "timeutil.h"
#include "appid.h"
#include "config.h"
#include "util.h"
#include "guild/guildbindgrouphelper.h"

static std::string InterFace = "/relation/get_group_detail?";

GetGroupDetailTask::GetGroupDetailTask()
:m_platCode(0)
{
	m_strUrl = MSConfig::Instance()->GetMsdkUrl();
}

GetGroupDetailTask::~GetGroupDetailTask()
{

}

const std::string GetGroupDetailTask::GetUrl()
{
	AddParam("appid", m_strAppID);
	AddParam("openid", m_openId);
	AddParam("accessToken", m_token);
	AddParam("opt", "0");
	AddParam("unionid", ToString(m_guildId));
	AddParam("zoneid", ToString(m_serverId));
	BuildPostData();

	std::ostringstream oss;
	oss << m_strUrl << InterFace << GetUri();
//	SSDebug << oss.str() << END;
	return oss.str();
}

void GetGroupDetailTask::OnResult(const std::string &response)
{
	SSDebug << "Result: " << response << END;
	rapidjson::Document doc;
	doc.Parse(response.c_str());
	if (doc.HasParseError())
	{
		OnFailed();
		SSError << "parse error,res=" << response << END;
		return;
	}
	GetRetCode(doc);
	if (m_retCode != 0)
	{
		std::string str = JsonUtil::GetJsonString(doc, "platCode");
		m_platCode = convert<INT32>(str);
		OnFailed();
		SSError << "return failed,res=" << response << END;
		return;
	}

	std::string groupName = JsonUtil::GetJsonString(doc, "groupName");
	std::string groupOpenId = JsonUtil::GetJsonString(doc, "groupOpenid");

	BindGroupHelper::OnGetGroupDetailSuccess(this, groupName, groupOpenId);
}

void GetGroupDetailTask::OnFailed()
{
	BindGroupHelper::OnGetGroupDetailFailed(this, m_retCode, m_platCode);
}
