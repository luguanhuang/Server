#include "pch.h"
#include "getuserinfo.h"
#include "httptaskmgr.h"
#include "config.h"
#include "util/jsonutil.h"
#include "util.h"
#include "unit/rolemanager.h"
#include "idip/idiprecord.h"
#include "gamelogic/antiaddictionmgr.h"

GetUserInfoTask::GetUserInfoTask()
{
	SetType(true);
}

GetUserInfoTask::~GetUserInfoTask()
{

}

const std::string GetUserInfoTask::GetUrl()
{
	return GSConfig::Instance()->GetAntiAddictionUrl();
}

void GetUserInfoTask::OnFailed()
{

}

void GetUserInfoTask::OnResult(const std::string &response)
{
	SSDebug << response << END;
	//std::string str = "{\"comm_rsp\":{\"ret\":0,\"err_msg\":\"ok!\"},\"user_info\":[{\"account_id\":\"573\",\"accumu_time\":29060,\"is_need_reminded\":1,\"character_id\":\"567242104338705\"}]}";
	rapidjson::Document doc;
	doc.Parse(response.c_str());
	if (doc.HasParseError())
	{
		SSWarn << "parser update user info res failed, " << response << END;
		return;
	}

	if (!doc.HasMember("comm_rsp"))
	{
		SSWarn << "has no comm_rsp" << END; 
		return;
	}
	const rapidjson::Value& roRsp = JsonUtil::GetJsonValue(doc, "comm_rsp");

	if (!roRsp.HasMember("ret"))
	{
		SSWarn << "has no ret in comm_rsp" << END; 
		return;
	}
	INT32 ret = JsonUtil::GetJsonInt(roRsp, "ret");
	if (ret != 0)
	{
		std::string msg = JsonUtil::GetJsonString(roRsp, "err_msg");
		SSWarn <<  "return failed, ret=" << ret << "err_msg=" << msg << END;
		return;
	}
	if (!doc.HasMember("user_info"))
	{
		SSWarn << "no user_info info" << END;
		return;
	}
	const rapidjson::Value& roMember = JsonUtil::GetJsonValue(doc, "user_info");
	std::string character_id = JsonUtil::GetJsonString(roMember, "character_id");
	INT32 isaduct = JsonUtil::GetJsonInt(roMember, "is_adult");
	UINT64 roleid = convert<UINT64>(character_id);
	Role* pRole = RoleManager::Instance()->FindByRoleID(roleid);
	if (NULL == pRole)
	{
		return;
	}
	SSDebug << "roleid=" << roleid << ",isadult=" << isaduct << END;
	pRole->Get<CIdipRecord>()->SetAdultType(isaduct);
	pRole->SetIsOpenHg(true);
}

void GetUserInfoTask::BuildPostData(Role* role)
{
	JsonBuilder oBuilder;
	rapidjson::Value oBody(rapidjson::kObjectType);
	oBody.AddMember("account_id", role->GetAccount(), oBuilder.Allocator());
	oBody.AddMember("character_id", ToString(role->GetID()), oBuilder.Allocator());
	oBody.AddMember("access_token", role->GetClientConf().token().empty()?"963D455F07C9983CA6204B6D84755194":role->GetClientConf().token(), oBuilder.Allocator());//Îª¿ÕÐ´ËÀÖ÷Òª·½±ãÄÚÍøÕËºÅ²âÊÔ;
	SetHttpPostString(CAntiAddictionMgr::Instance()->FillPostData(role->GetClientConf().platid(), role->GetServerId(), MSG_TYPE_GET_USERINFO_SINGLE, oBody, oBuilder));
}

