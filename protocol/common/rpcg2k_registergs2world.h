#ifndef __RPCG2K_REGISTERGS2WORLD_H__
#define __RPCG2K_REGISTERGS2WORLD_H__

// generate by ProtoGen at date: 2016/10/27 10:46:17

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2K_REGISTERGS2WORLD_TYPE 43007

class RpcG2K_RegisterGs2World : public CRpc
{
private:

	RpcG2K_RegisterGs2World():CRpc(RPCG2K_REGISTERGS2WORLD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2K_RegisterGs2World *CreateRpc()
	{
		return new RpcG2K_RegisterGs2World;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RegisterGs2WorldArg &roArg, const RegisterGs2WorldRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RegisterGs2WorldArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RegisterGs2WorldArg &roArg, RegisterGs2WorldRes &roRes);
	void OnDelayReplyRpc(const RegisterGs2WorldArg &roArg, RegisterGs2WorldRes &roRes, const CUserData &roUserData);

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
	RegisterGs2WorldArg m_oArg;
	RegisterGs2WorldRes m_oRes;
};

#endif
