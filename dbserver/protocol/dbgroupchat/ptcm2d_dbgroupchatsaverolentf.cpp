#include "pch.h"
#include "dbgroupchat/ptcm2d_dbgroupchatsaverolentf.h"
#include "db/task/dbgroupchatsaverole.h"
#include "db/mysqlmgr.h"

// generate by ProtoGen at date: 2017/7/28 15:43:41

void PtcM2D_DBGroupChatSaveRoleNtf::Process(UINT32 dwConnID)
{
    CTaskGroupChatSaveRole* poTask = new CTaskGroupChatSaveRole();
    poTask->m_Index = m_Data.index();
    poTask->m_Time = m_Data.time();
    poTask->m_data.CopyFrom(m_Data);
    CMysqlMgr::Instance()->PushTask(poTask);
}
