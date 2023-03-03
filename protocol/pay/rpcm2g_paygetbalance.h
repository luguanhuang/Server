#ifndef __RPCM2G_PAYGETBALANCE_H__
#define __RPCM2G_PAYGETBALANCE_H__

// generate by ProtoGen at date: 2016/12/21 23:38:22

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_PAYGETBALANCE_TYPE 10665

class RpcM2G_PayGetBalance : public CRpc
{
private:

	RpcM2G_PayGetBalance():CRpc(RPCM2G_PAYGETBALANCE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_PayGetBalance *CreateRpc()
	{
		return new RpcM2G_PayGetBalance;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const PayGetBalanceArg &roArg, const PayGetBalanceRes &roRes, const CUserData &roUserData);
	void OnTimeout(const PayGetBalanceArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const PayGetBalanceArg &roArg, PayGetBalanceRes &roRes);
	void OnDelayReplyRpc(const PayGetBalanceArg &roArg, PayGetBalanceRes &roRes, const CUserData &roUserData);

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
	PayGetBalanceArg m_oArg;
	PayGetBalanceRes m_oRes;
};

#endif
