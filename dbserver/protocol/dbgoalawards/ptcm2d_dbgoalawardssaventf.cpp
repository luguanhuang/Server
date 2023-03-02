#include "pch.h"
#include "dbgoalawards/ptcm2d_dbgoalawardssaventf.h"
#include "db/task/dbgoalawardssave.h"
#include "db/mysqlmgr.h"
// generate by ProtoGen at date: 2017/9/6 20:17:26

void PtcM2D_DBGoalAwardsSaveNtf::Process(UINT32 dwConnID)
{
    CTaskGoalAwardsSave* poTask = new CTaskGoalAwardsSave();
    poTask->m_Index = m_Data.roleid();
    poTask->m_data.CopyFrom(m_Data);
    CMysqlMgr::Instance()->PushTask(poTask);
}
