#ifndef __RPCA2S_UPAUDIOTOCENTER_H__
#define __RPCA2S_UPAUDIOTOCENTER_H__

// generate by ProtoGen at date: 2017/6/5 22:59:39

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCA2S_UPAUDIOTOCENTER_TYPE 30706

class RpcA2S_UpAudioToCenter : public CRpc
{
private:

	RpcA2S_UpAudioToCenter():CRpc(RPCA2S_UPAUDIOTOCENTER_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcA2S_UpAudioToCenter *CreateRpc()
	{
		return new RpcA2S_UpAudioToCenter;
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
