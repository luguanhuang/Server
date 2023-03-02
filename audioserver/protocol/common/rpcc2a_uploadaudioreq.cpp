#include "pch.h"
#include "common/rpcc2a_uploadaudioreq.h"
#include "audio/audioMgr.h"

// generate by ProtoGen at date: 2016/6/20 16:15:20

RPC_SERVER_IMPLEMETION(RpcC2A_UpLoadAudioReq, UpLoadAudioReq, UpLoadAudioRes)

void RpcC2A_UpLoadAudioReq::OnCall(const UpLoadAudioReq &roArg, UpLoadAudioRes &roRes)
{
	SSInfo << "begin to handle RpcC2A_UpLoadAudioReq" << END;
	std::string audio = roArg.audio();
	std::string text = roArg.text();
	UINT64 uid = 0; 
	AudioMgr::Instance()->AddAudio(audio, text, uid);
	roRes.set_result(ERR_SUCCESS);
	roRes.set_audiodownuid(uid);
	SSWarn << "Finish Send D->C Message, CMD=Handle RpcC2A_UpLoadAudioReq, AudioUid=" << uid << ", Text:" << text << ", AudioSize:" << audio.size() << END;
}

void RpcC2A_UpLoadAudioReq::OnDelayReplyRpc(const UpLoadAudioReq &roArg, UpLoadAudioRes &roRes, const CUserData &roUserData)
{
}
