#ifndef __RPCG2M_PAYDEGREEREQ_H__
#define __RPCG2M_PAYDEGREEREQ_H__

// generate by ProtoGen at date: 2017/4/24 11:09:30

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_PAYDEGREEREQ_TYPE 15797

class RpcG2M_PayDegreeReq : public CRpc
{
private:

	RpcG2M_PayDegreeReq():CRpc(RPCG2M_PAYDEGREEREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_PayDegreeReq *CreateRpc()
	{
		return new RpcG2M_PayDegreeReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const PayDegreeArg &roArg, const PayDegreeRes &roRes, const CUserData &roUserData);
	void OnTimeout(const PayDegreeArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const PayDegreeArg &roArg, PayDegreeRes &roRes);
	void OnDelayReplyRpc(const PayDegreeArg &roArg, PayDegreeRes &roRes, const CUserData &roUserData);

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
	PayDegreeArg m_oArg;
	PayDegreeRes m_oRes;
};

#endif
