#include "pch.h"
#include "dragonguild/rpcc2m_dragonguildunbindgroup.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "dragonguild/dragonguild.h"
#include "dragonguild/dragonguildmgr.h"
#include "dragonguild/dragonguildbindinfo.h"
#include "timeutil.h"
#include "httptask/unlinkgrouptask.h"
#include "httptask/httptaskmgr.h"

// generate by ProtoGen at date: 2017/9/13 20:19:53

RPC_SERVER_IMPLEMETION(RpcC2M_DragonGuildUnBindGroup, DragonGuildUnBindGroupArg, DragonGuildUnBindGroupRes)

void RpcC2M_DragonGuildUnBindGroup::OnCall(const DragonGuildUnBindGroupArg &roArg, DragonGuildUnBindGroupRes &roRes)
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
	// 没有绑定
	if (!bindInfo.IsBindGroup())
	{
		SSWarn << "not bind, roleId: " << role->GetID() << END;
		roRes.set_result(KKSG::ERR_GUILD_ALREADY_BIND);
		return;
	}

	// 没有权限
	if (pGuild->GetLeaderId() != role->GetID())
	{
		roRes.set_result(KKSG::ERR_GUILD_NO_PERMISSION);
		return;
	}

	UnlinkGroupTask* task = new UnlinkGroupTask;
	task->SetRpcId(DelayRpc());
	task->SetOpenId(role->GetAccount());
	task->SetToken(roArg.token());
	task->SetGuild(pGuild->GetID(), pGuild->GetName(), pGuild->GetServerID());
	task->SetRoleId(role->GetID());
	task->SetGroupCode(bindInfo.GetQQGroupCode());
	HttpTaskMgr::Instance()->AddTask(task);
}

void RpcC2M_DragonGuildUnBindGroup::OnDelayReplyRpc(const DragonGuildUnBindGroupArg &roArg, DragonGuildUnBindGroupRes &roRes, const CUserData &roUserData)
{
	roRes.set_result((KKSG::ErrorCode)roUserData.m_dwUserData);
}
