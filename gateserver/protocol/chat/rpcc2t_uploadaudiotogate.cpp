#include "pch.h"
#include "chat/rpcc2t_uploadaudiotogate.h"
#include "session/sessionmanager.h"
#include "chat/rpct2a_uploadaudiotoaudio.h"
#include "network/audiolink.h"

// generate by ProtoGen at date: 2017/5/31 11:52:47

RPC_SERVER_IMPLEMETION(RpcC2T_UpLoadAudioToGate, UpLoadAudioReq, UpLoadAudioRes)

void RpcC2T_UpLoadAudioToGate::OnCall(const UpLoadAudioReq &roArg, UpLoadAudioRes &roRes)
{
	SessionInfo *pSession = SessionManager::Instance()->FindSession(m_sessionID);
	if (pSession&&pSession->m_state == SESSION_LOGIN)
	{
		RpcT2A_UpLoadAudioToAudio *pRpc = RpcT2A_UpLoadAudioToAudio::CreateRpc();
		pRpc->m_oArg.CopyFrom(roArg);
		pRpc->m_oArg.set_rpcid(DelayRpc());
		if (pSession->m_crossGs)
			pRpc->m_oArg.set_iscross(true);
		else
			pRpc->m_oArg.set_iscross(false); 
		AudioLink::Instance()->SendTo(*pRpc);
	}
}

void RpcC2T_UpLoadAudioToGate::OnDelayReplyRpc(const UpLoadAudioReq &roArg, UpLoadAudioRes &roRes, const CUserData &roUserData)
{
	if (roUserData.m_dwUserData == KKSG::ERR_SUCCESS && roUserData.m_pUserPtr)
	{
		KKSG::UpLoadAudioRes* pInfo = (KKSG::UpLoadAudioRes*)roUserData.m_pUserPtr;
		roRes.CopyFrom(*pInfo);

	}else
	{
		roRes.set_result((KKSG::ErrorCode)roUserData.m_dwUserData);
	}
}
