#ifndef __RPCC2G_BACKFLOWACTIVITYOPERATION_H__
#define __RPCC2G_BACKFLOWACTIVITYOPERATION_H__

// generate by ProtoGen at date: 2017/10/19 11:11:20

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_BACKFLOWACTIVITYOPERATION_TYPE 61579

class RpcC2G_BackFlowActivityOperation : public CRpc
{
private:

	RpcC2G_BackFlowActivityOperation():CRpc(RPCC2G_BACKFLOWACTIVITYOPERATION_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_BackFlowActivityOperation *CreateRpc()
	{
		return new RpcC2G_BackFlowActivityOperation;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const BackFlowActivityOperationArg &roArg, const BackFlowActivityOperationRes &roRes, const CUserData &roUserData);
	void OnTimeout(const BackFlowActivityOperationArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const BackFlowActivityOperationArg &roArg, BackFlowActivityOperationRes &roRes);
	void OnDelayReplyRpc(const BackFlowActivityOperationArg &roArg, BackFlowActivityOperationRes &roRes, const CUserData &roUserData);

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
	BackFlowActivityOperationArg m_oArg;
	BackFlowActivityOperationRes m_oRes;
};

#endif
