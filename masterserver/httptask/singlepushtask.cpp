#include "pch.h"
#include "singlepushtask.h"
#include <time.h>
#include <sstream>

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "util.h"
#include "util/UtilEncode.h"
#include "HashUtil.h"
#include "util/jsonutil.h"
#include "config.h"
#include <iostream>
#include <fstream>

#include "appid.h"
#include "role/rolesummarymgr.h"
#include "role/rolemanager.h"

const std::string SinglePushTask::GetUrl()
{
	AddCommParams();
	if (m_platType == KKSG::PLAT_ANDROID) 
	{
		SetParam("message_type", "1");	// 1 通知 2透传消息
	}
	else 
	{
		SetParam("message_type", "0");
	}

	SetParam("message", m_message.toJson(m_platType));
	//SSDebug << "message: " << m_message.toJson(m_platType) << END;

	if (m_pushType == PushType_Account)
	{
		SetParam("account", m_message.token);
		return GenerateUrl(API_SINGLE_ACCOUNT);
	}
	else
	{
		SetParam("device_token", m_message.token);
		return GenerateUrl(API_SINGLE_DEVICE);
	}
}

void SinglePushTask::OnResult(const std::string &response)
{
	SSDebug << "Result: " << response << END;
	rapidjson::Document doc;
	doc.Parse(response.c_str());
	if (doc.HasParseError() || !IsSuccess(doc))
	{
		OnFailed();
	}
}

void SinglePushTask::OnFailed()
{

}


