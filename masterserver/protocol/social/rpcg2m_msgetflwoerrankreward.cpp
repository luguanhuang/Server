#include "pch.h"
#include "social/rpcg2m_msgetflwoerrankreward.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "ranklist/flowerranklist.h"
#include "ranklist/ranklistmgr.h"

// generate by ProtoGen at date: 2016/8/18 11:28:28

RPC_SERVER_IMPLEMETION(RpcG2M_MSGetFlwoerRankReward, MSGetFlowerRewardArg, MSGetFlowerRewardRes)

void RpcG2M_MSGetFlwoerRankReward::OnCall(const MSGetFlowerRewardArg &roArg, MSGetFlowerRewardRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (!role)
	{
		roRes.set_error(KKSG::ERR_UNKNOWN);
		return;
	}
	FlowerRankList* list = (FlowerRankList*)CRankListMgr::Instance()->GetRankList(KKSG::FlowerYesterdayRank);
	UINT32 rank = INVALID_RANK;
	if (list)
		rank = list->GetRank(role->GetID());

	//roRes.set_error((ErrorCode)error);
	roRes.set_rank(rank);

	list = (FlowerRankList*)CRankListMgr::Instance()->GetRankList(KKSG::FlowerTotalRank);
	rank = INVALID_RANK;
	if (list)
		rank = list->GetRank(role->GetID());

	//roRes.set_error((ErrorCode)error);
	roRes.set_totalrank(rank);
	roRes.set_error(KKSG::ERR_SUCCESS);
}

void RpcG2M_MSGetFlwoerRankReward::OnDelayReplyRpc(const MSGetFlowerRewardArg &roArg, MSGetFlowerRewardRes &roRes, const CUserData &roUserData)
{
}
