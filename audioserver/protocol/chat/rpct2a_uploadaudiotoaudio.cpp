#include "pch.h"
#include "chat/rpct2a_uploadaudiotoaudio.h"
#include "audio/audioMgr.h"
#include "common/rpcc2a_uploadaudioreq.h"
#include "network/centerlink.h"
#include "audio/rpca2s_upaudiotocenter.h"

// generate by ProtoGen at date: 2017/5/31 11:55:48

RPC_SERVER_IMPLEMETION(RpcT2A_UpLoadAudioToAudio, UpLoadAudioReq, UpLoadAudioRes)

void RpcT2A_UpLoadAudioToAudio::OnCall(const UpLoadAudioReq &roArg, UpLoadAudioRes &roRes)
{
	UINT64 dwUid = 0; 
	AudioMgr::Instance()->AddAudio(roArg.audio(), roArg.text(), dwUid, roArg.iscross());
	if (roArg.iscross()==true)
	{
		RpcA2S_UpAudioToCenter *pRpc = RpcA2S_UpAudioToCenter::CreateRpc();
		pRpc->m_oArg.CopyFrom(roArg);
		pRpc->m_oArg.set_rpcid(DelayRpc());
		pRpc->m_oArg.set_audiouid(dwUid);
		bool bRet = CenterLink::Instance()->SendTo(*pRpc);
		if (bRet ==  false)
		{
			delete pRpc;
		}
		return;
	}
	roRes.set_result(ERR_SUCCESS);
	roRes.set_audiodownuid(dwUid);
}

void RpcT2A_UpLoadAudioToAudio::OnDelayReplyRpc(const UpLoadAudioReq &roArg, UpLoadAudioRes &roRes, const CUserData &roUserData)
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
