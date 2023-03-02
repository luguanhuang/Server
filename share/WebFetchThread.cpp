#include "pch.h"
#include "WebFetchThread.h"
#include "timeutil.h"
#include "util/atomic.h"

#define REAL_HTTP_VERYFIY
#define MAX_RUNNING_TASK 100

const static UINT32 kPipeLen = 1024 * 1024 *  sizeof(void*);

WebFetchThread::WebFetchThread(bool init)
:m_init(init)
{
	m_addTaskCount = 0;
	m_taskPipe.Init(kPipeLen);
	m_finishPipe.Init(kPipeLen);
}

WebFetchThread::~WebFetchThread()
{

}

static size_t DataCB(void *buffer, size_t size, size_t nmemb, void *userp)
{
	std::string &s = *(std::string *)userp;
	s.append((char *)buffer, size*nmemb);
	return size * nmemb;
}

void WebFetchThread::Run()
{
	CURLM *handler;

	int still_running = 0;
	if (m_init)
	{
		curl_global_init(CURL_GLOBAL_ALL);
	}
	handler = curl_multi_init();

	while (true)
	{
#ifdef REAL_HTTP_VERYFIY
		if (still_running < MAX_RUNNING_TASK)
		{
			IWebFetchTask *pTask = PumpTask();
			if (pTask != NULL)
			{
				TaskStub *pStub = new TaskStub;
				pStub->pTask = pTask;
				pStub->startTm = TimeUtil::GetMilliSecond();
				CURL *curl = curl_easy_init();
				curl_easy_setopt(curl, CURLOPT_URL, pTask->GetUrl().c_str());
				if (pTask->GetType())
				{
					curl_easy_setopt(curl, CURLOPT_POST, 1); 
					curl_easy_setopt(curl, CURLOPT_POSTFIELDS, pTask->GetHttpPostString().c_str());
				}

				if (!pTask->GetHttpCookie().empty())
				{
					curl_easy_setopt(curl, CURLOPT_COOKIE, pTask->GetHttpCookie().c_str());
				}

				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, DataCB);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, &pStub->buffer);
				curl_easy_setopt(curl, CURLOPT_PRIVATE, pStub);
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
				// 设置超时会抛出信号，必须禁止掉
				curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
				curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, pTask->GetTimeOut());

				if (pTask->Verbose())
				{
					curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
				}

				curl_multi_add_handle(handler, curl);
			}
		}
		
		curl_multi_perform(handler, &still_running);
		
		int q;
		CURLMsg *msg;
		while ((msg = curl_multi_info_read(handler, &q)))
		{
			TaskStub *pStub;
			CURL *curl = msg->easy_handle;
			curl_easy_getinfo(curl, CURLINFO_PRIVATE, &pStub);

			if (msg->msg == CURLMSG_DONE)
			{
				pStub->success = true;
			}
			else
			{
				pStub->success = false;
			}
			pStub->curlCode = msg->data.result;
			if (pStub->curlCode == CURLE_OK)
			{
				pStub->success = true;
			}
			else
			{
				pStub->success = false;
			}

			curl_multi_remove_handle(handler, curl);
			curl_easy_cleanup(curl);

			AddFinishTask(pStub);
		}
#else
		IWebFetchTask *pTask = PumpTask();
		if (pTask != NULL)
		{
			TaskStub *pStub = new TaskStub;
			pStub->pTask = pTask;
			pStub->success = true;
			pStub->buffer = "{\"status\":0,\"code\":0,\"data\":{\"userid\":\"123\"}}";
			AddFinishTask(pStub);
		}
#endif
		// 没有新的task 或者 处理中的请求数比较多， sleep一段时间
		if (m_addTaskCount == 0 || still_running >= MAX_RUNNING_TASK)
		{
			TimeUtil::Sleep(5);
		}


		if (m_isCancel == true)
		{
			break;
		}
	}

	curl_multi_cleanup(handler);
	if (m_init)
	{
		curl_global_cleanup();
	}
}

bool WebFetchThread::AddTask(IWebFetchTask *pTask)
{
	bool success = m_taskPipe.Write((char*)(&pTask), sizeof(pTask));
	if (success)
	{
		atomic_increase(&m_addTaskCount);
	}
	else 
	{
		SSError << "add Task error" << END;
	}
	return success;
}

IWebFetchTask *WebFetchThread::PumpTask()
{
	IWebFetchTask* pTask = NULL;
	if (m_taskPipe.Read((char*)(&pTask), sizeof(pTask)))
	{
		atomic_decrease(&m_addTaskCount);
		return pTask;
	}
	return NULL;
}

void WebFetchThread::FinishTask()
{
	TaskStub* pStub = NULL;
	while (m_finishPipe.Read((char*)(&pStub), sizeof(pStub)))
	{
		UINT64 endTm = TimeUtil::GetMilliSecond();
		UINT64 useTm = endTm - pStub->startTm;
		if (useTm > pStub->pTask->GetTimeOut())
		{
			pStub->pTask->SetTimeOutStatus();
			SSError << "use time: " << useTm << ", Request Url: " <<pStub->pTask->GetUrl()<<END;
		}
		pStub->Perform();
		pStub->Release();
	}
}

void WebFetchThread::AddFinishTask(TaskStub *pStub)
{
	m_finishPipe.Write((char*)(&pStub), sizeof(pStub));
}

void WebFetchThread::TaskStub::Perform()
{
	if (success == false)
	{
		if (curlCode != CURLE_OK)
		{
			LogWarn("Curl error: %d, Request Url: %s", curlCode, pTask->GetUrl().c_str());
		}
		pTask->OnFailed();
	}
	else
	{
		pTask->OnResult(buffer);
	}
}

void WebFetchThread::TaskStub::Release()
{
	delete pTask;
	delete this;
}

WebFetchThreadPool::WebFetchThreadPool()
:m_curIndex(0)
,m_started(false)
,m_ended(false)
{

}

WebFetchThreadPool::~WebFetchThreadPool()
{
	Terminate();
}

bool WebFetchThreadPool::Start(UINT32 threadNum)
{
	if (m_started)
	{
		LogError("Already started!");
		return false;
	}
	if (threadNum == 0)
	{
		LogError("Thread num can't be zero");
		return false;
	}

	if (threadNum > 10)
	{
		threadNum = 10;
	}

	curl_global_init(CURL_GLOBAL_ALL);
	for (UINT32 i = 0; i < threadNum; ++i)
	{
		WebFetchThread* thread = new WebFetchThread(false);
		thread->Start();
		m_threads.push_back(thread);
	}

	m_started = true;
	return true;
}

void WebFetchThreadPool::Terminate()
{
	if (m_ended)
	{
		return;
	}

	for (auto it = m_threads.begin(); it != m_threads.end(); ++it)
	{
		WebFetchThread* thread = *it;
		thread->Cancel();
		thread->Join();
		delete thread;
	}
	m_threads.clear();
	curl_global_cleanup();

	m_ended = true;
}

void WebFetchThreadPool::FinishTask()
{
	for (auto it = m_threads.begin(); it != m_threads.end(); ++it)
	{
		WebFetchThread* thread = *it;
		thread->FinishTask();
	}
}

bool WebFetchThreadPool::AddTask(IWebFetchTask *pTask)
{
	UINT32 index = (m_curIndex++) % m_threads.size();
	return m_threads[index]->AddTask(pTask);
}

UINT32 WebFetchThreadPool::GetTaskNum()
{
	UINT32 num = 0;
	for (size_t i = 0; i < m_threads.size(); ++i)	
	{
		num += m_threads[i]->GetTaskNum();
	}
	return num;
}

