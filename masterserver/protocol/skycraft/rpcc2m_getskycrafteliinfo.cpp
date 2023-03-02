#include "pch.h"
#include "skycraft/rpcc2m_getskycrafteliinfo.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "skycraft/skycraftelimgr.h"

// generate by ProtoGen at date: 2017/4/11 15:17:27

RPC_SERVER_IMPLEMETION(RpcC2M_GetSkyCraftEliInfo, GetSkyCraftEliInfoArg, GetSkyCraftEliInfoRes)

void RpcC2M_GetSkyCraftEliInfo::OnCall(const GetSkyCraftEliInfoArg &roArg, GetSkyCraftEliInfoRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	roRes.set_result(KKSG::ERR_SUCCESS);
	SkyCraftEliMgr::Instance()->FillEliInfo(roRes);
	SkyCraftEliMgr::Instance()->AddWatcher(pRole->GetID());
}

void RpcC2M_GetSkyCraftEliInfo::OnDelayReplyRpc(const GetSkyCraftEliInfoArg &roArg, GetSkyCraftEliInfoRes &roRes, const CUserData &roUserData)
{
}
