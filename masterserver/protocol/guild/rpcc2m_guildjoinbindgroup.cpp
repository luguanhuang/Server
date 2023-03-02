#include "pch.h"
#include "guild/rpcc2m_guildjoinbindgroup.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildbindinfo.h"
#include "timeutil.h"
#include "httptask/httptaskmgr.h"
#include "httptask/joingrouptask.h"

// generate by ProtoGen at date: 2016/10/10 15:55:03

RPC_SERVER_IMPLEMETION(RpcC2M_GuildJoinBindGroup, GuildJoinBindGroupReq, GuildJoinBindGroupRes)

void RpcC2M_GuildJoinBindGroup::OnCall(const GuildJoinBindGroupReq &roArg, GuildJoinBindGroupRes &roRes)
{
	roRes.set_result(KKSG::ERR_UNKNOWN);
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		SSWarn << "role is NULL and session is :" << m_sessionID <<END;
		return;
	}
	// 平台判断

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
	if (pGuild == NULL)
	{
		SSWarn << "role has no guild" << END;
		roRes.set_result(KKSG::ERR_GUILD_NOT_EXIST);
		return;
	}

	GuildBindInfo& bindInfo = pGuild->GetBindGroupInfo();
	// 公会尚未绑定qq群
	if (!bindInfo.IsBindGroup())
	{
		SSWarn << "not bind " << END;
		roRes.set_result(KKSG::ERR_GUILD_NOT_BIND);
		return;
	}
	GuildMember* member = pGuild->GetMember(role->GetID());
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
	task->SetGuild(pGuild->GetID(), pGuild->GetName(), pGuild->GetServerId());
	task->SetRoleId(role->GetID());
	task->SetGroupCode(bindInfo.GetQQGroupCode());
	HttpTaskMgr::Instance()->AddTask(task);
}

void RpcC2M_GuildJoinBindGroup::OnDelayReplyRpc(const GuildJoinBindGroupReq &roArg, GuildJoinBindGroupRes &roRes, const CUserData &roUserData)
{
	roRes.set_result((KKSG::ErrorCode)roUserData.m_dwUserData);
}
