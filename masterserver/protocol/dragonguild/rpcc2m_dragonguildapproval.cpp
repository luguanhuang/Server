#include "pch.h"
#include "dragonguild/rpcc2m_dragonguildapproval.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "dragonguild/dragonguild.h"
#include "dragonguild/dragonguilddef.h"
#include "dragonguild/dragonguildmgr.h"
#include "dragonguild/dragonguildapps.h"

// generate by ProtoGen at date: 2017/9/7 17:22:05

RPC_SERVER_IMPLEMETION(RpcC2M_DragonGuildApproval, DragonGuildApprovalArg, DragonGuildApprovalRes)

void RpcC2M_DragonGuildApproval::OnCall(const DragonGuildApprovalArg &roArg, DragonGuildApprovalRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_result(ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	UINT64 dgid = DragonGuildMgr::Instance()->GetDragonGuildIdByRole(pRole->GetID());
	if (dgid == INVAILD_DRAGONGUILD)
	{
		roRes.set_result(KKSG::ERR_DG_NOT_IN_GUILD);
		return ;
	}
	
	DragonGuild* pDGuild = DragonGuildMgr::Instance()->GetDragonGuild(dgid);
	if (pDGuild == NULL)
	{
		roRes.set_result(KKSG::ERR_DG_DGUILD_NOT_EXIST);
		return ;
	}

	DragonGuildMember* m = pDGuild->GetMember(pRole->GetID());
	if (m == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return ;
	}

	if (!DragonGuildMgr::Instance()->HasPermission(m->position, DGEM_APPROVAL))
	{
		roRes.set_result(KKSG::ERR_DG_NO_PERMISSION);
		return ;
	}

	if (roArg.type() == DGA_ACCEPT)
	{
		KKSG::ErrorCode result;
		if ((result = pDGuild->GetDragonGuildApplication().Accept(roArg.roleid())) != KKSG::ERR_SUCCESS)
		{
			roRes.set_result(result);
			return ;
		}
	}
	else if(roArg.type() == DGA_REJECT)
	{
		pDGuild->GetDragonGuildApplication().Reject(roArg.roleid());
	}
	else
	{
		pDGuild->GetDragonGuildApplication().RejectAll();
	}
	roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcC2M_DragonGuildApproval::OnDelayReplyRpc(const DragonGuildApprovalArg &roArg, DragonGuildApprovalRes &roRes, const CUserData &roUserData)
{
}
