#include "pch.h"
#include "task/rpcc2g_dailytaskgiveup.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/designationMgr.h"
#include "gamelogic/QAMgr.h"
#include "gamelogic/QARecord.h"
#include "gamelogic/taskrecord.h"

// generate by ProtoGen at date: 2016/11/21 10:54:21

RPC_SERVER_IMPLEMETION(RpcC2G_DailyTaskGiveUp, DailyTaskGiveUpArg, DailyTaskGiveUpRes)

void RpcC2G_DailyTaskGiveUp::OnCall(const DailyTaskGiveUpArg &roArg, DailyTaskGiveUpRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (!pRole)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	TaskRecord* taskRecord = pRole->Get<TaskRecord>();
	DailyTaskRecord& dailyRecord = taskRecord->GetDailyRecord();
	roRes.set_result(dailyRecord.GiveUp());
}

void RpcC2G_DailyTaskGiveUp::OnDelayReplyRpc(const DailyTaskGiveUpArg &roArg, DailyTaskGiveUpRes &roRes, const CUserData &roUserData)
{
}
