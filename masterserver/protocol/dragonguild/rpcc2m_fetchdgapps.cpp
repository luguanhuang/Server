#include "pch.h"
#include "dragonguild/rpcc2m_fetchdgapps.h"
#include "dragonguild/dragonguildapps.h"
#include "dragonguild/dragonguild.h"
#include "dragonguild/dragonguildmgr.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "dragonguild/dragonguilddef.h"
#include "tablemgr/dragonguildtablemgr.h"

// generate by ProtoGen at date: 2017/9/7 15:02:17

RPC_SERVER_IMPLEMETION(RpcC2M_FetchDGApps, FetchDGAppArg, FetchDGAppRes)

void RpcC2M_FetchDGApps::OnCall(const FetchDGAppArg &roArg, FetchDGAppRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	UINT64 gid = DragonGuildMgr::Instance()->GetDragonGuildIdByRole(pRole->GetID());
	if (gid == INVAILD_DRAGONGUILD)
	{
		roRes.set_result(KKSG::ERR_DG_NOT_IN_GUILD);
		return;
	}

	DragonGuild* pDGuild = DragonGuildMgr::Instance()->GetDragonGuild(gid);
	if (NULL == pDGuild)
	{
		roRes.set_result(KKSG::ERR_DG_DGUILD_NOT_EXIST);
		return;
	}
	
	DragonGuildMember *m = pDGuild->GetMember(pRole->GetID());
	if ( m == NULL || !DragonGuildMgr::Instance()->HasPermission(m->position, DGEM_APPROVAL))
	{
		roRes.set_result(KKSG::ERR_DG_NO_PERMISSION);
		return ;
	}

	pDGuild->GetDragonGuildApplication().GetApplicationInfo(roRes);
	pDGuild->GetDragonGuildApplication().ClearDeleted();

	roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcC2M_FetchDGApps::OnDelayReplyRpc(const FetchDGAppArg &roArg, FetchDGAppRes &roRes, const CUserData &roUserData)
{
}
