#ifndef __RPCC2G_MAKEPAY_H__
#define __RPCC2G_MAKEPAY_H__

// generate by ProtoGen at date: 2015/7/9 15:43:15

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_MAKEPAY_TYPE 60105

class RpcC2G_MakePay : public CRpc
{
private:

	RpcC2G_MakePay():CRpc(RPCC2G_MAKEPAY_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_MakePay *CreateRpc()
	{
		return new RpcC2G_MakePay;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const PayArg &roArg, const PayRes &roRes, const CUserData &roUserData);
	void OnTimeout(const PayArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const PayArg &roArg, PayRes &roRes);
	void OnDelayReplyRpc(const PayArg &roArg, PayRes &roRes, const CUserData &roUserData);

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
	PayArg m_oArg;
	PayRes m_oRes;
};

#endif
