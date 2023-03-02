#include "pch.h"
#include "setsubscribe.h"
#include "config.h"
#include "crpc.h"
#include "util/jsonutil.h"

CSetSubscribeTask::CSetSubscribeTask()
{
	m_strUri = "/profile/subscribe_setlist";
}

CSetSubscribeTask::~CSetSubscribeTask()
{

}

void CSetSubscribeTask::BuildPostData(const KKSG::SetSubscirbeArg& oData)
{
	m_Token = oData.token();
	JsonBuilder oBuilder;
	oBuilder.AddMember_String("appid", m_strAppid);
	oBuilder.AddMember_String("accessToken", m_Token);
	oBuilder.AddMember_String("openid", m_strOpenId);
	rapidjson::Value oList(rapidjson::kArrayType);
	for (INT32 i = 0; i < oData.msgid_size(); ++ i)
	{
		oList.PushBack(oData.msgid(i), oBuilder.Allocator());
	}
	oBuilder.AddMember_Value("msgId", oList);
	oBuilder.AddMember_Int("opType", oData.msgtype());

	std::string str = oBuilder.GetString();
	SSDebug << str << END;
	SetHttpPostString(str);
}

void CSetSubscribeTask::OnResult(const std::string &response)
{
	SSDebug << response << END;
	rapidjson::Document doc;
	doc.Parse(response.c_str());
	if (doc.HasParseError())
	{
		SSWarn << "parser set subscribe res failed, " << response << END;
		OnFailed();
		return;
	}

	if (!doc.HasMember("ret"))
	{
		SSWarn << "has no ret in res" << END;
		OnFailed();
		return;
	}
	INT32 err = doc["ret"].GetInt();
	if (err != 0)
	{
		SSWarn << "failed,ret=" << err << END;
		OnFailed();
		return;
	}
	CUserData data;
	data.m_dwUserData = KKSG::ERR_SUCCESS;
	data.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(m_rpcId, data);
}

void CSetSubscribeTask::OnFailed()
{
	CUserData data;
	data.m_dwUserData = KKSG::ERR_FAILED;
	data.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(m_rpcId, data);
	SSDebug << "failed" << END;
}

