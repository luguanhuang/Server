#include "pch.h"
#include "battlewatch/rpcc2g_changelivevisible.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "config.h"
#include "live/liverecord.h"

// generate by ProtoGen at date: 2017/5/9 20:17:46

RPC_SERVER_IMPLEMETION(RpcC2G_ChangeLiveVisible, ChangeLiveVisibleArg, ChangeLiveVisibleRes)

void RpcC2G_ChangeLiveVisible::OnCall(const ChangeLiveVisibleArg &roArg, ChangeLiveVisibleRes &roRes)
{
	roRes.set_error(KKSG::ERR_FAILED);

	if (!GSConfig::Instance()->IsHallGS())
	{
		return;
	}

	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (!role)
	{
		return;
	}

	UINT32 error = role->Get<CLiveRecord>()->ChangeVisible(roArg.visible());
	roRes.set_error((ErrorCode)error);
}

void RpcC2G_ChangeLiveVisible::OnDelayReplyRpc(const ChangeLiveVisibleArg &roArg, ChangeLiveVisibleRes &roRes, const CUserData &roUserData)
{
}
