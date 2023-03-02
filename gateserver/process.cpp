#include "pch.h"
#include "process.h"
#include "ptcregister.h"
#include "gslink.h"
#include "session/sessionmanager.h"
#include "config.h"
#include "serverstateinfo.h"
#include "protocolstat.h"
#include "protocolcoder.h"
#include "profile/gateprofile.h"
#include "protocolban/crossprotocolbanmgr.h"

static bool IsProcessRunning = true;


bool Process_Setup(const char *file)
{
	SINGLETON_CREATE_INIT_ARG1(GateConfig, file)
	SINGLETON_CREATE_INIT(SessionManager);
	SINGLETON_CREATE_INIT(ServerStateInfo);
	SINGLETON_CREATE_INIT(CProtocolStat);
	SINGLETON_CREATE_INIT(CProtocolCoder);
	SINGLETON_CREATE_INIT(CGateProfile);
	SINGLETON_CREATE_INIT(CrossProtocolBanMgr);
	CProtocolRegister::Regiter();

	CmdLine::Setup();

	return true;
}

void Process_Cleanup()
{
	CmdLine::Stop();

	SINGLETON_DESTORY_UNINIT(CrossProtocolBanMgr);
	SINGLETON_DESTORY_UNINIT(CGateProfile);
	SINGLETON_DESTORY_UNINIT(CProtocolCoder);
	SINGLETON_DESTORY_UNINIT(CProtocolStat);
	SINGLETON_DESTORY_UNINIT(ServerStateInfo)
	SINGLETON_DESTORY_UNINIT(SessionManager)
	SINGLETON_DESTORY_UNINIT(GateConfig)
}

bool Process_Update()
{
	CmdLine::Run();
	GateConfig::Instance()->ProcessNetMessage();
	return IsProcessRunning;
}

void Process_Stop()
{
	IsProcessRunning = false;
}

