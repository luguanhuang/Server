#ifndef __RPCC2G_PAYCLICK_H__
#define __RPCC2G_PAYCLICK_H__

// generate by ProtoGen at date: 2016/8/17 17:31:02

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_PAYCLICK_TYPE 20376

class RpcC2G_PayClick : public CRpc
{
private:

	RpcC2G_PayClick():CRpc(RPCC2G_PAYCLICK_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_PayClick *CreateRpc()
	{
		return new RpcC2G_PayClick;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const PayClickArg &roArg, const PayClickRes &roRes, const CUserData &roUserData);
	void OnTimeout(const PayClickArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const PayClickArg &roArg, PayClickRes &roRes);
	void OnDelayReplyRpc(const PayClickArg &roArg, PayClickRes &roRes, const CUserData &roUserData);

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
	PayClickArg m_oArg;
	PayClickRes m_oRes;
};

#endif
