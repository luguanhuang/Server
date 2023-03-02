#include "pch.h"
#include "getlistsubscribe.h"
#include "config.h"
#include "crpc.h"
#include "util/jsonutil.h"
#include "appid.h"
#include "push/pushsubscribemgr.h"
#include "util/gametime.h"

CGetListSubscribeTask::CGetListSubscribeTask()
{
	m_strUri= "/profile/subscribe_getlist";
}

CGetListSubscribeTask::~CGetListSubscribeTask()
{

}

void CGetListSubscribeTask::OnResult(const std::string &response)
{
	//SSDebug << response << END;
	rapidjson::Document doc;
	doc.Parse(response.c_str());
	if (doc.HasParseError())
	{
		SSWarn << "parser getlist subscribe res failed, " << response << END;
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
	if (!doc.HasMember("more"))
	{
		SSWarn << "has no more in res" << END;
		OnFailed();
		return;
	}
	KKSG::GetListSubscribeRes oData;
	bool ismore = doc["more"].GetBool();
	oData.set_more(ismore);
	if (!doc.HasMember("subscribeList"))
	{
		SSWarn << "has no subscribeList in res" << END;
		OnFailed();
		return;
	}
	const rapidjson::Value& roMember = JsonUtil::GetJsonValue(doc, "subscribeList");
	if (!roMember.IsArray())
	{
		SSWarn << "subscribeList not a list" << END;
		OnFailed();
		return;
	}
	for (UINT32 i = 0; i < roMember.Size(); i ++)
	{
		UINT32 id = JsonUtil::GetJsonUint(roMember[i], "id");
		std::string title = JsonUtil::GetJsonString(roMember[i], "title");
		bool status = JsonUtil::GetJsonBool(roMember[i], "status");
		UINT32 MsgId = CPushSubscribeMgr::Instance()->GetMsgIdByTxMsgId(id);
		if (MsgId == UINT32(-1))
		{
			SSWarn << "config error,txmsgid=" << id <<   "not find in table/pushsubscribe.txt" << END;
			continue;
		}
		KKSG::SubScribe*plist =  oData.add_list();
		plist->set_id(MsgId);
		plist->set_title(title);
		plist->set_status(status);
	}
	oData.set_result(KKSG::ERR_SUCCESS);
	CUserData data;
	data.m_dwUserData = KKSG::ERR_SUCCESS;
	data.m_pUserPtr = (void*)&oData;
	CRpc::ReplyDelayRpc(m_rpcId, data);
}

void CGetListSubscribeTask::OnFailed()
{
	CUserData data;
	data.m_dwUserData = KKSG::ERR_FAILED;
	data.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(m_rpcId, data);
	SSDebug << "failed" << END;
}

void CGetListSubscribeTask::BuildPostData()
{
	JsonBuilder oBuilder;
	oBuilder.AddMember_String("appid", m_strAppid);
	oBuilder.AddMember_String("accessToken", m_Token);
	oBuilder.AddMember_String("openid", m_strOpenId);
	std::string str = oBuilder.GetString();
	SSDebug << str << END;
	SetHttpPostString(str);
}
