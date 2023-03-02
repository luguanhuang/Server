#ifndef __SVTHREAD_H__
#define __SVTHREAD_H__

#ifdef WIN32
#include <windows.h>
typedef HANDLE thread_type;
#define ThreadFunSignature(Fun) DWORD WINAPI Fun(LPVOID param)
#else
#include <pthread.h>
typedef pthread_t thread_type;
#define ThreadFunSignature(Fun) void *Fun(void *param)
#endif

class Runable
{
public:
	virtual ~Runable(){}
	virtual void Run() = 0;
};

class Thread : public Runable
{
public:
	Thread();
	Thread(Runable *task);
	virtual ~Thread();

	virtual void Run();
	void Start();
	void Kill();
	void Cancel();
	void Join();

protected:
	thread_type   m_threadID;
	Runable      *m_task;
	volatile bool m_isCancel;
};

#endif