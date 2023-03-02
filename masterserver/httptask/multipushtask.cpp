#include "pch.h"
#include "multipushtask.h"
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
#include "push/xingepushmgr.h"

const std::string CreateMultiPushTask::GetUrl()
{
	AddCommParams();
	if (m_platType == KKSG::PLAT_ANDROID) 
	{
		SetParam("message_type", "1");
	}
	else 
	{
		SetParam("message_type", "0");
	}
	SetParam("message", m_message.toJson(m_platType));
//	SSDebug << "message: " << m_message.toJson(m_platType) << END;
	return GenerateUrl(API_CREATE_MULTIPUSH);
}

void CreateMultiPushTask::OnResult(const std::string &response)
{
	SSDebug << "Result: " << response << END;
	rapidjson::Document doc;
	doc.Parse(response.c_str());
	if (doc.HasParseError() || !IsSuccess(doc))
	{
		OnFailed();
		return;
	}
	if (!doc.HasMember("result") || !doc["result"].HasMember("push_id"))
	{
		SSError << "no push_id" << END;
		OnFailed();
		return;
	}
	std::string sPushId = doc["result"]["push_id"].GetString();
	UINT32 pushId = convert<UINT32>(sPushId);
	m_mgr->OnCreatePushSuccess(m_id, pushId);
}

void CreateMultiPushTask::OnFailed()
{
	m_mgr->OnCreatePushFailed(m_id);
}

const std::string MultiPushTask::GetUrl()
{
	AddCommParams();
	SetParam("push_id", ToString(m_pushId));
	if (m_pushType == PushType_Account)
	{
		return GenerateUrl(API_MULTI_ACCOUNT);
	}
	else 
	{
		return GenerateUrl(API_MULTI_DEVICE);
	}
}

void MultiPushTask::PackTokens(std::list<CRoleSummary*>& roleList)
{
	rapidjson::Document	doc(rapidjson::kArrayType); 
	UINT32 i = 0;
	while (i < kMaxNumPerPush)
	{
		if (roleList.empty())
		{
			break;
		}
		CRoleSummary* role = roleList.front();
		roleList.pop_front();
		if (role == NULL)
		{
			continue;
		}
		rapidjson::Value token(rapidjson::kStringType);
		if (m_pushType == PushType_Account) {
			token.SetString(role->GetAccount(), doc.GetAllocator());
		} else {
			token.SetString(role->GetDeviceId(), doc.GetAllocator());
		}
		doc.PushBack(token, doc.GetAllocator());
		i++;
	}

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	if (m_pushType == PushType_Account) {
		SetParam("account_list", buffer.GetString());
	} else {
		SetParam("device_list", buffer.GetString());
	}
}

void MultiPushTask::OnResult(const std::string &response)
{
	SSInfo << "Result: " << response << END;
}

void MultiPushTask::OnFailed()
{
}