#include "pch.h"
#include "dragonguild/rpcc2m_dragonguildjoinbindgroup.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "dragonguild/dragonguild.h"
#include "dragonguild/dragonguildmgr.h"
#include "dragonguild/dragonguildbindinfo.h"
#include "timeutil.h"
#include "httptask/httptaskmgr.h"
#include "httptask/joingrouptask.h"
// generate by ProtoGen at date: 2017/9/13 20:18:29

RPC_SERVER_IMPLEMETION(RpcC2M_DragonGuildJoinBindGroup, DragonGuildJoinBindGroupArg, DragonGuildJoinBindGroupRes)

void RpcC2M_DragonGuildJoinBindGroup::OnCall(const DragonGuildJoinBindGroupArg &roArg, DragonGuildJoinBindGroupRes &roRes)
{
	roRes.set_result(KKSG::ERR_UNKNOWN);
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		SSWarn << "role is NULL and session is :" << m_sessionID <<END;
		return;
	}
	// 平台判断
	DragonGuild* pGuild = DragonGuildMgr::Instance()->GetDragonGuildByRole(role->GetID());
	if (pGuild == NULL)
	{
		SSWarn << "role has no guild" << END;
		roRes.set_result(KKSG::ERR_GUILD_NOT_EXIST);
		return;
	}

	DragonGuildBindInfo& bindInfo = pGuild->GetDragonGuildBindInfo();
	// 公会尚未绑定qq群
	if (!bindInfo.IsBindGroup())
	{
		SSWarn << "not bind " << END;
		roRes.set_result(KKSG::ERR_GUILD_NOT_BIND);
		return;
	}
	DragonGuildMember* member = pGuild->GetMember(role->GetID());
	if (!member)
	{
		SSWarn << "not member, roleId: " << role->GetID() << END;
		return;
	}

	// 已经是群成员
	if (member->groupPos != KKSG::GBS_NotBind && member->groupPos != KKSG::GBS_NotMember)
	{
		SSWarn << "already in qq group" << END;
		roRes.set_result(KKSG::ERR_ALREADY_IN_QQGROUP);
		return;
	}

	JoinGroupTask* task = new JoinGroupTask;
	task->SetRpcId(DelayRpc());
	task->SetOpenId(role->GetAccount());
	task->SetToken(roArg.token());
	task->SetGuild(pGuild->GetID(), pGuild->GetName(), pGuild->GetServerID());
	task->SetRoleId(role->GetID());
	task->SetGroupCode(bindInfo.GetQQGroupCode());
	HttpTaskMgr::Instance()->AddTask(task);
}

void RpcC2M_DragonGuildJoinBindGroup::OnDelayReplyRpc(const DragonGuildJoinBindGroupArg &roArg, DragonGuildJoinBindGroupRes &roRes, const CUserData &roUserData)
{
	roRes.set_result((KKSG::ErrorCode)roUserData.m_dwUserData);
}
