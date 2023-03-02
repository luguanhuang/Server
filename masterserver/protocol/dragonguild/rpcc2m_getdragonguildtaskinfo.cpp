#include "pch.h"
#include "dragonguild/rpcc2m_getdragonguildtaskinfo.h"
#include "dragonguild/dragonguildmgr.h"
#include "role/rolemanager.h"
#include "dragonguild/rpcm2g_getdragonguildrecieveinfo.h"
// generate by ProtoGen at date: 2017/9/11 11:00:56

RPC_SERVER_IMPLEMETION(RpcC2M_GetDragonGuildTaskInfo, GetDragonGuildTaskInfoArg, GetDragonGuildTaskInfoRes)

void RpcC2M_GetDragonGuildTaskInfo::OnCall(const GetDragonGuildTaskInfoArg &roArg, GetDragonGuildTaskInfoRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		LogWarn("can't find role by session[%llu]", m_sessionID);
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return ;
	}

	DragonGuild* guild = DragonGuildMgr::Instance()->GetDragonGuildByRole(pRole->GetID());
	if (guild == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_PARTNER_NO_PARTNER);
		return;
	}
	DragonGuildTask& taskRecord = guild->GetDragonGuildTask();

	std::vector<KKSG::DragonGuildTaskInfo>tasks;
	tasks.clear();
	taskRecord.GetDragonGuildTaskIdList(KKSG::TASK_NORMAL, tasks);
	std::vector<KKSG::DragonGuildTaskInfo>achieves;
	achieves.clear();
	taskRecord.GetDragonGuildTaskIdList(KKSG::TASK_ACHIVEMENT, achieves);

	//去gs查本玩家是否领过
	RpcM2G_GetDragonGuildRecieveInfo* rpc = RpcM2G_GetDragonGuildRecieveInfo::CreateRpc();
	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_rpcid(DelayRpc());
	for(size_t i = 0; i<tasks.size(); ++i)
		rpc->m_oArg.add_tasklist(tasks[i].taskid());
	for(size_t i = 0; i<achieves.size(); ++i)
		rpc->m_oArg.add_achievelist(achieves[i].taskid());
	pRole->SendMsgToGS(*rpc);
}

void RpcC2M_GetDragonGuildTaskInfo::OnDelayReplyRpc(const GetDragonGuildTaskInfoArg &roArg, GetDragonGuildTaskInfoRes &roRes, const CUserData &roUserData)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		LogWarn("can't find role by session[%llu]", m_sessionID);
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return ;
	}
	DragonGuild* guild = DragonGuildMgr::Instance()->GetDragonGuildByRole(pRole->GetID());
	if (guild == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_PARTNER_NO_PARTNER);
		return;
	}
	if (roUserData.m_pUserPtr == NULL)
	{
		roRes.set_errorcode((KKSG::ErrorCode)roUserData.m_dwUserData);
		return;	
	}
	roRes.set_errorcode(KKSG::ERR_SUCCESS);
	KKSG::GetDragonGuildRecieveInfoRes* res = (KKSG::GetDragonGuildRecieveInfoRes*)(roUserData.m_pUserPtr);
	if(!res)
		return;
	DragonGuildTask& taskRecord = guild->GetDragonGuildTask();
	std::vector<KKSG::DragonGuildTaskInfo>tasks;
	tasks.clear();
	taskRecord.GetDragonGuildTaskIdList(KKSG::TASK_NORMAL, tasks);
	std::vector<KKSG::DragonGuildTaskInfo>achieves;
	achieves.clear();
	taskRecord.GetDragonGuildTaskIdList(KKSG::TASK_ACHIVEMENT, achieves);
	for(size_t i = 0; i<tasks.size(); ++i)
	{
		if(i >= res->task_receieved_size())
			break;
		KKSG::DragonGuildTaskInfo* info = roRes.add_taskrecord();
		info->CopyFrom(tasks[i]);
		roRes.add_taskcompleted(res->task_receieved(i));
	}
	for(size_t i = 0; i<achieves.size(); ++i)
	{
		if(i >= res->achieve_recieved_size())
			break;
		KKSG::DragonGuildTaskInfo* info = roRes.add_achiverecord();
		info->CopyFrom(achieves[i]);
		roRes.add_achivecompleted(res->achieve_recieved(i));
	}
	roRes.set_exp(guild->GetExp());
	roRes.set_task_refreshtime(taskRecord.GetRefreshTimeStr());
}
