#include "pch.h"
#include "db/task/accountdeltask.h"
#include "db/mysqlmgr.h"
#include "account/ptcn2d_accountlogout.h"

// generate by ProtoGen at date: 2017/2/27 14:13:50

void PtcN2D_AccountLogout::Process(UINT32 dwConnID)
{
	CAccountDelTask* poTask = new CAccountDelTask();
	poTask->m_strAccount = m_Data.account();
	poTask->m_serverId = m_Data.serverid();
	CMysqlMgr::Instance()->PushTask(poTask, GetAccountThreadIndex());

	SSInfo<<"push task delete account, account: "<<m_Data.account() << " serverid: " << m_Data.serverid() <<END;
}
