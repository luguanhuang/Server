#include "pch.h"
#include "dbgetbespeaktask.h"

#include "crpc.h"
#include "SQLStmt.h"
#include "verify/logincontrol.h"
#include "gmmgr/gmmgr.h"
#include "gatemgr/gatemgr.h"

DBGetBespeakTask::DBGetBespeakTask(UINT32 rpcid, const std::string& account)
:m_rpcid(rpcid)
,m_account(account)
{
	m_qwBeginTime = TimeUtil::GetMilliSecond();
	m_info.account = m_account;
}

DBGetBespeakTask::~DBGetBespeakTask()
{
}

int DBGetBespeakTask::Execute(MYSQL *mysql)
{
	{
		const char query[] = "select serverid from selfserver where userid=?";
		SQLStmt<1,1> stmt(mysql);
		if (!stmt.SetQuery(query, sizeof(query)))
		{
			stmt.ReportError();
			return stmt.GetError();
		}

		int serverid = 0;
		stmt.BindIn(&m_account[0], m_account.size());
		stmt.BindOut(&serverid);

		if (!stmt.Execute())
		{
			stmt.ReportError();
			return stmt.GetError();
		}

		while (stmt.FetchResult())
		{
			m_info.hasRoleServerIds.insert(serverid);
		}	
	}

	{
		const char query[] = "select serverid from bespeak where openid=?";
		SQLStmt<1,1> stmt(mysql);
		if (!stmt.SetQuery(query, sizeof(query)))
		{
			stmt.ReportError();
			return stmt.GetError();
		}

		int serverid = 0;
		stmt.BindIn(&m_account[0], m_account.size());
		stmt.BindOut(&serverid);

		if (!stmt.Execute())
		{
			stmt.ReportError();
			return stmt.GetError();
		}

		while (stmt.FetchResult())
		{
			m_info.bespeakServerIds.push_back(serverid);
		}	
	}

	return 0;
}

void DBGetBespeakTask::FinishTask()
{
	if (m_result != 0)
	{
		LogError("sql error[%d]", m_result);
		CUserData oUserData;
		oUserData.m_dwUserData = 0;
		oUserData.m_pUserPtr = NULL;
		CRpc::ReplyDelayRpc(m_rpcid, oUserData);
		return;
	}

	UINT64 qwEndTime = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = qwEndTime >= m_qwBeginTime ? qwEndTime - m_qwBeginTime : 0;
	if(qwTimeUsed > 200)
	{
		LogWarn("Get bespeak task finish use time %llu ms", qwTimeUsed);
	}
	CUserData oUserData;
	oUserData.m_dwUserData = 0;
	oUserData.m_pUserPtr = &m_info;
	CRpc::ReplyDelayRpc(m_rpcid, oUserData);
}
