#include "pch.h"
#include "process.h"
#include "config.h"
#include "appid.h"
#include "ptcregister.h"
#include "verify/tokenverifymgr.h"
#include "gatemgr/gatemgr.h"
#include "gmmgr/gmmgr.h"
#include "sql/mysqlmgr.h"
#include "iplist/whitelistmgr.h"
#include "gmmgr/reloadmgr.h"
#include "verify/logincontrol.h"
#include "connection/connectionmgr.h"
#include "notice/platnoticemgr.h"
#include "sql/mysqltest.h"
#include "profile/loginprofile.h"
#include "iplist/accountbanmgr.h"
#include "serverrole/readrolecontrol.h"

static bool IsProcessRunning = true;

bool Process_Setup(const char *configfile)
{
	CProtocolRegister::Regiter();

	SINGLETON_CREATE_INIT_ARG1(LoginConfig, configfile);
	SINGLETON_CREATE_INIT(CAppConfig);
	SINGLETON_CREATE_INIT(MysqlMgr);
	SINGLETON_CREATE_INIT(GateMgr);
	SINGLETON_CREATE_INIT(GMMgr);
	SINGLETON_CREATE_INIT(WhiteListMgr);
	SINGLETON_CREATE_INIT(CReloadMgr);
	SINGLETON_CREATE_INIT(CLoginControl);
	SINGLETON_CREATE_INIT(ConnectionMgr);
	SINGLETON_CREATE_INIT(CPlatNoticeMgr);
	SINGLETON_CREATE_INIT(CMysqlTest);
	SINGLETON_CREATE_INIT(CLoginProfiler);
	SINGLETON_CREATE_INIT(TokenVerifyMgr);
	SINGLETON_CREATE_INIT(AccountBanMgr);
	SINGLETON_CREATE_INIT(ReadRoleControl);

	CmdLine::Setup();

	return true;
}

void Process_Cleanup()
{
	CmdLine::Stop();
	SINGLETON_DESTORY_UNINIT(AccountBanMgr);
	SINGLETON_DESTORY_UNINIT(TokenVerifyMgr);
	SINGLETON_DESTORY_UNINIT(CLoginProfiler);
	SINGLETON_DESTORY_UNINIT(CMysqlTest);
	SINGLETON_DESTORY_UNINIT(CPlatNoticeMgr);
	SINGLETON_DESTORY_UNINIT(ConnectionMgr);
	SINGLETON_DESTORY_UNINIT(CLoginControl);
	SINGLETON_DESTORY_UNINIT(CReloadMgr);
	SINGLETON_DESTORY_UNINIT(WhiteListMgr);
	SINGLETON_DESTORY_UNINIT(GMMgr);
	SINGLETON_DESTORY_UNINIT(GateMgr);
	SINGLETON_DESTORY_UNINIT(MysqlMgr);
	SINGLETON_DESTORY_UNINIT(CAppConfig);
	SINGLETON_DESTORY_UNINIT(LoginConfig);
}

bool Process_Update()
{
	CmdLine::Run();

	LoginConfig::Instance()->ProcessNetMessage();
	TokenVerifyMgr::Instance()->Update();
	return IsProcessRunning;
}

void Process_Stop()
{
	IsProcessRunning = false;
}

