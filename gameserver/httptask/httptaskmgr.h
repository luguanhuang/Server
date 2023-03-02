#ifndef __HTTPTASK_MGR_H__
#define __HTTPTASK_MGR_H__

#include "WebFetchThread.h"
#include "util/utilsingleton.h"

class HttpTaskMgr : public Singleton<HttpTaskMgr>
{
public:
	bool Init();
	void Uninit();
	void Update();
	void AddTask(IWebFetchTask* task);

private:
	WebFetchThreadPool m_webThread;
};

#endif
