#ifndef __RPCC2A_AUDIOAUTHKEY_H__
#define __RPCC2A_AUDIOAUTHKEY_H__

// generate by ProtoGen at date: 2016/12/21 10:08:34

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2A_AUDIOAUTHKEY_TYPE 19391

class RpcC2A_AudioAuthKey : public CRpc
{
private:

	RpcC2A_AudioAuthKey():CRpc(RPCC2A_AUDIOAUTHKEY_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2A_AudioAuthKey *CreateRpc()
	{
		return new RpcC2A_AudioAuthKey;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const AudioAuthKeyArg &roArg, const AudioAuthKeyRes &roRes, const CUserData &roUserData);
	void OnTimeout(const AudioAuthKeyArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const AudioAuthKeyArg &roArg, AudioAuthKeyRes &roRes);
	void OnDelayReplyRpc(const AudioAuthKeyArg &roArg, AudioAuthKeyRes &roRes, const CUserData &roUserData);

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
	AudioAuthKeyArg m_oArg;
	AudioAuthKeyRes m_oRes;
};

#endif
