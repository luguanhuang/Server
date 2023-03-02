#include "pch.h"
#include "audio/rpca2s_upaudiotocenter.h"

// generate by ProtoGen at date: 2017/6/5 17:56:06

RPC_CLIENT_IMPLEMETION(RpcA2S_UpAudioToCenter, UpLoadAudioReq, UpLoadAudioRes)

void RpcA2S_UpAudioToCenter::OnReply(const UpLoadAudioReq &roArg, const UpLoadAudioRes &roRes, const CUserData &roUserData)
{
	UINT32 nRpcID =roArg.rpcid();
	CUserData oUser;
	UpLoadAudioRes res;
	res.CopyFrom(roRes);
	oUser.m_dwUserData = roRes.result();
	oUser.m_pUserPtr = (void*)&res;
	CRpc::ReplyDelayRpc(nRpcID, oUser);
}

void RpcA2S_UpAudioToCenter::OnTimeout(const UpLoadAudioReq &roArg, const CUserData &roUserData)
{
	CUserData oUserData;
	oUserData.m_dwUserData = ERR_TIMEOUT;
	oUserData.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUserData);
}
