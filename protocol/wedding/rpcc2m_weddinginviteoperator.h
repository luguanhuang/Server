#ifndef __RPCC2M_WEDDINGINVITEOPERATOR_H__
#define __RPCC2M_WEDDINGINVITEOPERATOR_H__

// generate by ProtoGen at date: 2017/7/25 9:58:08

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_WEDDINGINVITEOPERATOR_TYPE 8562

class RpcC2M_WeddingInviteOperator : public CRpc
{
private:

	RpcC2M_WeddingInviteOperator():CRpc(RPCC2M_WEDDINGINVITEOPERATOR_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_WeddingInviteOperator *CreateRpc()
	{
		return new RpcC2M_WeddingInviteOperator;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const WeddingInviteOperatorArg &roArg, const WeddingInviteOperatorRes &roRes, const CUserData &roUserData);
	void OnTimeout(const WeddingInviteOperatorArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const WeddingInviteOperatorArg &roArg, WeddingInviteOperatorRes &roRes);
	void OnDelayReplyRpc(const WeddingInviteOperatorArg &roArg, WeddingInviteOperatorRes &roRes, const CUserData &roUserData);

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
	WeddingInviteOperatorArg m_oArg;
	WeddingInviteOperatorRes m_oRes;
};

#endif
