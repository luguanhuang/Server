#include "pch.h"
#include "chat/rpct2a_uploadaudiotoaudio.h"

// generate by ProtoGen at date: 2017/5/31 11:55:48

RPC_CLIENT_IMPLEMETION(RpcT2A_UpLoadAudioToAudio, UpLoadAudioReq, UpLoadAudioRes)

void RpcT2A_UpLoadAudioToAudio::OnReply(const UpLoadAudioReq &roArg, const UpLoadAudioRes &roRes, const CUserData &roUserData)
{
	UINT32 nRpcID =roArg.rpcid();
	CUserData oUser;
	UpLoadAudioRes res;
	res.CopyFrom(roRes);
	oUser.m_dwUserData = roRes.result();
	oUser.m_pUserPtr = (void*)&res;
	CRpc::ReplyDelayRpc(nRpcID, oUser);

}

void RpcT2A_UpLoadAudioToAudio::OnTimeout(const UpLoadAudioReq &roArg, const CUserData &roUserData)
{
	CUserData oUserData;
	oUserData.m_dwUserData = ERR_TIMEOUT;
	oUserData.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUserData);
}
