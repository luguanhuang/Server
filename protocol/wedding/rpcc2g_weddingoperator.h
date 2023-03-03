#ifndef __RPCC2G_WEDDINGOPERATOR_H__
#define __RPCC2G_WEDDINGOPERATOR_H__

// generate by ProtoGen at date: 2017/7/26 16:34:00

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_WEDDINGOPERATOR_TYPE 38050

class RpcC2G_WeddingOperator : public CRpc
{
private:

	RpcC2G_WeddingOperator():CRpc(RPCC2G_WEDDINGOPERATOR_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_WeddingOperator *CreateRpc()
	{
		return new RpcC2G_WeddingOperator;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const WeddingOperatorArg &roArg, const WeddingOperatorRes &roRes, const CUserData &roUserData);
	void OnTimeout(const WeddingOperatorArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const WeddingOperatorArg &roArg, WeddingOperatorRes &roRes);
	void OnDelayReplyRpc(const WeddingOperatorArg &roArg, WeddingOperatorRes &roRes, const CUserData &roUserData);

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
	WeddingOperatorArg m_oArg;
	WeddingOperatorRes m_oRes;
};

#endif
