#include "pch.h"
#include "task/rpcc2g_refreshweeklytask.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/taskrecord.h"

// generate by ProtoGen at date: 2017/9/5 10:05:10

RPC_SERVER_IMPLEMETION(RpcC2G_RefreshWeeklyTask, RefreshWeeklyTaskArg, RefreshWeeklyTaskRes)

void RpcC2G_RefreshWeeklyTask::OnCall(const RefreshWeeklyTaskArg &roArg, RefreshWeeklyTaskRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (!pRole)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	TaskRecord* taskRecord = pRole->Get<TaskRecord>();
	WeeklyTaskRecord& wRecord = taskRecord->GetWeeklyRecord();
	KKSG::ErrorCode code = wRecord.RefreshOne(roArg.index());
	WeeklyTaskOne* pSubTask = wRecord.GetSubTaskByIndex<WeeklyTaskOne>(roArg.index());
	if (pSubTask)
	{
		pSubTask->ToKKSG(*roRes.mutable_task());
	}
	roRes.set_result(code);
}

void RpcC2G_RefreshWeeklyTask::OnDelayReplyRpc(const RefreshWeeklyTaskArg &roArg, RefreshWeeklyTaskRes &roRes, const CUserData &roUserData)
{
}
