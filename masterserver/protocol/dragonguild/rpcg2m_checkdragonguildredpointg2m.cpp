#include "pch.h"
#include "dragonguild/rpcg2m_checkdragonguildredpointg2m.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "dragonguild/dragonguild.h"
#include "dragonguild/dragonguildmgr.h"
#include "dragonguild/dragonguildbindinfo.h"
// generate by ProtoGen at date: 2017/9/14 17:41:46

RPC_SERVER_IMPLEMETION(RpcG2M_CheckDragonGuildRedPointG2M, CheckDragonGuildRedPointG2MArg, CheckDragonGuildRedPointG2MRes)

void RpcG2M_CheckDragonGuildRedPointG2M::OnCall(const CheckDragonGuildRedPointG2MArg &roArg, CheckDragonGuildRedPointG2MRes &roRes)
{
	DragonGuild* guild = DragonGuildMgr::Instance()->GetDragonGuild(roArg.dragonguildid());
	if(guild == NULL)
	{
		SSWarn << "role  DragonGuild is null" << m_sessionID <<END;
		return;
	}
	DragonGuildTask& taskrecord = guild->GetDragonGuildTask();
	std::vector<UINT32>tasks;
	tasks.clear();
	std::vector<UINT32>achieves;
	achieves.clear();
	taskrecord.FillTaskFinishedList(tasks, achieves);
	for(UINT32 i = 0; i<tasks.size(); ++i)
	{
		roRes.add_tasklist(tasks[i]);
	}
	for(UINT32 i = 0; i<achieves.size(); ++i)
	{
		roRes.add_achivelist(achieves[i]);
	}
	DragonGuildLiveness& info = guild->GetDragonGuildLiveness();
	roRes.set_liveness(info.GetLiveness());
}

void RpcG2M_CheckDragonGuildRedPointG2M::OnDelayReplyRpc(const CheckDragonGuildRedPointG2MArg &roArg, CheckDragonGuildRedPointG2MRes &roRes, const CUserData &roUserData)
{
}
