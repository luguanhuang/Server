#include "pch.h"
#include "common/rpcc2g_queryopengameactivitytime.h"
#include "gamelogic/AchivementActivityManager.h"
#include "unit/rolemanager.h"
#include "unit/role.h"

// generate by ProtoGen at date: 2015/9/9 15:12:34

RPC_SERVER_IMPLEMETION(RpcC2G_QueryOpenGameActivityTime, QueryOpenGameArg, QueryOpenGameRes)

void RpcC2G_QueryOpenGameActivityTime::OnCall(const QueryOpenGameArg &roArg, QueryOpenGameRes &roRes)
{
	roRes.set_errcode(KKSG::ERR_SUCCESS);
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if(NULL == pRole)
	{
		roRes.set_errcode(KKSG::ERR_INVALID_REQUEST);
		return ;
	}
	roRes.set_timeleft((UINT32)AchivementActivityManager::Instance()->GetActivityTimeLeft());
}

void RpcC2G_QueryOpenGameActivityTime::OnDelayReplyRpc(const QueryOpenGameArg &roArg, QueryOpenGameRes &roRes, const CUserData &roUserData)
{
}
