#include "pch.h"
#include "dbprocess.h"
#include "ptcregister.h"
#include "cmdline.h"
#include "config.h"
#include "appid.h"
#include "debugflag.h"
#include "protostatistics.h"
#include "timer.h"
#include "db/mysqlmgr.h"
#include "profile/dbprofiler.h"

static bool IsProcessRunning = true;

class PrintProtoStatisticsTimer : public ITimer
{
public:
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount)
	{
		if(!ProtoStatistics::Instance()->IsEnable()) return;

		time_t tNow = time(NULL);
		tm* pt = localtime(&tNow);
		char buffer[64] = {0}; 
		strftime(buffer, 64, "%Y-%m-%d-%H-%M", pt);

		std::stringstream ss;
		ss << "log/dbserver_proto_" << buffer << ".html";
		FILE *fp = fopen(ss.str().c_str(), "w");
		if(fp == NULL)
		{
			LogError("Create file %s failed", ss.str().c_str());
			return;
		}

		std::string info = ProtoStatistics::Instance()->HtmlInfo();
		fwrite(info.c_str(), info.size(), 1, fp);
		fclose(fp);

		ProtoStatistics::Instance()->Reset();
	}
};

PrintProtoStatisticsTimer PrintTimer;

bool Process_Setup(const char *confFile)
{
	SINGLETON_CREATE_INIT_ARG1(DBConfig, confFile)
	SINGLETON_CREATE_INIT(CAppConfig)
	SINGLETON_CREATE_INIT(CMysqlMgr)
	SINGLETON_CREATE_INIT(CDbProfiler)

	CmdLine::RegistCmd("debug", CmdLineSetDebugFlag);
	CmdLine::Setup();

	ProtoStatistics::Instance()->SetEnable(false);
	CTimerMgr::Instance()->SetTimer(&PrintTimer, 0, 60 * 60 * 1000, -1, __FILE__, __LINE__);

	CProtocolRegister::Regiter();

	return true;
}

void Process_Cleanup()
{
	LogWarn("db stop...");
	CmdLine::Stop();

	SINGLETON_DESTORY_UNINIT(CDbProfiler)
	SINGLETON_DESTORY_UNINIT(CMysqlMgr)
	SINGLETON_DESTORY_UNINIT(CAppConfig)
	SINGLETON_DESTORY_UNINIT(DBConfig)

}

bool Process_Update()
{
	CmdLine::Run();
	DBConfig::Instance()->ProcessNetMessage();
	CMysqlMgr::Instance()->Process();

	return IsProcessRunning;
}

void Process_Stop()
{
	IsProcessRunning = false;
}

