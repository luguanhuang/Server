#include "pch.h"
#include "forward.h"
#include "config.h"
#include "ptcregister.h"
#include "crpc.h"
#include "protocol.h"

// generate by ProtoGen at date: 2017/6/6 10:12:34

#include "common/ptca2l_audiolognotify.h"
#include "common/ptca2c_audioaidntf.h"
#include "common/rpcc2a_getaudiolistreq.h"
#include "common/rpcc2a_uploadaudioreq.h"
#include "audiotext/rpcc2a_audioauthkey.h"
#include "audiotxt/rpcc2a_audiotext.h"
#include "common/rpca2s_registeraudiotocenter.h"
#include "audio/rpca2s_getaudiofilecenter.h"
#include "audio/rpca2s_upaudiotocenter.h"
#include "chat/rpct2a_uploadaudiotoaudio.h"


static BaseLink *FindLink(const char *linkName)
{
	return Config::GetConfig()->FindLinkHandler(linkName);
}

void CProtocolRegister::Regiter() 
{
	CRpc::RegistRpc(RpcC2A_GetAudioListReq::CreateRpc());
	CRpc::RegistRpc(RpcC2A_UpLoadAudioReq::CreateRpc());
	CRpc::RegistRpc(RpcC2A_AudioAuthKey::CreateRpc());
	CRpc::RegistRpc(RpcC2A_AudioText::CreateRpc());
	CRpc::RegistRpc(RpcA2S_RegisterAudioToCenter::CreateRpc());
	CRpc::RegistRpc(RpcA2S_GetAudioFileCenter::CreateRpc());
	CRpc::RegistRpc(RpcA2S_UpAudioToCenter::CreateRpc());
	CRpc::RegistRpc(RpcT2A_UpLoadAudioToAudio::CreateRpc());


	CProtocol::RegistProtoName(PTCA2L_AUDIOLOGNOTIFY_TYPE, "PTCA2L_AUDIOLOGNOTIFY");
	CProtocol::RegistProtoName(PTCA2C_AUDIOAIDNTF_TYPE, "PTCA2C_AUDIOAIDNTF");
	CProtocol::RegistProtoName(RPCC2A_GETAUDIOLISTREQ_TYPE, "RPCC2A_GETAUDIOLISTREQ");
	CProtocol::RegistProtoName(RPCC2A_UPLOADAUDIOREQ_TYPE, "RPCC2A_UPLOADAUDIOREQ");
	CProtocol::RegistProtoName(RPCC2A_AUDIOAUTHKEY_TYPE, "RPCC2A_AUDIOAUTHKEY");
	CProtocol::RegistProtoName(RPCC2A_AUDIOTEXT_TYPE, "RPCC2A_AUDIOTEXT");
	CProtocol::RegistProtoName(RPCA2S_REGISTERAUDIOTOCENTER_TYPE, "RPCA2S_REGISTERAUDIOTOCENTER");
	CProtocol::RegistProtoName(RPCA2S_GETAUDIOFILECENTER_TYPE, "RPCA2S_GETAUDIOFILECENTER");
	CProtocol::RegistProtoName(RPCA2S_UPAUDIOTOCENTER_TYPE, "RPCA2S_UPAUDIOTOCENTER");
	CProtocol::RegistProtoName(RPCT2A_UPLOADAUDIOTOAUDIO_TYPE, "RPCT2A_UPLOADAUDIOTOAUDIO");

}