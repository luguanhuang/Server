#include "pch.h"
#include "bigmelee/rpcm2k_querybigmeleerank2k.h"

// generate by ProtoGen at date: 2017/6/20 16:34:35

RPC_CLIENT_IMPLEMETION(RpcM2K_QueryBigMeleeRank2k, QueryMayhemRank2kArg, QueryMayhemRankRes)

void RpcM2K_QueryBigMeleeRank2k::OnReply(const QueryMayhemRank2kArg &roArg, const QueryMayhemRankRes &roRes, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)roRes.err();
	oUser.m_pUserPtr = (void*)&roRes;
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}

void RpcM2K_QueryBigMeleeRank2k::OnTimeout(const QueryMayhemRank2kArg &roArg, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)(KKSG::ERR_TIMEOUT);
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}
