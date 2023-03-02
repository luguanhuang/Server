#include "pch.h"
#include "social/rpcc2g_getflowerreward.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/flowerrecord.h"
#include "social/rpcg2m_msgetflwoerrankreward.h"
#include "network/mslink.h"

// generate by ProtoGen at date: 2016/6/25 17:04:39

RPC_SERVER_IMPLEMETION(RpcC2G_GetFlowerReward, GetFlowerRewardArg, GetFlowerRewardRes)

void RpcC2G_GetFlowerReward::OnCall(const GetFlowerRewardArg &roArg, GetFlowerRewardRes &roRes)
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

	bool canGet = record->CheckGetRankReward();
	if (!canGet)
	{
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}

	if (MSLink::Instance()->GetConnID() == INVALID_CONN_ID)
	{
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}

	RpcG2M_MSGetFlwoerRankReward* rpc = RpcG2M_MSGetFlwoerRankReward::CreateRpc();

	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_rpcid(DelayRpc());

	pRole->SendToMS(*rpc);
	//roRes.set_errorcode((ErrorCode)error);
}

void RpcC2G_GetFlowerReward::OnDelayReplyRpc(const GetFlowerRewardArg &roArg, GetFlowerRewardRes &roRes, const CUserData &roUserData)
{
	roRes.set_errorcode(KKSG::ErrorCode(roUserData.m_dwUserData));
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (!role)
	{
		return;
	}
	if(roRes.errorcode() == KKSG::ERR_SUCCESS && roUserData.m_pUserPtr != NULL)
	{
		MSGetFlowerRewardRes* poRes = (MSGetFlowerRewardRes*)roUserData.m_pUserPtr;
		roRes.set_errorcode((ErrorCode)role->Get<CFlowerRecord>()->GetRankReward(poRes->rank()));
	}
}
