#include "pch.h"
#include "DBGetSelfServerTask.h"
#include "crpc.h"
#include "SQLStmt.h"
#include "verify/logincontrol.h"
#include "gmmgr/gmmgr.h"
#include "gatemgr/gatemgr.h"

DBGetSelfServerTask::DBGetSelfServerTask(UINT32 rpcid, const std::string& account, const std::string& phone, UINT32 ip)
:m_rpcid(rpcid)
,m_ip(ip)
,m_account(account)
{
	m_qwBeginTime = TimeUtil::GetMilliSecond();
	m_info.phone = phone;
	m_info.serverID = -1;
}

DBGetSelfServerTask::~DBGetSelfServerTask()
{
}

int DBGetSelfServerTask::Execute(MYSQL *mysql)
{
	UINT32 lastLoginTime = 0;
	{
		const char query[] = "select serverid, level, UNIX_TIMESTAMP(time) from selfserver where userid=?";
		SQLStmt<1,3> stmt(mysql);
		if (!stmt.SetQuery(query, sizeof(query)))
		{
			stmt.ReportError();
			return stmt.GetError();
		}

		int level = 0;
		int serverid = 0;
		int timestamp = 0;
		stmt.BindIn(&m_account[0], m_account.size());
		stmt.BindOut(&serverid);
		stmt.BindOut(&level);
		stmt.BindOut(&timestamp);

		if (!stmt.Execute())
		{
			stmt.ReportError();
			return stmt.GetError();
		}

		while (stmt.FetchResult())
		{
			m_info.serverid2level[serverid] = level;
			if (timestamp > lastLoginTime)
			{
				lastLoginTime = timestamp;
			}
		}	
	}

	// 有角色才去lastlogin读取
	if (!m_info.serverid2level.empty())
	{
		const char query[] = "select serverid from lastlogin where userid=?";
		SQLStmt<1,1> stmt(mysql);

		if (!stmt.SetQuery(query, sizeof(query)))
		{
			stmt.ReportError();
			return stmt.GetError();
		}

		int serverID;
		stmt.BindIn(&m_account[0], m_account.size());
		stmt.BindOut(&serverID);
		
		if (!stmt.Execute())
		{
			stmt.ReportError();
			return stmt.GetError();
		}

		if (stmt.FetchResult())
		{
			m_info.serverID = serverID;
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

	UINT32 now = TimeUtil::GetTime();
	if (lastLoginTime != 0 && lastLoginTime < now && (now - lastLoginTime) >= LoginConfig::Instance()->GetAccountBackFlowTime())
	{
		m_userInfo.accountBackFlow = true;
		const char query[] = "replace into account_backflow (openid, expiretime) values (?,FROM_UNIXTIME(?))";
		int expireTime = now + LoginConfig::Instance()->GetAccountBackFlowExpireTime();
		SQLStmt<2, 0> stmt(mysql);
		if (!stmt.SetQuery(query, sizeof(query)))
		{
			stmt.ReportError();
			return stmt.GetError();
		}
		stmt.BindIn(&m_account[0], m_account.size());
		stmt.BindIn(&expireTime);

		if (!stmt.Execute())
		{
			stmt.ReportError();
			return stmt.GetError();
		}
	}
	else
	{
		const char query[] = "select UNIX_TIMESTAMP(expiretime) from account_backflow where openid=?";
		SQLStmt<1,1> stmt(mysql);
		if (!stmt.SetQuery(query, sizeof(query)))
		{
			stmt.ReportError();
			return stmt.GetError();
		}

		int expireTime = 0;
		stmt.BindIn(&m_account[0], m_account.size());
		stmt.BindOut(&expireTime);

		if (!stmt.Execute())
		{
			stmt.ReportError();
			return stmt.GetError();
		}

		if (stmt.FetchResult())
		{
			if (expireTime > now)
			{
				m_userInfo.accountBackFlow = true;
			}
		}	
	}


	return 0;
}

void DBGetSelfServerTask::FinishTask()
{
	if (m_result != 0)
	{
		LogError("sql error[%d]", m_result);
		CLoginControl::Instance()->GetLoginInfo(m_account).SetFinish(LoginFlowTask_GetSelfServer, TaskStatus_Failed);
		return;
	}

	UINT64 qwEndTime = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = qwEndTime >= m_qwBeginTime ? qwEndTime - m_qwBeginTime : 0;
	if(qwTimeUsed > 200)
	{
		LogWarn("Get self server task finish use time %llu ms", qwTimeUsed);
	}

	bool isgm = GMMgr::Instance()->IsGM(m_account/*, m_info.phone*/);
	//bool isiplimit = IPListMgr::Instance()->IsInSpan(m_ip);
	//isgm |= isiplimit;

	m_info.uid = TokenVerifyMgr::Instance()->CreateUUID(isgm);
	m_info.isgm = isgm;

	m_userInfo.isused = false;
	m_userInfo.isgm = isgm;
	m_userInfo.account = m_account;
	m_userInfo.tokenExpireTime = time(NULL) + 3600;
	TokenVerifyMgr::Instance()->AddToken(m_info.uid, m_userInfo);


	LoginInfo& info = CLoginControl::Instance()->GetLoginInfo(m_account);
	info.userGateInfo = m_info;
	info.SetFinish(LoginFlowTask_GetSelfServer, TaskStatus_Success);
}
