#include "pch.h"
#include "common/ptcn2i_updateserverstate2login.h"
#include "gatemgr/gatemgr.h"
#include "gatemgr/UpdateGateInfoTask.h"
#include "verify/tokenverifymgr.h"
#include "config.h"
#include "timeutil.h"

// generate by ProtoGen at date: 2016/12/23 11:10:50

void UpdateServerState(UINT32 serverId, UINT32 registerNum, UINT32 onlineRole, bool writeDb)
{
	LogInfo("server[%u] register [%u] online role [%u]", serverId, registerNum, onlineRole);
	GateInfo* gate = GateMgr::Instance()->GetServerInfo(serverId);
	if (gate == NULL)
	{
		return ;
	}
	gate->onlineRole = onlineRole;

	UINT32 fullRegisterTime = 0;
	if (registerNum)
	{
		UINT32 old = gate->registerAccount;
		gate->registerAccount = registerNum; 
		UINT32 fullRegisterNum = LoginConfig::Instance()->GetRegisterFullNum();
		// 首次到达爆满的时间
		if (old < fullRegisterNum && gate->registerAccount >= fullRegisterNum)
		{
			fullRegisterTime = TimeUtil::GetTime();
			gate->fullRegisterTime = fullRegisterTime;
		}
	}
	gate->UpdateState();

	if (writeDb)
	{
		UpdateGateInfoTask* task = new UpdateGateInfoTask;
		task->m_serverId = gate->serverID;
		task->m_registerAccount = gate->registerAccount;
		task->m_onlineRole = gate->onlineRole;
		task->m_fullRegisterTime = fullRegisterTime;
		TokenVerifyMgr::Instance()->AddDBTask(task);
	}
}

void PtcN2I_UpdateServerState2Login::Process(UINT32 dwConnID)
{
	std::set<UINT32> serverIds;
	if (m_Data.serverid())
	{
		serverIds.insert(m_Data.serverid());
	}
	for (int i = 0; i < m_Data.serverids_size(); i++)
	{
		serverIds.insert(m_Data.serverids(i));
	}

	for (auto it = serverIds.begin(); it != serverIds.end(); ++it)
	{
		UpdateServerState(*it, m_Data.register_account(), m_Data.online_role(), m_Data.write_db());
	}
}
