#include "pch.h"
#include "mail/ptcm2d_addtemplatemail.h"
#include "db/mysqlmgr.h"
#include "db/task/tmplmailinserttask.h"

// generate by ProtoGen at date: 2016/7/21 16:17:39

void PtcM2D_AddTemplateMail::Process(UINT32 dwConnID)
{
	CTemplateMailInsertTask* poTask = new CTemplateMailInsertTask();
	poTask->m_oData = m_Data.maildata();
	CMysqlMgr::Instance()->PushTask(poTask);
}
