#include "pch.h"
#include "mail/ptcm2d_mailoperation.h"
#include "db/mysqlmgr.h"
#include "db/task/mailoptask.h"

// generate by ProtoGen at date: 2016/7/21 16:26:51

void PtcM2D_MailOperation::Process(UINT32 dwConnID)
{
	CMailOpTask* poTask = new CMailOpTask();
	poTask->m_qwRoleID = m_Data.roleid();
	poTask->m_nOP = m_Data.op();
	poTask->m_oData = m_Data.maildata();
	poTask->m_bByTemplate = m_Data.bytemplate();
	poTask->m_qwTemplateID = m_Data.templateid();
	CMysqlMgr::Instance()->PushTask(poTask);
}
