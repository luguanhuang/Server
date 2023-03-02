#ifndef _Thread_Lock_H__
#define _Thread_Lock_H__


class CThreadLock
{
public: 
	CThreadLock()
	{
		InitializeCriticalSection(&m_cs);
	}

	~CThreadLock()
	{
		DeleteCriticalSection(&m_cs);
	}

	bool TryLock()
	{
		return TryEnterCriticalSection(&m_cs) != 0;
	}

	void Lock()
	{
		EnterCriticalSection(&m_cs);
	}

	void UnLock()
	{
		LeaveCriticalSection(&m_cs);
	} 

private:
	CRITICAL_SECTION	m_cs;
};


class CAutoLock
{
public:
	CAutoLock(CThreadLock& roLock)
		:m_roLock(roLock)
	{
		m_roLock.Lock(); 
	}

	~CAutoLock()
	{
		m_roLock.UnLock();
	}

private:
	CThreadLock&	m_roLock;
};

#endif