#include "pch.h"
#include "dailytaskrecord.h"
#include "table/TaskMgr.h"
#include "unit/role.h"
#include "scene/scene.h"
#include "utility/loghelper.h"
#include "foreach.h"
#include "task/ptcg2c_takerandomtask.h"
#include "util/XCommon.h"
#include "utility/tlogger.h"
#include "bag.h"
#include "payv2Record.h"
#include "task/ptcg2c_updatetaskstatus.h"
#include "foreach.h"
#include "designationMgr.h"
#include "gamelogic/taskrecord.h"
#include "bagtransition.h"
#include "gamelogic/bag.h"
#include "define/systemiddef.h"
#include "task/ptcg2m_removeaskitemntf.h"
#include "event/eventmgr.h"
#include "define/eventdef.h"
#include "task/rpcg2m_dailytaskrefreshg2m.h"
#include "spactivity/spactivityhandler.h"

// 每日任务次数
static const UINT32 kDailyTaskNum = 1;

DailyTaskRecord::DailyTaskRecord()
:PeriodTask(KKSG::PeriodTaskType_Daily)
,m_score(1)
,m_luck(1)
{
}

DailyTaskRecord::~DailyTaskRecord()
{
}

bool DailyTaskRecord::Load(const KKSG::RoleTask& data)
{
	m_acceptLevel = data.daily_accept_level();
	m_rewarded = data.daily_rewarded();
	m_completeNum = data.daily_complete_num();
	m_redPoint = data.daily_red_point();
	m_askHelpNum = data.daily_askhelp_num();
	m_DonateNum = data.today_donate_count();
	m_totalDonateNum = data.total_donate_count();
	m_accepted = data.dailyaccept();
	m_periodTaskNum = data.daily_count();
	m_score = data.score();
	m_luck = data.luck();
	ClearSubTasks();

	for (int i = 0; i < data.dailytask_size(); ++i)
	{
		const KKSG::DailyTaskInfo& info = data.dailytask(i);
		DailyTask::RowData* pTask = TaskMgr::Instance()->GetDailyTask(info.id());
		if (!pTask)
		{
			continue;
		}
		DailyTaskOne* task = new DailyTaskOne(this);
		task->FromKKSG(info, pTask);
		m_tasks.push_back(task);
	}
	CheckRedPoint();
	if (!data.has_score())
	{
		CheckScore();
	}
	return true;
}

void DailyTaskRecord::Save(KKSG::RoleTask& data)
{
	data.set_daily_accept_level(m_acceptLevel);
	data.set_daily_rewarded(m_rewarded);
	data.set_daily_complete_num(m_completeNum);
	data.set_daily_red_point(m_redPoint);
	data.set_daily_askhelp_num(m_askHelpNum);
	data.set_today_donate_count(m_DonateNum);
	data.set_total_donate_count(m_totalDonateNum);
	data.set_dailyaccept(m_accepted);
	data.set_daily_count(m_periodTaskNum);
	data.set_score(m_score);
	data.set_luck(m_luck);

	data.clear_dailytask();
	for (size_t i = 0; i < m_tasks.size(); ++i)
	{
		SubTask* task = m_tasks[i];
		KKSG::DailyTaskInfo* info = data.add_dailytask();	
		task->ToKKSG(*info);
	}
}

void DailyTaskRecord::OnSubTaskAccept(SubTask* pSubTask)
{
	CheckAndUpdate(pSubTask);
	DoTxLog(pSubTask->id, TASK_STATE_ACCEPTED);
}

void DailyTaskRecord::OnSubTaskFinish(SubTask* pSubTask)
{
	CheckRedPoint();
}

void DailyTaskRecord::OnTaskOneReward(SubTask* pSubTask, DailyTask::RowData* pConfig)
{
	CheckRedPoint();
	if (pSubTask->askHelp)
	{
		PtcG2M_RemoveAskItemNtf ntf;
		ntf.m_sessionID = m_role->GetSessionID();
		ntf.m_Data.set_taskid(pSubTask->id);
		m_role->SendToMS(ntf);
	}

	DoTxLog(pSubTask->id, TASK_STATE_COMPLETED);

	if (m_completeNum == 1)
	{
		UpdateCanRefreshToMs();
	}
	SpActivityHandler::GlobalHandler.OnSubTaskComplete(m_role, 1);
}

void DailyTaskRecord::OnFirstEnterScene()
{
	CheckRedPoint();
	UpdateCanRefreshToMs();
}

void DailyTaskRecord::OnDayPass()
{
	m_askHelpNum = 0;
	m_DonateNum = 0;
	m_redPoint = false;
	m_periodTaskNum = 0;

	for (auto it = m_tasks.begin(); it != m_tasks.end(); ++it)
	{
		SubTask* task = *it;
		task->askHelp = false;
	}
	m_record->SetDirty();
}

void DailyTaskRecord::Refresh()
{
	m_accepted = true;
	m_acceptLevel = m_role->GetLevel();
	m_rewarded = false;
	m_completeNum = 0;
	m_redPoint = false;

	UINT32 formatID = TaskMgr::Instance()->RandomDailyTaskFormat(m_luck);
	DailyTaskFormat::RowData* pFormat = TaskMgr::Instance()->GetFormat(formatID);
	if (formatID == 0 || pFormat == NULL)
	{
		LogWarn("random format failed, level[%u] periodType[%u], luck[%u]", m_role->GetLevel(), m_type, m_luck);
		return;
	}
	m_periodTaskNum++;
	Refresh(pFormat);
	UpdateNewAcceptToMs();
}

void DailyTaskRecord::RefreshWithCheck(UINT32 formatId)
{
	if (!CanRefresh())
	{
		return;
	}
	DailyTaskFormat::RowData* pFormat = TaskMgr::Instance()->GetFormat(formatId);
	if (pFormat == NULL)
	{
		return;
	}
	Refresh(pFormat);
}

void DailyTaskRecord::Refresh(DailyTaskFormat::RowData* format)
{
	std::vector<DailyTask::RowData*> tasks;
	TaskMgr::Instance()->RandomDailyTaskWithFormat(format, m_type, m_role->IsBackFlowRole(), m_role->GetLevel(), tasks);

	ClearSubTasks();
	m_tasks.resize(tasks.size());

	for (size_t i = 0; i < tasks.size(); ++i)
	{
		DailyTask::RowData* data = tasks[i];
		DailyTaskOne* task = new DailyTaskOne(this);
		m_tasks[i] = task;
		task->id = data->taskID;
		task->type = data->tasktype;
		task->step = 0;
		task->rewarded = false;
		task->condIds = data->conditionId;
		task->maxStep = data->conditionNum;
		OnSubTaskAccept(task);
	}
	CheckRedPoint();
	m_score = format->score;
	m_record->SetDirty();
}

KKSG::ErrorCode DailyTaskRecord::TakeReward()
{
	if (m_accepted == false)
	{
		return KKSG::ERR_TASK_NOT_ACCEPT;
	}
	if (m_rewarded)
	{
		return KKSG::ERR_TASK_ALREADY_TAKE;
	}
	UINT32 minNum = TaskMgr::Instance()->GetMinTaskNum(m_acceptLevel);
	if (m_completeNum < minNum)
	{
		return KKSG::ERR_TASK_NOT_FINISH;
	}
	DailyTaskReward::RowData* reward = TaskMgr::Instance()->GetDailyReward(m_type, m_acceptLevel, DailyTaskRewardType_Total, m_completeNum);
	if (reward == NULL)
	{
		return KKSG::ERR_TASK_NO_TABLE;
	}

	BagGiveItemTransition transition(m_role);
	transition.SetReason(ItemFlow_Task, ItemFlow_Task_Finish);
	std::vector<Sequence<UINT32, 2>>* taskreward = TaskMgr::Instance()->GetDailyTaskTotalReward(reward, m_score);
	if (taskreward)
	{
		for (auto i = taskreward->begin(); i != taskreward->end(); ++i)
		{
			transition.GiveItem(i->seq[0], i->seq[1]);
		}
	}
	std::vector<Sequence<UINT32, 2>>* extrareward =  TaskMgr::Instance()->GetDailyTaskExtraReward(reward, m_score);
	if (extrareward)
	{
		for (auto i = extrareward->begin(); i != extrareward->end(); ++i)
		{
			transition.GiveItem(i->seq[0], i->seq[1]);
		}
	}
	transition.NotifyClient();

	// 记录
	m_rewarded = true;
	m_accepted = false;

	m_record->FinishDailyTask();
	m_record->SetDirty();
	CheckRedPoint();

	// 日志
	SSInfo << "RoleId: " << m_role->GetID() << ", take total reward, complete num: " << m_completeNum << END;

	EventMgr::Instance()->AddEvent(m_role->GetID(), MENTOR_EVENT_OFFSET + KKSG::MentorTask_DailyTask, 0, 1);
	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode DailyTaskRecord::GiveUp()
{
	UINT32 minNum = TaskMgr::Instance()->GetMinTaskNum(m_acceptLevel);
	if (m_completeNum >= minNum)
	{
		return KKSG::ERR_TASK_CANNOT_GIVEUP;
	}
	// 记录
	m_rewarded = true;
	m_accepted = false;
	ClearSubTasks();
	m_record->FinishDailyTask();
	m_record->SetDirty();

	CheckRedPoint();
	// 日志
	SSInfo << "RoleId: " << m_role->GetID() << ", giveup daily task, complete num: " << m_completeNum << END;
	return KKSG::ERR_SUCCESS;
}

void DailyTaskRecord::FillDailyTaskInfo(KKSG::GetDailyTaskInfoRes& res)
{
	for (auto it = m_tasks.begin(); it != m_tasks.end(); ++it)
	{
		SubTask* task = *it;
		KKSG::DailyTaskInfo* info = res.add_task();
		task->ToKKSG(*info);
	}
	res.set_is_rewarded(m_rewarded);
	res.set_count(GetRemainDailyCount());
	res.set_accept_level(m_acceptLevel);
	res.set_askhelp_num(m_askHelpNum);
	res.set_score(m_score);
}

void DailyTaskRecord::CheckRedPoint()
{
	bool oldRedPoint = m_redPoint;

	m_redPoint = false;
	for (auto it = m_tasks.begin(); it != m_tasks.end(); ++it)
	{
		SubTask* task = *it;
		if (task->step >= task->maxStep && task->rewarded == false)
		{
			m_redPoint = true;
			break;
		}
	}
	if (m_completeNum >= TaskMgr::Instance()->GetMaxTaskNum(m_acceptLevel) && m_rewarded == false)
	{
		m_redPoint = true;
	}

	if (oldRedPoint == false && m_redPoint)
	{
		// 推送红点
		m_role->HintNotify(SYS_DAILY_TASK, false);
		m_record->SetDirty();
	}
	else if (oldRedPoint && m_redPoint == false)
	{
		// 推送红点消失
		m_role->HintNotify(SYS_DAILY_TASK, true);
		m_record->SetDirty();
	}
}

void DailyTaskRecord::GMFinish()
{
	for (size_t i = 0; i < m_tasks.size(); ++i)
	{
		SubTask* task = m_tasks[i];
		if (task->type == KKSG::TaskConn_StageID)
		{
			task->step = task->maxStep;
		}
		else  if (task->type == KKSG::TaskConn_ItemID)
		{
			BagGiveItemTransition transition(m_role);
			transition.SetReason(ItemFlow_GM, ItemFlow_GM_Give);
			transition.GiveItem(task->condIds[0], task->maxStep);
			transition.NotifyClient();
		}
	}
	CheckRedPoint();
}

void DailyTaskRecord::GMReset()
{
	ClearSubTasks();
	m_rewarded = false;
	m_completeNum = 0;
	m_askHelpNum = 0;
	m_DonateNum = 0;
	m_totalDonateNum = 0;
	m_periodTaskNum = 0;
	CheckRedPoint();
}

void DailyTaskRecord::GMResetHelpNum()
{
	m_askHelpNum = 0;
	m_DonateNum = 0;
	m_totalDonateNum = 0;
}

UINT32 DailyTaskRecord::GetRemainDailyCount()
{
	return kDailyTaskNum > m_periodTaskNum ? (kDailyTaskNum - m_periodTaskNum) : 0;
}

bool DailyTaskRecord::CanAccept()
{
	if (GetRemainDailyCount() == 0)
	{
		return false;
	}
	if (!m_role->Get<CRoleSystem>()->IsSystemOpened(SYS_DAILY_TASK))
	{
		return false;
	}
	return true;
}

void DailyTaskRecord::DoTxLog(UINT32 id, INT32 state)
{
	TTaskFlowLog oLog(m_role);
	oLog.m_nTaskID = id;
	oLog.m_nTaskType = TaskType_Daily;
	oLog.m_nState = state;
	oLog.Do();
}

bool DailyTaskRecord::CanRefresh()
{
	if (!m_accepted)
	{
		return false;
	}
	if (m_rewarded)
	{
		return false;
	}

	for (auto it = m_tasks.begin(); it != m_tasks.end(); ++it)
	{
		SubTask* pSubTask = *it;
		if (pSubTask->rewarded)
		{
			return false;
		}
	}
	return true;
}

void DailyTaskRecord::UpdateScoreToMs()
{
	RpcG2M_DailyTaskRefreshG2M* rpc = RpcG2M_DailyTaskRefreshG2M::CreateRpc();
	rpc->m_sessionID = m_role->GetSessionID();
	rpc->m_oArg.set_roleid(m_role->GetID());
	rpc->m_oArg.set_type(TaskRefreshType_UpdateScore);
	rpc->m_oArg.set_score(m_score);
	m_role->SendToMS(*rpc);
}

void DailyTaskRecord::UpdateCanRefreshToMs()
{
	RpcG2M_DailyTaskRefreshG2M* rpc = RpcG2M_DailyTaskRefreshG2M::CreateRpc();
	rpc->m_sessionID = m_role->GetSessionID();
	rpc->m_oArg.set_roleid(m_role->GetID());
	if (CanRefresh())
	{
		rpc->m_oArg.set_type(TaskRefreshType_CanRefresh);
	}
	else
	{
		rpc->m_oArg.set_type(TaskRefreshType_CantRefresh);
	}
	rpc->m_oArg.set_score(m_score);
	m_role->SendToMS(*rpc);
}

void DailyTaskRecord::UpdateNewAcceptToMs()
{
	RpcG2M_DailyTaskRefreshG2M* rpc = RpcG2M_DailyTaskRefreshG2M::CreateRpc();
	rpc->m_sessionID = m_role->GetSessionID();
	rpc->m_oArg.set_roleid(m_role->GetID());
	rpc->m_oArg.set_type(TaskRefreshType_NewAccept);
	rpc->m_oArg.set_score(m_score);
	m_role->SendToMS(*rpc);
}

void DailyTaskRecord::SetLuck(UINT32 luck)
{
	m_luck = luck;
	m_record->SetDirty();
}

void DailyTaskRecord::CheckScore()
{
	if (m_score)
	{
		return;
	}

	UINT32 num = 0;
	for (auto it = m_tasks.begin(); it != m_tasks.end(); ++it)
	{
		SubTask* pTask = *it;
		DailyTask::RowData* pData = TaskMgr::Instance()->GetDailyTask(pTask->id);
		if (pData == NULL)
		{
			return;
		}
		if (pData->taskquality == 3)
		{
			num++;
		}
	}
	if (num >= 4) m_score = DTFQ_S;
	else if (num >= 3) m_score = DTFQ_A;
	else if (num >= 2) m_score = DTFQ_B;
	else if (num >= 1) m_score = DTFQ_C;
	else m_score = DTFQ_D;
	SetDirty();
	UpdateScoreToMs();
}


