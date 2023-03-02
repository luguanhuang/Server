#include "pch.h"
#include "antiaddictionmgr.h"
#include "util/gametime.h"
#include "httptask/pushendgame.h"
#include "httptask/httptaskmgr.h"
#include "httptask/updateuserinfo.h"
#include "unit/rolemanager.h"
#include "httptask/reportreminded.h"
#include "common/ptcg2c_antiaddictionremindntf.h"
#include "reportdatamgr.h"
#include "kickaccount/ptcg2m_kickaccountjkydg2m.h"
#include "kickaccount/ptcg2c_kickaccountjkydntf.h"
#include "network/mslink.h"
#include "httptask/getuserinfo.h"
#include "httptask/getconf.h"
#include "idip/idiprecord.h"
#include "mail/mailconfig.h"
#include "timeutil.h"

INSTANCE_SINGLETON(CAntiAddictionMgr)

CAntiAddictionMgr::CAntiAddictionMgr()
{
	m_iSeqID = 0;
	m_IsRetryGetConf = false;
	m_IsGetConf = false;

	m_ChildOnceGameRestTimeList.insert(m_ChildOnceGameRestTimeList.end(), GetGlobalConfig().ChildOnceGameRestTimeList.begin(), GetGlobalConfig().ChildOnceGameRestTimeList.end());
	m_ChildOnceGameForceExitTime = GetGlobalConfig().ChildOnceGameForceExitTime;
	m_ChildOnceGameForceRestTime = GetGlobalConfig().ChildOnceGameForceRestTime;

	m_AdultOnceGameRestTimeList.insert(m_AdultOnceGameRestTimeList.end(), GetGlobalConfig().AdultOnceGameRestTimeList.begin(), GetGlobalConfig().AdultOnceGameRestTimeList.end());
	m_AdultOnceGameForceExitTime = GetGlobalConfig().AdultOnceGameForceExitTime;
	m_AdultOnceGameForceRestTime = GetGlobalConfig().AdultOnceGameForceRestTime;
	m_TimeHandler = CTimerMgr::Instance()->SetTimer(this, 0, 30*1000, -1, __FILE__, __LINE__);
}

CAntiAddictionMgr::~CAntiAddictionMgr()
{

}

bool CAntiAddictionMgr::Init()
{
	GetConf();
	return true;
}

void CAntiAddictionMgr::Uninit()
{
	if (m_TimeHandler)
	{
		CTimerMgr::Instance()->KillTimer(m_TimeHandler);
	}
}

//每分钟调用该接口一次;
void CAntiAddictionMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	if (!GetGlobalConfig().HgIsIpen)
	{
		return;
	}
	std::vector<Role*> oUserList;
	std::vector<Role*> oRemindList;
	for(auto i = RoleManager::Instance()->Begin(); i != RoleManager::Instance()->End(); i++)
	{
		Role* pRole = *i;
		if (NULL == pRole || !pRole->IsGetHg())
		{
			continue;
		}
		//检查是否满了5分钟，上报数据;
		if ((pRole->GetAntiAddictionTime() + AntiAddictionCheckTime) <= GameTime::GetTime())
		{
			oUserList.push_back(pRole);

		}

		//检查单次在线时间是否满了X小时 且没有通知过;
		if (!pRole->GetSendAntiAddictionRemind() && pRole->GetOnlineTimeFromLogin() >= GetRemindTime(pRole))
		{
			oRemindList.push_back(pRole);
			PtcG2C_AntiAddictionRemindNtf ptc;
			ptc.m_Data.set_type(KKSG::ReportTypeSingle);
			ptc.m_Data.set_remindmsg(GetRemindMsg(pRole, GetRemindTime(pRole), KKSG::ReportTypeSingle));
			pRole->Send(ptc);

			pRole->SetAntiAddictionRemindCount(pRole->GetAntiAddictionRemindCount()+1);
			//通知客户端弹框，然后上报tx防沉迷服务器;

			if (pRole->GetAntiAddictionRemindCount() >= GetConfRemindCount(pRole))//不需要再提醒了
			{
				pRole->SetSendAntiAddictionRemind(true);
			}
		}

		//检查是否单次踢下线;
		if (!pRole->GetIsForceExit() && pRole->GetOnlineTimeFromLogin() >= GetForceExitTime(pRole))
		{
			pRole->SetAntiAddictionHg(HealthyGameFlag5, GetRestTime(pRole), GetForceExitTime(pRole));
			pRole->SetIsForceExit(true);
		}

	}
	UpdateUserInfo(oUserList);
	UpdateRemind(oRemindList);

}
void CAntiAddictionMgr::UpdateUserInfo(std::vector<Role*>& oUserList)
{
	for (UINT32 i = 0; i < oUserList.size(); i += MAX_POST_COUNT)
	{
		JsonBuilder oBuilder;
		rapidjson::Value oBodyList(rapidjson::kArrayType);
		UpdateUserInfoTask* poTask = new UpdateUserInfoTask();
		poTask->SetPlatID(oUserList[0]->GetClientConf().platid());
		poTask->SetServerID(oUserList[0]->GetServerId());
		for (UINT32 j = i; j < i + MAX_POST_COUNT && j < oUserList.size(); j ++)
		{
			poTask->AddBodyData(oUserList[j]->GetID(), oUserList[j]->GetAccount(), INT32(GameTime::GetTime()-oUserList[j]->GetAntiAddictionTime()), oBodyList, oBuilder);
			ReportDataMgr::Instance()->DailyOnline(oUserList[j], INT32(GameTime::GetTime()-oUserList[j]->GetAntiAddictionTime()));
			oUserList[j]->SetAntiAddictionTime(GameTime::GetTime());
		}
		rapidjson::Value oBody(rapidjson::kObjectType);
		oBody.AddMember("user_info", oBodyList, oBuilder.Allocator());
		poTask->BuildPostData(oBody, oBuilder);
		HttpTaskMgr::Instance()->AddTask(poTask);
	}
}

void CAntiAddictionMgr::UpdateRemind(std::vector<Role*>& oRemindList)
{
	for (UINT32 i = 0; i < oRemindList.size(); i += MAX_POST_COUNT)
	{
		JsonBuilder oBuilder;
		rapidjson::Value oBodyList(rapidjson::kArrayType);
		ReportRemindedTask* poTask = new ReportRemindedTask();
		poTask->SetPlatID(oRemindList[0]->GetClientConf().platid());
		poTask->SetServerID(oRemindList[0]->GetServerId());
		for (UINT32 j = i; j < i + MAX_POST_COUNT && j < oRemindList.size(); j ++)
		{
			poTask->AddBodyData(oRemindList[j], HgReportType1, oBodyList, oBuilder);
		}
		rapidjson::Value oBody(rapidjson::kObjectType);
		oBody.AddMember("remind_info", oBodyList, oBuilder.Allocator());
		poTask->BuildPostData(oBody, oBuilder);
		HttpTaskMgr::Instance()->AddTask(poTask);
	}
}

void CAntiAddictionMgr::OnLogin(Role* role)
{
	if (NULL == role)
	{
		return;
	}
	GetUserInfoTask* poTask = new GetUserInfoTask();
	poTask->BuildPostData(role);
	HttpTaskMgr::Instance()->AddTask(poTask);
}

void CAntiAddictionMgr::LoginOut(Role* role)
{
	if (NULL == role || !role->IsGetHg() || (UINT32)GameTime::GetTime() <= role->GetAntiAddictionTime())
	{
		return ;
	}
	PushEndGameTask* poTask = new PushEndGameTask();
	poTask->SetPlatID(role->GetClientConf().platid());
	poTask->SetServerID(role->GetServerId());
	poTask->BuildPostData(role);
	HttpTaskMgr::Instance()->AddTask(poTask);
}
std::string CAntiAddictionMgr::FillPostData(INT32 iPlatId, UINT32 serverid, INT32 iMsgType, rapidjson::Value& oBody , JsonBuilder& oBuilder)
{
	rapidjson::Value oComMsg(rapidjson::kObjectType);
	oComMsg.AddMember("seq_id", ++ m_iSeqID, oBuilder.Allocator());
	oComMsg.AddMember("msg_type", iMsgType, oBuilder.Allocator());
	oComMsg.AddMember("version", "1.0", oBuilder.Allocator());
	oComMsg.AddMember("appid", ToString(GSConfig::Instance()->GetAppType()==KKSG::GAME_APP_QQ?MobileQQ_App_ID:WeChat_App_ID), oBuilder.Allocator());
	oComMsg.AddMember("plat_id", iPlatId==KKSG::PLAT_IOS?1:2, oBuilder.Allocator());
	oComMsg.AddMember("area", serverid, oBuilder.Allocator());
	oBuilder.AddMember_Value("common_msg", oComMsg);
	oBuilder.AddMember_Value("body_info", oBody);
	std::string str = oBuilder.GetString();
	SSDebug << str << END;
	return str;
}

void CAntiAddictionMgr::KickAccountNtfClientMsg(UINT64 roleidBeKicked, KKSG::KickType kt, UINT32 endTime, const std::string& strMsg, UINT32 hgflag)
{
	if (endTime < UINT32(GameTime::GetTime()))
	{
		return;
	}
    Role* pRole = RoleManager::Instance()->FindByRoleID(roleidBeKicked);
    if (NULL == pRole)
    {
        return;
    }
	ReportRemindByHgFlag(pRole, hgflag);

	PtcG2C_KickAccountJkydNtf msg2client;
	msg2client.m_Data.set_kt(kt);
	msg2client.m_Data.set_msg(strMsg);
	pRole->Send(msg2client);

    PtcG2M_KickAccountJkydG2M msg2ms;
    msg2ms.m_Data.set_roleid(roleidBeKicked);
    msg2ms.m_Data.set_kt(kt);
	msg2ms.m_Data.mutable_odata()->set_account(pRole->GetAccount());
	msg2ms.m_Data.mutable_odata()->set_roleid(pRole->GetID());
	msg2ms.m_Data.mutable_odata()->set_punishdate(UINT32(GameTime::GetTime()));
	msg2ms.m_Data.mutable_odata()->set_punishtype(KKSG::PUNISH_USER_HG);
	msg2ms.m_Data.mutable_odata()->set_bantime(endTime-UINT32(GameTime::GetTime()));
	msg2ms.m_Data.mutable_odata()->set_reason(strMsg);
	msg2ms.m_Data.mutable_odata()->set_op(KKSG::PUNISH_DATA_UPATE);

    MSLink::Instance()->SendTo(roleidBeKicked, msg2ms);


}

void CAntiAddictionMgr::GetConf()
{
	GetConfTask* poTask = new GetConfTask();
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
}

void CAntiAddictionMgr::CheckRetryGetConf()
{
	if (m_IsRetryGetConf)
	{
		return;
	}
	GetConf();
	m_IsRetryGetConf = true;
}

void CAntiAddictionMgr::SetConf(std::vector<UINT32>& ChildList, UINT32 ChildExitTime, UINT32 ChildRestTime, std::vector<UINT32>& AdultTimeList, UINT32 AdultExitTime, UINT32 AdultRestTime)
{
	if (ChildList.size())
	{
		m_ChildOnceGameRestTimeList.clear();
		m_ChildOnceGameRestTimeList.insert(m_ChildOnceGameRestTimeList.end(), ChildList.begin(), ChildList.end());
	}
	m_ChildOnceGameForceExitTime = ChildExitTime?ChildExitTime:m_ChildOnceGameForceExitTime;
	m_ChildOnceGameForceRestTime = ChildRestTime?ChildRestTime:m_ChildOnceGameForceRestTime;

	if (AdultTimeList.size())
	{
		m_AdultOnceGameRestTimeList.clear();
		m_AdultOnceGameRestTimeList.insert(	m_AdultOnceGameRestTimeList.end(), AdultTimeList.begin(), AdultTimeList.end());
	}
	m_AdultOnceGameForceExitTime = AdultExitTime?AdultExitTime:m_AdultOnceGameForceExitTime;
	m_AdultOnceGameForceRestTime = AdultRestTime?AdultRestTime:m_AdultOnceGameForceRestTime;
}

std::string CAntiAddictionMgr::GetRemindMsg(Role* role, UINT32 iTime, INT32 type)
{
	std::string str;
	if (role)
	{
		str = MailConfig::Instance()->ReplaceString("X", ToString(iTime/3600), type==KKSG::ReportTypeSingle?GetGlobalConfig().OnceGameRemindMsg:GetGlobalConfig().TotalGameRemindMsg);
		UINT32 mins = iTime%3600/60;
		mins = mins>=30?30:0;
		str = MailConfig::Instance()->ReplaceString("M", ToString(mins), str); 
	}
	return str;
}


UINT32 CAntiAddictionMgr::GetConfRemindCount(Role* role)
{
	if (role == NULL)
	{
		return 0;
	}
	return role->Get<CIdipRecord>()->GetAdultType()?m_AdultOnceGameRestTimeList.size():m_ChildOnceGameRestTimeList.size();
}

UINT32 CAntiAddictionMgr::GetRemindTime(Role* role)
{
	if (role == NULL)
	{
		return 0;
	}

	if (role->Get<CIdipRecord>()->GetAdultType())
	{
		return role->GetAntiAddictionRemindCount() >= m_AdultOnceGameRestTimeList.size() ? m_AdultOnceGameRestTimeList[m_AdultOnceGameRestTimeList.size()-1] : m_AdultOnceGameRestTimeList[role->GetAntiAddictionRemindCount()];
	}
	return role->GetAntiAddictionRemindCount() >= m_ChildOnceGameRestTimeList.size() ? m_ChildOnceGameRestTimeList[m_ChildOnceGameRestTimeList.size()-1] : m_ChildOnceGameRestTimeList[role->GetAntiAddictionRemindCount()];
}
UINT32 CAntiAddictionMgr::GetRestTime(Role* role)
{
	if (role == NULL)
	{
		return 0;
	}
	return role->Get<CIdipRecord>()->GetAdultType()?m_AdultOnceGameForceRestTime:m_ChildOnceGameForceRestTime;
}

UINT32 CAntiAddictionMgr::GetForceExitTime(Role* role)
{
	if (role == NULL)
	{
		return 0;
	}
	return role->Get<CIdipRecord>()->GetAdultType()?m_AdultOnceGameForceExitTime:m_ChildOnceGameForceExitTime;
}
void CAntiAddictionMgr::ReportRemindByHgFlag(Role* role, UINT32 hgflag)
{
	if (NULL == role)
	{
		return;
	}
	INT32 reporttype = 0;
	if (hgflag == HealthyGameFlag2) reporttype = HgReportType4;
	else if (hgflag == HealthyGameFlag3) reporttype = HgReportType5;
	else if (hgflag == HealthyGameFlag4) reporttype = HgReportType6;
	else if (hgflag == HealthyGameFlag5) reporttype = HgReportType3;
	else return;
	ReportRemind(role, reporttype);
}
void CAntiAddictionMgr::ReportRemind(Role* pRole, INT32 reporttype)
{
	if (NULL == pRole)
	{
		return;
	}
	JsonBuilder oBuilder;
	rapidjson::Value oBodyList(rapidjson::kArrayType);
	ReportRemindedTask* poTask = new ReportRemindedTask();
	poTask->SetPlatID(pRole->GetClientConf().platid());
	poTask->SetServerID(pRole->GetServerId());
	poTask->AddBodyData(pRole, reporttype, oBodyList, oBuilder);
	rapidjson::Value oBody(rapidjson::kObjectType);
	oBody.AddMember("remind_info", oBodyList, oBuilder.Allocator());
	poTask->BuildPostData(oBody, oBuilder);
	HttpTaskMgr::Instance()->AddTask(poTask);
}

std::string CAntiAddictionMgr::GetForceExitMsg(Role* role, INT32 type, UINT32 nextLoginTime, UINT32 gameTime, UINT32 restTime)
{
	std::string str;
	if (role)
	{
		if (type == HealthyGameFlag2)
		{
			str = GetGlobalConfig().TotalGameForceExitMsg;

		}
		else if (type == HealthyGameFlag5)
		{
			str = GetGlobalConfig().OnceGameForceExitMsg;
		}
		else
		{
			str = GetGlobalConfig().LoginRemindMsg;
		}
		str = MailConfig::Instance()->ReplaceString("X", ToString(gameTime/3600), str);
		str = MailConfig::Instance()->ReplaceString("Y", ToString((nextLoginTime-UINT32(GameTime::GetTime()))/60), str);
		str = MailConfig::Instance()->ReplaceString("Z", ToString(TimeUtil::GetDateTimeStr(nextLoginTime)), str);
	}
	return str;
}

