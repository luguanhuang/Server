#include "pch.h"
#include "joingrouptask.h"
#include "HashUtil.h"
#include "timeutil.h"
#include "appid.h"
#include "config.h"
#include "util.h"
#include "guild/guildbindgrouphelper.h"

static std::string InterFace = "/relation/join_groupv2?";

JoinGroupTask::JoinGroupTask()
:m_platCode(0)
{
	m_strUrl = MSConfig::Instance()->GetMsdkUrl();
}

JoinGroupTask::~JoinGroupTask()
{

}

const std::string JoinGroupTask::GetUrl()
{
	AddParam("appid", m_strAppID);
	AddParam("openid", m_openId);
	AddParam("accessToken", m_token);
	AddParam("guild_id", ToString(m_guildId));
	AddParam("zone_id", ToString(m_serverId));
	AddParam("gc", ToString(m_groupCode));
	AddParam("platid", m_platType == KKSG::PLAT_IOS ? "2" : "1");
	AddParam("roleid", ToString(m_roleId));
	AddParam("partition", ToString(m_serverId));
	BuildPostData();

	std::ostringstream oss;
	oss << m_strUrl << InterFace << GetUri();
//	SSDebug << oss.str() << END;
	return oss.str();
}

void JoinGroupTask::OnResult(const std::string &response)
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
		OnFailed();
		SSError << "return failed,res=" << response << END;
		return;
	}

	BindGroupHelper::OnJoinGroupSuccess(this);
}

void JoinGroupTask::OnFailed()
{
	BindGroupHelper::OnJoinGroupFailed(this, m_retCode, m_platCode);
}