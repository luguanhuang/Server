#include "pch.h"
#include "role/rpcm2d_readaccountdatanew.h"

// generate by ProtoGen at date: 2017/10/25 15:48:37

RPC_CLIENT_IMPLEMETION(RpcM2D_ReadAccountDataNew, ReadAccountDataNewArg, ReadAccountDataNewRes)

void RpcM2D_ReadAccountDataNew::OnReply(const ReadAccountDataNewArg &roArg, const ReadAccountDataNewRes &roRes, const CUserData &roUserData)
{
	CUserData userData(roRes.result(), (void*)&roRes);
	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
}

void RpcM2D_ReadAccountDataNew::OnTimeout(const ReadAccountDataNewArg &roArg, const CUserData &roUserData)
{
	CUserData userData(KKSG::ERR_TIMEOUT, NULL);
	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
}
