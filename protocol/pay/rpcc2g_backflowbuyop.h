#ifndef __RPCC2G_BACKFLOWBUYOP_H__
#define __RPCC2G_BACKFLOWBUYOP_H__

// generate by ProtoGen at date: 2017/8/4 15:47:17

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_BACKFLOWBUYOP_TYPE 16261

class RpcC2G_BackFlowBuyOp : public CRpc
{
private:

	RpcC2G_BackFlowBuyOp():CRpc(RPCC2G_BACKFLOWBUYOP_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_BackFlowBuyOp *CreateRpc()
	{
		return new RpcC2G_BackFlowBuyOp;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const BackFlowBuyOpArg &roArg, const BackFlowBuyOpRes &roRes, const CUserData &roUserData);
	void OnTimeout(const BackFlowBuyOpArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const BackFlowBuyOpArg &roArg, BackFlowBuyOpRes &roRes);
	void OnDelayReplyRpc(const BackFlowBuyOpArg &roArg, BackFlowBuyOpRes &roRes, const CUserData &roUserData);

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
	BackFlowBuyOpArg m_oArg;
	BackFlowBuyOpRes m_oRes;
};

#endif
