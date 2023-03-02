#ifndef __SQLTHREAD_H__
#define __SQLTHREAD_H__

#include "mutex.h"
#include "threads.h"
#include "timeutil.h"
#include "SQLTask.h"
#include "config.h"

struct DBInfo;
class SQLThread : public Thread
{
public:
	SQLThread();
	~SQLThread();

	bool Init(const DBInfo &dbInfo);
	void Uninit();

	void Run();
	void AddTask(ISQLTask *task);
	void FinishedTaskList();
	void RunInitScript();
	UINT32 GetTaskNum() { return m_newTaskCount; }

private:
	void AddFinishedTask(ISQLTask *task);
	ISQLTask *PumpTask();

	volatile int m_finishedTask;
	std::list<ISQLTask *> m_toDoList;
	std::list<ISQLTask *> m_finishList;
	std::list<ISQLTask*>  m_finishedTaskList;
	Mutex m_toDolistMux;
	Mutex m_finishListMux;
	MYSQL *m_sql;
	bool   m_isStop;
	volatile int m_newTaskCount;
};


#endif // __SQLTHREAD_H__
