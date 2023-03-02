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
	//ProfilerStart("gate.prof");
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

	std::string strLog("gateserver_");
	strLog.append(strLine);
	if(!Log_Init(strLog.c_str(), argv[1])) return -1;
	LogInfo("[%s] is start ....", argv[0]);

	signal(SIGINT, signal_handler);
	//signal(SIGTERM, signal_handler);

#ifdef WIN32
	SetUnhandledExceptionFilter(TopLevelFilter);
	SetConsoleOutputCP(CP_UTF8);
#endif

	if (!Process_Setup(argv[1]))
	{
		LogError("process setup failed!\n");
		return -1;
	}

	bool Runing = true;
	while (Runing)
	{
		Runing = Process_Update();
		TimeUtil::Sleep(2);
	}

	Process_Cleanup();

	LogInfo("[%s] is stopped ....", argv[0]);
	Log_Uninit();

#ifndef WIN32
	//ProfilerStop();
#endif
	return 0;
}
