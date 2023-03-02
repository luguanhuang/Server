#ifndef __PFLOGINTHREAD_H__
#define __PFLOGINTHREAD_H__

#include <list>
#include <string>

#include <curl/curl.h>

#include "mutex.h"
#include "threads.h"
#include "mempipe.h"


class IWebFetchTask
{
public:
	IWebFetchTask() { m_verbose = false; m_post = false; m_iTimeOut=5000; m_uPollID=0; m_bIsTimeOut=false;}
	virtual ~IWebFetchTask(){}
	virtual const std::string GetUrl() = 0;
	virtual void OnResult(const std::string &response) = 0;
	virtual void OnFailed() = 0;
	
	bool Verbose() { return m_verbose; }
	bool GetType() { return m_post; }
	void SetType(bool bType) { m_post = bType; }
	void SetTimeOut(INT32 iTime) { m_iTimeOut = iTime;}
	INT32 GetTimeOut() { return m_iTimeOut; }
	void SetTimeOutStatus(){ m_bIsTimeOut = true;}
	bool GetTimeOutStatus(){ return m_bIsTimeOut;}
	const std::string& GetHttpPostString() { return m_strPostString; }
	void SetHttpPostString(std::string str) { m_strPostString = str; }
	void SetPollID(UINT32 uID) { m_uPollID = uID;}
	UINT32 GetPollID() { return m_uPollID; }
	void SetHttpCookie(std::string str) { m_strCookie = str; }
	const std::string& GetHttpCookie() { return m_strCookie; }
protected:
	bool m_verbose;
	bool m_post;//false:get true:post  默认为get
	std::string m_strPostString;
	std::string m_strUrl;
	INT32 m_iTimeOut;//超时的毫秒数;
	UINT32 m_uPollID;//轮询ID;
	std::string m_strCookie;
	bool m_bIsTimeOut;
};

class WebFetchThread : public Thread
{
	struct TaskStub
	{
		IWebFetchTask *pTask;
		std::string buffer;
		bool success;
		CURLcode curlCode;
		UINT64 startTm;

		TaskStub()
		{
			pTask = NULL;
			success = false;
		}

		void Perform();
		void Release();
	};

public:
	WebFetchThread(bool init = true);
	~WebFetchThread();

	virtual void Run();
	
	bool AddTask(IWebFetchTask *pTask);
	void FinishTask();
	UINT32 GetTaskNum() { return (UINT32)m_addTaskCount; }

private:
	void AddFinishTask(TaskStub *pStub);
	IWebFetchTask *PumpTask();

	bool m_init;
	UINT64 m_addTaskCount;

	CMemPipe	m_taskPipe;
	CMemPipe	m_finishPipe;
};

class WebFetchThreadPool
{
public:
	WebFetchThreadPool();
	~WebFetchThreadPool();

	bool Start(UINT32 threadNum = 1);
	void Terminate();

	bool AddTask(IWebFetchTask *pTask);
	void FinishTask();

	UINT32 GetTaskNum();

private:
	UINT32 m_curIndex;
	bool m_started;
	bool m_ended;
	std::vector<WebFetchThread*> m_threads;
};

#endif // __PFLOGINTHREAD_H__