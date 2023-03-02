#include "pch.h"
#include "reportreminded.h"
#include "config.h"
#include "gamelogic/antiaddictionmgr.h"
#include "util/gametime.h"
#include "idip/idiprecord.h"

ReportRemindedTask::ReportRemindedTask()
{
	SetType(true);
	m_iPlatID = 0;
	m_uServerID = 0;
}

ReportRemindedTask::~ReportRemindedTask()
{

}

const std::string ReportRemindedTask::GetUrl()
{
	return GSConfig::Instance()->GetAntiAddictionUrl();
}

void ReportRemindedTask::SetPlatID(INT32 iPlatID)
{
	m_iPlatID = iPlatID;
}

void ReportRemindedTask::SetServerID(UINT32 uServerID)
{
	m_uServerID = uServerID;
}

void ReportRemindedTask::OnResult(const std::string &response)
{
	SSDebug << response << END;
}

void ReportRemindedTask::OnFailed()
{

}

void ReportRemindedTask::AddBodyData(Role* pRole, INT32 iType, rapidjson::Value& oBodyList , JsonBuilder& oBuilder)
{
	rapidjson::Value body(rapidjson::kObjectType);
	body.AddMember("account_id", pRole->GetAccount(), oBuilder.Allocator());
	body.AddMember("character_id", ToString(pRole->GetID()), oBuilder.Allocator());
	body.AddMember("report_type", iType, oBuilder.Allocator());
	body.AddMember("report_time", INT32(GameTime::GetTime()), oBuilder.Allocator());
	body.AddMember("is_adult", pRole->Get<CIdipRecord>()->GetAdultType()?AdultType1:AdultType0, oBuilder.Allocator());
	oBodyList.PushBack(body, oBuilder.Allocator());
}
void ReportRemindedTask::BuildPostData(rapidjson::Value& oBodyList , JsonBuilder& oBuilder)
{
	SetHttpPostString(CAntiAddictionMgr::Instance()->FillPostData(m_iPlatID, m_uServerID, MSG_TYPE_REPORT_REMINDED_BATCH, oBodyList, oBuilder));
}
