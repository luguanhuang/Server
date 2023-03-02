#include "pch.h"
#include "task/rpcc2g_getweeklytaskinfo.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/taskrecord.h"
#include "util/gametime.h"

// generate by ProtoGen at date: 2017/9/5 9:59:47

RPC_SERVER_IMPLEMETION(RpcC2G_GetWeeklyTaskInfo, GetWeeklyTaskInfoArg, GetWeeklyTaskInfoRes)

void RpcC2G_GetWeeklyTaskInfo::OnCall(const GetWeeklyTaskInfoArg &roArg, GetWeeklyTaskInfoRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (!pRole)
	{
		roRes.set_score(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	TaskRecord* taskRecord = pRole->Get<TaskRecord>();
	WeeklyTaskRecord& wRecord = taskRecord->GetWeeklyRecord();
	wRecord.FillTaskInfo(roRes);
	wRecord.ClearHelperInfo();
	UINT32 now = GameTime::GetTime();
	UINT32 leftTime = GameTime::GetWeekBeginTime(now) + SECONDS_ONE_WEEK - now;
	roRes.set_lefttime(leftTime);
}

void RpcC2G_GetWeeklyTaskInfo::OnDelayReplyRpc(const GetWeeklyTaskInfoArg &roArg, GetWeeklyTaskInfoRes &roRes, const CUserData &roUserData)
{
}
