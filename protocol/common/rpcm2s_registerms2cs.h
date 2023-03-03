#ifndef __RPCM2S_REGISTERMS2CS_H__
#define __RPCM2S_REGISTERMS2CS_H__

// generate by ProtoGen at date: 2017/1/19 22:19:06

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2S_REGISTERMS2CS_TYPE 41166

class RpcM2S_RegisterMS2CS : public CRpc
{
private:

	RpcM2S_RegisterMS2CS():CRpc(RPCM2S_REGISTERMS2CS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2S_RegisterMS2CS *CreateRpc()
	{
		return new RpcM2S_RegisterMS2CS;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RegisterMSToCSData &roArg, const RegisterMSToCSDataRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RegisterMSToCSData &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RegisterMSToCSData &roArg, RegisterMSToCSDataRes &roRes);
	void OnDelayReplyRpc(const RegisterMSToCSData &roArg, RegisterMSToCSDataRes &roRes, const CUserData &roUserData);

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
	RegisterMSToCSData m_oArg;
	RegisterMSToCSDataRes m_oRes;
};

#endif
