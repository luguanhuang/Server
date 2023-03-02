#include "pch.h"
#include "dbgroupchat/ptcm2d_dbgroupchatsavegroupissuentf.h"
#include "db/task/dbgroupchatsavegroupissue.h"
#include "db/mysqlmgr.h"

// generate by ProtoGen at date: 2017/7/28 15:36:41

void PtcM2D_DBGroupChatSaveGroupIssueNtf::Process(UINT32 dwConnID)
{
    CTaskGroupChatSaveGroupIssue* poTask = new CTaskGroupChatSaveGroupIssue();
    poTask->m_Index = m_Data.index();
    poTask->m_Time = m_Data.time();
    poTask->m_data.CopyFrom(m_Data);
    CMysqlMgr::Instance()->PushTask(poTask);
}
