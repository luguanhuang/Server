#include "pch.h"
#include "superrisk/rpcc2g_riskbuyrequest.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/superriskrecord.h"

// generate by ProtoGen at date: 2016/9/9 12:13:57

RPC_SERVER_IMPLEMETION(RpcC2G_RiskBuyRequest, RiskBuyRequestArg, RiskBuyRequestRes)

void RpcC2G_RiskBuyRequest::OnCall(const RiskBuyRequestArg &roArg, RiskBuyRequestRes &roRes)
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

	UINT32 error = record->m_superRisk.RiskBuy();
	roRes.set_error((ErrorCode)error);
}

void RpcC2G_RiskBuyRequest::OnDelayReplyRpc(const RiskBuyRequestArg &roArg, RiskBuyRequestRes &roRes, const CUserData &roUserData)
{
}
