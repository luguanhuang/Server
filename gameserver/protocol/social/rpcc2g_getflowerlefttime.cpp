#include "pch.h"
#include "social/rpcc2g_getflowerlefttime.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/flowerrecord.h"

// generate by ProtoGen at date: 2016/6/25 15:46:04

RPC_SERVER_IMPLEMETION(RpcC2G_GetFlowerLeftTime, GetFlowerLeftTimeArg, GetFlowerLeftTimeRes)

void RpcC2G_GetFlowerLeftTime::OnCall(const GetFlowerLeftTimeArg &roArg, GetFlowerLeftTimeRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL || !pRole->GetCurrScene())
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	CFlowerRecord* record = pRole->Get<CFlowerRecord>();
	if (!record)
	{
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}

	roRes.set_errorcode(KKSG::ERR_SUCCESS);
	roRes.set_lefttime(record->GetLeftFlowerTime());
	roRes.set_canget(roRes.lefttime() != (UINT32)(-1));
}

void RpcC2G_GetFlowerLeftTime::OnDelayReplyRpc(const GetFlowerLeftTimeArg &roArg, GetFlowerLeftTimeRes &roRes, const CUserData &roUserData)
{
}
