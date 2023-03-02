#include "pch.h"
#include "guildbindgrouphelper.h"
#include "httptask/qqgrouptaskbase.h"
#include "guildmgr.h"
#include "guild.h"
#include "guildbindinfo.h"
#include "timeutil.h"
#include "define.h"
#include "httptask/getgrouprelation.h"
#include "httptask/httptaskmgr.h"
#include "httptask/getgroupdetailtask.h"
#include "loghelper/reportdatamgr.h"

#define GET_GUILD \
	Guild* guild = CGuildMgr::Instance()->GetGuild(task->m_guildId); \
	if (!guild)	\
	{ \
		SSError << "cannot found guild: " << task->m_guildId << END; \
		return;\
	}

#define GET_MEMBER \
	GuildMember* member = guild->GetMember(task->m_roleId);	\
	if (!member)\
	{\
		SSError << "not in guild, roleId: " << task->m_roleId << END;\
		return;\
	}

static void UnlinkMember(GuildMember* member, void* arg)
{
	member->groupPos = KKSG::GBS_NotBind;
}

void BindGroupHelper::OnCreateBindSuccess(QQGroupTaskBase* task, const std::string& groupCode, const std::string& groupName)
{
	GET_GUILD;
	GET_MEMBER;
	SSInfo << "Create Guild Group, roleId: " << task->m_roleId << ", guildId: " << task->m_guildId
		   << ", groupCode: " << groupCode << ", groupName: " << groupName << END;
	UINT32 now = TimeUtil::GetTime();
	member->updateBindTime = now;
	member->groupPos = KKSG::GBS_Owner;

	GuildBindInfo& info = guild->GetBindGroupInfo();
	info.SetQQGroupCode(groupCode);
	info.SetQQGroupName(groupName);

	// 绑定之后立即查询也没用。。。
	CUserData data;
	data.m_dwUserData = 0;
	data.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(task->m_rpcId, data);

	CReportDataMgr::Instance()->GuildBindQQ(task->m_roleId, now);
}

void BindGroupHelper::OnCreateBindFailed(QQGroupTaskBase* task, INT32 code)
{
	SSError << "bind qq group failed, roleId: " << task->m_roleId  << ", guildName" << task->m_guildName << ", code: " << code << END;
	CUserData data;
	data.m_dwUserData = KKSG::ERR_UNKNOWN;
	data.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(task->m_rpcId, data);
}

void BindGroupHelper::OnGetRelationSuccess(QQGroupTaskBase* task, UINT32 relation)
{
	GET_GUILD;
	GET_MEMBER;
	member->updateBindTime = TimeUtil::GetTime();
	bool isBind = true;
	switch (relation)
	{
	case 1:
		member->groupPos = KKSG::GBS_Owner;
		break;
	case 2:
		member->groupPos = KKSG::GBS_Admin;
		break;
	case 3:
		member->groupPos = KKSG::GBS_Member;
		break;
	case 4:
	{
		member->groupPos = KKSG::GBS_NotMember;
		// 会长都不是群成员了，那么这个群??
		if (guild->isLeader(task->m_roleId))
		{
			isBind = false;
		}
	}
		break;
	default:
		SSWarn << "Unkown relation: " << relation << ", roleId: " << task->m_roleId << ", guildId: " << task->m_guildId << END;
		break;
	}
	SSInfo << "GetRelation Success, roleId: " << task->m_roleId << ", guildId: " << task->m_guildId
		   << ", GuildName: " << task->m_guildName << ", relation: " << relation << END;


	CUserData data;
	data.m_dwUserData = 0;
	data.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(task->m_rpcId, data);
}

void BindGroupHelper::OnGetRelationFailed(QQGroupTaskBase* task, INT32 code)
{
	SSError << "Get Relation Failed, roleId: " << task->m_roleId << ", guildName: " << task->m_guildName << ", code: " << code << END;
	CUserData data;
	data.m_dwUserData = 0;
	data.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(task->m_rpcId, data);
}

void BindGroupHelper::OnJoinGroupSuccess(QQGroupTaskBase* task)
{
	GET_GUILD;
	GET_MEMBER;
	SSInfo << "Join Guild Group, roleId: " << task->m_roleId << ", guildId: " << guild->GetID() 
		   << ", guildName: " << task->m_guildName << END;

	member->updateBindTime = TimeUtil::GetTime();
	member->groupPos = KKSG::GBS_Member;

	CUserData data;
	data.m_dwUserData = 0;
	data.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(task->m_rpcId, data);
}

void BindGroupHelper::OnJoinGroupFailed(QQGroupTaskBase* task, INT32 code, INT32 platCode)
{
	SSError << "join group failed, roleId: " << task->m_roleId << ", code: " << code << ", platCode: " << platCode << END;
	GET_GUILD;
	CUserData data;
	data.m_dwUserData = KKSG::ERR_UNKNOWN;

	// 未绑定群
	if (platCode == 221002)
	{
		GuildBindInfo& info = guild->GetBindGroupInfo();
		info.SetQQGroupCode("");
		info.SetQQGroupName("");
		info.SetQQGroupOpenId("");
		guild->ForeachMember(UnlinkMember, NULL);
		data.m_dwUserData = KKSG::ERR_GUILD_NOT_BIND;
	}

	data.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(task->m_rpcId, data);
}

void BindGroupHelper::OnUnlinkGroupSuccess(QQGroupTaskBase* task, const std::string& groupCode)
{
	GET_GUILD;
	SSInfo << "Unlink Guild Group, roleId: " << task->m_roleId << ", guildId: " << guild->GetID() 
		   << "GuildName: " << task->m_guildName << ", groupCode:" << groupCode << END;

	GuildBindInfo& info = guild->GetBindGroupInfo();
	info.SetQQGroupCode("");
	info.SetQQGroupName("");
	info.SetQQGroupOpenId("");
	guild->ForeachMember(UnlinkMember, NULL);

	if (task->m_rpcId)
	{
		CUserData data;
		data.m_dwUserData = 0;
		data.m_pUserPtr = NULL;
		CRpc::ReplyDelayRpc(task->m_rpcId, data);
	}
}

void BindGroupHelper::OnUnlinkGroupFailed(QQGroupTaskBase* task, INT32 code)
{
	SSInfo << "Unlink Guild Group Failed, roleId: " << task->m_roleId << ", guildId: " << task->m_guildId << ", code:" << code << END;
	if (task->m_rpcId)
	{
		CUserData data;
		data.m_dwUserData = KKSG::ERR_UNKNOWN;
		data.m_pUserPtr = NULL;
		CRpc::ReplyDelayRpc(task->m_rpcId, data);
	}
}

void BindGroupHelper::OnGetGroupDetailSuccess(QQGroupTaskBase* task, const std::string& groupName, const std::string& groupOpenId)
{
	GET_GUILD;
	GET_MEMBER;
	SSInfo << "Get Group Detail, roleId: " << task->m_roleId << ", guildId: " << guild->GetID()
		   << ", GroupName: " << groupName << ", GroupOpenId: " << groupOpenId << END;
	GuildBindInfo& info = guild->GetBindGroupInfo();
	info.SetQQGroupName(groupName);
	info.SetQQGroupOpenId(groupOpenId);
	info.SetLastUpdateTime(TimeUtil::GetTime());

	// 成功之后拉取群关系
	member->updateBindTime = TimeUtil::GetTime();
	GetGroupRelationTask* ntask = new GetGroupRelationTask;
	ntask->SetRpcId(task->m_rpcId);
	ntask->SetOpenId(task->m_openId);
	ntask->SetToken(task->m_token);
	ntask->SetGuild(guild->GetID(), guild->GetName(), guild->GetServerId());
	ntask->SetRoleId(task->m_roleId);
	ntask->SetGroupCode(info.GetQQGroupCode());
	ntask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(ntask);
}

void BindGroupHelper::OnGetGroupDetailFailed(QQGroupTaskBase* task, INT32 code, INT32 platCode)
{
	GET_GUILD;
	SSInfo << "Get Group Detail Failed, roleId: " << task->m_roleId << ", guildId: " << task->m_guildId
		   << ", GuildName: " << task->m_guildName << ", code: " << code << ", platCode: " << platCode << END;

	// 根据platCode，做群绑定状态容错
	if (platCode == 2002)
	{
		GuildBindInfo& info = guild->GetBindGroupInfo();
		info.SetQQGroupCode("");
		info.SetQQGroupName("");
		info.SetQQGroupOpenId("");
		guild->ForeachMember(UnlinkMember, NULL);
	}
	CUserData data;
	data.m_dwUserData = 0;
	data.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(task->m_rpcId, data);
}
