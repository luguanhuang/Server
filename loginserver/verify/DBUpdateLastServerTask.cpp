#include "pch.h"
#include "DBUpdateLastServerTask.h"
#include "SQLStmt.h"

DBUpdateLastServerTask::DBUpdateLastServerTask(const std::string &UserID, int ServerID)
{
	m_UserID = UserID;
	m_ServerID = ServerID;
}

DBUpdateLastServerTask::~DBUpdateLastServerTask()
{

}

int DBUpdateLastServerTask::Execute(MYSQL *mysql)
{
	const char query[] = "replace into lastlogin (userid, serverid) values (?,?)";

	SQLStmt<2,0> stmt(mysql);
	if (!stmt.SetQuery(query, sizeof(query)))
	{
		stmt.ReportError();
		return stmt.GetError();
	}

	UINT32 level;
	stmt.BindIn(&m_UserID[0], m_UserID.size());
	stmt.BindIn(&m_ServerID);
	stmt.BindIn(&m_ServerID);
	
	if (!stmt.Execute())
	{
		stmt.ReportError();
		return stmt.GetError();
	}

	if (stmt.FetchResult())
	{

	}

	return 0;
}

void DBUpdateLastServerTask::FinishTask()
{
}



