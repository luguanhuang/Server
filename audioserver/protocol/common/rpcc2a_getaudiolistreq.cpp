#include "pch.h"
#include "common/rpcc2a_getaudiolistreq.h"
#include "audio/audioMgr.h"
#include "LogInit.h"
#include "common/ptca2c_audioaidntf.h"
#include "network/gatelink.h"
#include "audio/rpca2s_getaudiofilecenter.h"
#include "network/centerlink.h"
#include "util.h"

// generate by ProtoGen at date: 2016/6/20 17:58:57

RPC_SERVER_IMPLEMETION(RpcC2A_GetAudioListReq, GetAudioListReq, GetAudioListRes)

void RpcC2A_GetAudioListReq::OnCall(const GetAudioListReq &roArg, GetAudioListRes &roRes)
{
	ErrorCode code = ERR_SUCCESS;
	for(size_t i=0; i< roArg.audiouidlist_size(); i++)
	{
		UINT64 dwUid = roArg.audiouidlist(i);
		UINT32 serverid =  AudioMgr::Instance()->GetSvrID(dwUid); 
		if (serverid > 0)
		{
			RpcA2S_GetAudioFileCenter *pRpc = RpcA2S_GetAudioFileCenter::CreateRpc(); 
			pRpc->m_oArg.CopyFrom(m_oArg);
			pRpc->m_oArg.set_rpcid(DelayRpc());
			CenterLink::Instance()->SendTo(*pRpc);
			return; 
		}
		stAudioInfo* pAudio = AudioMgr::Instance()->GetAudio(dwUid);
		if (pAudio)
		{
			KKSG::AudioBrief* pData = roRes.add_datalist();
			pData->set_audiouid(pAudio->audioUid);
			pData->set_audio(pAudio->audio);
			pData->set_text(pAudio->text);
			SSWarn << "Audio Info, UID:" << pData->audiouid() << ", Text:" << pData->text() << END;
			UINT32 nFlag = AudioMgr::Instance()->GetGayFlag();
			if(i == 0&&nFlag==1)
			{
				PtcA2C_AudioAIDNtf ntf;
				ntf.m_sessionID = m_sessionID;
				std::stringstream ss;
				ss << pAudio->audioUid;
				ntf.m_Data.set_file_id(ToString(pAudio->audioUid));
				GateLink::Instance()->Send(m_dwConnID, ntf);
			}
			roRes.set_result(code);
		}

	}

}

void RpcC2A_GetAudioListReq::OnDelayReplyRpc(const GetAudioListReq &roArg, GetAudioListRes &roRes, const CUserData &roUserData)
{
	if (roUserData.m_dwUserData == KKSG::ERR_SUCCESS && roUserData.m_pUserPtr)
	{
		KKSG::GetAudioListRes* pInfo = (KKSG::GetAudioListRes*)roUserData.m_pUserPtr;
		roRes.CopyFrom(*pInfo);
		if(AudioMgr::Instance()->GetGayFlag()==1)
		{
			for(size_t i=0; i< roArg.audiouidlist_size(); i++)
			{
				PtcA2C_AudioAIDNtf ntf;
				ntf.m_sessionID = m_sessionID;
				ntf.m_Data.set_file_id(ToString(roArg.audiouidlist(i)));
				GateLink::Instance()->Send(m_dwConnID, ntf);
			}
		}
	}else
	{
		roRes.set_result((KKSG::ErrorCode)roUserData.m_dwUserData);
	}
}
