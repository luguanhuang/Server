#include "pch.h"
#include "dbgroupchat/ptcm2d_dbgroupchatdelrolentf.h"
#include "db/mysqlmgr.h"
#include "db/task/dbgroupchatdelrole.h"
// generate by ProtoGen at date: 2017/7/28 15:44:02

void PtcM2D_DBGroupChatDelRoleNtf::Process(UINT32 dwConnID)
{
    CTaskGroupChatDelRole* poTask = new CTaskGroupChatDelRole();
    poTask->m_Index = m_Data.roleid();
    poTask->m_TodayBeginTime = m_Data.todaybegintime();
    poTask->m_IsClear = m_Data.isclear();
    CMysqlMgr::Instance()->PushTask(poTask);
}
