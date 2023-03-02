#include "pch.h"
#include "audiotxt/rpcc2a_audiotext.h"
#include "audio/audioMgr.h"
// generate by ProtoGen at date: 2016/12/21 10:14:17

RPC_SERVER_IMPLEMETION(RpcC2A_AudioText, AudioTextArg, AudioTextRes)

void RpcC2A_AudioText::OnCall(const AudioTextArg &roArg, AudioTextRes &roRes)
{
	AudioMgr::Instance()->Translate(roArg.file_id(), DelayRpc());
}

void RpcC2A_AudioText::OnDelayReplyRpc(const AudioTextArg &roArg, AudioTextRes &roRes, const CUserData &roUserData)
{
	AudioTextRes* pTemp = (AudioTextRes*)(roUserData.m_pUserPtr);
	roRes.CopyFrom(*pTemp);
}
