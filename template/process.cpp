#include "pch.h"
#include "process.h"
#include "config.h"
#include "ptcregister.h"
#include "httpservicemgr.h"

static bool IsProcessRunning = true;


bool Process_Setup(const char *configfile)
{
	SINGLETON_CREATE_INIT_ARG1(GMConfig, configfile)
	SINGLETON_CREATE_INIT(HttpServiceMgr);

	CProtocolRegister::Regiter();

	return true;
}

void Process_Cleanup()
{
	SINGLETON_DESTORY_UNINIT(HttpServiceMgr);
	SINGLETON_DESTORY_UNINIT(GMConfig);
}

bool Process_Update()
{
	GMConfig::Instance()->ProcessNetMessage();
	HttpServiceMgr::Instance()->Update();
	return IsProcessRunning;
}

void Process_Stop()
{
	IsProcessRunning = false;
}

