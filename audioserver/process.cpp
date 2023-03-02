#include "pch.h"
#include "process.h"
#include "config.h"
#include "ptcregister.h"
#include "audio/audioMgr.h"


static bool IsProcessRunning = true;

bool Process_Setup(const char *configfile)
{
	SINGLETON_CREATE_INIT_ARG1(AudioConfig, configfile);
	SINGLETON_CREATE_INIT(AudioMgr);

	CProtocolRegister::Regiter();
	return true;
}

void Process_Cleanup()
{
	SINGLETON_DESTORY_UNINIT(AudioMgr);
	SINGLETON_DESTORY_UNINIT(AudioConfig);
}

bool Process_Update()
{
	AudioConfig::Instance()->ProcessNetMessage();
	AudioMgr::Instance()->Update();
	return IsProcessRunning;
}

void Process_Stop()
{
	IsProcessRunning = false;
}

