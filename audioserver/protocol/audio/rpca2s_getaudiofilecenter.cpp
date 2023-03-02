#include "pch.h"
#include "audio/rpca2s_getaudiofilecenter.h"


// generate by ProtoGen at date: 2017/6/1 11:43:50

RPC_CLIENT_IMPLEMETION(RpcA2S_GetAudioFileCenter, GetAudioListReq, GetAudioListRes)

void RpcA2S_GetAudioFileCenter::OnReply(const GetAudioListReq &roArg, const GetAudioListRes &roRes, const CUserData &roUserData)
{
	UINT32 nRpcID =roArg.rpcid();
	CUserData oUser;
	oUser.m_dwUserData = roRes.result();
	oUser.m_pUserPtr = (void*)&roRes;
	CRpc::ReplyDelayRpc(nRpcID, oUser);
}

void RpcA2S_GetAudioFileCenter::OnTimeout(const GetAudioListReq &roArg, const CUserData &roUserData)
{
	CUserData oUserData;
	oUserData.m_dwUserData = ERR_TIMEOUT;
	oUserData.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUserData);
}
