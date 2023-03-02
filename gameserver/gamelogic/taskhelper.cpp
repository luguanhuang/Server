#include "pch.h"
#include "taskhelper.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/taskrecord.h"
#include "event/eventmgr.h"

void TaskHelper::UpdateTask(UINT64 roleId, KKSG::TaskOper oper, UINT32 type, UINT32 id, UINT32 step)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(roleId);
	if (pRole)
	{
		pRole->Get<TaskRecord>()->UpdateTask(oper, type, id, step);
	}
	else
	{
		UINT32 eventID = TASK_EVENT_OFFSET;
		switch (oper)
		{
		case KKSG::TaskOper_Set: 
			eventID += TASK_EVENT_SET;
			break;
		case KKSG::TaskOper_Add:
			eventID += TASK_EVENT_ADD;
			break;
		case KKSG::TaskOper_Del:
			eventID += TASK_EVENT_DEL;
			break;
		}
		EventMgr::Instance()->AddEvent(roleId, eventID, type, id, step);
	}
}