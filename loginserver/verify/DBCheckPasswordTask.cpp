#include "pch.h"
#include <time.h>
#include "DBCheckPasswordTask.h"
#include "DBGetSelfServerTask.h"
#include "tokenverifymgr.h"
#include "crpc.h"
#include "SQLStmt.h"
#include "iplist/whitelistmgr.h"
#include "logincontrol.h"


DBCheckPasswordTask::DBCheckPasswordTask()
{
	m_dwRpcID = 0;
}

DBCheckPasswordTask::~DBCheckPasswordTask()
{

}

int DBCheckPasswordTask::Execute(MYSQL *mysql)
{
	const char query[] = "select `password` from users where userid=?";
	SQLStmt<1,1> stmt(mysql);

	if (!stmt.SetQuery(query, sizeof(query)))
	{
		stmt.ReportError();
		return stmt.GetError();
	}

	char passwd[128];
	stmt.BindIn(&m_userid[0], m_userid.size());
	stmt.BindOut(passwd, 128);

	if (!stmt.Execute())
	{
		stmt.ReportError();
		return stmt.GetError();
	}

	if (stmt.FetchResult())
	{
		m_dbpassword = passwd;
	}
	
	return 0;
}

void DBCheckPasswordTask::FinishTask()
{
	if (m_result != 0 || m_dbpassword.empty())
	{
		LogWarn("select password failed for userid [%s]", m_userid.c_str());
		CUserData oUserData;
		oUserData.m_dwUserData = 0;
		oUserData.m_pUserPtr = NULL;
		CRpc::ReplyDelayRpc(m_dwRpcID, oUserData);
		CLoginControl::Instance()->EndLogin(m_userid);
		return ;
	}

	if (m_dbpassword != m_password)
	{
		LogWarn("userid [%s] password [%s] error", m_userid.c_str(), m_password.c_str());

		CUserData oUserData;
		oUserData.m_dwUserData = 0;
		oUserData.m_pUserPtr = NULL;
		CRpc::ReplyDelayRpc(m_dwRpcID, oUserData);
		CLoginControl::Instance()->EndLogin(m_userid);
		return ;
	}

	DBGetSelfServerTask* pTask = new DBGetSelfServerTask(m_dwRpcID, m_userid, m_userid, m_ip);
	TokenVerifyMgr::Instance()->AddDBTask(pTask);
}
