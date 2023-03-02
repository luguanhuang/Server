#include "pch.h"
#include "superrisk/rpcc2g_playdicerequest.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/superriskrecord.h"

// generate by ProtoGen at date: 2016/9/5 16:11:14

RPC_SERVER_IMPLEMETION(RpcC2G_PlayDiceRequest, PlayDiceRequestArg, PlayDiceRequestRes)

void RpcC2G_PlayDiceRequest::OnCall(const PlayDiceRequestArg &roArg, PlayDiceRequestRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL || !pRole->GetCurrScene())
	{
		roRes.set_error(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	roRes.set_error(KKSG::ERR_FAILED);
	if(pRole->CheckIdipAndNotify(KKSG::PUNISH_USER_DAILY_PLAY))
	{
		return ;
	}

	CSuperRiskRecord* record = pRole->Get<CSuperRiskRecord>();
	if (!record)
	{
		roRes.set_error(KKSG::ERR_FAILED);
		return;
	}

	int finalvalue = 0;
	if(record->m_superRisk.PlayDice(roArg.mapid(), roArg.randvalue(), finalvalue))
	{
		roRes.set_error(KKSG::ERR_SUCCESS);
		roRes.set_getvalue(finalvalue);
		roRes.set_leftdicetime(record->GetLeftDiceTime());
		return;
	}
	roRes.set_error(KKSG::ERR_FAILED);
}

void RpcC2G_PlayDiceRequest::OnDelayReplyRpc(const PlayDiceRequestArg &roArg, PlayDiceRequestRes &roRes, const CUserData &roUserData)
{
}
