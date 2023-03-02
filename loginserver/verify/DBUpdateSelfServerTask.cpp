#include "pch.h"
#include "DBUpdateSelfServerTask.h"
#include "SQLStmt.h"

DBUpdateSelfServerTask::DBUpdateSelfServerTask(const std::string &UserID, int ServerID, int level)
{
	m_UserID = UserID;
	m_ServerID = ServerID;
	m_level = level;
}

DBUpdateSelfServerTask::~DBUpdateSelfServerTask()
{

}

int DBUpdateSelfServerTask::Execute(MYSQL *mysql)
{
	const char query[] = "select level from selfserver where userid=? and serverid=?";
	SQLStmt<2,1> stmt(mysql);
	if (!stmt.SetQuery(query, sizeof(query)))
	{
		stmt.ReportError();
		return stmt.GetError();
	}

	int level = 0;
	stmt.BindIn(&m_UserID[0], m_UserID.size());
	stmt.BindIn(&m_ServerID);
	stmt.BindOut(&level);

	if (!stmt.Execute())
	{
		stmt.ReportError();
		return stmt.GetError();
	}

	if (stmt.FetchResult())
	{
		
	}

	SQLStmt<3,0> stmtupdate(mysql);
	if (0 == level)
	{
		const char query[] = "insert into selfserver values(?, ?, ?, Now())";
		if (!stmtupdate.SetQuery(query, sizeof(query)))
		{
			stmtupdate.ReportError();
			return stmtupdate.GetError();
		}
		stmtupdate.BindIn(&m_UserID[0], m_UserID.size());
		stmtupdate.BindIn(&m_ServerID);
		stmtupdate.BindIn(&m_level);
		if (!stmtupdate.Execute())
		{
			stmtupdate.ReportError();
			return stmtupdate.GetError();
		}
	}
	else
	{
		if (level > m_level)
		{
			return 0;
		}
		const char query[] = "update selfserver set level=?, time=Now() where userid=? and serverid=?"; 
		if (!stmtupdate.SetQuery(query, sizeof(query)))
		{
			stmtupdate.ReportError();
			return stmtupdate.GetError();
		}
		stmtupdate.BindIn(&m_level);
		stmtupdate.BindIn(&m_UserID[0], m_UserID.size());
		stmtupdate.BindIn(&m_ServerID);
		if (!stmtupdate.Execute())
		{
			stmtupdate.ReportError();
			return stmtupdate.GetError();
		}
	}

	return 0;
}

void DBUpdateSelfServerTask::FinishTask()
{
}



