#ifndef __RPCG2M_MSCOMMENDFIRSTPASS_H__
#define __RPCG2M_MSCOMMENDFIRSTPASS_H__

// generate by ProtoGen at date: 2016/8/24 17:31:58

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_MSCOMMENDFIRSTPASS_TYPE 54283

class RpcG2M_MSCommendFirstPass : public CRpc
{
private:

	RpcG2M_MSCommendFirstPass():CRpc(RPCG2M_MSCOMMENDFIRSTPASS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_MSCommendFirstPass *CreateRpc()
	{
		return new RpcG2M_MSCommendFirstPass;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const MSCommendFirstPassArg &roArg, const MSCommendFirstPassRes &roRes, const CUserData &roUserData);
	void OnTimeout(const MSCommendFirstPassArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const MSCommendFirstPassArg &roArg, MSCommendFirstPassRes &roRes);
	void OnDelayReplyRpc(const MSCommendFirstPassArg &roArg, MSCommendFirstPassRes &roRes, const CUserData &roUserData);

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
	MSCommendFirstPassArg m_oArg;
	MSCommendFirstPassRes m_oRes;
};

#endif
