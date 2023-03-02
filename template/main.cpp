#include "pch.h"
#include "process.h"
#include "timeutil.h"
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
#ifndef WIN32
	//ProfilerStart("gate.prof");
#endif

	Log_Init("gmserver");
	LogDebug("[%s] is start ....", argv[0]);

	signal(SIGINT, signal_handler);
	//signal(SIGTERM, signal_handler);

#ifdef WIN32
	SetUnhandledExceptionFilter(TopLevelFilter);
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
		TimeUtil::Sleep(1);
	}

	Process_Cleanup();

	LogDebug("[%s] is stopped ....", argv[0]);
	Log_Uninit();

#ifndef WIN32
	//ProfilerStop();
#endif
	return 0;
}
