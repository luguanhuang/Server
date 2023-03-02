#include "pch.h"
#include "process.h"
#include "config.h"
#include "ptcregister.h"
#include "account/logincontrol.h"
#include "account/accountsessionmgr.h"
#include "role/rolemanager.h"
#include "scene/gsmanager.h"
#include "scene/scenemanager.h"
#include "scene/sceneconfig.h"
#include "account/whitelistmgr.h"
#include "queuing/queuingmgr.h"
#include "account/accountkick.h"
#include "shm/shmmgr.h"
#include "table/globalconfig.h"
#include "profile/ctrlprofiler.h"

static bool IsProcessRunning = true;


bool Process_Setup(const char *configfile)
{
	SINGLETON_CREATE_INIT_ARG1(ControlConfig, configfile)

	// 共享内存先初始化， RoleManager要在AccountSessionMgr之前
	SINGLETON_CREATE_INIT(ShmMgr);
	SINGLETON_CREATE_INIT(CGsManager);
	SINGLETON_CREATE_INIT(CLoginControl);
	SINGLETON_CREATE_INIT(CAccountKicker);
	SINGLETON_CREATE_INIT(CWhiteListMgr);
	SINGLETON_CREATE_INIT(CSceneConfig);
	SINGLETON_CREATE_INIT(QueuingMgr);

	GetGlobalConfig().LoadFile();

	SINGLETON_CREATE_INIT(CSceneManager);
	SINGLETON_CREATE_INIT(CRoleManager);
	SINGLETON_CREATE_INIT(CAccountSessionMgr);
	SINGLETON_CREATE_INIT(CControlProfiler)

	CProtocolRegister::Regiter();

	return true;
}

void Process_Cleanup()
{
	CAccountSessionMgr::Instance()->OnlineReport();

	SINGLETON_DESTORY_UNINIT(CControlProfiler)
	SINGLETON_DESTORY_UNINIT(CAccountSessionMgr);
	SINGLETON_DESTORY_UNINIT(CRoleManager);
	SINGLETON_DESTORY_UNINIT(CSceneManager);

	SINGLETON_DESTORY_UNINIT(QueuingMgr);
	SINGLETON_DESTORY_UNINIT(CSceneConfig);
	SINGLETON_DESTORY_UNINIT(CSceneManager);
	SINGLETON_DESTORY_UNINIT(CWhiteListMgr);
	SINGLETON_DESTORY_UNINIT(CAccountKicker);
	SINGLETON_DESTORY_UNINIT(CLoginControl);
	SINGLETON_DESTORY_UNINIT(CGsManager);
	SINGLETON_DESTORY_UNINIT(ShmMgr);

	SINGLETON_DESTORY_UNINIT(ControlConfig);
}

bool Process_Update()
{
	ControlConfig::Instance()->ProcessNetMessage();
	return IsProcessRunning;
}

void Process_Stop()
{
	IsProcessRunning = false;
}

