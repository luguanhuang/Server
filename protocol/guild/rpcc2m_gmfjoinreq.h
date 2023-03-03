#ifndef __RPCC2M_GMFJOINREQ_H__
#define __RPCC2M_GMFJOINREQ_H__

// generate by ProtoGen at date: 2016/10/10 19:14:35

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GMFJOINREQ_TYPE 37651

class RpcC2M_gmfjoinreq : public CRpc
{
private:

	RpcC2M_gmfjoinreq():CRpc(RPCC2M_GMFJOINREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_gmfjoinreq *CreateRpc()
	{
		return new RpcC2M_gmfjoinreq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const gmfjoinarg &roArg, const gmfjoinres &roRes, const CUserData &roUserData);
	void OnTimeout(const gmfjoinarg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const gmfjoinarg &roArg, gmfjoinres &roRes);
	void OnDelayReplyRpc(const gmfjoinarg &roArg, gmfjoinres &roRes, const CUserData &roUserData);

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
	gmfjoinarg m_oArg;
	gmfjoinres m_oRes;
};

#endif
