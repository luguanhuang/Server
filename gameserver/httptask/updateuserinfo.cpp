#include "pch.h"
#include "updateuserinfo.h"
#include "config.h"
#include "gamelogic/antiaddictionmgr.h"
#include "util/gametime.h"
#include "httptaskmgr.h"
#include "unit/rolemanager.h"
#include "common/ptcg2c_antiaddictionremindntf.h"
#include "reportreminded.h"

UpdateUserInfoTask::UpdateUserInfoTask()
{
	SetType(true);
}

UpdateUserInfoTask::~UpdateUserInfoTask()
{

}

const std::string UpdateUserInfoTask::GetUrl()
{
	return GSConfig::Instance()->GetAntiAddictionUrl();
}

void UpdateUserInfoTask::SetPlatID(INT32 iPlatID)
{
	m_iPlatID = iPlatID;
}

void UpdateUserInfoTask::SetServerID(UINT32 uServerID)
{
	m_uServerID = uServerID;
}

void UpdateUserInfoTask::OnResult(const std::string &response)
{
	SSDebug << response << END;
	//std::string str = "{\"comm_rsp\":{\"ret\":0,\"err_msg\":\"ok!\"},\"user_info\":[{\"account_id\":\"573\",\"accumu_time\":55800,\"is_need_reminded\":0,\"character_id\":\"285767142757870\",\"healthy_game_flag\":4,\"force_exit_rest_time\":0,\"curfew_end_time\":0,\"ban_end_time\":1508358880}]}";
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
		SSWarn << "no user_info list" << END;
		return;
	}
	const rapidjson::Value& roMember = JsonUtil::GetJsonValue(doc, "user_info");
	if (!roMember.IsArray())
	{
		SSWarn << "user_info not a list" << END;
		return;
	}
	for (UINT32 i = 0; i < roMember.Size(); i ++)
	{
		INT32 healthy_game_flag = JsonUtil::GetJsonInt(roMember[i], "healthy_game_flag");
		if (healthy_game_flag == HealthyGameFlag0 || healthy_game_flag > HealthyGameFlag4)
		{
			continue;
		}
		std::string strOpenId = JsonUtil::GetJsonString(roMember[i], "account_id");
		std::string strRoleId = JsonUtil::GetJsonString(roMember[i], "character_id");
		if (strRoleId.empty())
		{
			continue;
		}
		UINT64 RoleId = convert<UINT64>(strRoleId);
		INT32 iAccumuTime = JsonUtil::GetJsonInt(roMember[i], "accumu_time");

		Role* pRole = RoleManager::Instance()->FindByRoleID(RoleId);
		if (NULL == pRole)
		{
			continue;
		}
		UINT32 force_exit_rest_time = JsonUtil::GetJsonUint(roMember[i], "force_exit_rest_time");
		UINT32 curfew_end_time = JsonUtil::GetJsonUint(roMember[i], "curfew_end_time");
		UINT32 ban_end_time = JsonUtil::GetJsonUint(roMember[i], "ban_end_time");
		if (healthy_game_flag == HealthyGameFlag1)//ÐÝÏ¢ÌáÐÑ;
		{
			PtcG2C_AntiAddictionRemindNtf ptc;
			ptc.m_Data.set_type(KKSG::ReportTypeTotal);
			ptc.m_Data.set_remindmsg(CAntiAddictionMgr::Instance()->GetRemindMsg(pRole, iAccumuTime, KKSG::ReportTypeTotal));
			pRole->Send(ptc);

			CAntiAddictionMgr::Instance()->ReportRemind(pRole, HgReportType2);
		}
		else
		{
			UINT32 bantime = 0;
			if (healthy_game_flag == HealthyGameFlag2)
			{
				bantime = force_exit_rest_time;
			}
			else if (healthy_game_flag == HealthyGameFlag3)
			{
				bantime = curfew_end_time;
			}
			else if (healthy_game_flag == HealthyGameFlag4)
			{
				bantime =  ban_end_time;
			}
			pRole->SetAntiAddictionHg(healthy_game_flag, bantime, iAccumuTime);
		}
	}
}

void UpdateUserInfoTask::OnFailed()
{

}

void UpdateUserInfoTask::BuildPostData(rapidjson::Value& oBody , JsonBuilder& oBuilder)
{
	SetHttpPostString(CAntiAddictionMgr::Instance()->FillPostData(m_iPlatID, m_uServerID, MSG_TYPE_UPDATE_USERINFO_BATCH, oBody, oBuilder));
}
void UpdateUserInfoTask::AddBodyData(UINT64 roleid, std::string strOpenId, INT32 iOnlineTime, rapidjson::Value& oBodyList , JsonBuilder& oBuilder)
{
	rapidjson::Value body(rapidjson::kObjectType);
	body.AddMember("account_id", strOpenId, oBuilder.Allocator());
	body.AddMember("character_id", ToString(roleid), oBuilder.Allocator());
	//body.AddMember("this_period_time", INT32(GameTime::GetTime()), oBuilder.Allocator());
	body.AddMember("this_period_time", iOnlineTime, oBuilder.Allocator());
	oBodyList.PushBack(body, oBuilder.Allocator());
}
