#include "pch.h"
#include "mysqlthread.h"
#include "timeutil.h"
#include "LogInit.h"
#include "util.h"
#include "timeutil.h"
#include "mysqlmgr.h"


static const UINT32	MysqlTask_Pipe_Len = 1024 * 1024 * sizeof(CMysqlTask*);


CMysqlThread::CMysqlThread()
{
	m_UseTotalTime = 0;
	m_ProcessedTask = 0;
	m_TaskInQueue = 0;
	m_MaxTaskInQueue = 0;
	m_poMysqlConn = NULL;
}

CMysqlThread::~CMysqlThread()
{
}

bool CMysqlThread::Init(const DBInfo &dbInfo)
{
	m_poMysqlConn = CreateMysqlConn();
	if(m_poMysqlConn == NULL)
	{
		LogError("Create mysql connection failed");
		return false;
	}

	if(!m_poMysqlConn->Connect(dbInfo.m_ip.c_str(), convert<UINT32>(dbInfo.m_port), dbInfo.m_user.c_str(), dbInfo.m_pswd.c_str(), dbInfo.m_database.c_str()))
	{
		LogError("Can't connect to database %s:%s:%s, ErrDesc: %s", dbInfo.m_ip.c_str(), dbInfo.m_port.c_str(), dbInfo.m_database.c_str(), m_poMysqlConn->GetError());
		return false;
	}

	LogInfo("Connect to %s:%s:%s succ.", dbInfo.m_ip.c_str(), dbInfo.m_port.c_str(), dbInfo.m_database.c_str());

	m_oReqPipe.Init(MysqlTask_Pipe_Len);
	m_oReplyPipe.Init(MysqlTask_Pipe_Len);
	return true;
}

void CMysqlThread::Uninit()
{
	if(m_poMysqlConn != NULL)
	{
		m_poMysqlConn->Close();
		m_poMysqlConn->Release();
		m_poMysqlConn = NULL;
	}
}

enum SQLThreadState
{
	TH_IDLE,
	TH_WORKING,
};

void CMysqlThread::Run()
{
	INT32 nBuffLen = 1024 * 1024 * 4;
	char* pcBuffer = new char[nBuffLen];

	UINT32 cancelCount = 0;
	while (true)
	{
		CMysqlTask* poTask = NULL;
		if(PopTask(&poTask))
		{
			UINT64 qwBegin = TimeUtil::GetMilliSecond();
			poTask->SetSqlBeginTime(qwBegin);
			poTask->Execute(m_poMysqlConn, pcBuffer, nBuffLen);
			UINT64 qwEnd = TimeUtil::GetMilliSecond();
			poTask->SetSqlEndTime(qwEnd);

			PushReply(poTask);
		}
		else
		{
//			if (m_isCancel == true)
			if (m_isCancel && CMysqlMgr::Instance()->GetMSSaveDBEnd())
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

			TimeUtil::Sleep(5);
		}
	}
	

	delete [] pcBuffer;
	LogDebug("thread [%x] stopped!", this);
}

void CMysqlThread::ProcessReply()
{
	CMysqlTask* poTask = NULL;
	UINT64 qwEndTime = TimeUtil::GetMilliSecond();
	while(PopReply(&poTask))
	{
		poTask->SetEndTime(qwEndTime);
		poTask->OnReply();

		if(poTask->GetTimeUsed() > 20 || poTask->GetSqlTimeUsed() > 10)
		{
			LogWarn("task %s finished, sql use %d(ms), total use %d(ms)", poTask->GetName().c_str(), poTask->GetSqlTimeUsed(), poTask->GetTimeUsed());
		}
		else
		{
			LogWarn("task %s finished, sql use %d(ms), total use %d(ms)", poTask->GetName().c_str(), poTask->GetSqlTimeUsed(), poTask->GetTimeUsed());
		}

		--m_TaskInQueue;
		++m_ProcessedTask;
		delete poTask;
	}

	while(!m_oTempList.empty())
	{
		poTask = m_oTempList.front();
		if(!m_oReqPipe.Write((char*)(&poTask), sizeof(poTask))) break;

		m_oTempList.pop_front();
	}
}

void CMysqlThread::PushTask(CMysqlTask* poTask)
{
	if(!m_oTempList.empty())
	{
		m_oTempList.push_back(poTask);
	}
	else
	{
		if(!m_oReqPipe.Write((char*)(&poTask), sizeof(poTask)))
		{
			m_oTempList.push_back(poTask);
		}
	}

	++m_TaskInQueue;
	if(m_TaskInQueue > m_MaxTaskInQueue)
	{
		m_MaxTaskInQueue = m_TaskInQueue;
	}
}

bool CMysqlThread::PopTask(CMysqlTask** ppoTask)
{
	if(ppoTask == NULL)
	{
		return false;
	}

	if(!m_oReqPipe.Read((char*)ppoTask, sizeof(CMysqlTask*)))
	{
		return false;
	}

	return true;
}

void CMysqlThread::PushReply(CMysqlTask* poTask)
{
	while(!m_oReplyPipe.Write((char*)(&poTask), sizeof(poTask)))
	{
		TimeUtil::Sleep(1);
	}
}

bool CMysqlThread::PopReply(CMysqlTask** ppoTask)
{
	if(ppoTask == NULL)
	{
		return false;
	}

	if(!m_oReplyPipe.Read((char*)ppoTask, sizeof(CMysqlTask*)))
	{
		return false;
	}

	return true;
}

void CMysqlThread::ShowThreadInfo()
{
	UINT32 avgTime = 0;
	if (m_ProcessedTask > 0)
	{
		avgTime = m_UseTotalTime/m_ProcessedTask;
	}

	LogWarn("thread[%x] task in queue: curr %u, processed %u, max %u, process agv time: %u", 
		this, m_TaskInQueue, m_ProcessedTask, m_MaxTaskInQueue, avgTime);
}