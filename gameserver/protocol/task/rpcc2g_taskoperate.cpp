#include "pch.h"
#include "task/rpcc2g_taskoperate.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/designationMgr.h"
#include "gamelogic/QAMgr.h"
#include "gamelogic/QARecord.h"
#include "gamelogic/taskrecord.h"

// generate by ProtoGen at date: 2015/4/17 10:21:58

RPC_SERVER_IMPLEMETION(RpcC2G_TaskOperate, TaskOPArg, TaskOPRes)

void RpcC2G_TaskOperate::OnCall(const TaskOPArg &roArg, TaskOPRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	KKSG::ErrorCode code = KKSG::ERR_INVALID_REQUEST;
	roRes.set_result(false);
	if (pRole != NULL)
	{
		if (roArg.taskop() == KKSG::TAKE_TASK)
		{
			code = pRole->Get<TaskRecord>()->TakeTask(roArg.taskid());
			roRes.set_result(true);
			SSDebug << "roleId: " << pRole->GetID() << ", Take Task: " << roArg.taskid() << ", code = " << code << END;
		}
		else if (roArg.taskop() == KKSG::FINISH_TASK)
		{
			roRes.set_result(true);
			code = pRole->Get<TaskRecord>()->TakeReward(roArg.taskid());
			QAMgr::Instance()->TriggerQA(pRole, QATYPE_ONE);
			SSDebug << "roleId: " << pRole->GetID() << ", Take Task Reward: " << roArg.taskid() << ", code = " << code << END;
		}
	}

	roRes.set_errorcode(code);
}

void RpcC2G_TaskOperate::OnDelayReplyRpc(const TaskOPArg &roArg, TaskOPRes &roRes, const CUserData &roUserData)
{
}
