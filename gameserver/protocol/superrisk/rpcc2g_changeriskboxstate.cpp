#include "pch.h"
#include "superrisk/rpcc2g_changeriskboxstate.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/superriskrecord.h"

// generate by ProtoGen at date: 2016/9/5 16:18:08

RPC_SERVER_IMPLEMETION(RpcC2G_ChangeRiskBoxState, ChangeRiskBoxStateArg, ChangeRiskBoxStateRes)

void RpcC2G_ChangeRiskBoxState::OnCall(const ChangeRiskBoxStateArg &roArg, ChangeRiskBoxStateRes &roRes)
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

	UINT32 error = record->ChangeBoxState(roArg.mapid(), roArg.slot(), roArg.deststate(), roRes);
	roRes.set_error((ErrorCode)error);
}

void RpcC2G_ChangeRiskBoxState::OnDelayReplyRpc(const ChangeRiskBoxStateArg &roArg, ChangeRiskBoxStateRes &roRes, const CUserData &roUserData)
{
}
