#include "pch.h"
#include "taskrecord.h"
#include "pb/project.pb.h"
#include "pb/enum.pb.h"
#include "bagtransition.h"
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
#include "util/gametime.h"
#include "findback/findbackrecord.h"
#include "define/systemiddef.h"
#include "util/dbutil.h"
#include "mail/mailsender.h"
#include "mail/mailconfig.h"
#include "config.h"
#include "tablemgr/backflowtablemgr.h"

void TaskOneRecord::ToKKSG(KKSG::TaskInfo& taskinfo)
{
	taskinfo.set_id(id);
	taskinfo.set_status(status);
	taskinfo.clear_conds();
	foreach (it in conds)
	{
		KKSG::TaskConditionInfo* cond = taskinfo.add_conds();
		cond->set_type((KKSG::TaskConnType)it->type);
		cond->set_id(it->id);
		cond->set_step(it->step);
		cond->set_max_step(it->maxStep);
	}
}

void TaskOneRecord::FromKKSG(const KKSG::TaskInfo& taskinfo)
{
	id = taskinfo.id();
	status = taskinfo.status();
	conds.clear();
	for (INT32 i = 0; i < taskinfo.conds_size(); ++i)
	{
		TaskCondition cond;
		cond.type = taskinfo.conds(i).type();
		cond.id = taskinfo.conds(i).id();
		cond.step = taskinfo.conds(i).step();
		cond.maxStep = taskinfo.conds(i).max_step();
		conds.push_back(cond);
	}
}

TaskRecord::TaskRecord(Role *pRole)
:m_role(pRole)
{
	m_dailyTask.SetHandler(pRole, this);
	m_weeklyTask.SetHandler(pRole, this);
}

TaskRecord::~TaskRecord()
{

}

void TaskRecord::FirstInit(UINT32 roleNum)
{
	if (GSConfig::Instance()->IsBackFlowServer() && roleNum == 1)
	{
		BackFlowServerConfig::RowData* pData = BackFlowTableMgr::Instance()->GetServerConfig(m_role->GetProfession(), GSConfig::Instance()->GetBackFlowLevel());
		if (pData == NULL)
		{
			return;
		}
		// 主线任务进度
		std::list<UINT32> taskIds;
		TaskMgr::Instance()->GetMainTaskUntil(pData->MainTaskProcess, taskIds);
		for (auto it = taskIds.begin(); it != taskIds.end(); ++it)
		{
			m_finishTasks.insert(*it);
		}
		// 支线任务进度
		for (auto it = pData->BranchTaskProcess.begin(); it != pData->BranchTaskProcess.end(); ++it)
		{
			m_finishTasks.insert(*it);
		}
	}
	else
	{
		const std::vector<TaskConfig*>& initTask = TaskMgr::Instance()->GetInitTask();
		for (size_t i = 0; i < initTask.size(); ++i)
		{
			AcceptTask(initTask[i]->id, false);
		}
	}
}
	
bool TaskRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	m_doingTasks.clear();
	m_finishTasks.clear();

	const KKSG::RoleTask& record = poRoleAllInfo->task_record();
	m_lastUpdateTime = record.lastupdatetime(); 
	for (INT32 i = 0; i < record.tasks_size(); ++i)
	{
		TaskOneRecord oneTask;
		oneTask.FromKKSG(record.tasks(i));
		TaskConfig* config = TaskMgr::Instance()->GetTaskConfig(oneTask.id);
		if (config == NULL)
		{
			m_dirty.Set();
			continue;
		}
		oneTask.type = config->type;
		m_doingTasks.push_back(oneTask);
	}
	for (INT32 i = 0; i < record.finished_size(); ++i)
	{
		m_finishTasks.insert(record.finished(i));
	}

	m_dailyTask.Load(record);
	m_weeklyTask.Load(record);
	return true;
}

void TaskRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (!m_dirty.TestAndReset())
	{
		return;
	}
	KKSG::RoleTask& record = *poRoleAllInfo->mutable_task_record();
	record.clear_tasks();
	record.clear_finished();

	record.set_lastupdatetime(m_lastUpdateTime);
	foreach (i in m_doingTasks)
	{
		KKSG::TaskInfo* taskinfo = record.add_tasks();
		i->ToKKSG(*taskinfo);
	}
	foreach (i in m_finishTasks)
	{
		record.add_finished(*i);
	}

	m_dailyTask.Save(record);
	m_weeklyTask.Save(record);

	roChanged.insert(&record);
}

void TaskRecord::OnDayPass()
{
	UINT32 now = GameTime::GetTime();
	if (XCommon::IsDaySame(m_lastUpdateTime, now))
	{
		return;
	}
	m_dailyTask.OnDayPass();
	m_weeklyTask.OnDayPass();
	AcceptTask(TaskMgr::Instance()->GetDailyTaskId());

	// 跨周
	if (!XCommon::IsWeekSame(m_lastUpdateTime, now))
	{
		m_weeklyTask.OnWeekPass();
		AcceptTask(TaskMgr::Instance()->GetWeeklyTaskId());
	}
	m_lastUpdateTime = now;
	m_dirty.Set();
}

void TaskRecord::OnLevelUp(UINT32 oldLevel, UINT32 newLevel)
{
	std::vector<UINT32> newTasks;
	TaskMgr::Instance()->GetTaskByLevel(newLevel, newTasks);
	for (auto it = newTasks.begin(); it != newTasks.end(); ++it)
	{
		AcceptTask(*it);
	}
}

void TaskRecord::UpdateTask(KKSG::TaskOper oper, UINT32 type, UINT32 id, UINT32 step)
{
	if (!FilterCondition(type, id))
	{
		return;
	}
	m_dailyTask.UpdateTask(oper, type, id, step);
	m_weeklyTask.UpdateTask(oper, type, id, step);

	for (size_t i = 0; i < m_doingTasks.size(); ++i)
	{
		TaskOneRecord& task = m_doingTasks[i];
		// TODO 没有接受的任务是否更新
		bool found = false;
		for (size_t j = 0; j < task.conds.size(); j++)
		{
			TaskCondition& cond = task.conds[j];
			if (cond.type != type) continue;
			if (cond.id != 0 && cond.id != id) continue;
			found = true;
			UpdateStep(oper, cond, step);
			break;
		}
		if (found)
		{
			CheckFinish(task);
			PushTaskState(task);
			//break;
		}
	}

	if (type == KKSG::TaskConn_StageType)
	{
		if (id == KKSG::SCENE_DRAGON_EXP)
		{
			UpdateTask(KKSG::TaskOper_Add, KKSG::TaskConn_Activity, KKSG::TaskActType_Dragonexp);
		}
		/*
		else if (id == KKSG::SCENE_TOWER)
		{
			UpdateTask(KKSG::TaskOper_Add, KKSG::TaskConn_Activity, KKSG::TaskActType_Tower);
		}
		*/
		else if (id == KKSG::SCENE_HEROBATTLE)
		{
			UpdateTask(KKSG::TaskOper_Add, KKSG::TaskConn_Activity, KKSG::TaskActType_HeroBattleWin);
		}
		else if (id == KKSG::SCENE_PK)
		{
			UpdateTask(KKSG::TaskOper_Add, KKSG::TaskConn_Activity, KKSG::TaskActType_PkWin);
		}
	}
}

KKSG::ErrorCode TaskRecord::TakeTask(UINT32 taskId)
{
	TaskOneRecord* task = GetTask(taskId);
	// 任务没有获得
	if (!task)
	{
		return KKSG::ERR_TASK_NOT_ACCEPT;
	}
	// 已经领取
	if (task->status != KKSG::TaskStatus_CanTake)
	{
		return KKSG::ERR_TASK_ALREADY_TAKE;
	}

	UINT32 score = 0;
	task->status = KKSG::TaskStatus_Taked;
	// 刷新每日任务, 记录次数
	if (task->type == TaskType_Daily)
	{
		m_dailyTask.Refresh();
		m_role->Get<CFindBackRecord>()->AddItemUsedCount(KKSG::COMMERCETASK_BACK, 1, 1);
		score = m_dailyTask.GetScore();
	}
	else if (task->type == TaskType_Weekly)
	{
		m_weeklyTask.Refresh();
	}

	CheckAndUpdateStep(*task);
	PushTaskState(*task);

	DoLog(taskId, task->type, KKSG::TaskStatus_Taked, score);
	m_dirty.Set();
	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode TaskRecord::TakeReward(UINT32 taskId)
{
	TaskOneRecord* task = GetTask(taskId);
	// 任务没有获得
	if (!task)
	{
		return KKSG::ERR_TASK_NOT_FOUND;
	}
	// 不是可以领取状态
	if (task->status != KKSG::TaskStatus_Finish)
	{
		return KKSG::ERR_TASK_NOT_FINISH;
	}
	TaskConfig* config = TaskMgr::Instance()->GetTaskConfig(taskId);
	if (!config)
	{
		return KKSG::ERR_TASK_NO_TABLE;
	}

	// 发送奖励
	GiveTaskReward(config);

	// 设置已经完成
	DealTaskFinish(config);

	// 完成
	OnTaskFinish(config);
	// 道具类任务扣道具
	for (size_t i = 0; i < config->conds.size(); ++i)
	{
		TaskCondition& cond = config->conds[i];
		if (cond.type == KKSG::TaskConn_ItemID)
		{
			BagTakeItemTransition trans(m_role);
			trans.SetReason(ItemFlow_Task, ItemFlow_Task_Finish);
			trans.TakeItem(cond.id, cond.maxStep);
			trans.NotifyClient();
		}
	}

	// 接后续任务
	for (size_t i = 0; i < config->nextTaskids.size(); ++i)
	{
		AcceptTask(config->nextTaskids[i], true, config->activityId);
	}

	DoLog(taskId, config->type, KKSG::TaskStatus_Over);
	m_dirty.Set();
	return KKSG::ERR_SUCCESS;
}

void TaskRecord::FinishBackFlowTask(BackFlowServerConfig::RowData* pData)
{
	std::list<UINT32> taskIds;
	TaskMgr::Instance()->GetMainTaskUntil(pData->MainTaskProcess, taskIds);
	for (auto it = taskIds.begin(); it != taskIds.end(); ++it)
	{
		FinishTaskWithReward(*it);
	}
	// 支线任务进度
	for (auto it = pData->BranchTaskProcess.begin(); it != pData->BranchTaskProcess.end(); ++it)
	{
		FinishTaskWithReward(*it);
	}

	CheckTaskTableChange();
}

void TaskRecord::FinishTaskWithReward(UINT32 taskId)
{
	if (IsTaskFinish(taskId))
	{
		return;
	}
	TaskConfig* config = TaskMgr::Instance()->GetTaskConfig(taskId);
	if (!config)
	{
		return ;
	}
	if (config->type == TaskType_Daily || config->type == TaskType_Weekly)
	{
		return;
	}

	GiveTaskReward(config, true);
	DealTaskFinish(config);
	OnTaskFinish(config);
	PushOverState(taskId);
	DoLog(taskId, config->type, KKSG::TaskStatus_Over);
	m_dirty.Set();
}

void TaskRecord::FinishDailyTask()
{
	UINT32 taskId = 0;
	for (auto it = m_doingTasks.begin(); it != m_doingTasks.end(); ++it)
	{
		TaskOneRecord& task = *it;
		if (task.type != TaskType_Daily)
		{
			continue;
		}
		taskId = task.id;
		TaskConfig* config = TaskMgr::Instance()->GetTaskConfig(taskId);
		if (config == NULL)
		{
			continue;
		}
		// 每日任务不放到已完成的任务里
		m_role->OnTaskComplete(taskId);
		OnTaskFinish(config);
		PushOverState(taskId);

		// 日志
		DoLog(taskId, config->type, KKSG::TaskStatus_Over);

		m_doingTasks.erase(it);
		break;
	}
	AcceptTask(TaskMgr::Instance()->GetDailyTaskId());
}

void TaskRecord::FinishWeeklyTask(bool finish)
{
	for (auto it = m_doingTasks.begin(); it != m_doingTasks.end(); ++it)
	{
		TaskOneRecord& task = *it;
		if (task.type != TaskType_Weekly)
		{
			continue;
		}
		UINT32 taskId = task.id;
		TaskConfig* config = TaskMgr::Instance()->GetTaskConfig(taskId);
		if (config == NULL)
		{
			continue;
		}
		if (finish)
		{
			m_role->OnTaskComplete(taskId);
			OnTaskFinish(config);
			DoLog(taskId, config->type, KKSG::TaskStatus_Over);
		}
		PushOverState(taskId);
		m_doingTasks.erase(it);
		break;
	}
	AcceptTask(TaskMgr::Instance()->GetWeeklyTaskId());
}

void TaskRecord::GiveTaskReward(TaskConfig* config, bool noExp)
{
	BagGiveItemTransition transition(m_role);
	transition.SetReason(ItemFlow_Task, ItemFlow_Task_Finish);

	foreach (i in config->rewards)
	{
		if (noExp && i->seq[0] == EXP)
		{
			continue;
		}
		transition.GiveItem(i->seq[0], i->seq[1]);
	}

	transition.NotifyClient();
}

void TaskRecord::OnEnterScene()
{
	CheckTimeTaskStart();
	CheckTimeTaskEnd();
	OnDayPass();
}

void TaskRecord::OnFirstTimeEnterScene()
{
	// 检测表格可能的条件变化
	CheckTaskTableChange();
	OnDayPass();
	m_dailyTask.OnFirstEnterScene();
	m_weeklyTask.OnFirstEnterScene();
}

void TaskRecord::CheckTaskTableChange()
{
	// 条件变化
	for (auto it = m_doingTasks.begin(); it != m_doingTasks.end(); )
	{
		TaskOneRecord& task = *it;
		TaskConfig* config = TaskMgr::Instance()->GetTaskConfig(task.id);
		// 表格不存在
		if (!config) {
			SSWarn << "task config not found: " << task.id << END;
			it = m_doingTasks.erase(it);	
			continue;
		}

		// 限时任务时间到了
		if (config->startTime && config->endTime)
		{
			UINT32 now = GameTime::GetTime();
			if (now < config->startTime || now >= config->endTime)
			{
				it = m_doingTasks.erase(it);	
				PushOverState(config->id);
				continue;
			}
		}

		std::vector<TaskCondition> conds;
		for (size_t i = 0; i < config->conds.size(); i++)
		{
			TaskCondition& tableCond = config->conds[i];
			bool found = false;
			for (size_t j = 0; j < task.conds.size(); j++)
			{
				TaskCondition& cond = task.conds[j];
				if (cond.type == tableCond.type && cond.id == tableCond.id)
				{
					cond.maxStep = tableCond.maxStep;
					conds.push_back(cond);
					found = true;
					break;
				}
			}
			if (!found)
			{
				conds.push_back(tableCond);
			}
		}
		task.conds.swap(conds);
		if (task.status == KKSG::TaskStatus_Finish)
		{
			task.status = KKSG::TaskStatus_Taked;
		}
		CheckAndUpdateStep(task);
		CheckFinish(task);
		it++;
	}

	// 表格改动带来的新任务
	std::vector<UINT32> newTaskIds;
	TaskMgr::Instance()->GetTaskByLevel(m_role->GetLevel(), newTaskIds);
	for (auto it = newTaskIds.begin(); it != newTaskIds.end(); ++it)
	{
		AcceptTask(*it);
	}

	// 表格里删了的任务，已完成也删了，避免重用引起问题
	for (auto it = m_finishTasks.begin(); it != m_finishTasks.end(); )
	{
		if (TaskMgr::Instance()->GetTaskConfig(*it) == NULL)
		{
			m_finishTasks.erase(it++);
			m_dirty.Set();
		}
		else
		{
			it++;
		}
	}
}

void TaskRecord::CheckTimeTaskStart()
{
	std::vector<UINT32>& taskIds = TaskMgr::Instance()->GetTimeTaskIds();
	for (auto it = taskIds.begin(); it != taskIds.end(); ++it)
	{
		AcceptTask(*it);
	}
}

void TaskRecord::CheckTimeTaskEnd()
{
	for (auto it = m_doingTasks.begin(); it != m_doingTasks.end(); )
	{
		TaskOneRecord& task = *it;
		TaskConfig* config = TaskMgr::Instance()->GetTaskConfig(task.id);
		// 表格不存在
		if (!config)
		{
			SSWarn << "task config not found: " << task.id << END;
			PushOverState(task.id);
			it = m_doingTasks.erase(it);	
			m_dirty.Set();
			continue;
		}
		// 限时任务时间到了
		if (config->startTime && config->endTime)
		{
			UINT32 now = GameTime::GetTime();
			if (now < config->startTime || now >= config->endTime)
			{
				PushOverState(config->id);
				it = m_doingTasks.erase(it);	
				m_dirty.Set();
				continue;
			}
		}
		it++;
	}
}

void TaskRecord::OnActivityOpen(UINT32 activityId)
{
	std::vector<UINT32> tasks;
	TaskMgr::Instance()->GetActivityOpenTask(activityId, tasks);
	for (auto it = tasks.begin(); it != tasks.end(); ++it)
	{
		AcceptTask(*it, false, activityId);
	}
}

void TaskRecord::OnActivityClose(UINT32 activityId)
{
	std::vector<UINT32> tasks;
	TaskMgr::Instance()->GetActivityOpenTask(activityId, tasks);
	for (auto it = tasks.begin(); it != tasks.end(); ++it)
	{
		RemoveDoingTask(*it);
		m_finishTasks.erase(*it);
	}
}

void TaskRecord::AcceptTask(UINT32 taskId, bool push, UINT32 activityId)
{
	TaskConfig* config = TaskMgr::Instance()->GetTaskConfig(taskId);
	if (!config)
	{
		return;
	}
	if (config->activityId && config->activityId != activityId)
	{
		return;
	}
	if (IsTaskFinish(taskId))
	{
		return;
	}
	if (config->preTaskId && !IsTaskFinish(config->preTaskId))
	{
		return;
	}
	if (GetTask(taskId))
	{
		return;
	}
	// 每日任务需要判断次数和系统开放
	if (config->type == TaskType_Daily && !m_dailyTask.CanAccept())
	{
		return;
	}
	if (config->type == TaskType_Weekly && !m_weeklyTask.CanAccept())
	{
		return;
	}

	// 等级判断
	UINT32 level = m_role->GetLevel();
	if (level < config->needLevel)
	{
		return;
	}
	// 开放时间
	if (config->startTime && config->endTime)
	{
		UINT32 now = GameTime::GetTime();
		if (now < config->startTime || now >= config->endTime)
		{
			return;
		}
	}

	TaskOneRecord record;
	record.id = taskId;
	record.type = config->type;
	record.status = KKSG::TaskStatus_CanTake;
	for (size_t i = 0; i < config->conds.size(); i++)
	{
		TaskCondition& cond = config->conds[i];
		record.conds.push_back(cond);
	}
	m_doingTasks.push_back(record);

	if (push)
	{
		PushTaskState(record);		
	}

	m_dirty.Set();

	SSDebug << "roleId: " << m_role->GetID() << ", taskId: " << taskId << END;
}

TaskOneRecord* TaskRecord::GetTask(UINT32 taskId)
{
	for (auto it = m_doingTasks.begin(); it != m_doingTasks.end(); ++it)
	{
		if (it->id == taskId)
		{
			return &(*it);
		}
	}
	return NULL;
}

void TaskRecord::CheckAndUpdateStep(TaskOneRecord& task)
{
	for (size_t i = 0; i < task.conds.size(); ++i)
	{
		TaskCondition& cond = task.conds[i];
		switch (cond.type)
		{
		case KKSG::TaskConn_ItemID:
			{
				UINT32 num = m_role->Get<Bag>()->CountItemInBag(cond.id);
				UpdateStep(KKSG::TaskOper_Set, cond, num);
			}
			break;
		}
	}

	CheckFinish(task);
}

void TaskRecord::CheckFinish(TaskOneRecord& task)
{
	if (task.type == TaskType_Daily || task.type == TaskType_Weekly)
	{
		return ;
	}

	if (task.status == KKSG::TaskStatus_CanTake)
	{
		return ;
	}

	bool finish = true;
	for (size_t i = 0; i < task.conds.size(); i++)
	{
		TaskCondition& cond = task.conds[i];
		if (cond.step < cond.maxStep)
		{
			finish = false;
			break;
		}
	}
	if (finish)
	{
		task.status = KKSG::TaskStatus_Finish;
	}
	else 
	{
		task.status = KKSG::TaskStatus_Taked;
	}
}

void TaskRecord::UpdateStep(KKSG::TaskOper oper, TaskCondition& cond, UINT32 step)
{
	if (oper == KKSG::TaskOper_Set)
	{
		cond.step = step;
	}
	else if (oper == KKSG::TaskOper_Add)
	{
		cond.step += step;	
	}
	else if (oper == KKSG::TaskOper_Del)
	{
		cond.step = (cond.step > step) ? cond.step - step : 0;
	}

	if (cond.step >= cond.maxStep)
	{
		cond.step = cond.maxStep;
	}
	m_dirty.Set();
}

void TaskRecord::RemoveDoingTask(UINT32 taskId)
{
	for (auto it = m_doingTasks.begin(); it != m_doingTasks.end(); ++it)
	{
		if (it->id == taskId)
		{
			m_doingTasks.erase(it);
			if (!m_role->GetFlag(RSF_ISWaitLogin))
			{
				PushOverState(taskId);
			}
			break;
		}
	}
}

void TaskRecord::DealTaskFinish(TaskConfig* pConfig)
{
	UINT32 taskId = pConfig->id;
	RemoveDoingTask(taskId);
	m_finishTasks.insert(taskId);
	m_role->OnTaskComplete(taskId);
	m_dirty.Set();
}

// 任务完成后做的事情, 比如扣道具等
void TaskRecord::OnTaskFinish(TaskConfig* config)
{
	// 成就
	DesignationMgr::Instance()->OnFinishTask(m_role, config->id);
	// 邮件
	if (!config->mailIds.empty())
	{
		INT32 slot = MapProfessionToSlot(m_role->GetProfession());
		if (slot >= config->mailIds.size())
		{
			LogError("profession[%d] has no mail, taskid[%u]", m_role->GetProfession(), config->id);
		}
		else
		{
			const MailConf* conf = MailConfig::Instance()->GetMailConf(config->mailIds[slot]);
			if (conf)
			{
				MailSender sender;
				sender.SetContent(Mail_System, conf->m_title,  conf->m_content);
				sender.SetReason(ItemFlow_Task_Finish);
				sender.AddConfigItem(conf);
				sender.Send(m_role->GetID());
			}
		}
	}
}

bool TaskRecord::IsTaskFinish(UINT32 taskId)
{
	auto it = m_finishTasks.find(taskId);
	return it != m_finishTasks.end();
}

bool TaskRecord::IsTaskDoing(UINT32 taskId)
{
	TaskOneRecord* task = GetTask(taskId);
	if (!task)
	{
		return false;
	}
	return task->status > KKSG::TaskStatus_CanTake;
}

void TaskRecord::PushTaskState(TaskOneRecord& task)
{
	PtcG2C_UpdateTaskStatus ptc;
	task.ToKKSG(ptc.m_Data);
	m_role->Send(ptc);
	SSDebug << "roleId: " << m_role->GetID() << ", taskId: " << task.id << ", status: " << task.status << END;
}

void TaskRecord::PushOverState(UINT32 taskId)
{
	PtcG2C_UpdateTaskStatus ptc;
	ptc.m_Data.set_id(taskId);
	ptc.m_Data.set_status(KKSG::TaskStatus_Over);
	m_role->Send(ptc);
}

void TaskRecord::DoLog(UINT32 taskId, UINT32 taskType, UINT32 taskStatus, UINT32 score)
{
	TTaskFlowLog oLog(m_role);
	oLog.m_nTaskID = taskId;
	oLog.m_nTaskType = taskType;
	oLog.m_nVipLevel = m_role->Get<CPayV2Record>()->GetVipLevel();
	if (taskStatus == KKSG::TaskStatus_Taked)
	{
		oLog.m_nState = TASK_STATE_ACCEPTED;
	}
	else if (taskStatus == KKSG::TaskStatus_Over)
	{
		oLog.m_nState = TASK_STATE_COMPLETED;
	}
	oLog.m_Score = 0;
	oLog.Do();
}

bool TaskRecord::FilterCondition(UINT32 type, UINT32 id)
{
	if (type == KKSG::TaskConn_ItemID)
	{
		return TaskMgr::Instance()->IsTaskItem(id);
	}
	if (type == KKSG::TaskConn_MonsterID)
	{
		return TaskMgr::Instance()->IsTaskMonster(id);
	}
	return true;
}

void TaskRecord::GMFinishTask(UINT32 taskId)
{
	for (size_t i = 0; i < m_doingTasks.size(); ++i)
	{
		TaskOneRecord& task = m_doingTasks[i];
		if (task.id != taskId)
		{
			continue;
		}

		// 日常任务
		if (task.type == TaskType_Daily)
		{
			m_dailyTask.GMFinish();
			break;
		}
		if (task.type == TaskType_Weekly)
		{
			m_weeklyTask.GMFinish();
			break;
		}

		for (size_t j = 0; j < task.conds.size(); j++)
		{
			TaskCondition& cond = task.conds[j];
			cond.step = cond.maxStep;
		}
		task.status = KKSG::TaskStatus_Finish;
		PushTaskState(task);
	}
	m_dirty.Set();
	SSDebug << "roleId: " << m_role->GetID() << ", taskId: " << taskId << END;
}

void TaskRecord::GMFinishDoingTask()
{
	for (size_t i = 0; i < m_doingTasks.size(); ++i)
	{
		TaskOneRecord& task = m_doingTasks[i];
		// 日常任务
		if (task.type == TaskType_Daily)
		{
			m_dailyTask.GMFinish();
			continue;
		}
		if (task.type == TaskType_Weekly)
		{
			m_weeklyTask.GMFinish();
			continue;
		}

		for (size_t j = 0; j < task.conds.size(); j++)
		{
			TaskCondition& cond = task.conds[j];
			cond.step = cond.maxStep;
		}	
		task.status = KKSG::TaskStatus_Finish;
		PushTaskState(task);
	}
	m_dirty.Set();
	SSDebug << "roleId: " << m_role->GetID() << END;
}

void TaskRecord::GMResetTask()
{
	// 当前的任务全删除
	for (auto it = m_doingTasks.begin(); it != m_doingTasks.end(); ++it)
	{
		PushOverState(it->id);
	}
	// 清空记录
	m_doingTasks.clear();
	m_finishTasks.clear();
	m_dailyTask.GMReset();
	m_weeklyTask.GMReset();
	// 接新的
	CheckTaskTableChange();
	m_dirty.Set();
	SSDebug << "roleId: " << m_role->GetID() << END;
}

void TaskRecord::GMTakeTask(UINT32 taskId)
{
	TaskConfig* config = TaskMgr::Instance()->GetTaskConfig(taskId);
	if (!config)
	{
		return;
	}
	if (GetTask(taskId))
	{
		return;
	}
	TaskOneRecord record;
	record.id = taskId;
	record.status = KKSG::TaskStatus_CanTake;
	for (size_t i = 0; i < config->conds.size(); i++)
	{
		TaskCondition& cond = config->conds[i];
		record.conds.push_back(cond);
	}
	m_doingTasks.push_back(record);
	PushTaskState(record);		
	m_dirty.Set();

	SSDebug << "roleId: " << m_role->GetID() << ", taskId: " << taskId << END;
}

void TaskRecord::GMFinishAll()
{
	SSDebug << "roleId: " << m_role->GetID() << END;
	for (auto it = m_doingTasks.begin(); it != m_doingTasks.end(); )
	{
		if (it->type == TaskType_Daily || it->type == TaskType_Weekly)
		{
			++it;
			continue;
		}
		PushOverState(it->id);
		it = m_doingTasks.erase(it);
	}

	const std::vector<UINT32>& taskIds = TaskMgr::Instance()->GetAllTask();
	for (auto it = taskIds.begin(); it != taskIds.end(); ++it)
	{
		TaskConfig* config = TaskMgr::Instance()->GetTaskConfig(*it);
		if (!config)
		{
			continue;
		}
		if (config->type != TaskType_Daily && config->type != TaskType_Weekly && config->type != TaskType_Activity)
		{
			DealTaskFinish(config);
		}
	}
	m_dirty.Set();
}

UINT32 TaskRecord::GetRemainDailyCount()
{
	return m_dailyTask.GetRemainDailyCount();
}