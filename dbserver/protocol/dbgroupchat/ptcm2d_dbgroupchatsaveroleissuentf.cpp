#include "pch.h"
#include "dbgroupchat/ptcm2d_dbgroupchatsaveroleissuentf.h"
#include "db/task/dbgroupchatsaveroleissue.h"
#include "db/mysqlmgr.h"

// generate by ProtoGen at date: 2017/7/28 15:39:00

void PtcM2D_DBGroupChatSaveRoleIssueNtf::Process(UINT32 dwConnID)
{
    CTaskGroupChatSaveRoleIssue* poTask = new CTaskGroupChatSaveRoleIssue();
    poTask->m_Index = m_Data.index();
    poTask->m_Time = m_Data.time();
    poTask->m_data.CopyFrom(m_Data);
    CMysqlMgr::Instance()->PushTask(poTask);
}
