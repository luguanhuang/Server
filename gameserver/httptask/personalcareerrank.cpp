#include "pch.h"
#include "personalcareerrank.h"
#include "config.h"


PersonalCarrerRankTask::PersonalCarrerRankTask()
	:rpc_id_(0)
{
	SetType(false);
	SetTimeOut(3000);
}

PersonalCarrerRankTask::~PersonalCarrerRankTask()
{

}

const std::string PersonalCarrerRankTask::GetUrl()
{
	string str_get = "openid="+open_id_+"&zoneid="+ToString(server_id_)+"&roleid="+ToString(role_id_);
	//string str_get = "openid=oiz-ewXeHfq434XGai3ghDAeGVaA&zoneid=9501&roleid=208613765713308932";
	return GSConfig::Instance()->GetDataMoreUrl()+"/rank?"+str_get;
}

void PersonalCarrerRankTask::OnResult(const std::string &response)
{
	LogDebug("get tencent response result: %s", response.c_str());

	rapidjson::Document doc;
	doc.Parse(response.c_str());
	if (doc.HasParseError())
	{
		LogError("parser json failed, %s", response.c_str());
		OnFailed();
		return;
	}

	if (!doc.HasMember("Code"))
	{
		OnFailed();
		return;
	}
	int ret = doc["Code"].GetInt();
	if (ret != 0)
	{
		LogWarn("failed, code[%d]", ret);
		OnFailed();
		return;
	}
	if (!doc.HasMember("Data"))
	{
		OnFailed();
		return;
	}
	const rapidjson::Value& roMember = JsonUtil::GetJsonValue(doc, "Data");
	if(roMember.IsNull()) 
	{
		OnFailed();
		return;
	}

	KKSG::PersonalCareerTotalInfoRes  res;
	res.set_pk_rank(pk_rank_);
	res.set_profession_rank(profession_rank_);

	res.set_all_roles(JsonUtil::GetJsonString(roMember,"pvp_defeat"));
	res.set_warrior(JsonUtil::GetJsonString(roMember,"solid_defeat"));
	res.set_archer(JsonUtil::GetJsonString(roMember,"bowman_defeat"));
	res.set_master(JsonUtil::GetJsonString(roMember,"master_defeat"));
	res.set_minister(JsonUtil::GetJsonString(roMember,"minister_defeat"));
	res.set_max_score_history(GetIntFromString(roMember, "round_score"));
	if (rpc_id_ > 0)
	{
		CUserData oUserData;
		oUserData.m_dwUserData = 0;
		oUserData.m_pUserPtr = (void*)(&res);
		CRpc::ReplyDelayRpc(rpc_id_, oUserData);
	}
}

void PersonalCarrerRankTask::OnFailed()
{
	KKSG::PersonalCareerTotalInfoRes  res;
	res.set_pk_rank(pk_rank_);
	res.set_profession_rank(profession_rank_);

	SSWarn << "query PersonalCarrerRankTask failed, openId = " << open_id_ << ", role= " << role_id_ <<  END;
	if (rpc_id_ > 0)
	{
		CUserData oUserData;
		oUserData.m_dwUserData = 0;
		oUserData.m_pUserPtr = (void*)(&res);;
		CRpc::ReplyDelayRpc(rpc_id_, oUserData);
	}
}



int PersonalCarrerRankTask::GetIntFromString(const rapidjson::Value& json, const char* key)
{
	if(json.IsNull()) return 0;

	const rapidjson::Value& member = JsonUtil::GetJsonValue(json, key);
	if(member.IsNull()) return 0;

	if(!member.IsString()) return 0;

	return convert<int>(member.GetString());
}