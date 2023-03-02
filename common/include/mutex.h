#ifndef __MUTEX_H__
#define __MUTEX_H__

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

class Mutex
{
public:
	Mutex();
	~Mutex();

	void lock();
	void unlock();

private:

#ifdef WIN32
	CRITICAL_SECTION m_mutex;
#else
	pthread_mutex_t m_mutex;
#endif
};

class MutexGuard
{
private: 
	Mutex *m_mutex;

public:
	MutexGuard(Mutex *mutex)
	{
		mutex->lock();
		m_mutex = mutex;
	}

	~MutexGuard()
	{
		m_mutex->unlock();
	}
};

#endif