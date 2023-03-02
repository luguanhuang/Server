#include "pch.h"
#include "audiotext/rpcc2a_audioauthkey.h"
#include "audio/audioMgr.h"

// generate by ProtoGen at date: 2016/12/21 10:08:34

RPC_SERVER_IMPLEMETION(RpcC2A_AudioAuthKey, AudioAuthKeyArg, AudioAuthKeyRes)

void RpcC2A_AudioAuthKey::OnCall(const AudioAuthKeyArg &roArg, AudioAuthKeyRes &roRes)
{
	AudioMgr::Instance()->SendAuthKey(roArg.ip(),roArg.open_id(), DelayRpc());
}

void RpcC2A_AudioAuthKey::OnDelayReplyRpc(const AudioAuthKeyArg &roArg, AudioAuthKeyRes &roRes, const CUserData &roUserData)
{
	AudioAuthKeyRes* pTemp = (AudioAuthKeyRes*)(roUserData.m_pUserPtr);
	roRes.CopyFrom(*pTemp);
}
