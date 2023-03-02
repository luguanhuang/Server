#include "pch.h"
#include "weeklytaskrecord.h"
#include "unit/role.h"
#include "taskrecord.h"
#include "gamelogic/bagtransition.h"
#include "foreach.h"
#include "table/globalconfig.h"
#include "task/rpcg2m_taskdonateg2m.h"
#include "utility/tlogger.h"
#include "spactivity/spactivityhandler.h"

static const UINT32 kWeeklyTaskNum = 1;

void WeeklyTaskOne::FromKKSG(const KKSG::WeeklyTaskInfo& info, DailyTask::RowData* pTask)
{
	index = info.index();
	id = info.id();
	type = pTask->tasktype;
	step = info.step();
	condIds = pTask->conditionId;
	maxStep = pTask->conditionNum;
	rewarded = info.is_rewarded();
	askHelp = info.ask_help();
	refreshCount = info.refresh_count();
}

void WeeklyTaskOne::ToKKSG(KKSG::WeeklyTaskInfo& info)
{
	info.set_index(index);
	info.set_id(id);
	info.set_step(step);
	info.set_is_rewarded(rewarded);
	info.set_ask_help(askHelp);
	info.set_refresh_count(refreshCount);
}	

WeeklyTaskRecord::WeeklyTaskRecord()
:PeriodTask(KKSG::PeriodTaskType_Weekly)
,m_score(0)
,m_freeRefreshCount(0)
{

}

WeeklyTaskRecord::~WeeklyTaskRecord()
{

}

bool WeeklyTaskRecord::Load(const KKSG::RoleTask& data)
{
	const KKSG::WeeklyTaskData& wdata = data.weekrecord();
	m_acceptLevel = wdata.accept_level();
	m_rewarded = wdata.rewarded();
	m_askHelpNum = wdata.askhelp_num();
	m_accepted = wdata.accept();
	m_periodTaskNum = wdata.accept_count();
	m_score = wdata.score();
	m_freeRefreshCount = wdata.free_refresh_count();
	ClearSubTasks();
	m_helperInfo.Clear();
	m_helperInfo.CopyFrom(wdata.helpinfo());

	for (int i = 0; i < wdata.tasks_size(); ++i)
	{
		const KKSG::WeeklyTaskInfo& info = wdata.tasks(i);
		DailyTask::RowData* pTask = TaskMgr::Instance()->GetDailyTask(info.id());
		if (!pTask)
		{
			continue;
		}
		WeeklyTaskOne* task = new WeeklyTaskOne(this);
		task->FromKKSG(info, pTask);
		m_tasks.push_back(task);
	}

	m_rewardedBox.clear();
	for (int i = 0; i < wdata.rewarded_box_size(); ++i)
	{
		m_rewardedBox.push_back(wdata.rewarded_box(i));
	}

	return true;
}

void WeeklyTaskRecord::Save(KKSG::RoleTask& data)
{
	KKSG::WeeklyTaskData& wdata = *data.mutable_weekrecord();
	wdata.set_accept_level(m_acceptLevel);
	wdata.set_rewarded(m_rewarded);
	wdata.set_askhelp_num(m_askHelpNum);
	wdata.set_accept(m_accepted);
	wdata.set_accept_count(m_periodTaskNum);
	wdata.set_score(m_score);
	wdata.set_free_refresh_count(m_freeRefreshCount);

	wdata.clear_tasks();
	for (size_t i = 0; i < m_tasks.size(); ++i)
	{
		WeeklyTaskOne* pSubTask = (WeeklyTaskOne*)m_tasks[i];
		KKSG::WeeklyTaskInfo* info = wdata.add_tasks();
		pSubTask->ToKKSG(*info);
	}

	wdata.clear_helpinfo();
	wdata.mutable_helpinfo()->CopyFrom(m_helperInfo);
	wdata.clear_rewarded_box();
	for (auto it = m_rewardedBox.begin(); it != m_rewardedBox.end(); ++it)
	{
		wdata.add_rewarded_box(*it);
	}
}

void WeeklyTaskRecord::OnDayPass()
{
	m_freeRefreshCount = 0;
	SetDirty();
}

void WeeklyTaskRecord::OnWeekPass()
{
	m_accepted = false;
	m_askHelpNum = 0;
	m_DonateNum = 0;
	m_redPoint = false;
	m_periodTaskNum = 0;
	m_score = 0;
	m_rewardedBox.clear();
	ClearSubTasks();
	m_record->FinishWeeklyTask(false);
	m_record->SetDirty();
}

void WeeklyTaskRecord::OnSubTaskAccept(SubTask* pSubTask)
{
	DoTxLog(pSubTask->index, TWeekTaskType0);
}

void WeeklyTaskRecord::OnSubTaskFinish(SubTask* pSubTask)
{
	CheckRedPoint();
}

void WeeklyTaskRecord::OnFirstEnterScene()
{
	CheckRedPoint();
}

void WeeklyTaskRecord::OnTaskOneReward(SubTask* pSubTask, DailyTask::RowData* pConfig)
{
	m_score += pConfig->score;
	CheckRedPoint();
	CheckFinish();
	DoTxLog(pSubTask->index, TWeekTaskType3);
	SpActivityHandler::GlobalHandler.OnSubTaskComplete(m_role, 2);
}

void WeeklyTaskRecord::Refresh()
{
	m_accepted = true;
	m_acceptLevel = m_role->GetLevel();
	m_rewarded = false;
	m_completeNum = 0;
	m_redPoint = false;

	std::vector<DailyTask::RowData*> tasks;
	TaskMgr::Instance()->RandomDailyTask(m_type, m_role->IsBackFlowRole(), m_role->GetLevel(), tasks);

	ClearSubTasks();
	m_tasks.resize(tasks.size());

	for (size_t i = 0; i < tasks.size(); ++i)
	{
		DailyTask::RowData* data = tasks[i];
		WeeklyTaskOne* task = new WeeklyTaskOne(this);
		m_tasks[i] = task;
		task->index = i;
		task->id = data->taskID;
		task->type = data->tasktype;
		task->step = 0;
		task->rewarded = false;
		task->condIds = data->conditionId;
		task->maxStep = data->conditionNum;
		OnSubTaskAccept(task);
	}
	m_record->SetDirty();
}

KKSG::ErrorCode WeeklyTaskRecord::RefreshOne(UINT32 index)
{
	WeeklyTaskOne* pOldTask = GetSubTaskByIndex<WeeklyTaskOne>(index);
	if (pOldTask == NULL)
	{
		return KKSG::ERR_TASK_NOT_FOUND;
	}
	if (pOldTask->rewarded)
	{
		return KKSG::ERR_TASK_ALREADY_FINISH;
	}
	bool isFree = false;
	if (m_freeRefreshCount < GetGlobalConfig().WeekyFreeRefreshNum)
	{
		isFree = true;
	}
	else
	{
		if (pOldTask->refreshCount >= GetGlobalConfig().WeeklyTaskRefreshCount)
		{
			return KKSG::ERR_TASK_NO_REFRESH_COUNT;
		}
	}

	UINT32 quality = 0;
	if (GetGlobalConfig().WeeklyTaskHighQualityNum && ((pOldTask->refreshCount + 1) % GetGlobalConfig().WeeklyTaskHighQualityNum == 0))
	{
		quality = GetGlobalConfig().WeeklyTaskHighQuality;
	}
	else
	{
		quality = GetGlobalConfig().RandomWeeklyTaskQuality();
	}
	std::vector<UINT32> except;
	for (auto it = m_tasks.begin(); it != m_tasks.end(); ++it)
	{
		if ((*it)->id != pOldTask->id)
		{
			except.push_back((*it)->id);
		}
	}
	DailyTask::RowData* pData = TaskMgr::Instance()->RandomOneSubTask(m_type, m_role->IsBackFlowRole(), m_acceptLevel, quality, except);
	if (pData == NULL)
	{
		return KKSG::ERR_FAILED;
	}

	if (isFree == false)
	{
		UINT32 cost = GetGlobalConfig().GetWeeklyTaskRefreshCost(pOldTask->refreshCount);
		BagTakeItemTransition trans(m_role);
		trans.SetReason(ItemFlow_Task, ItemFlow_WeeklyTaskRefresh);
		if (!trans.TakeItem(DRAGON_COIN, cost))
		{
			trans.RollBack();
			return KKSG::ERR_DRAGONCOIN_LIMIT;
		}
		trans.NotifyClient();
		pOldTask->refreshCount++;
	}
	else
	{
		m_freeRefreshCount++;
	}

	if (pOldTask->askHelp)
	{
		RpcG2M_TaskDonateG2M* rpc = RpcG2M_TaskDonateG2M::CreateRpc();
		rpc->m_sessionID = m_role->GetSessionID();
		rpc->m_oArg.set_type(TaskDonateOper_Remove);
		rpc->m_oArg.set_roleid(m_role->GetID());
		rpc->m_oArg.set_index(pOldTask->index);
		rpc->m_oArg.set_taskid(pOldTask->id);
		m_role->SendToMS(*rpc);
	}
	UINT32 preid = pOldTask->id;

	WeeklyTaskOne* pNewTask = new WeeklyTaskOne(this);
	pNewTask->index = index;
	pNewTask->id = pData->taskID;
	pNewTask->type = pData->tasktype;
	pNewTask->step = 0;
	pNewTask->rewarded = false;
	pNewTask->condIds = pData->conditionId;
	pNewTask->maxStep = pData->conditionNum;
	pNewTask->refreshCount = pOldTask->refreshCount;
	pNewTask->askHelp = false;
	delete pOldTask;
	m_tasks[index] = pNewTask;
	OnSubTaskAccept(pNewTask);

	m_record->SetDirty();

	DoTxLog(index, TWeekTaskType1, preid);
	CheckRedPoint();
	return KKSG::ERR_SUCCESS;
}

bool WeeklyTaskRecord::IsBoxRewarded(UINT32 score)
{
	return std::find(m_rewardedBox.begin(), m_rewardedBox.end(), score) != m_rewardedBox.end();
}

void WeeklyTaskRecord::SetBoxReward(UINT32 score)
{
	m_rewardedBox.push_back(score);
	m_record->SetDirty();
}

KKSG::ErrorCode WeeklyTaskRecord::TakeBoxReward(UINT32 score)
{
	if (m_accepted == false)
	{
		return KKSG::ERR_TASK_NOT_ACCEPT;
	}
	if (IsBoxRewarded(score))
	{
		return KKSG::ERR_TASK_ALREADY_TAKE;
	}
	DailyTaskReward::RowData* reward = TaskMgr::Instance()->GetDailyReward(m_type, m_acceptLevel, DailyTaskRewardType_Total, score);
	if (reward == NULL)
	{
		return KKSG::ERR_TASK_NO_TABLE;
	}

	BagGiveItemTransition transition(m_role);
	transition.SetReason(ItemFlow_Task, ItemFlow_Task_Finish);
	foreach (i in reward->taskreward)
	{
		transition.GiveItem(i->seq[0], i->seq[1]);
	}
	transition.NotifyClient();

	// 记录
	SetBoxReward(score);

	CheckFinish();
	m_record->SetDirty();

	CheckRedPoint();

	// 日志
	SSInfo << "RoleId: " << m_role->GetID() << ", take total reward" << END;

	//EventMgr::Instance()->AddEvent(m_role->GetID(), MENTOR_EVENT_OFFSET + KKSG::MentorTask_DailyTask, 0, 1);
	return KKSG::ERR_SUCCESS;
}

UINT32 WeeklyTaskRecord::GetRemainCount()
{
	return kWeeklyTaskNum > m_periodTaskNum ? (kWeeklyTaskNum - m_periodTaskNum) : 0;
}

bool WeeklyTaskRecord::CanAccept()
{
	if (GetRemainCount() == 0)
	{
		return false;
	}
	if (!m_role->Get<CRoleSystem>()->IsSystemOpened(SYS_WEEKLYTASK))
	{
		return false;
	}
	return true;
}

void WeeklyTaskRecord::FillTaskInfo(KKSG::GetWeeklyTaskInfoRes& res)
{
	res.set_result(KKSG::ERR_SUCCESS);
	res.set_score(m_score);
	res.set_askhelp_num(m_askHelpNum);
	res.set_accept_level(m_acceptLevel);
	res.set_remain_free_refresh_count(GetRemainFreeRefreshCount());

	for (auto it = m_tasks.begin(); it != m_tasks.end(); ++it)
	{
		WeeklyTaskOne* pSubTask = (WeeklyTaskOne*)(*it);
		pSubTask->ToKKSG(*res.add_task());
	}
	for (auto it = m_rewardedBox.begin(); it != m_rewardedBox.end(); ++it)
	{
		res.add_rewarded_box(*it);
	}

	res.mutable_helpinfo()->CopyFrom(m_helperInfo);
}

void WeeklyTaskRecord::AddHelperInfo(KKSG::TaskHelpInfo& info)
{
	m_helperInfo.Add()->CopyFrom(info);
}

void WeeklyTaskRecord::ClearHelperInfo()
{
	m_helperInfo.Clear();
	m_record->SetDirty();
}

void WeeklyTaskRecord::GMFinish()
{
	for (size_t i = 0; i < m_tasks.size(); ++i)
	{
		SubTask* task = m_tasks[i];
		task->step = task->maxStep;
	}
	m_record->SetDirty();
	CheckRedPoint();
}

void WeeklyTaskRecord::GMReset()
{
	ClearSubTasks();
	m_rewarded = false;
	m_completeNum = 0;
	m_askHelpNum = 0;
	m_DonateNum = 0;
	m_totalDonateNum = 0;
	m_periodTaskNum = 0;
	m_score = 0;
	m_rewardedBox.clear();
	CheckRedPoint();
}

void WeeklyTaskRecord::CheckRedPoint()
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
	std::vector<UINT32>* box = TaskMgr::Instance()->GetWeeklyBoxScores(m_acceptLevel);
	if (box)
	{
		for (auto it = box->begin(); it != box->end(); ++it)
		{
			if (m_score >= *it && !IsBoxRewarded(*it))
			{
				m_redPoint = true;
				break;
			}
		}
	}
	if (m_redPoint)
	{
		// 推送红点
		m_role->HintNotify(SYS_WEEKLYTASK, false);
		m_record->SetDirty();
	}
	else
	{
		// 推送红点消失
		m_role->HintNotify(SYS_WEEKLYTASK, true);
		m_record->SetDirty();
	}
}

bool WeeklyTaskRecord::IsAllBoxReward()
{
	std::vector<UINT32>* box = TaskMgr::Instance()->GetWeeklyBoxScores(m_acceptLevel);
	if (box == NULL)
	{
		return true;
	}

	for (auto it = box->begin(); it != box->end(); ++it)
	{
		if (!IsBoxRewarded(*it))
		{
			return false;
		}
	}
	return true;
}

void WeeklyTaskRecord::CheckFinish()
{
	if (IsAllBoxReward() && IsAllSubTaskRewarded())
	{
		m_periodTaskNum++;
		m_record->FinishWeeklyTask(true);
		RpcG2M_TaskDonateG2M* rpc = RpcG2M_TaskDonateG2M::CreateRpc();
		rpc->m_sessionID = m_role->GetSessionID();
		rpc->m_oArg.set_type(TaskDonateOper_RemoveAllWeek);
		rpc->m_oArg.set_roleid(m_role->GetID());
		m_role->SendToMS(*rpc);
	}
}

void WeeklyTaskRecord::DoTxLog(UINT32 index, UINT32 op, UINT32 preid)
{

	WeeklyTaskOne* pTask = GetSubTaskByIndex<WeeklyTaskOne>(index);
	if (NULL == pTask)
	{
		return;
	}
	auto pConf = TaskMgr::Instance()->GetDailyTask(pTask->id);
	if (NULL == pConf || pConf->category != KKSG::PeriodTaskType_Weekly)
	{
		return;
	}
	TWeekTaskFlow oLog(m_role);
	oLog.m_TaskId = pTask->id;
	oLog.m_Progress = pTask->step;
	oLog.m_MaxPreogress = pTask->maxStep;
	oLog.m_Quality = pConf->taskquality;
	if (op == TWeekTaskType3)
	{
		oLog.m_ChgValue = pConf->score;
	}
	else if (op == TWeekTaskType1)
	{
		oLog.m_PreTaskId = preid;
	}
	oLog.m_TaskId = pConf->taskID;
	oLog.m_TotalValue = m_score;
	oLog.m_AskHelperNum = m_askHelpNum;
	oLog.m_Op = op;
	oLog.m_CompleteNum = m_completeNum;
	oLog.Do();
}

UINT32 WeeklyTaskRecord::GetRemainFreeRefreshCount()
{
	return GetGlobalConfig().WeekyFreeRefreshNum > m_freeRefreshCount ? (GetGlobalConfig().WeekyFreeRefreshNum - m_freeRefreshCount) : 0;
}
