#include "pch.h"
#include "social/rpcc2g_getflowerrewardlist.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/flowerrecord.h"

// generate by ProtoGen at date: 2016/6/25 18:04:23

RPC_SERVER_IMPLEMETION(RpcC2G_GetFlowerRewardList, GetFlowerRewardListArg, GetFlowerRewardListRes)

void RpcC2G_GetFlowerRewardList::OnCall(const GetFlowerRewardListArg &roArg, GetFlowerRewardListRes &roRes)
{
	/*Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
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

	RpcG2M_MSGetFlwoerRankReward* rpc = RpcG2M_MSGetFlwoerRankReward::CreateRpc();

	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_rpcid(DelayRpc());

	MSLink::Instance()->SendTo(*rpc);*/
}

void RpcC2G_GetFlowerRewardList::OnDelayReplyRpc(const GetFlowerRewardListArg &roArg, GetFlowerRewardListRes &roRes, const CUserData &roUserData)
{
}
