#include "pch.h"
#include "dbgroupchat/ptcm2d_dbgroupchatsavegroupntf.h"
#include "db/task/dbgroupchatsavegroup.h"
#include "db/mysqlmgr.h"

// generate by ProtoGen at date: 2017/7/28 15:41:26

void PtcM2D_DBGroupChatSaveGroupNtf::Process(UINT32 dwConnID)
{
    CTaskGroupChatSaveGroup* poTask = new CTaskGroupChatSaveGroup();
    poTask->m_Index = m_Data.index();
    poTask->m_Time = m_Data.time();
    poTask->m_data.CopyFrom(m_Data);
    CMysqlMgr::Instance()->PushTask(poTask);
}
