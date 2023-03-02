#include "pch.h"
#include "qqgrouptaskbase.h"
#include "HashUtil.h"
#include "timeutil.h"
#include "appid.h"
#include "config.h"
#include "util.h"
#include "util/UtilEncode.h"
#include "util/jsonutil.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "config.h"

QQGroupTaskBase::QQGroupTaskBase()
:m_rpcId(0)
,m_guildId(0)
,m_serverId(0)
,m_roleId(0)
,m_retCode(-1)
,m_strAppID(MobileQQ_App_ID)
,m_strAppKey(MobileQQ_App_Key)
{
	SetType(true);
}

QQGroupTaskBase::~QQGroupTaskBase()
{

}

void QQGroupTaskBase::AddParam(const std::string& key, const std::string& value)
{
	m_params[key] = value;
}

std::string QQGroupTaskBase::GetParamJsonString()
{
	JsonBuilder builder;
	for (auto it = m_params.begin(); it != m_params.end(); ++it)
	{
		//builder.AddMember_String(it->first, UtilEncode::UrlEncode(it->second));
		builder.AddMember_String(it->first, it->second);
	}

	return builder.GetString();
}

std::string QQGroupTaskBase::GetSign(UINT32 now, const std::string& key)
{
	std::ostringstream oss;
	oss << key << now;
	return Md5Hash(oss.str());
}
std::string QQGroupTaskBase::GetUri()
{
	UINT32 now = TimeUtil::GetTime();
	std::ostringstream ss;
	ss << "timestamp=" << now
		<< "&appid=" << m_strAppID
		<< "&sig=" << GetSign(now, m_strAppKey)
		<< "&openid=" << m_openId
		<< "&encode=1";
	return ss.str();
}

void QQGroupTaskBase::SetGuild(UINT64 id, const std::string& name, UINT32 serverId)
{ 
	m_guildId = id;
	m_guildName = name;
	m_serverId = serverId;
}

void QQGroupTaskBase::SetRoleId(UINT64 id)
{ 
	m_roleId = id; 
	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(id);
	if (summary)
	{
		SetPlatType(summary->GetPlatType());
	}
	else
	{
		SetPlatType(MSConfig::Instance()->GetPlatType());
	}
}

INT32 QQGroupTaskBase::GetRetCode(rapidjson::Value& json)
{
	if (!json.HasMember("ret"))
	{
		SSError << "no ret field" << END;
		return -1;
	}
	m_retCode = json["ret"].GetInt();	
	if (m_retCode != 0)
	{
		//std::string errMsg = JsonUtil::GetJsonString(dataJson, "msg");
		//SSError << "code: " << m_retCode << ", msg: " << errMsg << END;
		for (auto it = m_params.begin(); it != m_params.end(); it++)
		{
			SSError << "key: " << it->first << ", value: " << it->second << END;
		}
	}
	return m_retCode;
}
void QQGroupTaskBase::BuildPostData()
{
	std::string str = GetParamJsonString();
	SSDebug << str << END;
	SetHttpPostString(str);
}