#include "pch.h"
#include "task/rpcc2g_getweeklytaskreward.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/taskrecord.h"

// generate by ProtoGen at date: 2017/9/5 10:03:12

RPC_SERVER_IMPLEMETION(RpcC2G_GetWeeklyTaskReward, GetWeeklyTaskRewardArg, GetWeeklyTaskRewardRes)

void RpcC2G_GetWeeklyTaskReward::OnCall(const GetWeeklyTaskRewardArg &roArg, GetWeeklyTaskRewardRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (!pRole)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	TaskRecord* taskRecord = pRole->Get<TaskRecord>();
	WeeklyTaskRecord& wRecord = taskRecord->GetWeeklyRecord();
	KKSG::ErrorCode code = KKSG::ERR_INVALID_REQUEST;
	if (roArg.type() == DailyTaskRewardType_Sub)
	{
		code = wRecord.TakeOneReward(roArg.value());
	}
	else if (roArg.type() == DailyTaskRewardType_Total)
	{
		code = wRecord.TakeBoxReward(roArg.value());
	}
	roRes.set_result(code);
	roRes.set_score(wRecord.GetScore());
}

void RpcC2G_GetWeeklyTaskReward::OnDelayReplyRpc(const GetWeeklyTaskRewardArg &roArg, GetWeeklyTaskRewardRes &roRes, const CUserData &roUserData)
{
}
