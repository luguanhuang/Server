#include "pch.h"
#include "process.h"
#include "config.h"
#include "connection/connectionmgr.h"
#include "cmdline.h"
#include "singleton.h"
#include "reload/reloadmgr.h"
#include "ptcregister.h"

static bool IsProcessRunning = true;

bool Process_Setup(const char *configfile)
{
	CProtocolRegister::Regiter();

	SINGLETON_CREATE_INIT_ARG1(VersionConfig, configfile);
	SINGLETON_CREATE_INIT(CReloadMgr);
	SINGLETON_CREATE_INIT(ConnectionMgr);

	CmdLine::Setup();

	return true;
}

void Process_Cleanup()
{
	CmdLine::Stop();

	SINGLETON_DESTORY_UNINIT(CReloadMgr);
	SINGLETON_DESTORY_UNINIT(ConnectionMgr);
}

bool Process_Update()
{
	CmdLine::Run();

	VersionConfig::Instance()->ProcessNetMessage();
	return IsProcessRunning;
}

void Process_Stop()
{
	IsProcessRunning = false;
}

