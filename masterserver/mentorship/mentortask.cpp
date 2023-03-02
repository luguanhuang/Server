#include "pch.h"
#include <time.h>
#include "mentortask.h"
#include "mentoroneside.h"
#include "mentormgr.h"
#include "mentorrelationlist.h"
#include "mentormsg.h"
#include "table/MentorTaskTable.h"
#include "mentorconfig.h"
#include "define/itemdef.h"
#include "mail/maildata.h"
#include "mail/mailmgr.h"
#include "table/globalconfig.h"
#include "util/gametime.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "common/ptcm2c_hintnotifyms.h"
#include "define/systemiddef.h"
#include "util/XCommon.h"
#include "foreach.h"
#include "loghelper/tlogr.h"

bool OneMentorTask::IsTaskComplete()
{
	if (m_completeTime)
	{
		return true;
	}

	int taskMax = MentorConfig::Instance()->GetMentorTaskMaxProgress(m_taskID);
	if (m_completeProgress >= taskMax)
	{
		return true;
	}
	
	return false;
}

bool OneMentorTask::CanReport(MentorShipOneSide* self, UINT64 roleID)
{
	if (!IsTaskComplete())
	{
		return false;
	}
	if (IsCompleteBeforeMentor(self, roleID))
	{
		return false;
	}
	auto findReport = m_reportStatus.find(roleID);
	if (findReport == m_reportStatus.end())
	{
		return true;
	}

	return false;
}

bool OneMentorTask::Report(MentorShipOneSide* self, UINT64 roleID)
{
	if (!CanReport(self, roleID))
	{
		return false;
	}

	m_reportStatus.insert(std::make_pair(roleID, KKSG::EMentorTask_AlreadyReport));
	return true;
}

bool OneMentorTask::Confirm(UINT64 roleID)
{
	auto findReport = m_reportStatus.find(roleID);
	if (findReport == m_reportStatus.end())
	{
		return false;
	}

	if (findReport->second == KKSG::EMentorTask_AlreadyReport)
	{
		findReport->second = KKSG::EMentorTask_ConfirmReport;

		return true;
	}

	return false;
}

bool OneMentorTask::IsCompleteBeforeMentor(MentorShipOneSide* self, UINT64 roleID)
{
	if (!IsTaskComplete() || !self)
	{
		return false;
	}
	if (m_completeTime < self->GetRelationList()->GetRelationBuildTime(roleID))
	{
		return true;
	}
	return false;
}

bool OneMentorTask::CheckComplete(bool useMax)
{
	int taskMax = MentorConfig::Instance()->GetMentorTaskMaxProgress(m_taskID);
	if ((useMax && m_completeProgress >= taskMax) || (!useMax && m_completeProgress && m_completeProgress <= taskMax))
	{
		m_completeProgress = taskMax;
		m_completeTime = GameTime::GetTime();
		return true;
	}
	
	return false;
}

void OneMentorTask::Convert2KKSGOneMentorTaskInfo(MentorShipOneSide* self, KKSG::OneMentorTaskInfo& info, UINT64 masterID)
{
	info.set_taskid(m_taskID);
	info.set_completeprogress(m_completeProgress);
	info.set_completetime(m_completeTime);

	if (masterID)
	{
		auto status = info.add_taskapplystatus();
		status->set_key(self->GetOwnerID());
		if (IsTaskComplete())
		{
			auto find = m_reportStatus.find(masterID);
			if (find == m_reportStatus.end())
			{
				if (m_completeTime < self->GetRelationList()->GetRelationBuildTime(masterID))
				{
					status->set_value(KKSG::EMentorTask_CompleteBefore);
				}
				else
					status->set_value(KKSG::EMentorTask_CanReport);
			}
			else
			{
				status->set_value(find->second);
			}
		}
		else
		{
			status->set_value(KKSG::EMentorTask_UnComplete);
		}
	}
	else
	{
		foreach (i in m_reportStatus)
		{
			auto status = info.add_taskapplystatus();
			status->set_key(i->first);
			status->set_value(i->second);
		}
	}
}

MentorTask::MentorTask(MentorShipOneSide* pOneSide):m_pMentorOneSide(pOneSide)
{

}

MentorTask::~MentorTask()
{

}

bool MentorTask::Init()
{
	Refresh();
	return true;
}

void MentorTask::Uninit()
{

}

bool MentorTask::Load(const KKSG::MentorAllData2Db& data)
{
	for (int i = 0; i < data.selftasklist_size(); ++i)
	{
		auto task = data.selftasklist(i);
		OneMentorTask temp;
		temp.m_taskID = task.taskid();
		temp.m_completeProgress = task.completeprogress();
		temp.m_completeTime = task.completetime();
		temp.m_var.m_taskType = task.tasktype();
		temp.m_var.m_var = task.taskvar();
		for (auto j = 0; j <task.taskapplystatus_size(); ++j)
		{
			temp.m_reportStatus[task.taskapplystatus(j).key()] = task.taskapplystatus(j).value();
		}
		m_selfTaskList[temp.m_taskID] = temp;
	}
	for (int i = 0; i < data.selftaskvars_size(); ++i)
	{
		auto var = data.selftaskvars(i);
		TaskTypeVar temp;
		temp.m_taskType = var.tasktype();
		temp.m_var = var.var();
		m_taskTypeValue[temp] = var.value();
		
		UINT32 time = var.updatetime();
		m_taskTypeValueChangeTime[temp] = time;
	}

	return true;
}

void MentorTask::Save(KKSG::MentorAllData2Db& data)
{
	for (auto iter = m_selfTaskList.begin(); iter != m_selfTaskList.end(); ++iter)
	{
		auto task = data.add_selftasklist();
		task->set_taskid(iter->first);
		task->set_completeprogress(iter->second.m_completeProgress);
		task->set_completetime(iter->second.m_completeTime);
		task->set_tasktype(iter->second.m_var.m_taskType);
		task->set_taskvar(iter->second.m_var.m_var);
		for (auto j = iter->second.m_reportStatus.begin(); j != iter->second.m_reportStatus.end(); ++j)
		{
			auto status = task->add_taskapplystatus();
			status->set_key(j->first);
			status->set_value(j->second);
		}
	}
	for (auto iter = m_taskTypeValue.begin(); iter != m_taskTypeValue.end(); ++iter)
	{
		auto var = data.add_selftaskvars();
		var->set_tasktype(iter->first.m_taskType);
		var->set_var(iter->first.m_var);
		var->set_value(iter->second);
		var->set_updatetime(m_taskTypeValueChangeTime[iter->first]);
	}
}

void MentorTask::DoOpen()
{
	//m_taskTypeValue.clear();
	for (auto iter = m_taskTypeValue.begin(); iter != m_taskTypeValue.end();)
	{
		if (_IsTypeOpenClear(iter->first.m_taskType))
		{
			m_taskTypeValue.erase(iter++);
		}
		else
		{
			++iter;
		}
		
	}
	m_selfTaskList.clear();

	Init();

	MentorMgr::Instance()->QuickSave(m_pMentorOneSide);

	LogInfo("role %llu, open mentor.", m_pMentorOneSide->GetOwnerID());
}

void MentorTask::Refresh()
{
	bool changed = false;
	bool canAddNewTask = false;
	if (m_pMentorOneSide->GetRelationList()->GetSelfPos() == KKSG::EMentorPosMaster)
		return;
	if (m_selfTaskList.empty())
	{
		canAddNewTask = true;
	}

	auto& table = MentorConfig::Instance()->GetMentorTaskTable();
	for (std::vector<MentorTaskTable::RowData*>::const_iterator iter = table.Table.begin(); iter != table.Table.end(); ++iter)
	{
		TaskTypeVar tempVar;
		tempVar.m_taskType = (*iter)->TaskType;
		tempVar.m_var = (*iter)->TaskVar.seq[0];

		auto find = m_selfTaskList.find((*iter)->TaskID);
		if (find == m_selfTaskList.end())
		{
			if (!canAddNewTask || (*iter)->UseThisTask == 0)
			{
				continue;
			}
			OneMentorTask temp;
			temp.m_taskID = (*iter)->TaskID;
			temp.m_var = tempVar;
			m_selfTaskList.insert(std::make_pair(temp.m_taskID, temp));

			changed = true;
		}
		else
		{
			if (find->second.m_completeTime)
			{
				continue;
			}
			else
			{
				if (find->second.m_var == tempVar)
				{
					continue;
				}
				changed = true;

				find->second.m_var = tempVar;

				UINT32 value = 0;
				auto findType = m_taskTypeValue.find(tempVar);
				if (findType != m_taskTypeValue.end())
				{
					value = findType->second;
				}
				find->second.m_completeProgress = value;

				find->second.CheckComplete(!_IsTaskTypeUseMinValue(tempVar.m_taskType));

				LogInfo("role:%llu, modify mentortask table:%u, %u, %llu", m_pMentorOneSide->GetOwnerID(),\
					find->second.m_taskID,tempVar.m_taskType, tempVar.m_var);
			}
		}
	}
	for (auto iter = m_selfTaskList.begin(); iter != m_selfTaskList.end();)
	{
		auto find = table.GetByTaskID(iter->first);
		if (!find)
		{
			changed = true;
			m_selfTaskList.erase(iter++);
			continue;
		}
		
		++iter;
	}

	if (changed)
	{
		//m_pMentorOneSide->SetChanged();
		MentorMgr::Instance()->AddChangedMentor(m_pMentorOneSide->GetOwnerID());
	}
}

void MentorTask::AddTaskValue(UINT32 taskType, UINT64 taskVar, UINT32 add, UINT32 time)
{
	if (taskType == KKSG::MentorTask_DailyActive)
	{
		_AddDailyActiveTaskValue(taskType, taskVar, add, time);
		return;
	}

	TaskTypeVar temp;
	temp.m_taskType = taskType;
	temp.m_var = taskVar;

	const std::set<UINT32>* ids = MentorConfig::Instance()->GetMentorTaskIDs(temp);
	if (!ids)
	{
		return;
	}

	bool change = false;
	auto find = m_taskTypeValue.find(temp);
	if (find == m_taskTypeValue.end())
	{
		change = true;
		m_taskTypeValue.insert(std::make_pair(temp, add));
	}
	else
	{
		if (_IsTaskTypeUseMaxValue(taskType))
		{
			if (add > find->second)
			{
				change = true;
				find->second = add;
			}
		}
		else if (_IsTaskTypeUseMinValue(taskType))
		{
			if (add < find->second)
			{
				change = true;
				find->second = add;
			}
		}
		else
		{
			change = true;
			find->second += add;
		}
	}

	if (!change)
	{
		return;
	}

	bool triggerFinish = false;
	for (auto iter = ids->begin(); iter != ids->end(); ++iter)
	{
		if(_CheckFinishTask(*iter, m_taskTypeValue[temp]))
		{
			triggerFinish = true;
		}
	}

	if (triggerFinish)
	{
		_NotifyTaskHint();
	}

	//m_pMentorOneSide->SetChanged();
	MentorMgr::Instance()->AddChangedMentor(m_pMentorOneSide->GetOwnerID());

	LogInfo("%llu, taskvar:%u, %llu, %u", m_pMentorOneSide->GetOwnerID(), taskType, taskVar, m_taskTypeValue[temp]);
}

bool MentorTask::_CheckFinishTask(UINT32 taskID, UINT32 progress)
{
	auto find = m_selfTaskList.find(taskID);
	if (find == m_selfTaskList.end())
	{
		LogError("%llu, not find taskid:%u", m_pMentorOneSide->GetOwnerID(), taskID);
		return false;
	}
	if (!find->second.m_completeTime)
	{
		find->second.m_completeProgress = progress;
		if(find->second.CheckComplete(!_IsTaskTypeUseMinValue(find->second.m_var.m_taskType)))
		{
			LogInfo("%llu, complete task %u", m_pMentorOneSide->GetOwnerID(), find->first);
			DoTxLog(find->first, m_pMentorOneSide->GetOwnerID());
			return true;
		}
	}

	return false;
}

void MentorTask::_AddDailyActiveTaskValue(UINT32 taskType, UINT64 taskVar, UINT32 add, UINT32 time)
{
	std::set<UINT64> setTaskVar;
	auto& table = MentorConfig::Instance()->GetMentorTaskTable();
	for (auto iter = table.Table.begin(); iter != table.Table.end(); ++iter)
	{
		if ((*iter)->TaskType == taskType && taskVar >= (*iter)->TaskVar.seq[0])
		{
			setTaskVar.insert((*iter)->TaskVar.seq[0]);
		}
	}

	if (setTaskVar.empty())
	{
		return;
	}

	bool triggerFinish = false;
	for (auto iter = setTaskVar.begin(); iter != setTaskVar.end(); ++iter)
	{
		TaskTypeVar temp;
		temp.m_taskType = taskType;
		temp.m_var = *iter;

		auto find = m_taskTypeValue.find(temp);
		if (find == m_taskTypeValue.end())
		{
			m_taskTypeValue.insert(std::make_pair(temp, add));
			m_taskTypeValueChangeTime.insert(std::make_pair(temp, time));
		}
		else
		{
			if (!XCommon::IsDaySame(m_taskTypeValueChangeTime[temp], time))
			{
				find->second += add;
				m_taskTypeValueChangeTime[temp] = time;
			}
		}

		const std::set<UINT32>* ids = MentorConfig::Instance()->GetMentorTaskIDs(temp);
		if (!ids)
		{
			continue;
		}

		for (auto idIter = ids->begin(); idIter != ids->end(); ++idIter)
		{
			if(_CheckFinishTask(*idIter, m_taskTypeValue[temp]))
			{
				triggerFinish = true;
			}
		}

		//m_pMentorOneSide->SetChanged();

		LogInfo("%llu, taskvar:%u, daily active %u, %u", m_pMentorOneSide->GetOwnerID(), taskType, *iter, m_taskTypeValue[temp]);
	}

	MentorMgr::Instance()->AddChangedMentor(m_pMentorOneSide->GetOwnerID());

	if (triggerFinish)
	{
		_NotifyTaskHint();
	}
}

bool MentorTask::_IsTaskTypeUseMaxValue(UINT32 taskType)
{
	switch(taskType)
	{
	case KKSG::MentorTask_StageStar:
	case KKSG::MentorTask_Title:
	case KKSG::MentorTask_Emblem:
	case KKSG::MentorTask_SkyFloor:
	case KKSG::MentorTask_TowerFloor:
	case KKSG::MentorTask_AllEquipStengthen:
	case KKSG::MentorTask_MentorIntimacy:
	case KKSG::MentorTask_AllEquipQuality:
		return true;
	default:
		return false;
	}
}

bool MentorTask::_IsTaskTypeUseMinValue(UINT32 taskType)
{
	switch(taskType)
	{
	case KKSG::MentorTask_WorldBossRank:
		return true;
	default:
		return false;
	}

}

void MentorTask::_NotifyTaskHint()
{
	CRole* role = CRoleManager::Instance()->GetByRoleID(m_pMentorOneSide->GetOwnerID());
	if (!role)
	{
		return;
	}
	if (m_pMentorOneSide->GetRelationList()->GetSelfPos() != KKSG::EMentorPosStudent)
	{
		return;
	}
	PtcM2C_HintNotifyMS hintntf;
	hintntf.m_Data.add_systemid(SYS_MENTOR);
	role->Send(hintntf);
}

bool MentorTask::ConfirmTask(int taskID, UINT64 roleID)
{
	if(!_UpdateTaskStatus(taskID, roleID, KKSG::EMentorTask_ConfirmReport))
	{
		LogError("%llu, confirm task %u from %llu, error", m_pMentorOneSide->GetOwnerID(), taskID, roleID);
		return false;
	}

	_GiveReward(m_pMentorOneSide->GetOwnerID(), roleID, taskID);

	LogInfo("%llu, confirm task %u from %llu", m_pMentorOneSide->GetOwnerID(), taskID, roleID);

	return true;
}

bool MentorTask::ReportTask(int taskID, UINT64 roleID)
{
	MentorShipOneSide* pMaster = MentorMgr::Instance()->GetMentorShip(roleID);
	if (!pMaster)
	{
		return false;
	}
	if (!m_pMentorOneSide->GetRelationList()->IsInMasterStudent(roleID))
	{
		return false;
	}
	if (m_pMentorOneSide->GetRelationList()->GetRolePos(roleID) != KKSG::EMentorPosMaster)
	{
		return false;
	}

	if(!_UpdateTaskStatus(taskID, roleID, KKSG::EMentorTask_AlreadyReport))
	{
		return false;
	}

	pMaster->GetMentorMsg()->AddBeApplyMsg(m_pMentorOneSide->GetOwnerID(), KKSG::MentorMsgApplyReportTask, taskID);

	MentorMgr::Instance()->QuickSave(m_pMentorOneSide);
	MentorMgr::Instance()->QuickSave(pMaster);

	return true;
}

bool MentorTask::ReportAllTask(UINT64 roleID)
{
	MentorShipOneSide* pMaster = MentorMgr::Instance()->GetMentorShip(roleID);
	if (!pMaster)
	{
		return false;
	}
	if (!m_pMentorOneSide->GetRelationList()->IsInMasterStudent(roleID))
	{
		return false;
	}
	if (m_pMentorOneSide->GetRelationList()->GetRolePos(roleID) != KKSG::EMentorPosMaster)
	{
		return false;
	}
	std::vector<int> alltask;
	_GetCanReportList(roleID, alltask);

	for (auto iter = alltask.begin(); iter != alltask.end(); ++iter)
	{
		_UpdateTaskStatus(*iter, roleID, KKSG::EMentorTask_AlreadyReport);
		pMaster->GetMentorMsg()->AddBeApplyMsg(m_pMentorOneSide->GetOwnerID(), KKSG::MentorMsgApplyReportTask, *iter);
	}

	MentorMgr::Instance()->QuickSave(m_pMentorOneSide);
	MentorMgr::Instance()->QuickSave(pMaster);

	return true;
}

void MentorTask::RemoveReportTask(UINT64 roleID)
{
	for (auto iter = m_selfTaskList.begin(); iter != m_selfTaskList.end(); ++iter)
	{
		iter->second.m_reportStatus.erase(roleID);
	}
	MentorMgr::Instance()->AddChangedMentor(m_pMentorOneSide->GetOwnerID());
}

void MentorTask::CompleteAutoReport(UINT64 roleID)
{
	foreach (i in m_selfTaskList)
	{
		if (!i->second.IsTaskComplete())
		{
			continue;
		}
		if (i->second.IsCompleteBeforeMentor(m_pMentorOneSide, roleID))
		{
			continue;
		}
		auto find = i->second.m_reportStatus.find(roleID);
		if (find == i->second.m_reportStatus.end())
		{
			i->second.Report(m_pMentorOneSide, roleID);
		}
		else
		{
			if (find->second == KKSG::EMentorTask_ConfirmReport)
			{
				continue;
			}
		}

		i->second.Confirm(roleID);
		_GiveReward(m_pMentorOneSide->GetOwnerID(), roleID, i->first);
	}
}

bool MentorTask::CanReportTask()
{
	bool canReport = false;
	auto& list = m_pMentorOneSide->GetRelationList()->GetRelationList();
	if (list.empty())
	{
		return false;
	}
	for (auto iter = m_selfTaskList.begin(); iter != m_selfTaskList.end(); ++iter)
	{
		for (auto i = list.begin(); i != list.end(); ++i)
		{
			if (iter->second.CanReport(m_pMentorOneSide, i->second.m_roleID))
			{
				canReport = true;
				break;
			}
		}
	}

	return canReport;
}

void MentorTask::Convert2OneRelationClient(KKSG::OneMentorRelationInfo2Client& relation, UINT64 masterID)
{
	if (!masterID)
	{
		return;
	}
	foreach (i in m_selfTaskList)
	{
		auto task = relation.add_studenttasklist();
		i->second.Convert2KKSGOneMentorTaskInfo(m_pMentorOneSide, *task, masterID);
	}
}

void MentorTask::Convert2SelfClient(KKSG::MentorSelfInfo& selfInfo)
{
	foreach (i in m_selfTaskList)
	{
		auto task = selfInfo.add_selftasklist();
		i->second.Convert2KKSGOneMentorTaskInfo(m_pMentorOneSide, *task);
	}
}

void MentorTask::DebugClear()
{
	m_selfTaskList.clear();
	m_taskTypeValue.clear();
	Refresh();
}

int MentorTask::GetTaskCompleteNum()
{
	int num = 0;
	for (auto iter = m_selfTaskList.begin(); iter != m_selfTaskList.end(); ++iter)
	{
		if (iter->second.IsTaskComplete())
		{
			++num;
		}
	}

	return num;
}

bool MentorTask::_IsTypeOpenClear(int type)
{
	switch(type)
	{
	case KKSG::MentorTask_JoinGuild:
	case KKSG::MentorTask_Title:
	case KKSG::MentorTask_Emblem:
	case KKSG::MentorTask_AllEquipStengthen:
	case KKSG::MentorTask_AllEquipQuality:
	case KKSG::MentorTask_BuyPrivilege:
		return false;
	default:
		return true;
	}
}

void MentorTask::_GiveReward(UINT64 selfID, UINT64 roleID, int taskID)
{
	const MentorTaskTable::RowData* data = MentorConfig::Instance()->GetMentorTaskData(taskID);
	if (!data)
	{
		return;
	}
	std::vector<ItemDesc> items;
	for (auto i = data->MasterReward.begin(); i != data->MasterReward.end(); ++i)
	{
		ItemDesc item(i->seq[0], i->seq[1]);
		items.push_back(item);
	}
	std::string tempstring = "xxx";
	if (items.size())
	{
		std::string content = GetGlobalConfig().MentorTask_MailMasterContent;
		std::string::size_type pos = 0;
		std::string name = "";
		CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(selfID); 
		if (summary)
		{
			name = summary->GetName();
		}
		pos = content.find(tempstring);
		if (pos != std::string::npos)
		{
			content.replace(pos, tempstring.size(), name);
		}
		MailData mailData( GetGlobalConfig().MentorTask_MailMasterTitle, content, items, MAIL_EXPIRE_TIME);
		mailData.SetReason(ItemFlow_MentorTask);
		CMailMgr::Instance()->SendMail(roleID, mailData);
	}
	
	items.clear();
	for (auto i = data->StudentReward.begin(); i != data->StudentReward.end(); ++i)
	{
		ItemDesc item(i->seq[0], i->seq[1]);
		items.push_back(item);
	}
	if (items.size())
	{
		std::string content = GetGlobalConfig().MentorTask_MailStudentContent;
		std::string::size_type pos = 0;
		std::string name = "";
		CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(roleID); 
		if (summary)
		{
			name = summary->GetName();
		}
		pos = content.find(tempstring);
		if (pos != std::string::npos)
		{
			content.replace(pos, tempstring.size(), name);
		}
		MailData mailData( GetGlobalConfig().MentorTask_MailStudentTitle, content, items, MAIL_EXPIRE_TIME);
		mailData.SetReason(ItemFlow_MentorTask);
		CMailMgr::Instance()->SendMail(selfID, mailData);
	}
}

bool MentorTask::_UpdateTaskStatus(int taskID, UINT64 roleID, KKSG::EMentorTaskStatus status)
{
	auto find = m_selfTaskList.find(taskID);
	if (find == m_selfTaskList.end())
	{
		return false;
	}

	switch(status)
	{
	case KKSG::EMentorTask_AlreadyReport:
		{
			if (!find->second.Report(m_pMentorOneSide, roleID))
			{
				return false;
			}
		}
		break;
	case KKSG::EMentorTask_ConfirmReport:
		{
			if (!find->second.Confirm(roleID))
			{
				return false;
			}
		}
		break;
	default:
		break;
	}


	return true;
}

bool MentorTask::_GetCanReportList(UINT64 roleID, std::vector<int>& taskList)
{
	for (auto iter = m_selfTaskList.begin(); iter != m_selfTaskList.end(); ++iter)
	{
		if (!iter->second.CanReport(m_pMentorOneSide, roleID))
		{
			continue;
		}

		taskList.push_back(iter->first);
	}

	return true;
}

void MentorTask::DoTxLog(int taskID, UINT64 roleID)
{
	TMentorFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_uRoleID = roleID;
	oLog.m_TaskId = taskID;
	oLog.m_op = Tx_MentorType4;
	oLog.Do();
}

