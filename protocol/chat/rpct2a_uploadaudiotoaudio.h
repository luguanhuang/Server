#ifndef __RPCT2A_UPLOADAUDIOTOAUDIO_H__
#define __RPCT2A_UPLOADAUDIOTOAUDIO_H__

// generate by ProtoGen at date: 2017/6/6 10:12:34

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCT2A_UPLOADAUDIOTOAUDIO_TYPE 2071

class RpcT2A_UpLoadAudioToAudio : public CRpc
{
private:

	RpcT2A_UpLoadAudioToAudio():CRpc(RPCT2A_UPLOADAUDIOTOAUDIO_TYPE)
	{
        m_dwTimeout = 10000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcT2A_UpLoadAudioToAudio *CreateRpc()
	{
		return new RpcT2A_UpLoadAudioToAudio;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const UpLoadAudioReq &roArg, const UpLoadAudioRes &roRes, const CUserData &roUserData);
	void OnTimeout(const UpLoadAudioReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const UpLoadAudioReq &roArg, UpLoadAudioRes &roRes);
	void OnDelayReplyRpc(const UpLoadAudioReq &roArg, UpLoadAudioRes &roRes, const CUserData &roUserData);

	virtual void OnReplyTimeout() 
	{ 
		OnTimeout(m_oArg, m_oUser); 
	}

	virtual void OnDelayRpc(const CUserData &userData)
	{
		OnDelayReplyRpc( m_oArg, m_oRes, userData);
	}

	virtual CRpc *NewRpc() 
	{
		return CreateRpc();
	}

public:
	UpLoadAudioReq m_oArg;
	UpLoadAudioRes m_oRes;
};

#endif
