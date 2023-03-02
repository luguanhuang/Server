#include "pch.h"
#include "battle/rpcc2g_doodaditemallskillreq.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "buff/buffrecord.h"

// generate by ProtoGen at date: 2017/9/23 15:55:01

RPC_SERVER_IMPLEMETION(RpcC2G_DoodadItemAllSkillReq, EmptyData, DoodadItemAllSkill)

void RpcC2G_DoodadItemAllSkillReq::OnCall(const EmptyData &roArg, DoodadItemAllSkill &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if(pRole && pRole->Get<BuffRecord>())
	{
		pRole->Get<BuffRecord>()->FillItemAllSkills(roRes);
	}
}

void RpcC2G_DoodadItemAllSkillReq::OnDelayReplyRpc(const EmptyData &roArg, DoodadItemAllSkill &roRes, const CUserData &roUserData)
{
}
