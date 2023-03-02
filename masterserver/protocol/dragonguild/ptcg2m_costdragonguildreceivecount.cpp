#include "pch.h"
#include "dragonguild/ptcg2m_costdragonguildreceivecount.h"
#include "dragonguild/dragonguildmgr.h"
// generate by ProtoGen at date: 2017/9/12 14:28:34

void PtcG2M_CostDragonGuildReceiveCount::Process(UINT32 dwConnID)
{
	DragonGuild* guild = DragonGuildMgr::Instance()->GetDragonGuildByRole(m_Data.roleid());
	if(guild == NULL)
		return;
	DragonGuildTask& record = guild->GetDragonGuildTask();
	//合法性之前检查过了, 故直接减
	record.CostTaskChestCount(m_Data.type(), m_Data.taskid());
}
