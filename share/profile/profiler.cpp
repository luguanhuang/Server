#include "pch.h"
#include "profiler.h"
#include "timermgr.h"
#include <time.h>


#define DO_PROFILE_INTERVAL (5 * 60)


CProfiler::CProfiler()
:m_hCheckTimer(INVALID_HTIMER)
{
}

CProfiler::~CProfiler()
{
}

void CProfiler::StartTimer()
{
	StopTimer();
	m_hCheckTimer = CTimerMgr::Instance()->SetTimer(this, 1, DO_PROFILE_INTERVAL * 1000, -1, __FILE__, __LINE__);
}

void CProfiler::StopTimer()
{
	if(m_hCheckTimer != INVALID_HTIMER)
	{
		CTimerMgr::Instance()->KillTimer(m_hCheckTimer);
		m_hCheckTimer = INVALID_HTIMER;
	}
}

void CProfiler::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	time_t tNow = time(NULL);
	tm* pt = localtime(&tNow);
	char buffer1[128] = {0}; 
	char buffer2[128] = {0}; 
	strftime(buffer1, 64, "%Y-%m-%d", pt);
	strftime(buffer2, 64, "%Y-%m-%d %H:%M", pt);

	std::stringstream ss;
	ss << "log/" << GetFileName() << "_" << buffer1 << ".profile";
	FILE *fp = fopen(ss.str().c_str(), "a");
	if(fp == NULL)
	{
		LogError("Create file %s failed", ss.str().c_str());
		return;
	}

	fprintf(fp, "\n\n%s\n", buffer2);
	DoProfile(fp);
	fclose(fp);
}