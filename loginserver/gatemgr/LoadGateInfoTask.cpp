typedef bool my_bool;

#include "pch.h"
#include "LoadGateInfoTask.h"
#include "SQLStmt.h"
#include "gatemgr/gatemgr.h"

LoadGateInfoTask::LoadGateInfoTask(bool reload/* = false*/)
{
	m_reload = reload;
}

LoadGateInfoTask::~LoadGateInfoTask()
{

}

int LoadGateInfoTask::Execute(MYSQL *mysql)
{
	const char query[] = "select server_id, channel, state, is_open, server_name, zone_name, ipaddr, free_ipaddr, register_account,"
						 "online_role, UNIX_TIMESTAMP(open_time), UNIX_TIMESTAMP(full_register_time), backflow_level from gateinfo";
	SQLStmt<0, 13> stmt(mysql);

	if (!stmt.SetQuery(query, sizeof(query)))
	{
		stmt.ReportError();
		return stmt.GetError();
	}
	int serverId = 0;
	char channel[255] = {0}; 
	int state = 0;
	int isOpen = 0;
	char serverName[255] = {0};
	char zoneName[255] = {0};
	char ipAddr[255] = {0};
	char freeIpAddr[255] = {0};
	int registerAccount = 0;
	int onlineRole = 0;
	int openTime = 0;
	int fullRegisterTime = 0;
	int backflowLevel = 0;

	stmt.BindOut(&serverId);
	stmt.BindOut(channel, 255);
	stmt.BindOut(&state);
	stmt.BindOut(&isOpen);
	stmt.BindOut(serverName, 255);
	stmt.BindOut(zoneName, 255);
	stmt.BindOut(ipAddr, 255);
	stmt.BindOut(freeIpAddr, 255);
	stmt.BindOut(&registerAccount);
	stmt.BindOut(&onlineRole);
	stmt.BindOut(&openTime);
	stmt.BindOut(&fullRegisterTime);
	stmt.BindOut(&backflowLevel);

	if (!stmt.Execute())
	{
		stmt.ReportError();
		return stmt.GetError();
	}

	m_infos.clear();
	while (stmt.FetchResult())
	{
		GateInfo info;
		info.serverID = serverId;
		info.ParseChannel(channel);
		info.setState = state;
		info.isOpen = isOpen == 0 ? false : true;
		info.serverName = std::string(serverName);
		info.zoneName = std::string(zoneName);
		info.ParseIpAddr(ipAddr, info.Hosts);
		info.ParseIpAddr(freeIpAddr, info.FreeHosts);
		info.registerAccount = registerAccount;
		info.onlineRole = onlineRole;
		info.openTime = openTime;
		info.fullRegisterTime = fullRegisterTime;
		info.backFlowLevel = backflowLevel;

		info.UpdateState();
		m_infos.push_back(info);
	}

	m_result = 0;
	return 0;
}

void LoadGateInfoTask::FinishTask()
{
	if (m_result != 0)
	{
		LogWarn("load server failed, %u", m_result);
		return;
	}
	GateMgr::Instance()->ParseFrom(m_infos, m_reload);
}
