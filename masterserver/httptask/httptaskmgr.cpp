#include "pch.h"
#include "httptaskmgr.h"
#include "cmdline.h"
#include "createbindtask.h"
#include "getgrouprelation.h"
#include "getgroupdetailtask.h"
#include "joingrouptask.h"
#include "unlinkgrouptask.h"
#include "config.h"

INSTANCE_SINGLETON(HttpTaskMgr)

static bool CmdCreateBind(const CmdLineArgs &args, std::string &outputMessage)
{
	CreateBindTask* task = new CreateBindTask;
	task->SetRpcId(0);
	task->SetOpenId("0C23A900A573DAE4A94981F97EE25A8E");
	task->SetToken("68465913125ADFE7D12EADF003FC8636");
	task->SetGuild(999, "GuildName", MSConfig::Instance()->GetServerID());
	task->SetRoleId(100001);
	HttpTaskMgr::Instance()->AddTask(task);
	return true;
}

static bool CmdQueryRelation(const CmdLineArgs &args, std::string &outputMessage)
{
	GetGroupRelationTask* task = new GetGroupRelationTask;
	task->SetRpcId(0);
	task->SetOpenId("0C23A900A573DAE4A94981F97EE25A8E");
	task->SetToken("68465913125ADFE7D12EADF003FC8636");
	task->SetGuild(999, "GuildName", MSConfig::Instance()->GetServerID());
	task->SetRoleId(100001);
	task->SetGroupCode("618798094");
	HttpTaskMgr::Instance()->AddTask(task);
	return true;
}

static bool CmdUnlinkGroup(const CmdLineArgs &args, std::string &outputMessage)
{
	UnlinkGroupTask* task = new UnlinkGroupTask;
	task->SetRpcId(0);
	task->SetOpenId("0C23A900A573DAE4A94981F97EE25A8E");
	task->SetToken("68465913125ADFE7D12EADF003FC8636");
	task->SetGuild(999, "GuildName", MSConfig::Instance()->GetServerID());
	task->SetRoleId(100001);
	task->SetGroupCode("618798094");
	HttpTaskMgr::Instance()->AddTask(task);
	return true;
}

HttpTaskMgr::HttpTaskMgr()
{

}

HttpTaskMgr::~HttpTaskMgr()
{

}

bool HttpTaskMgr::Init()
{
	m_webThread.Start(2);

	CmdLine::RegistCmd("createbind", CmdCreateBind);
	CmdLine::RegistCmd("relation", CmdQueryRelation);
	CmdLine::RegistCmd("unlinkgroup", CmdUnlinkGroup);

	return true;
}

void HttpTaskMgr::Uninit()
{
	m_webThread.Terminate();
}

void HttpTaskMgr::Update()
{
	m_webThread.FinishTask();
}

void HttpTaskMgr::AddTask(IWebFetchTask* task)
{
	m_webThread.AddTask(task);
}
