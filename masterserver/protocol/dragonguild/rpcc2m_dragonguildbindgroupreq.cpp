#include "pch.h"
#include "dragonguild/rpcc2m_dragonguildbindgroupreq.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "dragonguild/dragonguild.h"
#include "dragonguild/dragonguildmgr.h"
#include "dragonguild/dragonguildbindinfo.h"
#include "timeutil.h"
#include "httptask/createbindtask.h"
#include "httptask/httptaskmgr.h"
#include "define/systemiddef.h"

// generate by ProtoGen at date: 2017/9/13 20:17:00

RPC_SERVER_IMPLEMETION(RpcC2M_DragonGuildBindGroupReq, DragonGuildBindReq, DragonGuildBindRes)

void RpcC2M_DragonGuildBindGroupReq::OnCall(const DragonGuildBindReq &roArg, DragonGuildBindRes &roRes)
{
	roRes.set_result(KKSG::ERR_FAILED);
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		SSWarn << "role is NULL and session is :" << m_sessionID <<END;
		return;
	}
	// 平台判断
	if (!role->IsSystemOpened(SYS_DRAGONGUILD_BINDGROUP))
	{
		roRes.set_result(KKSG::ERR_SYS_NOTOPEN);
		return;
	}

	DragonGuild* pGuild = DragonGuildMgr::Instance()->GetDragonGuildByRole(role->GetID());
	if (pGuild == NULL)
	{
		SSWarn << "role has no guild" << END;
		roRes.set_result(KKSG::ERR_GUILD_NOT_EXIST);
		return;
	}

	DragonGuildBindInfo& bindInfo = pGuild->GetDragonGuildBindInfo();
	// 已经绑定
	if (bindInfo.IsBindGroup())
	{
		SSWarn << "alread bind " << END;
		roRes.set_result(KKSG::ERR_GUILD_ALREADY_BIND);
		return;
	}

	// 没有权限
	if (pGuild->GetLeaderId() != role->GetID())
	{
		roRes.set_result(KKSG::ERR_GUILD_NO_PERMISSION);
		return;
	}

	CreateBindTask* task = new CreateBindTask;
	task->SetRpcId(DelayRpc());
	task->SetOpenId(role->GetAccount());
	task->SetToken(roArg.token());
	task->SetGuild(pGuild->GetID(), pGuild->GetName(), pGuild->GetServerID());
	task->SetRoleId(role->GetID());
	HttpTaskMgr::Instance()->AddTask(task);
}

void RpcC2M_DragonGuildBindGroupReq::OnDelayReplyRpc(const DragonGuildBindReq &roArg, DragonGuildBindRes &roRes, const CUserData &roUserData)
{
	roRes.set_result(KKSG::ERR_SUCCESS);
	if (roUserData.m_dwUserData != 0)
	{
		roRes.set_result((KKSG::ErrorCode)roUserData.m_dwUserData);
		return;
	}
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		SSWarn << "role is NULL and session is :" << m_sessionID <<END;
		return;
	}

	DragonGuild* pGuild = DragonGuildMgr::Instance()->GetDragonGuildByRole(role->GetID());
	if (pGuild == NULL)
	{
		SSWarn << "role has no guild" << END;
		roRes.set_result(KKSG::ERR_GUILD_NOT_EXIST);
		return;
	}

	DragonGuildBindInfo& bindInfo = pGuild->GetDragonGuildBindInfo();
	// 绑定失败
	if (!bindInfo.IsBindGroup())
	{
		roRes.set_result(KKSG::ERR_GUILD_NOT_BIND);
		return;
	}
	roRes.set_group_name(bindInfo.GetQQGroupName());
}
