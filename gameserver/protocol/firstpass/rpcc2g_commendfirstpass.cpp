#include "pch.h"
#include "firstpass/rpcc2g_commendfirstpass.h"
#include "firstpass/rpcg2m_mscommendfirstpass.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "firstpass/rpcg2m_getrankinfo.h"
#include "gamelogic/firstpassrecord.h"
#include "network/mslink.h"

// generate by ProtoGen at date: 2016/8/24 17:24:53

RPC_SERVER_IMPLEMETION(RpcC2G_CommendFirstPass, CommendFirstPassArg, CommendFirstPassRes)

void RpcC2G_CommendFirstPass::OnCall(const CommendFirstPassArg &roArg, CommendFirstPassRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL || !pRole->GetCurrScene())
	{
		roRes.set_error(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	if (MSLink::Instance()->GetConnID() == INVALID_CONN_ID)
	{
		roRes.set_error(KKSG::ERR_FAILED);
		return;
	}

	//pRole->Get<CFirstPassRecord>()->BeforeGetCommendReward(roArg.firstpassid());

	RpcG2M_MSCommendFirstPass* rpc = RpcG2M_MSCommendFirstPass::CreateRpc();
	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_rpcid(DelayRpc());
	rpc->m_oArg.set_fistpassid(roArg.firstpassid());
	rpc->m_oArg.set_isgettopinfo(true);
	rpc->m_oArg.set_serverid(pRole->GetServerId());

	pRole->SendToMS(*rpc);
}

void RpcC2G_CommendFirstPass::OnDelayReplyRpc(const CommendFirstPassArg &roArg, CommendFirstPassRes &roRes, const CUserData &roUserData)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL || !pRole->GetCurrScene())
	{
		roRes.set_error(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	UINT32 error = roUserData.m_dwUserData;
	roRes.set_error((KKSG::ErrorCode)error);
	if (!roUserData.m_pUserPtr)
	{
		//pRole->Get<CFirstPassRecord>()->RevertGetCommendReward(roArg.firstpassid());
		return;
	}
	MSCommendFirstPassRes* poRes = (MSCommendFirstPassRes*)roUserData.m_pUserPtr;
	if (error == KKSG::ERR_SUCCESS)
	{
		if (poRes->has_starlevel())
		{
			/*error = pRole->Get<CFirstPassRecord>()->CheckCommend(roArg.firstpassid(), roRes.starlevel());
			if (error != KKSG::ERR_SUCCESS)
			{
				roRes.set_error((KKSG::ErrorCode)error);
				return;
			}*/

			error = pRole->Get<CFirstPassRecord>()->GetCommendReward(roArg.firstpassid(), poRes->starlevel());
			roRes.set_commendnum(poRes->commendnum());
			roRes.set_error((KKSG::ErrorCode)error);
		}
	}
	else
	{
		//pRole->Get<CFirstPassRecord>()->RevertGetCommendReward(roArg.firstpassid());
	}
}
