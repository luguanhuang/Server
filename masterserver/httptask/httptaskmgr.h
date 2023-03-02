#ifndef _HTTP_TASK_MGR_H_
#define _HTTP_TASK_MGR_H_

#include "WebFetchThread.h"

class HttpTaskMgr
{
	HttpTaskMgr();
	~HttpTaskMgr();
	DECLARE_SINGLETON(HttpTaskMgr)

public:
	bool Init();
	void Uninit();
	void Update();

	void AddTask(IWebFetchTask* task);

private:
	WebFetchThreadPool m_webThread;
};

#endif