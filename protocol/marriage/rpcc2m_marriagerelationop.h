#ifndef __RPCC2M_MARRIAGERELATIONOP_H__
#define __RPCC2M_MARRIAGERELATIONOP_H__

// generate by ProtoGen at date: 2017/7/24 21:06:10

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_MARRIAGERELATIONOP_TYPE 24966

class RpcC2M_MarriageRelationOp : public CRpc
{
private:

	RpcC2M_MarriageRelationOp():CRpc(RPCC2M_MARRIAGERELATIONOP_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_MarriageRelationOp *CreateRpc()
	{
		return new RpcC2M_MarriageRelationOp;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const MarriageRelationOpArg &roArg, const MarriageRelationOpRes &roRes, const CUserData &roUserData);
	void OnTimeout(const MarriageRelationOpArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const MarriageRelationOpArg &roArg, MarriageRelationOpRes &roRes);
	void OnDelayReplyRpc(const MarriageRelationOpArg &roArg, MarriageRelationOpRes &roRes, const CUserData &roUserData);

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
	MarriageRelationOpArg m_oArg;
	MarriageRelationOpRes m_oRes;
};

#endif
