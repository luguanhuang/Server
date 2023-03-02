#include "pch.h"
#include "common/rpca2s_registeraudiotocenter.h"
#include "network/centerlink.h"

// generate by ProtoGen at date: 2017/5/31 20:07:07

RPC_CLIENT_IMPLEMETION(RpcA2S_RegisterAudioToCenter, RegisterAudioToCenterArg, RegisterAudioToCenterRes)

void RpcA2S_RegisterAudioToCenter::OnReply(const RegisterAudioToCenterArg &roArg, const RegisterAudioToCenterRes &roRes, const CUserData &roUserData)
{
	if(roRes.result() == KKSG::ERR_SUCCESS)
	{
		CenterLink::Instance()->AddCSServer(m_dwConnID);
	}
}

void RpcA2S_RegisterAudioToCenter::OnTimeout(const RegisterAudioToCenterArg &roArg, const CUserData &roUserData)
{
}
