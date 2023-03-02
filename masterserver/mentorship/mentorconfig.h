#ifndef __MENTORCONFIG_H__
#define __MENTORCONFIG_H__
#include "table/MentorCompleteRewardTable.h"
#include "table/MentorTaskTable.h"

struct TaskTypeVar;
class MentorConfig
{
public:
	MentorConfig();
	~MentorConfig();
	DECLARE_SINGLETON(MentorConfig)

	bool Init();
	void Uninit();
	const std::set<UINT32>* GetMentorTaskIDs(TaskTypeVar& var);
	MentorTaskTable& GetMentorTaskTable(){return m_mentorTaskTable;}
	const MentorCompleteRewardTable::RowData* GetCompleteRewardData(int type);
	UINT32 GetMentorTaskMaxProgress(UINT32 taskID);
	const MentorTaskTable::RowData* GetMentorTaskData(UINT32 taskID);

private:
	void _InitMentorTaskTypeIDs();
private:
	MentorCompleteRewardTable m_completeReward;
	MentorTaskTable m_mentorTaskTable;
	std::map<TaskTypeVar, std::set<UINT32>> m_mentorTaskTypeIDs;

};

#endif // 