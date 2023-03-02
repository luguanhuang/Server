#include "pch.h"
#include "getgrouprelation.h"
#include "HashUtil.h"
#include "timeutil.h"
#include "appid.h"
#include "config.h"
#include "util.h"
#include "guild/guildbindgrouphelper.h"

static std::string InterFace = "/relation/get_groupinfov2?";

GetGroupRelationTask::GetGroupRelationTask()
{
	m_strUrl = MSConfig::Instance()->GetMsdkUrl();
}

GetGroupRelationTask::~GetGroupRelationTask()
{

}

const std::string GetGroupRelationTask::GetUrl()
{
	AddParam("appid", m_strAppID);
	AddParam("openid", m_openId);
	AddParam("accessToken", m_token);
	AddParam("gc", m_groupCode);
	BuildPostData();
	std::ostringstream oss;
	oss << m_strUrl << InterFace << GetUri();
//	SSDebug << oss.str() << END;
	return oss.str();
}

void GetGroupRelationTask::OnResult(const std::string &response)
{
	SSDebug << "Result: " << response << END;
	rapidjson::Document doc;
	doc.Parse(response.c_str());
	if (doc.HasParseError() || GetRetCode(doc) != 0)
	{
		OnFailed();
		SSError << "failed,res=" << response << END;
		return;
	}
	if (!doc.HasMember("data"))
	{
		SSError << "no data,res=" << response << END;
		OnFailed();
		return;
	}
	rapidjson::Value& dataJson = doc["data"];
	UINT32 relation = JsonUtil::GetJsonInt(dataJson, "relation");

	BindGroupHelper::OnGetRelationSuccess(this, relation);
}

void GetGroupRelationTask::OnFailed()
{
	BindGroupHelper::OnGetRelationFailed(this, m_retCode);
}
