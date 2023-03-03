#ifndef __RPCC2A_AUDIOTEXT_H__
#define __RPCC2A_AUDIOTEXT_H__

// generate by ProtoGen at date: 2017/2/6 19:57:21

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2A_AUDIOTEXT_TYPE 42254

class RpcC2A_AudioText : public CRpc
{
private:

	RpcC2A_AudioText():CRpc(RPCC2A_AUDIOTEXT_TYPE)
	{
        m_dwTimeout = 30000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2A_AudioText *CreateRpc()
	{
		return new RpcC2A_AudioText;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const AudioTextArg &roArg, const AudioTextRes &roRes, const CUserData &roUserData);
	void OnTimeout(const AudioTextArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const AudioTextArg &roArg, AudioTextRes &roRes);
	void OnDelayReplyRpc(const AudioTextArg &roArg, AudioTextRes &roRes, const CUserData &roUserData);

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
	AudioTextArg m_oArg;
	AudioTextRes m_oRes;
};

#endif
