#include "pch.h"
#include "process.h"
#include "timeutil.h"
#ifdef WIN32
#include "minidump.h"
#endif
#ifndef WIN32
#include <google/profiler.h>
#endif
#include "cmdline.h"

#include "audio/audioMgr.h"

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
#endif

	if(!Log_Init("audioserver", argv[1])) return -1;
	LogInfo("[%s] is start ....", argv[0]);

	signal(SIGINT, signal_handler);
	//signal(SIGTERM, signal_handler);

#ifdef WIN32
	SetUnhandledExceptionFilter(TopLevelFilter);
#endif

	//AudioMgr::Instance()->Init();
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
		TimeUtil::Sleep(1);
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
