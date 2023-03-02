#include "pch.h"
#include  "pushtask.h"
#include <time.h>
#include <sstream>

#include "rapidjson/document.h"
#include "util.h"
#include "util/UtilEncode.h"
#include "HashUtil.h"
#include "util/jsonutil.h"
#include "config.h"
#include <iostream>
#include <fstream>
#include "config.h"

#include "appid.h"

const std::string PushTask::g_sMethod = "GET";

const std::string PushTask::API_SINGLE_ACCOUNT = "/v2/push/single_account";
const std::string PushTask::API_SINGLE_DEVICE = "/v2/push/single_device";
const std::string PushTask::API_CREATE_MULTIPUSH = "/v2/push/create_multipush";
const std::string PushTask::API_MULTI_DEVICE = "/v2/push/device_list_multiple";
const std::string PushTask::API_MULTI_ACCOUNT = "/v2/push/account_list_multiple";

std::string PushMessage::toJson(UINT32 platType)
{
	if (platType == KKSG::PLAT_ANDROID) {
		return toJsonAndroid();
	} else {
		return toJsonIOS();
	}
}

static rapidjson::Value GetTimeObject(JsonBuilder& builder, time_t sec)
{
	struct tm st;
#ifdef WIN32
	localtime_s(&st, &sec);
#else
	localtime_r(&sec, &st);
#endif

	rapidjson::Value timeOject(rapidjson::kObjectType);
	rapidjson::Value hourValue(rapidjson::kStringType);
	hourValue.SetString(ToString(st.tm_hour), builder.Allocator());
	timeOject.AddMember("hour", hourValue, builder.Allocator());

	rapidjson::Value minValue(rapidjson::kStringType);
	minValue.SetString(ToString(st.tm_min), builder.Allocator());
	timeOject.AddMember("min", minValue, builder.Allocator());

	return timeOject;
}

std::string PushMessage::toJsonAndroid() 
{
	JsonBuilder builder;
	builder.AddMember_String("title", title);
	builder.AddMember_String("content", content);
	builder.AddMember_Int("builder_id", 0);//TODO, 必填

	// 推送时间
	rapidjson::Value acceptValue(rapidjson::kArrayType);
	UINT32 now = time(0) - 120;
	UINT32 endTime = now + 600;
	rapidjson::Value timeObject(rapidjson::kObjectType);
	timeObject.AddMember("start", GetTimeObject(builder, now), builder.Allocator());
	timeObject.AddMember("end", GetTimeObject(builder, endTime), builder.Allocator());
	acceptValue.PushBack(timeObject, builder.Allocator());
	builder.AddMember_Value("accept_time", acceptValue);

	return builder.GetString();
}

std::string PushMessage::toJsonIOS()
{
	JsonBuilder builder;
	rapidjson::Value appsValue(rapidjson::kObjectType);
	rapidjson::Value alertValue(rapidjson::kStringType);
	alertValue.SetString(content, builder.Allocator());
	appsValue.AddMember("alert", alertValue, builder.Allocator());

	rapidjson::Value badgeValue(rapidjson::kNumberType);
	badgeValue.SetInt(5);
	appsValue.AddMember("badge", badgeValue, builder.Allocator());

	builder.AddMember_Value("aps", appsValue);

	// 推送时间
	rapidjson::Value acceptValue(rapidjson::kArrayType);
	UINT32 now = time(0) - 120;
	UINT32 endTime = now + 600;
	rapidjson::Value timeObject(rapidjson::kObjectType);
	timeObject.AddMember("start", GetTimeObject(builder, now), builder.Allocator());
	timeObject.AddMember("end", GetTimeObject(builder, endTime), builder.Allocator());
	acceptValue.PushBack(timeObject, builder.Allocator());
	builder.AddMember_Value("accept_time", acceptValue);

	return builder.GetString();
}

PushTask::PushTask()
:m_envType(1)
,m_pushType(PushType_DeviceToken)
,m_platType(KKSG::PLAT_IOS)
{
}

 PushTask::~PushTask()
 {
 }

void PushTask::SetParam(const std::string& sKey, const std::string& value)
{
	m_params[sKey] = value;
}

void PushTask::AddCommParams()
{
	if (m_platType == KKSG::PLAT_ANDROID) 
	{
		SetParam("access_id", ToString(XinGeAccessIdAnd));
	}
	else 
	{
		SetParam("access_id", ToString(XinGeAccessIdIOS));
		SetParam("environment", ToString(m_envType)); // 仅ios必需
	}
	SetParam("timestamp", ToString(time(NULL)));
	SetParam("multi_pkg", "1");
}

std::string PushTask::GenerateUrl(const std::string& sApi)
{
	std::ostringstream oss;
	std::ostringstream ossSign;
	oss << "http://" << MSConfig::Instance()->GetXingeUrl() << sApi << "?";
	ossSign << g_sMethod << MSConfig::Instance()->GetXingeUrl() << sApi;
	for (auto it = m_params.begin(); it != m_params.end(); ++it)
	{
		if (it != m_params.begin()) 
		{
			oss << "&";
		}
		oss << it->first << "=" << UtilEncode::UrlEncode(it->second);
		ossSign << it->first << "=" << it->second;
	}
	if (m_platType == KKSG::PLAT_ANDROID) 
	{
		ossSign << XinGeSecretKeyAnd;
	}
	else 
	{
		ossSign << XinGeSecretKeyIOS;
	}
	oss << "&sign=" << Md5Hash(ossSign.str());

//	SSDebug << "Url: " <<  oss.str() << END;
	return oss.str();	
}

bool PushTask::IsSuccess(rapidjson::Value& json)
{
	INT32 ret = JsonUtil::GetJsonInt(json, "ret_code");	
	if (ret == 0)
	{
		return true;
	}
	std::string errMsg = JsonUtil::GetJsonString(json, "err_msg");
	SSError << "ret code: " << ret << ", err_msg: " << errMsg << END;
	for (auto it = m_params.begin(); it != m_params.end(); it++)
	{
		SSError << "key: " << it->first << ", value: " << it->second << END;
	}
	return false;
}
