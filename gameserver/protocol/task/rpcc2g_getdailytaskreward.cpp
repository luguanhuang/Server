#include "pch.h"
#include "task/rpcc2g_getdailytaskreward.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/designationMgr.h"
#include "gamelogic/QAMgr.h"
#include "gamelogic/QARecord.h"
#include "gamelogic/taskrecord.h"

// generate by ProtoGen at date: 2016/11/14 20:00:27

RPC_SERVER_IMPLEMETION(RpcC2G_GetDailyTaskReward, GetDailyTaskRewardArg, GetDailyTaskRewardRes)

void RpcC2G_GetDailyTaskReward::OnCall(const GetDailyTaskRewardArg &roArg, GetDailyTaskRewardRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (!pRole)
	{
		roRes.set_code(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	TaskRecord* taskRecord = pRole->Get<TaskRecord>();
	DailyTaskRecord& dailyRecord = taskRecord->GetDailyRecord();
	KKSG::ErrorCode code = KKSG::ERR_INVALID_REQUEST;
	if (roArg.type() == DailyTaskRewardType_Sub)
	{
		code = dailyRecord.TakeOneRewardById(roArg.id());
	}
	else if (roArg.type() == DailyTaskRewardType_Total)
	{
		code = dailyRecord.TakeReward();
	}
	roRes.set_code(code);

	std::vector<SubTask*>& subtasks = dailyRecord.GetAllSubTasks();
	for (auto it = subtasks.begin(); it != subtasks.end(); ++it)
	{
		SubTask* task = *it;
		KKSG::DailyTaskInfo* info = roRes.add_task();
		task->ToKKSG(*info);
	}
}

void RpcC2G_GetDailyTaskReward::OnDelayReplyRpc(const GetDailyTaskRewardArg &roArg, GetDailyTaskRewardRes &roRes, const CUserData &roUserData)
{
}
