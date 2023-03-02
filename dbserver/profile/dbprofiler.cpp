#include "pch.h"
#include "dbprofiler.h"
#include "db/mysqlmgr.h"


INSTANCE_SINGLETON(CDbProfiler)


CDbProfiler::CDbProfiler()
{
}

CDbProfiler::~CDbProfiler()
{
}

bool CDbProfiler::Init()
{
	StartTimer();
	SetFileName("dbserver");
	return true;
}

void CDbProfiler::Uninit()
{
	StopTimer();
}

void CDbProfiler::DoProfile(FILE* fp)
{
	UINT32 dwThreadNum = CMysqlMgr::Instance()->GetThreadNum();
	if(dwThreadNum == 0) return;

	for(UINT32 i = 0; i < dwThreadNum; ++i)
	{
		CMysqlThread* poThread = CMysqlMgr::Instance()->GetThread(i);
		if(poThread == NULL) continue;

		fprintf(fp, "db thread %u: TaskProcessed: %u, MaxTaskNumInQueue: %u\n", i, poThread->GetProcessedTask(), poThread->GetMaxTaskInQueue());

		poThread->SetProcessedTask(0);
		poThread->SetMaxTaskInQueue(0);
	}
}
