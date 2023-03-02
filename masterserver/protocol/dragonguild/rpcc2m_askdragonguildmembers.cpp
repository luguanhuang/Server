#include "pch.h"
#include "dragonguild/rpcc2m_askdragonguildmembers.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "network/gslink.h"
#include "dragonguild/dragonguild.h"
#include "dragonguild/dragonguildmgr.h"
#include "dragonguild/dragonguilddef.h"

// generate by ProtoGen at date: 2017/9/8 16:23:48

RPC_SERVER_IMPLEMETION(RpcC2M_AskDragonGuildMembers, DragonGuildMemberArg, DragonGuildMemberRes)

void RpcC2M_AskDragonGuildMembers::OnCall(const DragonGuildMemberArg &roArg, DragonGuildMemberRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}
	if (pRole->IsInCrossGs())
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	UINT64 dgid = roArg.guildid();
	if (0 == dgid)
	{
		dgid = DragonGuildMgr::Instance()->GetDragonGuildIdByRole(pRole->GetID());
		if (dgid == INVAILD_DRAGONGUILD)
		{
			roRes.set_result(KKSG::ERR_DG_NOT_IN_GUILD);
			return ;
		}
	}

	DragonGuild* pdguild = DragonGuildMgr::Instance()->GetDragonGuild(dgid);
	if (pdguild == NULL)
	{
		roRes.set_result(KKSG::ERR_DG_DGUILD_NOT_EXIST);
		return ;
	}

	roRes.set_result(KKSG::ERR_SUCCESS);
	for (auto it = pdguild->GetAllMember().begin(); it != pdguild->GetAllMember().end(); ++ it)
	{
		CRoleSummary *pRoleSum = CRoleSummaryMgr::Instance()->GetRoleSummary(it->second.roleId);
		if (pRoleSum == NULL)
		{
			continue;
		}
		KKSG::DragonGuildMembersInfo* info = roRes.add_members();
		info->set_name(pRoleSum->GetName());
		info->set_roleid(pRoleSum->GetID());
		info->set_position(it->second.position);
		info->set_ppt(pRoleSum->GetPowerPoint());
		info->set_level(pRoleSum->GetLevel());
		info->set_profession((KKSG::RoleType)pRoleSum->GetProfession());
		info->set_vip(pRoleSum->GetVipLevel());
		info->set_lastlogin(pRoleSum->GetLastLogout());
		info->set_paymemberid(pRoleSum->GetPayMemberID());
		info->set_isonline(CRoleManager::Instance()->IsOnline(it->second.roleId));
		info->set_activity(pdguild->GetDragonGuildLiveness().GetRoleActivity(pRoleSum->GetID()));
		info->set_title(pRoleSum->GetTitleID());
	}
}

void RpcC2M_AskDragonGuildMembers::OnDelayReplyRpc(const DragonGuildMemberArg &roArg, DragonGuildMemberRes &roRes, const CUserData &roUserData)
{
}
