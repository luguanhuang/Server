#include "pch.h"
#include "SQLThread.h"
#include "timeutil.h"
#include "config.h"
#include "util.h"
#include <time.h>
#include "DBInfo.h"
typedef char		my_bool; /* Small bool */

#define MYSQL_ERROR(mysql) LogError("SQL failed: errno=%d : %s\n", mysql_errno(mysql), mysql_error(mysql))

SQLThread::SQLThread()
{
	m_sql = NULL;
	m_isStop = false;
	m_finishedTask = 0;
	m_newTaskCount = 0;
}

SQLThread::~SQLThread()
{
	Uninit();
}

bool SQLThread::Init(const DBInfo &dbInfo)
{
	m_sql = mysql_init(NULL);
	if(m_sql == NULL)
	{
		LogError("mysql_init failed");
		return false;
	}

	if (mysql_real_connect(m_sql,
		dbInfo.m_ip.c_str(),
		dbInfo.m_user.c_str(),
		dbInfo.m_pswd.c_str(),
		dbInfo.m_database.c_str(),
		convert<int>(dbInfo.m_port), 
		NULL, 0) == NULL)
	{
		MYSQL_ERROR(m_sql);
		mysql_close(m_sql);
		m_sql = NULL;

		LogWarn("connect to database %s:%s:%s failed", dbInfo.m_ip.c_str(), dbInfo.m_port.c_str(), dbInfo.m_database.c_str());
		return false;
	}
	else
	{
		// ���ݿ����ӶϿ��Զ�����
		my_bool reconnect = 1;
		mysql_options(m_sql, MYSQL_OPT_RECONNECT, &reconnect);
		mysql_set_character_set(m_sql, "utf8");
	}

	LogInfo("connect to database %s:%s:%s success", dbInfo.m_ip.c_str(), dbInfo.m_port.c_str(), dbInfo.m_database.c_str());
	return true;
}

void SQLThread::Uninit()
{
	if (m_sql != NULL)
	{
		mysql_close(m_sql);
		m_sql = NULL;
	}
}

ISQLTask *SQLThread::PumpTask()
{
	if (m_newTaskCount > 0)
	{
		MutexGuard guard(&m_toDolistMux);
		if (!m_toDoList.empty())
		{
			ISQLTask *pTask = m_toDoList.front();
			m_toDoList.pop_front();
			--m_newTaskCount;
			return pTask;
		}
		else
		{
			m_newTaskCount = 0;
		}
	}
	
	return NULL;
}

void SQLThread::Run()
{
	time_t lastQueryTime = time(NULL);
	UINT32 cancelCount = 0;
	while (true)
	{
		ISQLTask *pTask = PumpTask();
		if (pTask != NULL)
		{
			lastQueryTime = time(NULL);
			pTask->m_result = pTask->Execute(m_sql);
			AddFinishedTask(pTask);
		}
		else
		{
			if (m_isCancel == true)
			{
				TimeUtil::Sleep(1000);
				cancelCount++;
				if (cancelCount >= 3)
				{
					break;
				}
				else
				{
					continue;
				}
			}
			if (m_newTaskCount == 0)
			{
				TimeUtil::Sleep(20);
			}

			if (time(NULL) - lastQueryTime > 600)
			{
				lastQueryTime = time(NULL);
				int ret = mysql_ping(m_sql);
				if (ret != 0)
				{
					LogWarn("ping mysql failed with errno=[%d]", ret);
				}
			}
		}
	}
}

void SQLThread::AddTask(ISQLTask *task)
{
	MutexGuard guard(&m_toDolistMux);
	m_toDoList.push_back(task);
	++m_newTaskCount;
}

void SQLThread::FinishedTaskList()
{
	if (m_finishedTask != 0)
	{
		{
			MutexGuard guard(&m_finishListMux);
			m_finishedTaskList.swap(m_finishList);
			m_finishedTask = 0;
		}

		auto end = m_finishedTaskList.end();
		for (auto i = m_finishedTaskList.begin(); i != end; ++i)
		{
			ISQLTask *pTask = *i;
			pTask->FinishTask();
			delete pTask;
		}

		m_finishedTaskList.clear();
	}
}

void SQLThread::AddFinishedTask(ISQLTask *task)
{
	MutexGuard guard(&m_finishListMux);
	m_finishList.push_back(task);
	++m_finishedTask;
}

void SQLThread::RunInitScript()
{
	std::string dbscript = Config::GetConfig()->ReadFileContent("dbinit.sql");
	std::vector<std::string> sqls = Split(dbscript, ';');
	for (UINT32 i = 0; i < sqls.size(); ++i)
	{
		std::string s = Trim(sqls[i]);
		if (s.empty() || s[0] == '#' || s[0] == '-' || s[0] == '/')
			continue;

		if (mysql_real_query(m_sql, s.c_str(), s.size()) != 0)
		{
			LogWarn("run sql [%s] failed!", s.c_str());
		}
	}
}
