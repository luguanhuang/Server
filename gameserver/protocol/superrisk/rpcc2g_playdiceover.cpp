#include "pch.h"
#include "superrisk/rpcc2g_playdiceover.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/superriskrecord.h"

// generate by ProtoGen at date: 2016/9/8 12:30:52

RPC_SERVER_IMPLEMETION(RpcC2G_PlayDiceOver, PlayDiceOverArg, PlayDiceOverRes)

void RpcC2G_PlayDiceOver::OnCall(const PlayDiceOverArg &roArg, PlayDiceOverRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL || !pRole->GetCurrScene())
	{
		roRes.set_error(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	roRes.set_error(KKSG::ERR_SUCCESS);
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

	if(record->m_superRisk.DiceOver(roRes))
	{
		roRes.set_error((ErrorCode)ERR_SUCCESS);
	}
	else
		roRes.set_error((ErrorCode)ERR_FAILED);
}

void RpcC2G_PlayDiceOver::OnDelayReplyRpc(const PlayDiceOverArg &roArg, PlayDiceOverRes &roRes, const CUserData &roUserData)
{
}
