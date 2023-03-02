#include "pch.h"
#include "httptaskmgr.h"

bool HttpTaskMgr::Init()
{ 
	// 两个线程
	m_webThread.Start(2); 
	return true; 
}

void HttpTaskMgr::Uninit()
{
	m_webThread.Terminate();
}

void HttpTaskMgr::Update()
{ 
	m_webThread.FinishTask(); 
}

void HttpTaskMgr::AddTask(IWebFetchTask* task)
{ 
	m_webThread.AddTask(task); 
}