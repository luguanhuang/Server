#include "pch.h"
#include "guildbindinfo.h"
#include "guild.h"
#include "pb/project.pb.h"
#include "config.h"
#include "role/rolemanager.h"
#include "role/rolesummarymgr.h"
#include "role/role.h"
#include "role/rolesummary.h"
#include "httptask/unlinkgrouptask.h"
#include "httptask/httptaskmgr.h"
#include "idutil.h"

GuildBindInfo::GuildBindInfo(GuildBrief* brief)
:m_owner(brief)
{

}

void GuildBindInfo::LoadFromKKSG(const KKSG::GuildBindGroupInfo& info)
{
	m_qqGroupCode = info.qq_group_code();
	m_qqGroupOpenId = info.qq_group_openid();
	m_qqGroupName = info.qq_group_name();
	m_lastUpdateBindTime = info.last_update_time();
}

void GuildBindInfo::ToKKSG(KKSG::GuildBindGroupInfo& info)
{
	info.set_qq_group_code(m_qqGroupCode);
	info.set_qq_group_openid(m_qqGroupOpenId);
	info.set_qq_group_name(m_qqGroupName);
	info.set_last_update_time(m_lastUpdateBindTime);
}

bool GuildBindInfo::IsBindGroup()
{
	return !m_qqGroupCode.empty();
}

void GuildBindInfo::SetQQGroupCode(const std::string& gc)
{
	m_qqGroupCode = gc;
	m_owner->SetChange();
}

void GuildBindInfo::SetQQGroupName(const std::string& name)
{
	m_qqGroupName = name;
	m_owner->SetChange();
}

void GuildBindInfo::SetQQGroupOpenId(const std::string& openId)
{
	m_qqGroupOpenId = openId;
	m_owner->SetChange();
}

void GuildBindInfo::SetLastUpdateTime(UINT32 t)
{
	m_lastUpdateBindTime = t;
	m_owner->SetChange();
}

void GuildBindInfo::DoUnlinkWhenLeaderChange(UINT64 oldLeaderId, UINT64 newLeaderId)
{
	if (!IsBindGroup())
	{
		return;
	}
	LogInfo("Unlink QQ Group, guildid[%llu] old leader[%llu] new leader[%llu]", m_owner->m_ID, oldLeaderId, newLeaderId);
	UnlinkGroupTask* task = new UnlinkGroupTask;
	task->SetRpcId(0);
	task->SetGuild(m_owner->m_ID, m_owner->m_Name, GetServerIDByGuildID(m_owner->m_ID));
	task->SetRoleId(oldLeaderId);
	task->SetGroupCode(m_qqGroupCode);

	std::string openId, token;
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(oldLeaderId);
	if (pRole)
	{
		openId = pRole->GetAccount();
		token = pRole->GetToken();
	}
	else
	{
		CRoleSummary* su = CRoleSummaryMgr::Instance()->GetRoleSummary(oldLeaderId);
		if (su)
		{
			openId = su->GetAccount();
			token = su->GetToken();
		}
	}
	task->SetOpenId(openId);
	task->SetToken(token);
	LogInfo("unlink qq group use openid[%s] token[%s]", openId.c_str(), token.c_str());
	HttpTaskMgr::Instance()->AddTask(task);
}
