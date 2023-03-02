#include "pch.h"
#include "chat/rpcm2f_upjoinfmroom.h"

// generate by ProtoGen at date: 2016/11/23 17:49:16

RPC_CLIENT_IMPLEMETION(RpcM2F_UPJoinFmRoom, JoinLargeRoomArg, JoinLargeRoomRes)

void RpcM2F_UPJoinFmRoom::OnReply(const JoinLargeRoomArg &roArg, const JoinLargeRoomRes &roRes, const CUserData &roUserData)
{
	UINT32 nRpcID =roArg.param();
	CUserData oUser;
	oUser.m_dwUserData = 0;
	oUser.m_pUserPtr = (void*)&roRes;
	CRpc::ReplyDelayRpc(nRpcID, oUser);
}


void RpcM2F_UPJoinFmRoom::OnTimeout(const JoinLargeRoomArg &roArg, const CUserData &roUserData)
{

}
