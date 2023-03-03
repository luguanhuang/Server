#ifndef __RPCC2M_HOLDWEDDING_H__
#define __RPCC2M_HOLDWEDDING_H__

// generate by ProtoGen at date: 2017/7/17 18:07:20

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_HOLDWEDDING_TYPE 51875

class RpcC2M_HoldWedding : public CRpc
{
private:

	RpcC2M_HoldWedding():CRpc(RPCC2M_HOLDWEDDING_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_HoldWedding *CreateRpc()
	{
		return new RpcC2M_HoldWedding;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const HoldWeddingReq &roArg, const HoldWeddingRes &roRes, const CUserData &roUserData);
	void OnTimeout(const HoldWeddingReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const HoldWeddingReq &roArg, HoldWeddingRes &roRes);
	void OnDelayReplyRpc(const HoldWeddingReq &roArg, HoldWeddingRes &roRes, const CUserData &roUserData);

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
	HoldWeddingReq m_oArg;
	HoldWeddingRes m_oRes;
};

#endif
