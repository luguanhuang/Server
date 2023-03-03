#ifndef __RPCT2R_REGISTERGT2ROUTER_H__
#define __RPCT2R_REGISTERGT2ROUTER_H__

// generate by ProtoGen at date: 2016/10/27 9:57:39

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCT2R_REGISTERGT2ROUTER_TYPE 7359

class RpcT2R_RegisterGt2Router : public CRpc
{
private:

	RpcT2R_RegisterGt2Router():CRpc(RPCT2R_REGISTERGT2ROUTER_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcT2R_RegisterGt2Router *CreateRpc()
	{
		return new RpcT2R_RegisterGt2Router;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RegisterGt2RouterArg &roArg, const RegisterGt2RouterRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RegisterGt2RouterArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RegisterGt2RouterArg &roArg, RegisterGt2RouterRes &roRes);
	void OnDelayReplyRpc(const RegisterGt2RouterArg &roArg, RegisterGt2RouterRes &roRes, const CUserData &roUserData);

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
	RegisterGt2RouterArg m_oArg;
	RegisterGt2RouterRes m_oRes;
};

#endif
