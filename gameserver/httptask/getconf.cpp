#include "pch.h"
#include "getuserinfo.h"
#include "httptaskmgr.h"
#include "config.h"
#include "util/jsonutil.h"
#include "util.h"
#include "unit/rolemanager.h"
#include "idip/idiprecord.h"
#include "gamelogic/antiaddictionmgr.h"
#include "getconf.h"

GetConfTask::GetConfTask()
{
	SetType(true);
}

GetConfTask::~GetConfTask()
{

}

const std::string GetConfTask::GetUrl()
{
	return GSConfig::Instance()->GetAntiAddictionUrl();
}

void GetConfTask::OnFailed()
{
	CAntiAddictionMgr::Instance()->CheckRetryGetConf();
}

void GetConfTask::OnResult(const std::string &response)
{
	SSDebug << response << END;
	rapidjson::Document doc;
	doc.Parse(response.c_str());
	if (doc.HasParseError())
	{
		SSWarn << "parser update user info res failed, " << response << END;
		OnFailed();
		return;
	}

	if (!doc.HasMember("comm_rsp"))
	{
		SSWarn << "has no comm_rsp" << END; 
		OnFailed();
		return;
	}
	const rapidjson::Value& roRsp = JsonUtil::GetJsonValue(doc, "comm_rsp");

	if (!roRsp.HasMember("ret"))
	{
		SSWarn << "has no ret in comm_rsp" << END; 
		OnFailed();
		return;
	}
	INT32 ret = JsonUtil::GetJsonInt(roRsp, "ret");
	if (ret != 0)
	{
		std::string msg = JsonUtil::GetJsonString(roRsp, "err_msg");
		SSWarn <<  "return failed, ret=" << ret << "err_msg=" << msg << END;
		OnFailed();
		return;
	}
	if (!doc.HasMember("game_conf_info"))
	{
		SSWarn << "no user_info info" << END;
		OnFailed();
		return;
	}
	const rapidjson::Value& roMember = JsonUtil::GetJsonValue(doc, "game_conf_info");
	if (!roMember.HasMember("child_once_game_rest_time_list") || !roMember.HasMember("adult_once_game_rest_time_list"))
	{
		SSWarn << "no child_once_game_rest_time_list or adult_once_game_rest_time_list" << END;
		OnFailed();
		return;
	}
	std::vector<UINT32> adult_once_game_rest_time_list;
	std::vector<UINT32> child_once_game_rest_time_list;
	JsonUtil::GetJsonArrayUint(roMember, "adult_once_game_rest_time_list", adult_once_game_rest_time_list);
	JsonUtil::GetJsonArrayUint(roMember, "child_once_game_rest_time_list", child_once_game_rest_time_list);
	UINT32 child_once_game_force_exit_time = JsonUtil::GetJsonInt(roMember, "child_once_game_force_exit_time");
	UINT32 child_once_game_force_rest_time = JsonUtil::GetJsonInt(roMember, "child_once_game_force_rest_time");
	UINT32 adult_once_game_force_exit_time = JsonUtil::GetJsonInt(roMember, "adult_once_game_force_exit_time");
	UINT32 adult_once_game_force_rest_time = JsonUtil::GetJsonInt(roMember, "adult_once_game_force_rest_time");

	CAntiAddictionMgr::Instance()->SetConf(child_once_game_rest_time_list, child_once_game_force_exit_time, child_once_game_force_rest_time, adult_once_game_rest_time_list, adult_once_game_force_exit_time, adult_once_game_force_rest_time);
}

void GetConfTask::BuildPostData()
{
	JsonBuilder oBuilder;
	rapidjson::Value oBody(rapidjson::kObjectType);
	SetHttpPostString(CAntiAddictionMgr::Instance()->FillPostData((INT32)GSConfig::Instance()->GetPlatType(), GSConfig::Instance()->GetServerID(), MSG_TYPE_GET_CONF, oBody, oBuilder));
}

