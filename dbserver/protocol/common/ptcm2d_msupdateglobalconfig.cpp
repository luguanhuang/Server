#include "pch.h"
#include "db/mysqlmgr.h"
#include "db/task/globalupdatetask.h"
#include "common/ptcm2d_msupdateglobalconfig.h"

// generate by ProtoGen at date: 2016/7/28 14:09:53

void PtcM2D_MsUpdateGlobalConfig::Process(UINT32 dwConnID)
{
	CGlobalUpdateTask* poTask = new CGlobalUpdateTask();
	poTask->m_uID = m_Data.configid();
	poTask->m_strData = m_Data.configvalue();
	CMysqlMgr::Instance()->PushTask(poTask);
	SSInfo<<"run update globalconfig task, id:"<<m_Data.configid()<<" value:"<<m_Data.configvalue()<<END;
}
