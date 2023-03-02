#include "pch.h"
#include "process.h"
#include "timeutil.h"
#include "parseline.h"
#ifdef WIN32
#include "minidump.h"
#endif
#ifndef WIN32
#include <google/profiler.h>
#endif
#include "cmdline.h"

void signal_handler(int sig)
{
	Process_Stop();
}
 
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

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		printf("require config.xml parameter\n");
		return -1;
	}
#ifndef WIN32
	//ProfilerStart("gs.prof");
	if (getenv("daemon") != NULL && *getenv("daemon") != 0)
	{
		daemon(1,0);
	}
	signal(SIGPIPE, SIG_IGN);
#endif

	std::string strLine = ParseLineID(argv[1]);
	if(strLine.empty())
	{
		return -1;
	}
	bool cross = ParseCross(argv[1]);

	std::string strLog("gameserver_");
	if (cross)
	{
		strLog.append("cross_");
	}
	strLog.append(strLine);
	if(!Log_Init(strLog.c_str(), argv[1])) return -1;
	LogInfo("[%s] is start ....", argv[0]);

	signal(SIGINT, signal_handler);
	//signal(SIGTERM, signal_handler);

#ifdef WIN32
	SetConsoleCtrlHandler(HandlerRoutine, TRUE);
	//AddVectoredExceptionHandler(1, TopLevelFilter);
	//SetUnhandledExceptionFilter(TopLevelFilter);
	SetConsoleOutputCP(CP_UTF8);
#endif

	CmdLine::Setup();
	if (!Process_Setup(argv[1]))
	{
		LogError("process setup failed!\n");
		return -1;
	}

	TimeUtil::UpdateFrameTime();
	TimeUtil::SleepAtLeast(1);

	bool Runing = true;
	while (Runing)
	{
		TimeUtil::UpdateFrameTime();

		UINT64 qwBegin = TimeUtil::GetMilliSecond();
		Runing = Process_Update();
		UINT64 qwEnd = TimeUtil::GetMilliSecond();
		if(qwEnd - qwBegin > 50)
		{
			LogWarn("Slow frame, time %llu ms", qwEnd - qwBegin);
		}

		TimeUtil::SleepAtLeast(2);
		CmdLine::Run();
	}

	Process_Cleanup();
	CmdLine::Stop();

	LogInfo("[%s] is stopped ....", argv[0]);
	Log_Uninit();

#ifndef WIN32
	//ProfilerStop();
#endif
	return 0;
}
