#ifndef __RPCC2G_INSPIREREQ_H__
#define __RPCC2G_INSPIREREQ_H__

// generate by ProtoGen at date: 2016/10/3 16:25:34

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_INSPIREREQ_TYPE 54147

class RpcC2G_InspireReq : public CRpc
{
private:

	RpcC2G_InspireReq():CRpc(RPCC2G_INSPIREREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_InspireReq *CreateRpc()
	{
		return new RpcC2G_InspireReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const InspireArg &roArg, const InspireRes &roRes, const CUserData &roUserData);
	void OnTimeout(const InspireArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const InspireArg &roArg, InspireRes &roRes);
	void OnDelayReplyRpc(const InspireArg &roArg, InspireRes &roRes, const CUserData &roUserData);

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
	InspireArg m_oArg;
	InspireRes m_oRes;
};

#endif
