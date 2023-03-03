#ifndef __RPCC2M_DERANKREQ_H__
#define __RPCC2M_DERANKREQ_H__

// generate by ProtoGen at date: 2016/9/3 17:33:58

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_DERANKREQ_TYPE 16406

class RpcC2M_DERankReq : public CRpc
{
private:

	RpcC2M_DERankReq():CRpc(RPCC2M_DERANKREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_DERankReq *CreateRpc()
	{
		return new RpcC2M_DERankReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const DERankArg &roArg, const DERankRes &roRes, const CUserData &roUserData);
	void OnTimeout(const DERankArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const DERankArg &roArg, DERankRes &roRes);
	void OnDelayReplyRpc(const DERankArg &roArg, DERankRes &roRes, const CUserData &roUserData);

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
	DERankArg m_oArg;
	DERankRes m_oRes;
};

#endif
