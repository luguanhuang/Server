#include "pch.h"
#include "firstpass/rpcg2m_getrankinfo.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/firstpassrecord.h"

// generate by ProtoGen at date: 2016/8/24 21:04:28

RPC_CLIENT_IMPLEMETION(RpcG2M_GetRankInfo, GetRankInfoArg, GetRankInfoRes)

void RpcG2M_GetRankInfo::OnReply(const GetRankInfoArg &roArg, const GetRankInfoRes &roRes, const CUserData &roUserData)
{
	CUserData userData;
	userData.m_pUserPtr = (void *)&roRes;
	if (roArg.has_rpcid())
	{
		CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
	}
	else
	{
		if (roArg.ranktype() == KKSG::FirstPassRank)
		{
			Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
			if (role)
			{
				role->Get<CFirstPassRecord>()->OnFirstEnterSceneReply(roRes);
			}
		}
	}

}

void RpcG2M_GetRankInfo::OnTimeout(const GetRankInfoArg &roArg, const CUserData &roUserData)
{
}
