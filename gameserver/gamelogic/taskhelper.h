#ifndef _TASK_HELPER_H_
#define _TASK_HELPER_H_

class TaskHelper
{
public:
	static void UpdateTask(UINT64 roleId, KKSG::TaskOper oper, UINT32 type, UINT32 id = 0, UINT32 step = 1);
};

#endif