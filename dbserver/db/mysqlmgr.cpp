#include "pch.h"
#include "mysqlmgr.h"
#include "util.h"
#include "util/dbutil.h"
#include "config.h"
#include "timeutil.h"
#include "network/controllink.h"
#include "common/ptcd2n_mysqlconnectlost.h"


INSTANCE_SINGLETON(CMysqlMgr)


CMysqlMgr::CMysqlMgr()
:m_bMysqlOK(false)
,m_bKickNtfSended(false)
,m_bMSSaveDBEnd(true)
{ 
}

CMysqlMgr::~CMysqlMgr()
{
}

bool CMysqlMgr::Init()
{
	const DBInfo& dbInfo = DBConfig::Instance()->GetDBInfo();
	UINT32 dwThreadCount = convert<UINT32>(dbInfo.m_threads);
	if(dwThreadCount == 0 || dwThreadCount > 20)
	{
		LogError("thread count [%u] is invalid", dwThreadCount);
		return false;
	}

	if(DBConfig::Instance()->AutoCreateDB())
	{
		IMysqlConn* poMysqlConn = CreateMysqlConn();
		if(poMysqlConn == NULL)
		{
			return false;
		}

		if(!poMysqlConn->Connect(dbInfo.m_ip.c_str(), convert<UINT32>(dbInfo.m_port), dbInfo.m_user.c_str(), dbInfo.m_pswd.c_str(), "mysql"))
		{
			LogError("Can't connect to database %s:%s:mysql, ErrDesc: %s", dbInfo.m_ip.c_str(), dbInfo.m_port.c_str(), poMysqlConn->GetError());
			return false;
		}

		std::stringstream ss;
		ss << "CREATE DATABASE IF NOT EXISTS " << dbInfo.m_database << " " << "DEFAULT CHARACTER SET utf8mb4 DEFAULT COLLATE utf8mb4_general_ci;";
		if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
		{
			LogError("Create database %s failed", dbInfo.m_database.c_str());
			return false;
		}

		poMysqlConn->Close();
		if(!poMysqlConn->Connect(dbInfo.m_ip.c_str(), convert<UINT32>(dbInfo.m_port), dbInfo.m_user.c_str(), dbInfo.m_pswd.c_str(), dbInfo.m_database.c_str()))
		{
			LogError("Can't connect to database %s:%s:%s, ErrDesc: %s", dbInfo.m_ip.c_str(), dbInfo.m_port.c_str(), dbInfo.m_database.c_str(), poMysqlConn->GetError());
			return false;
		}

		const char* pszSqlScript = "dbinit.sql";
		if(!RunSqlScript(pszSqlScript, poMysqlConn))
		{
			LogError("Run script: %s failed", pszSqlScript);
			return false;
		}

		poMysqlConn->Close();
		poMysqlConn->Release();
		LogInfo("Run script %s succ", pszSqlScript);
	}

	m_oThreadList.reserve(dwThreadCount);
	for(UINT32 i = 0; i < dwThreadCount; ++i)
	{
		CMysqlThread* poThread = new CMysqlThread();
		if(!poThread->Init(dbInfo))
		{
			poThread->Uninit();
			delete poThread;
			return false;
		}
		if(i == 0)
		{
			DeleteEmptyAccount(poThread->GetMysqlConn());
			UpdateAccountServerId(poThread->GetMysqlConn());
			UpdateRankListServerId(poThread->GetMysqlConn());
		}
		if(i == 0 && !ReadCurrentRegisterNum(poThread->GetMysqlConn()))
		{
			poThread->Uninit();
			delete poThread;
			return false;
		}

		poThread->GetMysqlConn()->SetMysqlLostCallBack(DBConfig::Instance()->GetReconTryInterval(), DBConfig::Instance()->GetReconMaxTryTimes(), CMysqlMgr::MysqlEventCallBack);
		poThread->Start();
		m_oThreadList.push_back(poThread);
	}

	const DBInfo& roOnlineDbInfo = DBConfig::Instance()->GetOnlineDBInfo();
	if(!m_oOnlineThread.Init(roOnlineDbInfo))
	{
		return false;
	}

	m_bMysqlOK = true;
	m_oOnlineThread.GetMysqlConn()->SetMysqlLostCallBack(30, 1000, NULL);
	m_oOnlineThread.Start();
	return true;
}

void CMysqlMgr::Uninit()
{
	for(UINT32 i = 0; i < m_oThreadList.size(); ++i)
	{
		CMysqlThread* poThread = m_oThreadList[i];
		poThread->Cancel();
		poThread->Join();
		poThread->Uninit();
		delete poThread;
	}

	m_oOnlineThread.Cancel();
	m_oOnlineThread.Join();
	m_oOnlineThread.Uninit();

	m_oThreadList.clear();
}

void CMysqlMgr::Process()
{
	for(UINT32 i = 0; i < m_oThreadList.size(); ++i)
	{
		CMysqlThread* poThread = m_oThreadList[i];
		poThread->ProcessReply();
	}
	m_oOnlineThread.ProcessReply();

	UpdateMysqlStatus();
}

void CMysqlMgr::PushTask(CMysqlTask* poTask, UINT32 dwThrdIndex)
{
	CMysqlThread* poThread = GetThread(dwThrdIndex);
	if(poThread == NULL)
	{
		return;
	}

	UINT64 qwStartTime = TimeUtil::GetMilliSecond();
	poTask->SetBeginTime(qwStartTime);
	poThread->PushTask(poTask);
}

CMysqlThread* CMysqlMgr::GetThread(UINT32 dwIndex)
{
	if(dwIndex >= m_oThreadList.size())
	{
		return NULL;
	}
	return m_oThreadList[dwIndex];
}

void CMysqlMgr::PushOnlineNumTask(CMysqlTask* poTask)
{
	UINT64 qwStartTime = TimeUtil::GetMilliSecond();
	poTask->SetBeginTime(qwStartTime);
	m_oOnlineThread.PushTask(poTask);
}

bool CMysqlMgr::ReadCurrentRegisterNum(IMysqlConn* poMysqlConn)
{
	if(poMysqlConn == NULL) return false;

	std::string str = "select count(*) from account";
	IMysqlRecordSet* poRes = NULL;
	if(!poMysqlConn->QueryWithRecord(str.c_str(), str.size(), &poRes))
	{
		LogError("Read table account failed, errorMsg: %s", poMysqlConn->GetError());
		return false;
	}

	if(!poRes->FetchRow())
	{
		LogError("Read table account failed, fetch row error, errorMsg: %s", poMysqlConn->GetError());
		return false;
	}

	char* pVal   = NULL;
	UINT32 dwLen = 0;
	UINT32 dwCurrRegNum = 0;
	if(poRes->GetFieldValue(0, &pVal, dwLen))
	{
		dwCurrRegNum = convert<UINT32>(pVal);
	}
	else
	{
		LogError("Read count(*) column failed");
		return false;
	}

	poRes->Release();
	DBConfig::Instance()->SetCurRegisterNum(dwCurrRegNum);
	LogInfo("Current register number: %u", dwCurrRegNum);
	return true;
}

void CMysqlMgr::DeleteEmptyAccount(IMysqlConn* poMysqlConn)
{
	std::string str = "select * from account";
	IMysqlRecordSet* poRes = NULL;
	if(!poMysqlConn->QueryWithRecord(str.c_str(), str.size(), &poRes))
	{
		LogError("Read table account failed, errorMsg: %s", poMysqlConn->GetError());
		return;
	}

	std::map<INT32, std::string> oIndex2ColName;
	INT32 nIndex = 2;
	oIndex2ColName[nIndex++] = "role1";
	oIndex2ColName[nIndex++] = "role2";
	oIndex2ColName[nIndex++] = "role3";
	oIndex2ColName[nIndex++] = "role4";
	oIndex2ColName[nIndex++] = "role5";
	oIndex2ColName[nIndex++] = "role6";
	oIndex2ColName[nIndex++] = "role7";
	oIndex2ColName[nIndex++] = "role8";
	oIndex2ColName[nIndex++] = "role9";
	oIndex2ColName[nIndex++] = "hiddenroles";

	std::vector<std::pair<std::string, UINT32>> oDeleteList;
	UINT64 qwBegin = TimeUtil::GetMilliSecond();
	while(poRes->FetchRow())
	{
		char* pVal   = NULL;
		UINT32 dwLen = 0;

		std::string strOpenId;
		if(poRes->GetFieldValue(0, &pVal, dwLen))
		{
			strOpenId.assign(pVal, dwLen);
		}
		else
		{
			LogError("Read '_id' column failed");
			return;
		}

		bool bHasRole = false;
		for(std::map<INT32, std::string>::iterator it = oIndex2ColName.begin(); it != oIndex2ColName.end(); ++it)
		{
			if(poRes->GetFieldValue(it->first, &pVal, dwLen))
			{
				if(dwLen != 0)
				{
					bHasRole = true;
					break;
				}
			}
			else
			{
				LogError("Read '%s' column failed", it->second.c_str());
				return;
			}
		}
		if(!bHasRole)
		{
			if(poRes->GetFieldValue(oIndex2ColName.rbegin()->first + 3, &pVal, dwLen))
			{
				UINT32 serverId = convert<UINT32>(pVal);
				oDeleteList.push_back(std::make_pair(strOpenId, serverId));
			}
			else
			{
				LogError("Read 'serverid' column failed");
				return;
			}
		}
	}

	poRes->Release();
	for(std::vector<std::string>::size_type i = 0; i < oDeleteList.size(); ++i)
	{
		std::stringstream ss;
		std::string& strOpenId = oDeleteList[i].first;
		UINT32 serverId = oDeleteList[i].second;
		ss << "delete from account where _id='" << strOpenId << "' and serverid=" << serverId << ";";
		poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL);
		LogInfo("Delete empty account [%s] serverid[%u] from db", strOpenId.c_str(), serverId);
	}

	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = (qwEnd >= qwBegin ? qwEnd - qwBegin : 0);
	LogInfo("Delete all empty account from db use time: %llu ms", qwTimeUsed);
}

void CMysqlMgr::UpdateAccountServerId(IMysqlConn* poMysqlConn)
{
	std::string sql = "update account set serverid=" + ToString(DBConfig::Instance()->GetServerID()) + " where serverid=0";
	UINT32 dwAffectRows = 0;
	if (!poMysqlConn->Query(sql.c_str(), sql.size(), &dwAffectRows))
	{
		LogError("Update Account ServerId Failed");	
		return;
	}
	if (dwAffectRows)
	{
		LogInfo("Update Account ServerId Num[%u]", dwAffectRows);
	}
}

void CMysqlMgr::UpdateRankListServerId(IMysqlConn* poMysqlConn)
{
	std::string sql = "update ranklist set serverid=" + ToString(DBConfig::Instance()->GetServerID()) + " where serverid=0";
	UINT32 dwAffectRows = 0;
	if (!poMysqlConn->Query(sql.c_str(), sql.size(), &dwAffectRows))
	{
		LogError("Update ranklist serverid Failed");
		return;
	}
	if (dwAffectRows)
	{
		LogInfo("Update ranklist serverid Num [%u]", dwAffectRows);
	}
}

void CMysqlMgr::SetMSSaveDBEnd(bool isEnd)
{
	m_bMSSaveDBEnd = isEnd;
}

void CMysqlMgr::UpdateMysqlStatus()
{
	static UINT64 sqwLastTime = 0;
	UINT64 qwNowTime = TimeUtil::GetMilliSecond();

	if(qwNowTime < sqwLastTime + 1000) return;

	sqwLastTime = qwNowTime;
	MutexGuard guard(&m_oMysqlEvtMux);
	for(std::vector<INT32>::size_type i = 0; i < m_oMysqlEvtList.size(); ++i)
	{
		EMysqlConnEvent nEvent = (EMysqlConnEvent)m_oMysqlEvtList[i];

		if(m_bMysqlOK)
		{
			switch(nEvent)
			{
			case MYSQL_CONN_LOST_EVENT:
				{
					m_bMysqlOK = false;
					LogError("!!!!!!! MysqlError: Mysql Connection Lost");
				}
				break;

			default:
				break;
			}
		}
		else
		{
			switch(nEvent)
			{
			case MYSQL_RECONNECT_TRY_MAX_EVENT:
				{
					if(!m_bKickNtfSended)
					{
						m_bKickNtfSended = true;
						LogFatal("!!!!!! MysqlError: Reconnect to mysql with max try, send kickout role protocol to control server");
						PtcD2N_MysqlConnectLost oPtc;
						ControlLink::Instance()->SendTo(oPtc);
					}
				}
				break;

			case MYSQL_RECONNECT_SUCCESS_EVENT:
				{
					m_bMysqlOK = true;
					m_bKickNtfSended = false;
					LogInfo("!!!!!! MysqlInfo: Reconnect to mysql successful");
				}
				break;

			default:
				break;
			}
		}
	}
	m_oMysqlEvtList.clear();
}

void CMysqlMgr::MysqlEventCallBack(EMysqlConnEvent nEvent)
{
	MutexGuard guard(&CMysqlMgr::Instance()->m_oMysqlEvtMux);
	CMysqlMgr::Instance()->m_oMysqlEvtList.push_back(nEvent);
}

UINT32 GetRoleThreadIndex(UINT64 qwRoleID)
{
	UINT32 dwTotal = CMysqlMgr::Instance()->GetThreadNum();
	if(dwTotal < 2)
	{
		return 0;
	}

	if(dwTotal == 2)
	{
		return 1;
	}

	return 2 + (qwRoleID % (dwTotal - 2));
}

UINT32 GetAccountThreadIndex()
{
	UINT32 dwTotal = CMysqlMgr::Instance()->GetThreadNum();
	if(dwTotal < 2)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
