#ifndef __RPCM2K_QUERYROUTER_H__
#define __RPCM2K_QUERYROUTER_H__

// generate by ProtoGen at date: 2017/3/15 10:02:48

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2K_QUERYROUTER_TYPE 53350

class RpcM2K_QueryRouter : public CRpc
{
private:

	RpcM2K_QueryRouter():CRpc(RPCM2K_QUERYROUTER_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2K_QueryRouter *CreateRpc()
	{
		return new RpcM2K_QueryRouter;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const QueryRouterArg &roArg, const QueryRouterRes &roRes, const CUserData &roUserData);
	void OnTimeout(const QueryRouterArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const QueryRouterArg &roArg, QueryRouterRes &roRes);
	void OnDelayReplyRpc(const QueryRouterArg &roArg, QueryRouterRes &roRes, const CUserData &roUserData);

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
	QueryRouterArg m_oArg;
	QueryRouterRes m_oRes;
};

#endif
