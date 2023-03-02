#include "pch.h"
#include "guild/rpcm2g_reqroleguildskill.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "guild/guildrecord.h"
#include "foreach.h"

// generate by ProtoGen at date: 2016/9/13 19:17:09

RPC_SERVER_IMPLEMETION(RpcM2G_ReqRoleGuildSkill, ReqRoleGuildSkillArg, ReqRoleGuildSkillRes)

void RpcM2G_ReqRoleGuildSkill::OnCall(const ReqRoleGuildSkillArg &roArg, ReqRoleGuildSkillRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if (pRole == NULL)
	{
		SSWarn << "pRole is NULL, roleid:" << roArg.roleid() << END;
		return ;
	}

	CGuildRecord* pGuildRecord = pRole->Get<CGuildRecord>();
	if (pGuildRecord == NULL)
	{
		return ;
	}

	const std::map<UINT32, UINT32>& guildSkillMap = pGuildRecord->GetGuildSkill();
	foreach (i in guildSkillMap)
	{
		KKSG::GuildSkillData* pData =  roRes.add_roleskills();
		pData->set_skillid(i->first);
		pData->set_maxlvl(i->second);
	}
}

void RpcM2G_ReqRoleGuildSkill::OnDelayReplyRpc(const ReqRoleGuildSkillArg &roArg, ReqRoleGuildSkillRes &roRes, const CUserData &roUserData)
{
}
