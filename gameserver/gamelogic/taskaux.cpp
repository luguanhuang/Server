#include "pch.h"
#include "taskaux.h"
#include "taskrecord.h"
#include "gamelogic/bag.h"
#include "unit/role.h"
#include "gamelogic/bagtransition.h"
#include "foreach.h"

void SubTask::FromKKSG(const KKSG::DailyTaskInfo& info, DailyTask::RowData* pTask)
{
	id = info.id();
	type = pTask->tasktype;
	step = info.step();
	condIds = pTask->conditionId;
	maxStep = pTask->conditionNum;
	rewarded = info.is_rewarded();
	askHelp = info.ask_help();
}

void SubTask::ToKKSG(KKSG::DailyTaskInfo& info)
{
	info.set_id(id);
	info.set_step(step);
	info.set_is_rewarded(rewarded);
	info.set_ask_help(askHelp);
}

bool SubTask::IsItemTypeTask()
{
	return type == KKSG::TaskConn_ItemID || type == KKSG::TaskConn_ItemTypeQuality;
}

bool SubTask::IsTaskItem(ItemConf* pItemConf)
{
	if (pItemConf == NULL)
	{
		return false;
	}
	if (!IsItemTypeTask())
	{
		return false;
	}
	if (type == KKSG::TaskConn_ItemID)
	{
		for (auto it = condIds.begin(); it != condIds.end(); ++it)
		{
			if (*it == pItemConf->ItemID)
			{
				return true;
			}
		}
	}
	else if (type == KKSG::TaskConn_ItemTypeQuality)
	{
		UINT32 itemConnId = pItemConf->ItemType * 100 + pItemConf->ItemQuality;
		for (auto it = condIds.begin(); it != condIds.end(); ++it)
		{
			if (*it == itemConnId)
			{
				return true;
			}
		}
	}
	return false;
}

bool SubTask::UpdateTask(UINT32 oper, UINT32 tasktype, UINT32 conndid, UINT32 operstep)
{
	if (tasktype != type)
	{
		return false;
	}
	SpecialHandle(oper, tasktype, conndid, operstep);
	if (std::find(condIds.begin(), condIds.end(), conndid) == condIds.end())
	{
		return false;
	}

	bool oldFinish = (step >= maxStep);
	switch (oper)
	{
		case KKSG::TaskOper_Add: step += operstep; break;
		case KKSG::TaskOper_Del: step > operstep ? (step - operstep) : 0; break;
		case KKSG::TaskOper_Set: step = operstep; break;
	}
	bool finish = (step >= maxStep);
	if (finish) step = maxStep;
	if (!oldFinish && finish)
	{
		m_pOwner->OnSubTaskFinish(this);
	}
	m_pOwner->SetDirty();

	if (m_pOwner->GetTaskRecord())
	{
		m_pOwner->GetTaskRecord()->GetWeeklyRecord().DoTxLog(index, TWeekTaskType4);
	}
	return true;
}

void SubTask::SpecialHandle(UINT32& oper, UINT32 tasktype, UINT32& conndid, UINT32& operstep)
{
	if (tasktype == KKSG::TaskConn_WorldBossRank)
	{
		for (auto it = condIds.begin(); it != condIds.end(); ++it)
		{
			if (conndid <= *it)
			{
				conndid = *it;
				return;
			}
		}
	}
	if (tasktype == KKSG::TaskConn_Activity && conndid == KKSG::TaskActType_SkyCityRound)
	{
		bool finish = false;
		for (auto it = condIds.begin(); it != condIds.end(); ++it)
		{
			if (operstep >= *it)
			{
				finish = true;
				break;
			}
		}
		oper = KKSG::TaskOper_Set;
		if (finish)
		{
			operstep = maxStep;
		}
		else
		{
			operstep = (operstep > step) ? operstep : step;
		}
	}
}

void SubTask::AddTaskStep(UINT32 operstep)
{
	bool oldFinish = (step >= maxStep);
	step += operstep;
	bool finish = (step >= maxStep);
	if (finish) step = maxStep;
	if (!oldFinish && finish)
	{
		m_pOwner->OnSubTaskFinish(this);
	}
	m_pOwner->SetDirty();
}

PeriodTask::PeriodTask(KKSG::PeriodTaskType type)
:m_type(type)
,m_role(NULL)
,m_record(NULL)
,m_accepted(false)
,m_acceptLevel(0)
,m_completeNum(0)
,m_askHelpNum(0)
,m_rewarded(false)
,m_redPoint(0)
,m_DonateNum(0)
,m_totalDonateNum(0)
,m_periodTaskNum(0)
{
}

PeriodTask::~PeriodTask()
{
	ClearSubTasks();
}

void PeriodTask::ClearSubTasks()
{
	for (auto it = m_tasks.begin(); it != m_tasks.end(); ++it)
	{
		delete *it;
	}
	m_tasks.clear();
}

void PeriodTask::UpdateTask(UINT32 oper, UINT32 type, UINT32 id, UINT32 step)
{
	if (m_accepted == false)
	{
		return;
	}

	UINT32 dailyType = TaskCondIdConvert(type);
	if (dailyType == 0)
	{
		return;
	}

	for (size_t i = 0; i < m_tasks.size(); ++i)
	{
		SubTask* task = m_tasks[i];
		if (task->type != dailyType)
		{
			continue;
		}
		if (dailyType == KKSG::TaskConn_ItemID)
		{
			if (m_type == KKSG::PeriodTaskType_Daily)
			{
				CheckAndUpdate(task);
			}
		}
		else 
		{
			task->UpdateTask(oper, dailyType, id, step);
		}

	}
}

UINT32 PeriodTask::TaskCondIdConvert(UINT32 connId)
{
	// TODO 枚举改了对线上的影响
	return connId;
	/*
	switch (connId)
	{
		case KKSG::TaskCondition_Item: return DailyTaskType_Collection;
		case KKSG::TaskCondition_Stage: return DailyTaskType_Stage;
	}
	*/
}

void PeriodTask::AddAskHelpNum(UINT32 num)
{
	m_askHelpNum += num;
	m_record->SetDirty();
}

void PeriodTask::AddDonateCount(UINT32 value)
{
	m_DonateNum += value;
	m_totalDonateNum += value;
	m_record->SetDirty();
}

void PeriodTask::CheckAndUpdate(SubTask* task)
{
	if (task->type == KKSG::TaskConn_ItemID)
	{
		UINT32 count = 0;
		Bag* bag = m_role->Get<Bag>();
		UINT32 condId = 0;
		for (size_t j = 0; j < task->condIds.size(); ++j)
		{
			condId = task->condIds[j];
			count += bag->CountItemInBag(condId);
		}
		task->UpdateTask(KKSG::TaskOper_Set, KKSG::TaskConn_ItemID, condId, count);
		m_record->SetDirty();
	}
}

void PeriodTask::SetDirty()
{
	m_record->SetDirty();
}

bool PeriodTask::IsAllSubTaskRewarded()
{
	for (auto it = m_tasks.begin(); it != m_tasks.end(); ++it)
	{
		SubTask* pTask = *it;
		if (pTask->rewarded == false)
		{
			return false;
		}
	}
	return true;
}

KKSG::ErrorCode PeriodTask::TakeOneRewardById(UINT32 id)
{
	SubTask* task = GetSubTask<SubTask>(id);
	return TakeOneReward(task);
}

KKSG::ErrorCode PeriodTask::TakeOneReward(UINT32 index)
{
	SubTask* task = GetSubTaskByIndex<SubTask>(index);
	return TakeOneReward(task);
}

KKSG::ErrorCode PeriodTask::TakeOneReward(SubTask* task)
{
	if (task == NULL)
	{
		return KKSG::ERR_TASK_NOT_FOUND;
	}
	if (task->step < task->maxStep)
	{
		return KKSG::ERR_TASK_NOT_FINISH;
	}
	if (task->rewarded)
	{
		return KKSG::ERR_TASK_ALREADY_TAKE;
	}
	DailyTask::RowData* tblTask = TaskMgr::Instance()->GetDailyTask(task->id);
	if (tblTask == NULL)
	{
		return KKSG::ERR_TASK_NO_TABLE;
	}
	DailyTaskReward::RowData* reward = TaskMgr::Instance()->GetDailyReward(m_type, m_acceptLevel, DailyTaskRewardType_Sub, tblTask->taskquality);
	if (reward == NULL)
	{
		LogError("not find reward, daily taskid[%u]", task->id);
		return KKSG::ERR_TASK_NO_TABLE;
	}

	// 收集类的扣道具
	if (m_type == KKSG::PeriodTaskType_Daily && KKSG::TaskConn_ItemID == tblTask->tasktype)
	{
		Bag* bag = m_role->Get<Bag>();
		BagTakeItemTransition trans(m_role);
		trans.SetReason(ItemFlow_Task, ItemFlow_Task_Finish);

		UINT32 maxNum = tblTask->conditionNum;
		for (size_t i = 0; i < tblTask->conditionId.size(); ++i)
		{
			UINT32 itemId = tblTask->conditionId[i];
			UINT32 itemCount = bag->CountItemInBag(itemId);
			UINT32 costCount = itemCount >= maxNum ? maxNum : itemCount;
			if (!trans.TakeItem(itemId, costCount))
			{
				trans.RollBack();
				return KKSG::ERR_ITEM_NOT_ENOUGH;
			}
			maxNum -= costCount;
			if (maxNum == 0)
			{
				break;
			}
		}
		if (maxNum != 0)
		{
			trans.RollBack();
			return KKSG::ERR_ITEM_NOT_ENOUGH;
		}
		trans.NotifyClient();
	}

	// 发放奖励
	BagGiveItemTransition transition(m_role);
	transition.SetReason(ItemFlow_Task, ItemFlow_Task_Finish);
	foreach (i in reward->taskreward)
	{
		transition.GiveItem(i->seq[0], i->seq[1]);
	}
	transition.NotifyClient();

	// 记录
	task->rewarded = true;
	m_completeNum++;
	m_record->SetDirty();
	SSInfo << "RoleId: " << m_role->GetID() << ", take sub task reward, taskId: " << task->id << ", completeNum: " << m_completeNum << END;
	OnTaskOneReward(task, tblTask);
	return KKSG::ERR_SUCCESS;
}
