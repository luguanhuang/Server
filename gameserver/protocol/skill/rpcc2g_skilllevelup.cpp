#include "pch.h"
#include "skill/rpcc2g_skilllevelup.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/skillchangedrecord.h"
#include "guild/rpcg2m_opguildreqdata.h"
#include "mslink.h"
#include "gamelogic/rolesummarymgr.h"

// generate by ProtoGen at date: 2015/1/21 10:44:49

RPC_SERVER_IMPLEMETION(RpcC2G_SkillLevelup, SkillLevelupArg, SkillLevelupRes)

void RpcC2G_SkillLevelup::OnCall(const SkillLevelupArg &roArg, SkillLevelupRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole != NULL)
	{
		SkillChangedRecord Record(pRole);
		int result = pRole->GetRoleSkillLevelMgr()->TryLevelup(roArg.skillhash());
		if (0 == result)
		{
			pRole->OnSkillUp();
			RoleSummaryMgr::Instance()->UpdateRoleSummary(pRole, SUMMARY_SKILL);
		}
		roRes.set_errorcode(result);

		Record.SendNtf();
	}
	else
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
	}
}

void RpcC2G_SkillLevelup::OnDelayReplyRpc(const SkillLevelupArg &roArg, SkillLevelupRes &roRes, const CUserData &roUserData)
{
}
