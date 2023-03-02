#include "pch.h"
#include "personalcareerinfo.h"
#include "config.h"


template <typename T, int N>
Sequence<T, N> ConvertToSequence(const std::string& s, char sep)
{
	Sequence<T, N> result;
	std::vector<std::string> vec = Split(s, sep);
	assert(vec.size() == N);
	for (unsigned i = 0; i < vec.size(); ++i)
	{
		result[i] = SafeConvert<T>(vec[i]);
	}

	return result;
}

PersonalCarrerInfoTask::PersonalCarrerInfoTask()
	:rpc_id_(0)
{
	SetType(false);
	SetTimeOut(3000);
}

PersonalCarrerInfoTask::~PersonalCarrerInfoTask()
{

}

const std::string PersonalCarrerInfoTask::GetUrl()
{
	string str_get = "openid="+open_id_+"&zoneid="+ToString(server_id_)+"&roleid="+ToString(role_id_);
	//string str_get = "openid=oiz-ewXeHfq434XGai3ghDAeGVaA&zoneid=9501&roleid=208613765713308932";
	return GSConfig::Instance()->GetDataMoreUrl()+"/info?"+str_get;
}

void PersonalCarrerInfoTask::OnResult(const std::string &response)
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
	res.set_server_name(server_name_);
	res.set_continue_login_days(JsonUtil::GetJsonInt(roMember,"max_login"));
	string d_rearch_date = JsonUtil::GetJsonString(roMember,"d_rearch_date");
	string d_chaoxue_date = JsonUtil::GetJsonString(roMember,"d_chaoxue_date");
	string d_longben_date = JsonUtil::GetJsonString(roMember,"d_longben_date");

	std::vector<std::string> vec;
	if (d_rearch_date.length() > 0 )
	{
		// 防错处理
		if (string::npos != d_rearch_date.find(';'))
		{
			vec.clear();
			vec = Split(d_rearch_date, ';');		
			for(size_t i = 0; i < vec.size(); ++i)
			{
				// 再防错
				if (string::npos != vec[i].find(':'))
				{
					Sequence<UINT32,2> seq = ConvertToSequence<UINT32,2>(vec[i],':');
					UINT32 scene_template_id = seq.seq[0];
					SceneInfo *sceneInfo = SceneConfig::Instance()->FindSceneInfo(scene_template_id);
					if (sceneInfo == NULL)
					{
						LogInfo("Scene template id %u is not exist", scene_template_id);
						continue;
					}
					KKSG::CareerData* data = res.add_career_data();
					data->set_type(KKSG::CARRER_DATA_LEVEL);
					data->set_para1(seq.seq[0]);
					data->set_time(seq.seq[1]);	
				}
			}
		}
	}

	if (d_chaoxue_date.length() > 0 )
	{
		// 防错处理
		if (string::npos != d_chaoxue_date.find(';'))
		{
			vec.clear();
			vec = Split(d_chaoxue_date, ';');		
			for(size_t i = 0; i < vec.size(); ++i)
			{
				// 再防错
				if (string::npos != vec[i].find(':'))
				{
					Sequence<UINT32,2> seq = ConvertToSequence<UINT32,2>(vec[i],':');
					UINT32 scene_template_id = seq.seq[0];
					SceneInfo *sceneInfo = SceneConfig::Instance()->FindSceneInfo(scene_template_id);
					if (sceneInfo == NULL)
					{
						LogInfo("Scene template id %u is not exist", scene_template_id);
						continue;
					}
					KKSG::CareerData* data = res.add_career_data();
					data->set_type(KKSG::CARRER_DATA_NEST);
					data->set_para1(seq.seq[0]);
					data->set_time(seq.seq[1]);	
				}
			}
		}
	}

	if (d_longben_date.length() > 0 )
	{
		// 防错处理
		if (string::npos != d_longben_date.find(';'))
		{
			vec.clear();
			vec = Split(d_longben_date, ';');
			for(size_t i = 0; i < vec.size(); ++i)
			{
				// 再防错
				if (string::npos != vec[i].find(':'))
				{
					Sequence<UINT32,2> seq = ConvertToSequence<UINT32,2>(vec[i],':');
					KKSG::CareerData* data = res.add_career_data();
					data->set_type(KKSG::CARRER_DATA_DRAGON);
					data->set_para1(seq.seq[0]);
					data->set_time(seq.seq[1]);		
				}
			}
		}
	}

	if (rpc_id_ > 0)
	{
		CUserData oUserData;
		oUserData.m_dwUserData = 0;
		oUserData.m_pUserPtr = (void*)(&res);
		CRpc::ReplyDelayRpc(rpc_id_, oUserData);
	}
}

void PersonalCarrerInfoTask::OnFailed()
{
	KKSG::PersonalCareerTotalInfoRes  res;
	res.set_server_name(server_name_);
	SSWarn << "query PersonalCarrerInfoTask failed, openId = " << open_id_ << ", role: " << role_id_ << END;
	if (rpc_id_ > 0)
	{
		CUserData oUserData;
		oUserData.m_dwUserData = 0;
		oUserData.m_pUserPtr = (void*)(&res);;
		CRpc::ReplyDelayRpc(rpc_id_, oUserData);
	}
}


