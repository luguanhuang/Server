#include "pch.h"
#include "db/mysqlmgr.h"
#include "db/task/friendwritetask.h"
#include "friend/ptcm2d_updatefriendondb.h"

// generate by ProtoGen at date: 2016/7/30 15:19:39

void PtcM2D_UpdateFriendOnDB::Process(UINT32 dwConnID)
{
	FriendInsertTask* task = new FriendInsertTask();
	task->m_roleid = m_Data.roleid();
	task->m_name = m_Data.name();
	task->m_info = m_Data.info();
	CMysqlMgr::Instance()->PushTask(task);

	SSInfo<<"update friend info, name:"<<m_Data.name()<<END;
}
