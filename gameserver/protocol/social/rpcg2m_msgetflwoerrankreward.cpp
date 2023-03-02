#include "pch.h"
#include "social/rpcg2m_msgetflwoerrankreward.h"
#include "gamelogic/flowerrecord.h"
#include "unit/role.h"
#include "unit/rolemanager.h"

// generate by ProtoGen at date: 2016/8/18 11:28:28

RPC_CLIENT_IMPLEMETION(RpcG2M_MSGetFlwoerRankReward, MSGetFlowerRewardArg, MSGetFlowerRewardRes)

void RpcG2M_MSGetFlwoerRankReward::OnReply(const MSGetFlowerRewardArg &roArg, const  MSGetFlowerRewardRes&roRes, const CUserData &roUserData)
{
	CUserData oUserData;
	oUserData.m_dwUserData = roRes.error();
	oUserData.m_pUserPtr = (void*)&roRes;

	if (roArg.has_rpcid())
	{
		CRpc::ReplyDelayRpc(roArg.rpcid(), oUserData);
	}
	else if (roRes.error() == KKSG::ERR_SUCCESS)
	{
		Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
		if (role)
		{
			role->Get<CFlowerRecord>()->OnFirstEnterSceneReply(roRes.rank(), roRes.totalrank());
		}
		else
		{
			LogError("session:%llu, rank :%d, totalrank:%d exit", m_sessionID, roRes.rank(), roRes.totalrank());
		}
	}
}

void RpcG2M_MSGetFlwoerRankReward::OnTimeout(const MSGetFlowerRewardArg &roArg, const CUserData &roUserData)
{
}
