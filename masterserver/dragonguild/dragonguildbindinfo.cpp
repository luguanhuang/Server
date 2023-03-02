#include "pch.h"
#include "dragonguildbindinfo.h"
#include "dragonguild.h"
#include "pb/project.pb.h"
#include "config.h"
#include "role/rolemanager.h"
#include "role/rolesummarymgr.h"
#include "role/role.h"
#include "role/rolesummary.h"
#include "httptask/unlinkgrouptask.h"
#include "httptask/httptaskmgr.h"
#include "idutil.h"

DragonGuildBindInfo::DragonGuildBindInfo(DragonGuild* pDGuild)
:DragonGuildBase(pDGuild)
{

}

bool DragonGuildBindInfo::LoadFromDB(const KKSG::DragonGuildSaveData& data)
{
	KKSG::GuildBindGroupInfo info;
	if (!info.ParseFromString(data.bindinfo()))
	{
		return false;
	}
	m_qqGroupCode = info.qq_group_code();
	m_qqGroupOpenId = info.qq_group_openid();
	m_qqGroupName = info.qq_group_name();
	m_lastUpdateBindTime = info.last_update_time();
	return true;
}

void DragonGuildBindInfo::SaveToDB(KKSG::DragonGuildSaveData& data)
{
	KKSG::GuildBindGroupInfo info;
	info.set_qq_group_code(m_qqGroupCode);
	info.set_qq_group_openid(m_qqGroupOpenId);
	info.set_qq_group_name(m_qqGroupName);
	info.set_last_update_time(m_lastUpdateBindTime);
	info.SerializeToString(data.mutable_bindinfo());
}

bool DragonGuildBindInfo::IsBindGroup()
{
	return !m_qqGroupCode.empty();
}

void DragonGuildBindInfo::SetQQGroupCode(const std::string& gc)
{
	m_qqGroupCode = gc;
	SetDirty();
}

void DragonGuildBindInfo::SetQQGroupName(const std::string& name)
{
	m_qqGroupName = name;
	SetDirty();
}

void DragonGuildBindInfo::SetQQGroupOpenId(const std::string& openId)
{
	m_qqGroupOpenId = openId;
	SetDirty();
}

void DragonGuildBindInfo::SetLastUpdateTime(UINT32 t)
{
	m_lastUpdateBindTime = t;
	SetDirty();
}

void DragonGuildBindInfo::DoUnlinkWhenLeaderChange(UINT64 oldLeaderId, UINT64 newLeaderId)
{
	if (!IsBindGroup())
	{
		return;
	}
	LogInfo("Unlink QQ Group, guildid[%llu] old leader[%llu] new leader[%llu]", m_owner->GetID(), oldLeaderId, newLeaderId);
	UnlinkGroupTask* task = new UnlinkGroupTask;
	task->SetRpcId(0);
	task->SetGuild(m_owner->GetID(), m_owner->GetName(), GetServerIDByGuildID(m_owner->GetID()) );
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
