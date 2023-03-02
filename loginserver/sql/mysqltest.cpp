#include "pch.h"
#include "mysqltest.h"
#include "mysqlmgr.h"
#include "config.h"
#include "util.h"
#include "SQLStmt.h"
#include "verify/DBGetSelfServerTask.h"
#include "verify/DBUpdateSelfServerTask.h"
#include "timeutil.h"


INSTANCE_SINGLETON(CMysqlTest)


CMysqlTest::CMysqlTest()
{
}

CMysqlTest::~CMysqlTest()
{
}

bool CMysqlTest::Init()
{
	CmdLine::RegistCmd("readTest", CMysqlTest::OnCmdLineRead);
	CmdLine::RegistCmd("updateTest", CMysqlTest::OnCmdLineUpdate);
	CmdLine::RegistCmd("insertTest", CMysqlTest::OnCmdLineInsert);

	m_oUsers.reserve(1024 * 1024 * 10);

	if (mysql_select_db(MysqlMgr::Instance()->GetMysqlConn(), LoginConfig::Instance()->GetDB().c_str()) != 0)
	{
		LogError("select db error");
		return false;
	}

	const char query[] = "select userid from selfserver";
	SQLStmt<0,1> stmt(MysqlMgr::Instance()->GetMysqlConn());
	if (!stmt.SetQuery(query, sizeof(query)))
	{
		stmt.ReportError();
		return false;
	}

	char account[128];
	stmt.BindOut(account, 128);
	if (!stmt.Execute())
	{
		stmt.ReportError();
		return false;
	}
	while (stmt.FetchResult())
	{
		m_oUsers.push_back(account);
	}
	LogDebug("Init from table selfserver, count %u", m_oUsers.size());
	return true;
}

void CMysqlTest::Uninit()
{
}

void CMysqlTest::ReadTest(UINT32 dwCount)
{
	if(m_oUsers.empty())
	{
		LogError("empty users");
		return;
	}

	std::vector<INT32> oIndex;
	for(UINT32 i = 0; i < dwCount; ++i)
	{
		INT32 nIndex = rand() % m_oUsers.size();
		oIndex.push_back(nIndex);
	}

	UINT64 qwBegin = TimeUtil::GetMilliSecond();

	std::string phone;
	for(std::vector<INT32>::size_type i = 0; i < oIndex.size(); ++i)
	{
		std::string& account = m_oUsers[oIndex[i]];
		DBGetSelfServerTask oTask(0, account, phone, 0);
		oTask.Execute(MysqlMgr::Instance()->GetMysqlConn());
	}

	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	LogDebug("Execute %u DBGetSelfServerTask use time %u ms", dwCount, qwEnd - qwBegin);
}

void CMysqlTest::UpdateTest(UINT32 dwCount)
{
	std::vector<INT32> oIndex;
	for(UINT32 i = 0; i < dwCount; ++i)
	{
		INT32 nIndex = rand() % m_oUsers.size();
		oIndex.push_back(nIndex);
	}

	static int smLevle = 0;
	UINT64 qwBegin = TimeUtil::GetMilliSecond();

	for(std::vector<INT32>::size_type i = 0; i < oIndex.size(); ++i)
	{
		std::string& account = m_oUsers[oIndex[i]];
		DBUpdateSelfServerTask oTask(account, 1, ++smLevle);
		oTask.Execute(MysqlMgr::Instance()->GetMysqlConn());
	}

	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	LogDebug("Execute %u DBUpdateSelfServerTask(Update) use time %u ms", dwCount, qwEnd - qwBegin);
}

void CMysqlTest::InsertTest(UINT32 dwCount)
{
	static UINT32 smIndex = 0;
	UINT32 dwTime = TimeUtil::GetTime();

	UINT64 qwBegin = TimeUtil::GetMilliSecond();
	for(std::vector<INT32>::size_type i = 0; i < dwCount; ++i)
	{
		std::stringstream ss;
		ss << "user_lzg_" << dwTime << "_" << ++smIndex;
		DBUpdateSelfServerTask oTask(ss.str(), 1, 1);
		oTask.Execute(MysqlMgr::Instance()->GetMysqlConn());
		m_oUsers.push_back(ss.str());
	}

	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	LogDebug("Execute %u DBUpdateSelfServerTask(Insert) use time %u ms", dwCount, qwEnd - qwBegin);
}

bool CMysqlTest::OnCmdLineRead(const CmdLineArgs &args, std::string &outputMessage)
{
	if(args.size() < 2)
	{
		LogError("less parameter");
		return false;
	}

	UINT32 dwCount = convert<UINT32>(args[1]);
	CMysqlTest::Instance()->ReadTest(dwCount);
	return true;
}

bool CMysqlTest::OnCmdLineUpdate(const CmdLineArgs &args, std::string &outputMessage)
{
	if(args.size() < 2)
	{
		LogError("less parameter");
		return false;
	}

	UINT32 dwCount = convert<UINT32>(args[1]);
	CMysqlTest::Instance()->UpdateTest(dwCount);
	return true;
}

bool CMysqlTest::OnCmdLineInsert(const CmdLineArgs &args, std::string &outputMessage)
{
	if(args.size() < 2)
	{
		LogError("less parameter");
		return false;
	}

	UINT32 dwCount = convert<UINT32>(args[1]);
	CMysqlTest::Instance()->InsertTest(dwCount);
	return true;
}