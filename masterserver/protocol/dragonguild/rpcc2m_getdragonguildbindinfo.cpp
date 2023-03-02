#include "pch.h"
#include "dragonguild/rpcc2m_getdragonguildbindinfo.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "dragonguild/dragonguild.h"
#include "dragonguild/dragonguildmgr.h"
#include "dragonguild/dragonguildbindinfo.h"
#include "timeutil.h"
#include "httptask/httptaskmgr.h"
#include "httptask/getgrouprelation.h"
#include "httptask/getgroupdetailtask.h"
// generate by ProtoGen at date: 2017/9/13 20:14:43

RPC_SERVER_IMPLEMETION(RpcC2M_GetDragonGuildBindInfo, GetDragonGuildBindInfoArg, GetDragonGuildBindInfoRes)

void RpcC2M_GetDragonGuildBindInfo::OnCall(const GetDragonGuildBindInfoArg &roArg, GetDragonGuildBindInfoRes &roRes)
{
	roRes.set_result(KKSG::ERR_SUCCESS);
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		SSWarn << "role is NULL and session is :" << m_sessionID <<END;
		roRes.set_result(KKSG::ERR_UNKNOWN);
		return;
	}
	// 平台判断
	DragonGuild* pGuild = DragonGuildMgr::Instance()->GetDragonGuildByRole(role->GetID());
	if (pGuild == NULL)
	{
		SSWarn << "role has no dragonguild" << END;
		roRes.set_result(KKSG::ERR_GUILD_NOT_EXIST);
		return;
	}
	DragonGuildBindInfo& bindInfo = pGuild->GetDragonGuildBindInfo();
	if (!bindInfo.IsBindGroup())
	{
		roRes.set_bind_status(KKSG::GBS_NotBind);
		return;
	}
	DragonGuildMember* member = pGuild->GetMember(role->GetID());
	if (!member)
	{
		roRes.set_result(KKSG::ERR_UNKNOWN);
		return;
	}

	roRes.set_group_name(bindInfo.GetQQGroupName());
	roRes.set_bind_status(member->groupPos);
	if (member->groupPos == KKSG::GBS_NotBind || member->groupPos == KKSG::GBS_NotMember)
	{
		return;
	}

	UINT32 now = TimeUtil::GetTime();
	// 群详情缓存过时
	if (now > bindInfo.GetLastUpdateTime() + 300)
	{
		bindInfo.SetLastUpdateTime(TimeUtil::GetTime());
		GetGroupDetailTask* task = new GetGroupDetailTask;
		task->SetRpcId(DelayRpc());
		task->SetOpenId(role->GetAccount());
		task->SetToken(roArg.token());
		task->SetGuild(pGuild->GetID(), pGuild->GetName(), pGuild->GetServerID());
		task->SetRoleId(role->GetID());
		task->SetGroupCode(bindInfo.GetQQGroupCode());
		HttpTaskMgr::Instance()->AddTask(task);
		return;
	}

	// 用户与群关系缓存过时
	if (now > member->updateBindTime + 300)
	{
		member->updateBindTime = TimeUtil::GetTime();
		GetGroupRelationTask* task = new GetGroupRelationTask;
		task->SetRpcId(DelayRpc());
		task->SetOpenId(role->GetAccount());
		task->SetToken(roArg.token());
		task->SetGuild(pGuild->GetID(), pGuild->GetName(), pGuild->GetServerID());
		task->SetRoleId(role->GetID());
		task->SetGroupCode(bindInfo.GetQQGroupCode());
		HttpTaskMgr::Instance()->AddTask(task);
	}
	
}

void RpcC2M_GetDragonGuildBindInfo::OnDelayReplyRpc(const GetDragonGuildBindInfoArg &roArg, GetDragonGuildBindInfoRes &roRes, const CUserData &roUserData)
{
	roRes.set_result((KKSG::ErrorCode)roUserData.m_dwUserData);
	if (roUserData.m_dwUserData != 0)
	{
		return;
	}
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		SSWarn << "role is NULL and session is :" << m_sessionID <<END;
		roRes.set_result(KKSG::ERR_UNKNOWN);
		return;
	}
	DragonGuild* pGuild = DragonGuildMgr::Instance()->GetDragonGuildByRole(role->GetID());
	if (pGuild == NULL)
	{
		SSWarn << "role has no dragonguild" << END;
		roRes.set_result(KKSG::ERR_GUILD_NOT_EXIST);
		return;
	}
	DragonGuildBindInfo& bindInfo = pGuild->GetDragonGuildBindInfo();
	DragonGuildMember* member = pGuild->GetMember(role->GetID());
	if (!member)
	{
		roRes.set_result(KKSG::ERR_GUILD_NOT_IN_GUILD);
		return;
	}
	roRes.set_group_name(bindInfo.GetQQGroupName());
	roRes.set_bind_status(member->groupPos);
}
