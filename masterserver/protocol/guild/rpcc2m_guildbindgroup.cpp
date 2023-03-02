#include "pch.h"
#include "guild/rpcc2m_guildbindgroup.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildbindinfo.h"
#include "timeutil.h"
#include "httptask/createbindtask.h"
#include "httptask/httptaskmgr.h"
#include "define/systemiddef.h"

// generate by ProtoGen at date: 2016/10/10 15:53:51

RPC_SERVER_IMPLEMETION(RpcC2M_GuildBindGroup, GuildBindGroupReq, GuildBindGroupRes)

void RpcC2M_GuildBindGroup::OnCall(const GuildBindGroupReq &roArg, GuildBindGroupRes &roRes)
{
	roRes.set_result(KKSG::ERR_FAILED);
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		SSWarn << "role is NULL and session is :" << m_sessionID <<END;
		return;
	}
	// 平台判断
	if (!role->IsSystemOpened(SYS_GUILD_BIND_GROUP))
	{
		roRes.set_result(KKSG::ERR_SYS_NOTOPEN);
		return;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
	if (pGuild == NULL)
	{
		SSWarn << "role has no guild" << END;
		roRes.set_result(KKSG::ERR_GUILD_NOT_EXIST);
		return;
	}

	GuildBindInfo& bindInfo = pGuild->GetBindGroupInfo();
	// 已经绑定
	if (bindInfo.IsBindGroup())
	{
		SSWarn << "alread bind " << END;
		roRes.set_result(KKSG::ERR_GUILD_ALREADY_BIND);
		return;
	}

	// 没有权限
	if (!pGuild->isLeader(role->GetID()))
	{
		roRes.set_result(KKSG::ERR_GUILD_NO_PERMISSION);
		return;
	}

	CreateBindTask* task = new CreateBindTask;
	task->SetRpcId(DelayRpc());
	task->SetOpenId(role->GetAccount());
	task->SetToken(roArg.token());
	task->SetGuild(pGuild->GetID(), pGuild->GetName(), pGuild->GetServerId());
	task->SetRoleId(role->GetID());
	HttpTaskMgr::Instance()->AddTask(task);
}

void RpcC2M_GuildBindGroup::OnDelayReplyRpc(const GuildBindGroupReq &roArg, GuildBindGroupRes &roRes, const CUserData &roUserData)
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

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
	if (pGuild == NULL)
	{
		SSWarn << "role has no guild" << END;
		roRes.set_result(KKSG::ERR_GUILD_NOT_EXIST);
		return;
	}

	GuildBindInfo& bindInfo = pGuild->GetBindGroupInfo();
	// 绑定失败
	if (!bindInfo.IsBindGroup())
	{
		roRes.set_result(KKSG::ERR_GUILD_NOT_BIND);
		return;
	}
	roRes.set_group_name(bindInfo.GetQQGroupName());
}
