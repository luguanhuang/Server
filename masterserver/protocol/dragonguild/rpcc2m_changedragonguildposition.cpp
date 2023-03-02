#include "pch.h"
#include "dragonguild/rpcc2m_changedragonguildposition.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "dragonguild/dragonguildmgr.h"

// generate by ProtoGen at date: 2017/9/13 19:59:43

RPC_SERVER_IMPLEMETION(RpcC2M_ChangeDragonGuildPosition, ChangeDragonGuildPositionArg, ChangeDragonGuildPositionRes)

void RpcC2M_ChangeDragonGuildPosition::OnCall(const ChangeDragonGuildPositionArg &roArg, ChangeDragonGuildPositionRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	if (pRole->GetID() == roArg.roleid() || !DragonGuildMgr::Instance()->IsInsameDragonGuild(pRole->GetID(), roArg.roleid()))
	{
		roRes.set_result(KKSG::ERR_DG_NOT_IN_SAME);
		return ;
	}

	DragonGuild* pDGuild = DragonGuildMgr::Instance()->GetDragonGuildByRole(pRole->GetID());
	if (pDGuild == NULL)
	{
		roRes.set_result(KKSG::ERR_DG_NOT_IN_GUILD);
		return ;
	}

	DragonGuildMember* pMember = pDGuild->GetMember(pRole->GetID());
	DragonGuildMember* pOther = pDGuild->GetMember(roArg.roleid());

	if (!pMember || !pOther)
	{
		roRes.set_result(KKSG::ERR_DG_NOT_IN_GUILD);
		return ;
	}

	if (!DragonGuildMgr::Instance()->HasPermission(pMember->position, DGEM_SET_VICELEADER))
	{
		roRes.set_result(KKSG::ERR_DG_NO_PERMISSION);
		return ;
	}

	KKSG::ErrorCode ret = pDGuild->ChangeMemberPosition(pOther, (DragonGuildPosition)roArg.position());
	roRes.set_result(ret);
}

void RpcC2M_ChangeDragonGuildPosition::OnDelayReplyRpc(const ChangeDragonGuildPositionArg &roArg, ChangeDragonGuildPositionRes &roRes, const CUserData &roUserData)
{
}
