#include "pch.h"
#include "findback/rpcc2g_expfindback.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "findback/findbackrecord.h"

// generate by ProtoGen at date: 2016/5/10 10:58:32

RPC_SERVER_IMPLEMETION(RpcC2G_ExpFindBack, ExpFindBackArg, ExpFindBackRes)

void RpcC2G_ExpFindBack::OnCall(const ExpFindBackArg &roArg, ExpFindBackRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL || !pRole->GetCurrScene())
	{
		roRes.set_error(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	CFindBackRecord* record = pRole->Get<CFindBackRecord>();
	if (!record)
	{
		roRes.set_error(KKSG::ERR_FAILED);
		return;
	}

	UINT32 error = record->GetFindBackReward(roArg.isfree());
	roRes.set_error((KKSG::ErrorCode)error);
}

void RpcC2G_ExpFindBack::OnDelayReplyRpc(const ExpFindBackArg &roArg, ExpFindBackRes &roRes, const CUserData &roUserData)
{
}
