#include "pch.h"
#include "social/rpcc2g_getflower.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/flowerrecord.h"

// generate by ProtoGen at date: 2016/6/25 16:14:12

RPC_SERVER_IMPLEMETION(RpcC2G_GetFlower, GetFlowerArg, GetFlowerRes)

void RpcC2G_GetFlower::OnCall(const GetFlowerArg &roArg, GetFlowerRes &roRes)
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

	UINT32 error = record->GetFlower();
	roRes.set_errorcode((ErrorCode)error);
	if (error == KKSG::ERR_SUCCESS)
	{
		roRes.set_lefttime( GetGlobalConfig().GetFlowerTime[1]);
		if (record->GetFlowerSuccessCount() >=  GetGlobalConfig().GetFlowerNum)
		{
			roRes.set_canget(false);
		}
		else
			roRes.set_canget(true);
	}
}

void RpcC2G_GetFlower::OnDelayReplyRpc(const GetFlowerArg &roArg, GetFlowerRes &roRes, const CUserData &roUserData)
{
}
