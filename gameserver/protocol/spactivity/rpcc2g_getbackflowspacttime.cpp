#include "pch.h"
#include "spactivity/rpcc2g_getbackflowspacttime.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "spactivity//spactivityrecord.h"
#include "util/gametime.h"

// generate by ProtoGen at date: 2017/3/22 19:09:28

RPC_SERVER_IMPLEMETION(RpcC2G_GetBackFlowSpActTime, GetBackFlowSpActTimeArg, GetBackFlowSpActTimeRes)

void RpcC2G_GetBackFlowSpActTime::OnCall(const GetBackFlowSpActTimeArg &roArg, GetBackFlowSpActTimeRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	roRes.set_errorcode(KKSG::ERR_SUCCESS);
	UINT32 lastBackFlowStartTime = pRole->Get<SpActivityRecord>()->GetLastBackFlowStartTime();
	UINT32 endTime = lastBackFlowStartTime + GetGlobalConfig().BackFlowActivityDayTime * 24 * 3600;
	UINT32 nowTime = GameTime::GetTime();
	if (endTime > nowTime)
	{
		roRes.set_lefttime(ToString(endTime - nowTime));
	}
	else
	{
		roRes.set_lefttime("0");
	}
}

void RpcC2G_GetBackFlowSpActTime::OnDelayReplyRpc(const GetBackFlowSpActTimeArg &roArg, GetBackFlowSpActTimeRes &roRes, const CUserData &roUserData)
{
}
