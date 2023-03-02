#include "pch.h"
#include "skyteam/rpcc2m_leaveskyteam.h"
#include "role/rolemanager.h"
#include "skyteam/skyteam.h"
#include "skyteam/skyteammgr.h"

// generate by ProtoGen at date: 2017/3/31 15:03:43

RPC_SERVER_IMPLEMETION(RpcC2M_LeaveSkyTeam, LeaveSkyTeamArg, LeaveSkyTeamRes)

void RpcC2M_LeaveSkyTeam::OnCall(const LeaveSkyTeamArg &roArg, LeaveSkyTeamRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	int ret = SkyTeamMgr::Instance()->LeaveMember(pRole->GetID());
	roRes.set_result((KKSG::ErrorCode)ret);
}

void RpcC2M_LeaveSkyTeam::OnDelayReplyRpc(const LeaveSkyTeamArg &roArg, LeaveSkyTeamRes &roRes, const CUserData &roUserData)
{
}
