#include "pch.h"
#include "spactivitydatabase.h"
#include "unit/role.h"
#include "spactivityconfig.h"
#include "spactivity/ptcg2c_spactivitychangentf.h"
#include "gamelogic/bagtransition.h"
#include "utility/tlogger.h"


SpActivityDataBase::SpActivityDataBase(Role* pRole, UINT32 actid)
{
	m_pRole = pRole;
	m_actid = actid;
	m_startTime = 0;
	m_actStage = 1;
}

SpActivityDataBase::~SpActivityDataBase()
{

}

void SpActivityDataBase::Load(const KKSG::SpActivityOne& spActOneData)
{
	m_mapTask.clear();
	for (int i = 0; i < spActOneData.task_size(); ++i)
	{
		SpActTask task(spActOneData.task(i).state(), spActOneData.task(i).progress());
		m_mapTask[spActOneData.task(i).taskid()] = task;
	}
	m_startTime = spActOneData.starttime();
	m_actStage = spActOneData.actstage();
}

void SpActivityDataBase::Save(KKSG::SpActivityOne& spActOneData)
{
	spActOneData.Clear();
	spActOneData.set_actid(m_actid);
	for (auto i = m_mapTask.begin(); i != m_mapTask.end(); ++i)
	{
		KKSG::SpActivityTask* task = spActOneData.add_task();
		task->set_taskid(i->first);
		task->set_state(i->second.state);
		task->set_progress(i->second.progress);
	}
	spActOneData.set_starttime(m_startTime);
	spActOneData.set_actstage(m_actStage);
}

void SpActivityDataBase::SetModify()
{
	m_pRole->Get<SpActivityRecord>()->SetModify();
}

void SpActivityDataBase::AddActStage()
{
	++m_actStage;

	SetModify();
}

void SpActivityDataBase::UpdateSpActivityTask(UINT32 startTime, UINT32 actStage, const std::vector<SpActivityTaskConf*>* allTask)
{
	m_startTime = startTime;
	m_actStage = actStage;

	std::vector<SpActivityTaskConf*> vecTask;
	if (allTask == NULL)
	{
		SpActivityConfig::Instance()->GetAllTaskByActID(m_actid, vecTask);
		allTask = &vecTask;
	}
	std::map<UINT32, SpActTask> newTask;
	for (auto i = allTask->begin(); i != allTask->end(); ++i)
	{
		const SpActivityTaskConf* taskConf = *i;
		if (taskConf->actid != m_actid)
		{
			LogWarn("taskid [%u] 's actid [%u] is not equal to actid [%u]", taskConf->taskid, taskConf->actid, m_actid);
			continue;
		}
		auto iter = m_mapTask.find(taskConf->taskid);
		if (iter == m_mapTask.end())
		{
			newTask.insert(std::make_pair(taskConf->taskid, SpActTask()));
		}
		else
		{
			if (taskConf->tasktype == SpActivityTask_Son && iter->second.state == SpActTaskState_AlreadyGet)
			{
				///> 子任务状态不可能是已经领取，如果是，说明改表了，会导致之前记录失效
				LogError("taskid:%u, it must be change the SuperActivityTask.txt table, wrong change", taskConf->taskid);
				iter->second.state = SpActTaskState_Completed;
			}
			newTask.insert(std::make_pair(taskConf->taskid, iter->second));	
		}	
	}

	m_mapTask.swap(newTask);

	SetModify();

	for (auto i = allTask->begin(); i != allTask->end(); ++i)
	{
		const SpActivityTaskConf* taskConf = *i;
		if (taskConf->actid != m_actid)
		{
			LogWarn("taskid [%u] 's actid [%u] is not equal to actid [%u]", taskConf->taskid, taskConf->actid, m_actid);
			continue;
		}
		if (taskConf->tasktype != SpActivityTask_Father)
		{
			///> 更新任务状态以防改表
			IncreaseProgress(taskConf, 0);
		}
	}
}

void SpActivityDataBase::IncreaseProgress(const SpActivityTaskConf* conf, UINT32 count)
{
	if (conf == NULL || conf->actid != m_actid)
	{
		return;
	}
	auto iter = m_mapTask.find(conf->taskid);
	if (iter == m_mapTask.end())
	{
		return;
	}
	SpActTask& spActTask = iter->second;

	SpActTask preActTask = spActTask;

	SpActTaskHandler* handler = SpActTaskHandler::GetTaskHandler(conf->basetask);
	if (handler == NULL)
	{
		LogError("basetask [%u] handler not find", conf->basetask);
		return;
	}
	if (!handler->IncreaseProgress(m_pRole, spActTask, conf, count))
	{
		return;
	}
	if (preActTask == spActTask)
	{
		return;
	}
	if (!m_pRole->GetFlag(RSF_ISWaitLogin))
	{
		_TaskChangeNtf(conf->taskid, spActTask);
	}

	LogInfo("role [%llu] increase progress SpActid [%u], SpTaskid [%u], state [%u], progress [%u]", m_pRole->GetID(), conf->actid, conf->taskid, spActTask.state, spActTask.progress);

	DoTxLog(conf->actid, conf->taskid, spActTask.state, spActTask.progress);
	if (conf->tasktype == SpActivityTask_Son)
	{
		_UpdateFatherTaskState(conf->taskfather);
	}

	SetModify();
}

void SpActivityDataBase::DecreaseProgress(const SpActivityTaskConf* conf, UINT32 count)
{
	if (conf == NULL || conf->actid != m_actid)
	{
		return;
	}
	auto iter = m_mapTask.find(conf->taskid);
	if (iter == m_mapTask.end())
	{
		return;
	}
	SpActTask& spActTask = iter->second;

	SpActTask preActTask = spActTask;

	SpActTaskHandler* handler = SpActTaskHandler::GetTaskHandler(conf->basetask);
	if (handler == NULL)
	{
		LogError("basetask [%u] handler not find", conf->basetask);
		return;
	}
	if (!handler->DecreaseProgress(m_pRole, spActTask, conf, count))
	{
		return;
	}
	if (preActTask == spActTask)
	{
		return;
	}
	if (!m_pRole->GetFlag(RSF_ISWaitLogin))
	{
		_TaskChangeNtf(conf->taskid, spActTask);
	}

	LogInfo("role [%llu] decrease progress SpActid [%u], SpTaskid [%u], state [%u], progress [%u]", m_pRole->GetID(), conf->actid, conf->taskid, spActTask.state, spActTask.progress);

	DoTxLog(conf->actid, conf->taskid, spActTask.state, spActTask.progress);
	if (conf->tasktype == SpActivityTask_Son)
	{
		_UpdateFatherTaskState(conf->taskfather);
	}

	SetModify();
}

const SpActTask* SpActivityDataBase::GetTask(UINT32 taskid)
{
	auto iter = m_mapTask.find(taskid);
	if (iter != m_mapTask.end())
	{
		return &iter->second;
	}
	return NULL;
}

void SpActivityDataBase::ResetTask()
{
	for (auto i = m_mapTask.begin(); i != m_mapTask.end(); ++i)
	{
		i->second.Reset();
		if (!m_pRole->GetFlag(RSF_ISWaitLogin))
		{
			_TaskChangeNtf(i->first, i->second);
		}
	}

	SetModify();
}

KKSG::ErrorCode SpActivityDataBase::GetReward(UINT32 taskid, const std::vector<Sequence<UINT32, 2>>& reward, int subreason)
{
	const SpActivityTimeConf* conf = SpActivityConfig::Instance()->GetTimeConf(m_actid);
	if (conf == NULL)
	{
		LogError("can't find SpActivityTimeConf, actid:%u", m_actid);
		return KKSG::ERR_UNKNOWN;
	}

	///> system open
	if (!m_pRole->Get<CRoleSystem>()->IsSystemOpened(conf->systemid))
	{
		return KKSG::ERR_SYS_NOTOPEN;
	}

	const SpActivityTaskConf* taskConf = SpActivityConfig::Instance()->GetTaskConf(taskid);
	if (taskConf == NULL)
	{
		LogWarn("can't find SpActivityTaskConf, taskid [%u]", taskid);
		return KKSG::ERR_UNKNOWN;
	}
	if (taskConf->actid != m_actid)
	{
		LogWarn("actid [%u] is not equal task [%u]'s actid:%u in table", m_actid, taskid, taskConf->actid);
		return KKSG::ERR_UNKNOWN;
	}
	if (taskConf->tasktype == SpActivityTask_Son)
	{
		LogWarn("actid [%u] taskid [%u], tasktype is taskson", m_actid, taskid);
		return KKSG::ERR_UNKNOWN;
	}

	auto iter = m_mapTask.find(taskid);
	if(iter == m_mapTask.end())
	{
		return KKSG::ERR_UNKNOWN;
	}

	SpActTask& spActTask = iter->second;
	if(spActTask.state == SpActTaskState_NotComplete)
	{
		return KKSG::ERR_SPACTIVITY_TASK_NOT_COMPLETE;
	}
	else if(spActTask.state == SpActTaskState_AlreadyGet)
	{
		return KKSG::ERR_SPACTIVITY_TASK_GET;
	}

	BagGiveItemTransition transition(m_pRole);
	transition.SetReason(ItemFlow_SpActivity, subreason);
	for(auto i = reward.begin(); i != reward.end(); ++i)
	{
		transition.GiveItem((*i)[0], (*i)[1]);
	}
	transition.NotifyClient();

	LogInfo("role [%llu] get reward, taskid:%u", m_pRole->GetID(), taskid);

	if (taskConf->tasktype == SpActivityTask_Father)
	{
		for (auto i = taskConf->taskson.begin(); i != taskConf->taskson.end(); ++i)
		{
			_DecreaseSonTaskProgressOneTime(*i);
		}

		++spActTask.progress;
		if (spActTask.progress >= taskConf->cnt)	///> 领取次数
		{
			spActTask.state = SpActTaskState_AlreadyGet;
		}
		else
		{
			_UpdateFatherTaskState(taskid);
		}
	}
	else
	{
		spActTask.state = SpActTaskState_AlreadyGet;
	}

	_TaskChangeNtf(taskid, spActTask);

	SetModify();
	return KKSG::ERR_SUCCESS;
}

void SpActivityDataBase::GMClear()
{
	for (auto i = m_mapTask.begin(); i != m_mapTask.end(); ++i)
	{
		i->second = SpActTask();
	}

	SetModify();
}

void SpActivityDataBase::GMCompleteAll()
{
	for (auto i = m_mapTask.begin(); i != m_mapTask.end(); ++i)
	{
		i->second.state = SpActTaskState_Completed;
	}

	SetModify();
}

void SpActivityDataBase::_UpdateFatherTaskState(UINT32 taskid)
{
	const SpActivityTaskConf* conf = SpActivityConfig::Instance()->GetTaskConf(taskid);
	if (conf == NULL)
	{
		LogError("can'f find the father taskid [%u]", taskid);
		return;
	}
	if (conf->tasktype != SpActivityTask_Father)
	{
		LogError("the taskid [%u] is not father task", taskid);
		return;
	}
	if (conf->actid != m_actid)
	{
		LogError("the father taskid [%u] 's actid is [%u], not [%u]", taskid, conf->actid, m_actid);
		return;
	}
	auto iter = m_mapTask.find(taskid);
	if (iter == m_mapTask.end())
	{
		LogError("can't find the father taskid [%u], role [%llu]", taskid, m_pRole->GetID());
		return;
	}
	SpActTask& spActTask = iter->second;
	if (spActTask.state == SpActTaskState_AlreadyGet)	///> 已经领完所有次数的奖励了
	{
		return;
	}

	UINT32 preState = spActTask.state;
	bool isComplete = true;
	for (auto i = conf->taskson.begin(); i != conf->taskson.end(); ++i)
	{
		if (m_mapTask.find(*i) == m_mapTask.end())
		{
			LogError("can't find father task:%u's son task:%u", taskid, *i);
			isComplete = false;
			break;
		}
		if (m_mapTask[*i].state == SpActTaskState_NotComplete)
		{
			isComplete = false;
			break;
		}
	}

	spActTask.state = isComplete ? SpActTaskState_Completed : SpActTaskState_NotComplete;

	///> 父任务状态没有改变
	if (preState == spActTask.state)
	{
		return;
	}

	if (!m_pRole->GetFlag(RSF_ISWaitLogin))
	{
		_TaskChangeNtf(taskid, spActTask);
	}

	if (spActTask.state == SpActTaskState_Completed)
	{
		LogInfo("role [%llu] complete Father task, SpActid [%u], SpTaskid [%u]", m_pRole->GetID(), m_actid, taskid);
	}
	DoTxLog(conf->actid, conf->taskid, spActTask.state, spActTask.progress);
	SetModify();
}

void SpActivityDataBase::_DecreaseSonTaskProgressOneTime(UINT32 taskid)
{
	const SpActivityTaskConf* conf = SpActivityConfig::Instance()->GetTaskConf(taskid);
	if (conf == NULL)
	{
		LogError("can'f find the father task:%u", taskid);
		return;
	}
	if (conf->actid != m_actid)
	{
		LogError("the taskid [%u] 's actid is [%u], not [%u]", taskid, conf->actid, m_actid);
		return;
	}
	auto iter = m_mapTask.find(taskid);
	if (iter == m_mapTask.end())
	{
		LogError("can't find this task:%u, actid:%u, role [%llu]", taskid, m_actid, m_pRole->GetID());
		return;
	}
	SpActTask& spActTask = iter->second;

	SpActTask preActTask = spActTask;

	SpActTaskHandler* handler = SpActTaskHandler::GetTaskHandler(conf->basetask);
	if (handler == NULL)
	{
		LogError("basetask [%u] handler not find", conf->basetask);
		return;
	}
	if (!handler->DecreaseSonTaskProgressOneTime(m_pRole, spActTask, conf))
	{
		return;
	}
	if (preActTask == spActTask)
	{
		return;
	}
	if (!m_pRole->GetFlag(RSF_ISWaitLogin))
	{
		_TaskChangeNtf(conf->taskid, spActTask);
	}

	SetModify();
}

void SpActivityDataBase::_TaskChangeNtf(UINT32 taskid, const SpActTask& spActTask)
{
	PtcG2C_SpActivityChangeNtf oPtc;
	oPtc.m_Data.set_actid(m_actid);
	oPtc.m_Data.set_taskid(taskid);
	oPtc.m_Data.set_state(spActTask.state);
	oPtc.m_Data.set_progress(spActTask.progress);
	m_pRole->Send(oPtc);
}

void SpActivityDataBase::DoTxLog(UINT32 actid, UINT32 taskid, UINT32 state, UINT32 progress)
{
	UINT32 FinishCount = 0;
	UINT32 TotalTaskCount = 0;
	for (auto it = m_mapTask.begin(); it != m_mapTask.end(); ++it)
	{
		if (it->second.state != SpActTaskState_NotComplete)
		{
			++FinishCount;
		}
		++TotalTaskCount;
	}
	TSpactivityFlow oLog(m_pRole);
	oLog.m_ActId = actid;
	oLog.m_TaskId = taskid;
	oLog.m_State = state;
	oLog.m_Progress = progress;
	oLog.m_FinishTaskCount = FinishCount;
	oLog.m_TotalTaskCount = TotalTaskCount;
	oLog.Do();
}

