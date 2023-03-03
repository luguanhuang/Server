#ifndef __RPCC2G_RISKBUYREQUEST_H__
#define __RPCC2G_RISKBUYREQUEST_H__

// generate by ProtoGen at date: 2016/9/9 12:13:57

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_RISKBUYREQUEST_TYPE 42935

class RpcC2G_RiskBuyRequest : public CRpc
{
private:

	RpcC2G_RiskBuyRequest():CRpc(RPCC2G_RISKBUYREQUEST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_RiskBuyRequest *CreateRpc()
	{
		return new RpcC2G_RiskBuyRequest;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RiskBuyRequestArg &roArg, const RiskBuyRequestRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RiskBuyRequestArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RiskBuyRequestArg &roArg, RiskBuyRequestRes &roRes);
	void OnDelayReplyRpc(const RiskBuyRequestArg &roArg, RiskBuyRequestRes &roRes, const CUserData &roUserData);

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
	RiskBuyRequestArg m_oArg;
	RiskBuyRequestRes m_oRes;
};

#endif
