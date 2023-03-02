#include "pch.h"
#include "dbgroupchat/ptcm2d_dbgroupchatdelroleissuentf.h"
#include "db/mysqlmgr.h"
#include "db/task/dbgroupchatdelroleissue.h"

// generate by ProtoGen at date: 2017/7/28 15:39:36

void PtcM2D_DBGroupChatDelRoleIssueNtf::Process(UINT32 dwConnID)
{
    CTaskGroupChatDelRoleIssue* poTask = new CTaskGroupChatDelRoleIssue();
    poTask->m_Index = m_Data.index();
    poTask->m_TodayBeginTime = m_Data.todaybegintime();
    poTask->m_IsClear = m_Data.isclear();
    CMysqlMgr::Instance()->PushTask(poTask);
}
