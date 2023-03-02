
typedef bool my_bool;
#include "pch.h"
#include "UpdateGateInfoTask.h"
#include "SQLStmt.h"

UpdateGateInfoTask::UpdateGateInfoTask()
:m_serverId(0)
,m_registerAccount(0)
,m_onlineRole(0)
,m_fullRegisterTime(0)
{

}

UpdateGateInfoTask::~UpdateGateInfoTask()
{

}

int UpdateGateInfoTask::Execute(MYSQL *mysql)
{
	{
		const char query[] = "update gateinfo set register_account=?, online_role=? where server_id=?";
		SQLStmt<3, 0> stmt(mysql);

		if (!stmt.SetQuery(query, sizeof(query)))
		{
			stmt.ReportError();
			return stmt.GetError();
		}

		stmt.BindIn(&m_registerAccount);
		stmt.BindIn(&m_onlineRole);
		stmt.BindIn(&m_serverId);

		if (!stmt.Execute())
		{
			stmt.ReportError();
			return stmt.GetError();
		}
	}

	// �״�ע����
	if (m_fullRegisterTime != 0)
	{
		const char query[] = "update gateinfo set full_register_time=FROM_UNIXTIME(?) where server_id=?";
		SQLStmt<2, 0> stmt(mysql);

		if (!stmt.SetQuery(query, sizeof(query)))
		{
			stmt.ReportError();
			return stmt.GetError();
		}
		stmt.BindIn(&m_fullRegisterTime);
		stmt.BindIn(&m_serverId);

		if (!stmt.Execute())
		{
			stmt.ReportError();
			return stmt.GetError();
		}
	}

	return 0;
}

void UpdateGateInfoTask::FinishTask()
{
	if (m_result != 0)
	{
		LogWarn("update server[%u] failed: %u", m_serverId, m_result);
	}
}
