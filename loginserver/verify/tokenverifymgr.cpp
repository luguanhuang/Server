#include "pch.h"
#include <time.h>
#include <stdlib.h>
#include "tokenverifymgr.h"
#include "config.h"
#include "util/XRandom.h"

INSTANCE_SINGLETON(TokenVerifyMgr)

TokenVerifyMgr::TokenVerifyMgr()
{
	srand((unsigned int)time(NULL));
	m_NextCheckTime = 0;
	m_sqlRandIndex = 0;
	m_httpTaskNum = 0;
	m_dbTaskNum = 0;
}

TokenVerifyMgr::~TokenVerifyMgr()
{

}

bool TokenVerifyMgr::Init()
{
	m_pfThread.Start(2);
	
	const DBInfo &db = LoginConfig::Instance()->GetDBInfo();
	for (int i = 0; i < SQLTHREAD_COUNT; ++i)
	{
		if (m_dbThread[i].Init(db))
		{
			m_dbThread[i].Start();
		}
		else
		{
			return false;
		}
	}
	
	return true;
}

void TokenVerifyMgr::Uninit()
{
	m_pfThread.Terminate();
	for (int i = 0; i < SQLTHREAD_COUNT; ++i)
	{
		m_dbThread[i].Cancel();
		m_dbThread[i].Join();
		m_dbThread[i].Uninit();
	}
}

UToken TokenVerifyMgr::CreateUUID(bool isgm)
{
	UToken uid;
	uid.data[0] = XRandom::randInt();
	uid.data[1] = XRandom::randInt();
	uid.data[2] = XRandom::randInt();
	uid.data[3] = XRandom::randInt();
	uid.data[4] = LoginConfig::Instance()->GetServerID();
	uid.data[5] = 0;
	if (isgm)
	{
		*(char*)(&uid.data[5]) = 1;
	}
	return uid;
}

void TokenVerifyMgr::AddToken(const UToken &token, const UserInfo& user)
{
	char buf[64];
	token.Format(buf, 64);
	LogInfo("add user [%s] with token [%s]", user.account.c_str(), buf);
	m_LoginTokenInfo.insert(std::make_pair(token, user));
}

void TokenVerifyMgr::Update()
{
	for (int i = 0; i < SQLTHREAD_COUNT; ++i)
	{
		m_dbThread[i].FinishedTaskList();
	}
	
	m_pfThread.FinishTask();

	char buf[64];
	time_t now = time(NULL);
	if (now > m_NextCheckTime)
	{
		for (auto i = m_LoginTokenInfo.begin(); i != m_LoginTokenInfo.end();)
		{
			UserInfo& pInfo = i->second;
			if (pInfo.isused || pInfo.tokenExpireTime < now)
			{
				i->first.Format(buf, 64);
				LogInfo("remove time expire or used token [%s] with userid [%s]", buf, pInfo.account.c_str());
				m_LoginTokenInfo.erase(i++);
			}
			else
			{
				++i;
			}
		}

		m_NextCheckTime = now + 10;
	}
}

UserInfo* TokenVerifyMgr::FindUserInfo(const UToken &token)
{
	auto it = m_LoginTokenInfo.find(token);
	return it == m_LoginTokenInfo.end() ? NULL : &it->second;
}

void TokenVerifyMgr::AddPFVerifyTask(IWebFetchTask *pTask)
{
	m_pfThread.AddTask(pTask);
	m_httpTaskNum++;
}

void TokenVerifyMgr::AddDBTask(ISQLTask *pTask)
{
	m_sqlRandIndex = (m_sqlRandIndex + 1)%SQLTHREAD_COUNT;
	m_dbThread[m_sqlRandIndex].AddTask(pTask);
	m_dbTaskNum++;
}