#include "pch.h"
#include "pushendgame.h"
#include "config.h"
#include "gamelogic/antiaddictionmgr.h"
#include "util/gametime.h"

PushEndGameTask::PushEndGameTask()
{
	m_iPlatID = 0;
	m_uServerID = 0;
	SetType(true);
}

PushEndGameTask::~PushEndGameTask()
{

}

const std::string PushEndGameTask::GetUrl()
{
	return GSConfig::Instance()->GetAntiAddictionUrl();
}

void PushEndGameTask::SetPlatID(INT32 iPlatID)
{
	m_iPlatID = iPlatID;
}

void PushEndGameTask::SetServerID(UINT32 uServerID)
{
	m_uServerID = uServerID;
}

void PushEndGameTask::OnResult(const std::string &response)
{
	SSDebug << response << END;
}

void PushEndGameTask::OnFailed()
{

}

void PushEndGameTask::BuildPostData(Role* role)
{
	JsonBuilder oBuilder;
	rapidjson::Value oBody(rapidjson::kObjectType);
	oBody.AddMember("account_id", role->GetAccount(), oBuilder.Allocator());
	oBody.AddMember("character_id", ToString(role->GetID()), oBuilder.Allocator());
	oBody.AddMember("this_period_time", INT32(GameTime::GetTime()-role->GetAntiAddictionTime()), oBuilder.Allocator());
	SetHttpPostString(CAntiAddictionMgr::Instance()->FillPostData(m_iPlatID, m_uServerID, MSG_TYPE_PUSH_EDNGAME, oBody, oBuilder));
}
