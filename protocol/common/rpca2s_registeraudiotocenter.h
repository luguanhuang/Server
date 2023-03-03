#ifndef __RPCA2S_REGISTERAUDIOTOCENTER_H__
#define __RPCA2S_REGISTERAUDIOTOCENTER_H__

// generate by ProtoGen at date: 2017/5/31 20:07:07

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCA2S_REGISTERAUDIOTOCENTER_TYPE 8273

class RpcA2S_RegisterAudioToCenter : public CRpc
{
private:

	RpcA2S_RegisterAudioToCenter():CRpc(RPCA2S_REGISTERAUDIOTOCENTER_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcA2S_RegisterAudioToCenter *CreateRpc()
	{
		return new RpcA2S_RegisterAudioToCenter;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RegisterAudioToCenterArg &roArg, const RegisterAudioToCenterRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RegisterAudioToCenterArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RegisterAudioToCenterArg &roArg, RegisterAudioToCenterRes &roRes);
	void OnDelayReplyRpc(const RegisterAudioToCenterArg &roArg, RegisterAudioToCenterRes &roRes, const CUserData &roUserData);

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
	RegisterAudioToCenterArg m_oArg;
	RegisterAudioToCenterRes m_oRes;
};

#endif
