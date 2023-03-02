#include "pch.h"
#include "team/rpcc2g_teaminvitelistreq.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/teaminvitemgr.h"

// generate by ProtoGen at date: 2016/7/6 10:28:07

RPC_SERVER_IMPLEMETION(RpcC2G_TeamInviteListReq, TeamInviteArg, TeamInviteRes)

void RpcC2G_TeamInviteListReq::OnCall(const TeamInviteArg &roArg, TeamInviteRes &roRes)
{
	if(!GSConfig::Instance()->IsHallGS())
	{
		LogWarn("Receive request not in hall gs");
		roRes.set_errcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	roRes.set_errcode(KKSG::ERR_SUCCESS);
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	
	TeamInviteMgr::Instance()->FillInviteList(pRole, roArg.expid(), roRes);

}

void RpcC2G_TeamInviteListReq::OnDelayReplyRpc(const TeamInviteArg &roArg, TeamInviteRes &roRes, const CUserData &roUserData)
{
}
