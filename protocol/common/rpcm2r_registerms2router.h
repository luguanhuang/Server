#ifndef __RPCM2R_REGISTERMS2ROUTER_H__
#define __RPCM2R_REGISTERMS2ROUTER_H__

// generate by ProtoGen at date: 2016/11/5 14:58:27

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2R_REGISTERMS2ROUTER_TYPE 24423

class RpcM2R_RegisterMs2Router : public CRpc
{
private:

	RpcM2R_RegisterMs2Router():CRpc(RPCM2R_REGISTERMS2ROUTER_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2R_RegisterMs2Router *CreateRpc()
	{
		return new RpcM2R_RegisterMs2Router;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RegisterMs2RouterArg &roArg, const RegisterMs2RouterRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RegisterMs2RouterArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RegisterMs2RouterArg &roArg, RegisterMs2RouterRes &roRes);
	void OnDelayReplyRpc(const RegisterMs2RouterArg &roArg, RegisterMs2RouterRes &roRes, const CUserData &roUserData);

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
	RegisterMs2RouterArg m_oArg;
	RegisterMs2RouterRes m_oRes;
};

#endif
