#ifndef __RPCR2K_REGISTERROUTER2WORLD_H__
#define __RPCR2K_REGISTERROUTER2WORLD_H__

// generate by ProtoGen at date: 2016/10/27 10:13:26

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCR2K_REGISTERROUTER2WORLD_TYPE 10474

class RpcR2K_RegisterRouter2World : public CRpc
{
private:

	RpcR2K_RegisterRouter2World():CRpc(RPCR2K_REGISTERROUTER2WORLD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcR2K_RegisterRouter2World *CreateRpc()
	{
		return new RpcR2K_RegisterRouter2World;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RegisterRouter2WorldArg &roArg, const RegisterRouter2WorldRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RegisterRouter2WorldArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RegisterRouter2WorldArg &roArg, RegisterRouter2WorldRes &roRes);
	void OnDelayReplyRpc(const RegisterRouter2WorldArg &roArg, RegisterRouter2WorldRes &roRes, const CUserData &roUserData);

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
	RegisterRouter2WorldArg m_oArg;
	RegisterRouter2WorldRes m_oRes;
};

#endif
