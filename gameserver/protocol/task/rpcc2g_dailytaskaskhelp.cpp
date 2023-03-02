#include "pch.h"
#include "task/rpcc2g_dailytaskaskhelp.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/designationMgr.h"
#include "gamelogic/QAMgr.h"
#include "gamelogic/QARecord.h"
#include "gamelogic/taskrecord.h"
#include "table/globalconfig.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "task/rpcg2m_askguildhelp.h"
#include "table/TaskMgr.h"
#include "task/rpcg2m_taskdonateg2m.h"


// generate by ProtoGen at date: 2016/11/16 10:01:12

RPC_SERVER_IMPLEMETION(RpcC2G_DailyTaskAskHelp, DailyTaskAskHelpArg, DailyTaskAskHelpRes)

void RpcC2G_DailyTaskAskHelp::OnCall(const DailyTaskAskHelpArg &roArg, DailyTaskAskHelpRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (!pRole)
	{
		roRes.set_code(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	CGuildSimple* guild = GuildSimpleMgr::Instance()->GetGuildSimple(pRole->getGuildId());
	if (!guild)
	{
		roRes.set_code(KKSG::ERR_NOTGUILD);
		return;
	}
	TaskRecord* taskRecord = pRole->Get<TaskRecord>();
	DailyTaskRecord& dailyRecord = taskRecord->GetDailyRecord();
	WeeklyTaskRecord& wRecord = taskRecord->GetWeeklyRecord();


	PeriodTask* pPeriodTask = NULL;
	SubTask* task = NULL;
	if (roArg.task_type() == KKSG::PeriodTaskType_Daily)
	{
		pPeriodTask = &dailyRecord;
	}
	else
	{
		pPeriodTask = &wRecord;
	}
	task = pPeriodTask->GetSubTask<SubTask>(roArg.task_id());

	if (!task)
	{
		roRes.set_code(KKSG::ERR_TASK_NOT_FOUND);
		return;
	}
	if (!task->IsItemTypeTask())
	{
		roRes.set_code(KKSG::ERR_TASK_CANNOT_HELP);
		return;	
	}
	if (task->askHelp == true)
	{
		roRes.set_code(KKSG::ERR_TASK_ALREADY_ASKED);
		return;
	}
	if (task->rewarded == true)
	{
		roRes.set_code(KKSG::ERR_TASK_ALREADY_FINISH);
		return;
	}
	if (task->step >= task->maxStep)
	{
		roRes.set_code(KKSG::ERR_TASK_ALREADY_FINISH);
		return;
	}

	if (task->condIds.empty())
	{
		roRes.set_code(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	UINT32 askNum = pPeriodTask->GetAskHelpNum();
	if (roArg.task_type() == KKSG::PeriodTaskType_Daily)
	{
		if (askNum >= GetGlobalConfig().DailyTaskAskHelpNum)
		{
			roRes.set_code(KKSG::ERR_TASK_NO_ASK_HELPNUM);
			return;
		}
	}
	else if (roArg.task_type() == KKSG::PeriodTaskType_Weekly)
	{
		if (askNum >= GetGlobalConfig().WeeklyTaskAskHelpNum)
		{
			roRes.set_code(KKSG::ERR_TASK_NO_ASK_HELPNUM);
			return;
		}
	}
	pPeriodTask->AddAskHelpNum();
	task->askHelp = true;

	RpcG2M_TaskDonateG2M* rpc = RpcG2M_TaskDonateG2M::CreateRpc();
	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_type(TaskDonateOper_Add);
	rpc->m_oArg.set_tasktype(roArg.task_type());
	rpc->m_oArg.set_roleid(pRole->GetID());
	rpc->m_oArg.set_index(task->index);

	if (task->type == KKSG::TaskConn_ItemID)
	{
		rpc->m_oArg.set_itemid(task->condIds[0]);
	}
	else
	{
		rpc->m_oArg.set_itemtype(task->condIds[0] / 100);
		rpc->m_oArg.set_itemquality(task->condIds[0] % 100);
	}

	if (roArg.task_type() == KKSG::PeriodTaskType_Daily)
	{
		rpc->m_oArg.set_itemcount(task->maxStep);
	}
	else
	{
		rpc->m_oArg.set_itemcount(task->maxStep - task->step);
	}

	rpc->m_oArg.set_rpcid(DelayRpc());
	rpc->m_oArg.set_level(pPeriodTask->GetAcceptLevel());
	rpc->m_oArg.set_taskid(task->id);
	DailyTask::RowData* tblTask = TaskMgr::Instance()->GetDailyTask(task->id);
	if (tblTask)
	{
		rpc->m_oArg.set_quality(tblTask->taskquality);
	}
	pRole->SendToMS(*rpc);

	if (roArg.task_type() == KKSG::PeriodTaskType_Weekly)
	{
		wRecord.DoTxLog(task->index, TWeekTaskType2);
	}
}

void RpcC2G_DailyTaskAskHelp::OnDelayReplyRpc(const DailyTaskAskHelpArg &roArg, DailyTaskAskHelpRes &roRes, const CUserData &roUserData)
{
	roRes.set_code((KKSG::ErrorCode)roUserData.m_dwUserData);
	if (roUserData.m_dwUserData != KKSG::ERR_SUCCESS || roUserData.m_pUserPtr == NULL)
	{
		return;
	}
	KKSG::TaskDonateG2MRes* res = (KKSG::TaskDonateG2MRes*)(roUserData.m_pUserPtr);
	roRes.set_ask_uid(res->ask_uid());
}
