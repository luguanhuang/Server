#include "pch.h"
#include "timeutil.h"
#include "dbprocess.h"
#include "cmdline.h"

#ifdef WIN32
#include "minidump.h"
#endif
#ifndef WIN32
#include <google/profiler.h>
#endif

#ifdef WIN32
BOOL WINAPI HandlerRoutine(_In_ DWORD dwCtrlType)
{
	if (dwCtrlType == CTRL_CLOSE_EVENT || dwCtrlType == CTRL_C_EVENT)
	{
		Process_Stop();
		TimeUtil::Sleep(1000000000);
		return TRUE;
	}

	return FALSE;
}
#endif

void signal_handler(int sig)
{
	Process_Stop();
}

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		printf("require config.xml parameter\n");
		return -1;
	}
#ifndef WIN32
	//ProfilerStart("db.prof");
	if (getenv("daemon") != NULL && *getenv("daemon") != 0)
	{
		daemon(1,0);
	}
	signal(SIGPIPE, SIG_IGN);
#endif

	if(!Log_Init("dbserver", argv[1])) return -1;
	LogInfo("[%s] is start ....", argv[0]);

	signal(SIGINT, signal_handler);
	//signal(SIGTERM, signal_handler);

#ifdef WIN32
	SetConsoleCtrlHandler(HandlerRoutine, TRUE);
	SetUnhandledExceptionFilter(TopLevelFilter);
	SetConsoleOutputCP(CP_UTF8);
#endif

	if (!Process_Setup(argv[1]))
	{
		return -1;
	}

	UINT64 start, end;
	bool Runing = true;
	while (Runing)
	{
		start = TimeUtil::GetMilliSecond();
		Runing = Process_Update();
		end = TimeUtil::GetMilliSecond();
		if (end < start + 5)
		{
			TimeUtil::Sleep((UINT32)(start + 5 - end));
		}
	}

	Process_Cleanup();

	LogInfo("[%s] is stopped ....", argv[0]);
	Log_Uninit();

#ifndef WIN32
	//ProfilerStop();
#endif
	return 0;
}
