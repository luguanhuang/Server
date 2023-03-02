#include "pch.h"
#include "process.h"
#include "timeutil.h"
#include "cmdline.h"
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
	signal(SIGPIPE, SIG_IGN);
#endif

	if(!Log_Init("controlserver", argv[1])) return -1;
	LogInfo("[%s] is start ....", argv[0]);

	signal(SIGINT, signal_handler);
	//signal(SIGTERM, signal_handler);

#ifdef WIN32
	SetUnhandledExceptionFilter(TopLevelFilter);
#endif
	CmdLine::Setup();
	if (!Process_Setup(argv[1]))
	{
		LogError("process setup failed!\n");
		return -1;
	}

	bool Runing = true;
	while (Runing)
	{
		Runing = Process_Update();
		CmdLine::Run();
		TimeUtil::Sleep(1);
	}

	CmdLine::Stop();
	Process_Cleanup();

	LogInfo("[%s] is stopped ....", argv[0]);
	Log_Uninit();

#ifndef WIN32
	//ProfilerStop();
#endif
	return 0;
}