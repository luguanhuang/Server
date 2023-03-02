#include "pch.h"
#include "gmmgr.h"
#include "util.h"
#include "sql/mysqlmgr.h"
#include "config.h"
#include <rapidjson/prettywriter.h>
#include "verify/TXLoginTask.h"
#include "verify/tokenverifymgr.h"

using namespace RAPIDJSON_NAMESPACE;

#define INTERNAL_ACCOUNT 1 
#define EXTERNAL_ACCOUNT 2
#define PHONE_ACCOUNT 3

INSTANCE_SINGLETON(GMMgr);

static bool Test(const CmdLineArgs &args, std::string &outputMessage)
{
	SSDebug << "Start" << END;
	for (UINT32 i = 0; i < 20000; ++i)
	{
		TXLoginTask* pTask = new TXLoginTask();
		pTask->SetIP(516775946);
		pTask->SetRpcID(0);
		pTask->SetToken("DB47F4C66752C21D7C075AF747BB58EC");
		pTask->SetOpenId("E3053D7142FF65C2F57983EA3E781E9D");
		pTask->SetLoginType(KKSG::LOGIN_QQ_PF);
		pTask->BuildPostData();

		TokenVerifyMgr::Instance()->AddPFVerifyTask(pTask);
	}
	return true;
}

bool GMMgr::Init()
{
	if (!Load())
	{
		return false;
	}
	CmdLine::RegistCmd("addgm", GMMgr::CmdAddGM);
	CmdLine::RegistCmd("delgm", GMMgr::CmdDelGM);
	CmdLine::RegistCmd("dumpgm", GMMgr::CmdDumpGM);
	CmdLine::RegistCmd("test", Test);
	return true;	
}

void GMMgr::Uninit()
{

}

void GMMgr::_Add(const std::string& account)
{
	m_gmAccounts.insert(account);
	LogInfo("add gm account:%s", account.c_str());
}

void GMMgr::_Del(const std::string& account)
{
	m_gmAccounts.erase(account);
	LogInfo("del gm account:%s", account.c_str());
}

bool GMMgr::Load()
{
	std::set<std::string> accounts;
	if (!MysqlMgr::Instance()->QueryGMAccount(accounts))
	{
		SSWarn<<"load gmaccount failed"<<END;
		return false;
	}

	for (auto i = accounts.begin(); i != accounts.end(); ++i)
	{
		_Add(*i);
	}
	return true;
}

bool GMMgr::Reload()
{
	m_gmAccounts.clear();
	return Load();
}

bool GMMgr::AddGM(const std::string& account)
{
	if (!MysqlMgr::Instance()->AddGMAccount(account))
	{
		//return false;
	}
	_Add(account);
	return true;
}

bool GMMgr::DelGM(const std::string& account)
{
	if (!MysqlMgr::Instance()->DelGMAccount(account))
	{
		//return false;
	}
	_Del(account);
	return true;
}

void GMMgr::DumpGM()
{
	SSInfo<<"\n";
	for (auto i = m_gmAccounts.begin(); i != m_gmAccounts.end(); ++i)
	{
		SSInfo<<*i<<"\n";
	}
	SSInfo<<END;
}

bool GMMgr::CmdAddGM(const CmdLineArgs &args, std::string &outputMessage)
{
	if (args.size() != 2)
	{
		return false;
	}

	std::string cmd = args[0];
	std::string account = args[1];

	if (GMMgr::Instance()->AddGM(account))
	{
		SSInfo<<"succeed"<<END;
		outputMessage = "succeed";
		return true;
	}
	else
	{
		SSInfo<<"failed"<<END;
		outputMessage = "failed";
		return false;
	}
}

bool GMMgr::CmdDelGM(const CmdLineArgs &args, std::string &outputMessage)
{
	if (args.size() != 2)
	{
		return false;
	}

	std::string cmd = args[0];
	std::string account = args[1];

	if (GMMgr::Instance()->DelGM(account))
	{
		SSInfo<<"succeed"<<END;
		outputMessage = "succeed";
		return true;
	}
	else
	{
		SSInfo<<"failed"<<END;
		outputMessage = "failed";
		return false;
	}
}

bool GMMgr::CmdDumpGM(const CmdLineArgs &args, std::string &outputMessage)
{
	if (args.size() != 1)
	{
		return false;
	}
	GMMgr::Instance()->DumpGM();
	return true;
}

bool GMMgr::IsGM(const std::string& account)
{
	if(LoginConfig::Instance()->IsDebug())
	{
		return true;
	}

	auto i = m_gmAccounts.find(account);
	if (i != m_gmAccounts.end())
	{
		SSInfo<<"GM login"<<account.c_str()<<END;
		return true;
	}
	return false;
}

bool GMMgr::IdipAdd(std::map<std::string, std::string>& keyvalues, INT32& errcode)
{
	std::string account;
	if (keyvalues.find("account") == keyvalues.end())
	{
		return false;
	}
	account = keyvalues["account"];
	std::string phone;
	if (IsGM(account))
	{
		errcode = IDIP_ERR_ALREADY_ACTIVE;
		return false;
	}
	AddGM(account);
	errcode = IDIP_ERR_SUCCESS;
	return true;
}
