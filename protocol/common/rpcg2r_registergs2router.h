#ifndef __RPCG2R_REGISTERGS2ROUTER_H__
#define __RPCG2R_REGISTERGS2ROUTER_H__

// generate by ProtoGen at date: 2016/10/27 10:20:42

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2R_REGISTERGS2ROUTER_TYPE 16574

class RpcG2R_RegisterGs2Router : public CRpc
{
private:

	RpcG2R_RegisterGs2Router():CRpc(RPCG2R_REGISTERGS2ROUTER_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2R_RegisterGs2Router *CreateRpc()
	{
		return new RpcG2R_RegisterGs2Router;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RegisterGs2RouterArg &roArg, const RegisterGs2RouterRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RegisterGs2RouterArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RegisterGs2RouterArg &roArg, RegisterGs2RouterRes &roRes);
	void OnDelayReplyRpc(const RegisterGs2RouterArg &roArg, RegisterGs2RouterRes &roRes, const CUserData &roUserData);

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
	RegisterGs2RouterArg m_oArg;
	RegisterGs2RouterRes m_oRes;
};

#endif
