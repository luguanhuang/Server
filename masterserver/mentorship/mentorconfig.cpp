#include "pch.h"
#include "mentorconfig.h"
#include "mentortask.h"

INSTANCE_SINGLETON(MentorConfig)

MentorConfig::MentorConfig()
{
}

MentorConfig::~MentorConfig()
{

}

bool MentorConfig::Init()
{
	if (!m_completeReward.LoadFile("table/MentorCompleteReward.txt"))
	{
		LogWarn("Load MentorCompleteReward.txt failed!");
		return false;
	}

	if (!m_mentorTaskTable.LoadFile("table/MentorTask.txt"))
	{
		LogWarn("Load MentorTask.txt failed!");
		return false;
	}

	_InitMentorTaskTypeIDs();

	return true;
}

void MentorConfig::Uninit()
{

}

const std::set<UINT32>* MentorConfig::GetMentorTaskIDs(TaskTypeVar& var)
{
	auto find = m_mentorTaskTypeIDs.find(var);
	if (find == m_mentorTaskTypeIDs.end())
	{
		return NULL;
	}

	return &find->second;
}

const MentorCompleteRewardTable::RowData* MentorConfig::GetCompleteRewardData(int type)
{
	return m_completeReward.GetByType(type);
}

UINT32 MentorConfig::GetMentorTaskMaxProgress(UINT32 taskID)
{
	auto iter = m_mentorTaskTable.GetByTaskID(taskID);
	if (!iter)
	{
		return 0;
	}
	return iter->TaskVar.seq[1];
}

const MentorTaskTable::RowData* MentorConfig::GetMentorTaskData(UINT32 taskID)
{
	auto iter = m_mentorTaskTable.GetByTaskID(taskID);
	return iter;
}

void MentorConfig::_InitMentorTaskTypeIDs()
{
	for (auto iter = m_mentorTaskTable.Table.begin(); iter != m_mentorTaskTable.Table.end(); ++iter)
	{
		TaskTypeVar temp;
		temp.m_taskType = (*iter)->TaskType;
		temp.m_var = (*iter)->TaskVar.seq[0];
		m_mentorTaskTypeIDs[temp].insert((*iter)->TaskID);
	}
}

