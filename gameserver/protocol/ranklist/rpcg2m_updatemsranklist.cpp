#include "pch.h"
#include "ranklist/rpcg2m_updatemsranklist.h"
#include "gamelogic/firstpassmgr.h"

// generate by ProtoGen at date: 2016/8/25 16:21:42

RPC_CLIENT_IMPLEMETION(RpcG2M_UpdateMSRankList, UpdateMSRankListArg, UpdateMSRankListRes)

void RpcG2M_UpdateMSRankList::OnReply(const UpdateMSRankListArg &roArg, const UpdateMSRankListRes &roRes, const CUserData &roUserData)
{
	FirstPassMgr::Instance()->OnUpdateMsRankReply(roArg, roRes);
}

void RpcG2M_UpdateMSRankList::OnTimeout(const UpdateMSRankListArg &roArg, const CUserData &roUserData)
{
}
