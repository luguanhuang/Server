#ifndef _H_MySql_Thread_H__
#define _H_MySql_Thread_H__

#include "imysql.h"
#include "threads.h"
#include "mempipe.h"
#include "mysqltask.h"
#include "config.h"


class CMysqlThread : public Thread
{
public:
	CMysqlThread();
	~CMysqlThread();

	bool Init(const DBInfo& dbInfo);
	void Uninit();

	virtual void Run();

	void ProcessReply();
	void PushTask(CMysqlTask* poTask); 

	void ShowThreadInfo();
	IMysqlConn* GetMysqlConn() { return m_poMysqlConn; }

	UINT32 GetProcessedTask() const { return m_ProcessedTask; }
	UINT32 GetMaxTaskInQueue() const { return m_MaxTaskInQueue; }
	void SetProcessedTask(UINT32 dwNum) { m_ProcessedTask = dwNum; }
	void SetMaxTaskInQueue(UINT32 dwNum) { m_MaxTaskInQueue = dwNum; }
private:
	void PushReply(CMysqlTask* poTask);
	bool PopTask(CMysqlTask** ppoTask); 
	bool PopReply(CMysqlTask** poTask);

private:
	UINT32		m_UseTotalTime;
	UINT32		m_ProcessedTask;
	UINT32		m_TaskInQueue;
	UINT32		m_MaxTaskInQueue;
	CMemPipe	m_oReqPipe;
	CMemPipe	m_oReplyPipe;
	IMysqlConn*	m_poMysqlConn;

	std::list<CMysqlTask*>	m_oTempList;
};

#endif

