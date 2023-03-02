#include "pch.h"
#include "unit/rolemanager.h"
#include "spactivity/spactivitydef.h"
#include "spactivity/spactivitymgr.h"
#include "spactivity/spactivitysmall.h"
#include "spactivity/rpcc2g_lotterydraw.h"

// generate by ProtoGen at date: 2017/9/12 20:48:58

RPC_SERVER_IMPLEMETION(RpcC2G_LotteryDraw, LotteryDrawReq, LotteryDrawRes)

void RpcC2G_LotteryDraw::OnCall(const LotteryDrawReq &roArg, LotteryDrawRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		SSError << "Cannot Find Role, SessionID:" << m_sessionID << END;
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	SpActivityBase* pBase = SpActivityMgr::Instance()->GetSpActivity(SpActivity_LuckyTurntable);
	if (pBase == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_ACT_NOT_OPEN);
		return;
	}
	static_cast<SpActivityLuckyTurntable*>(pBase)->LotterDraw(roRes, pRole);
}

void RpcC2G_LotteryDraw::OnDelayReplyRpc(const LotteryDrawReq &roArg, LotteryDrawRes &roRes, const CUserData &roUserData)
{
}
