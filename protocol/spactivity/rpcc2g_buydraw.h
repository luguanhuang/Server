#ifndef __RPCC2G_BUYDRAW_H__
#define __RPCC2G_BUYDRAW_H__

// generate by ProtoGen at date: 2017/9/12 16:03:13

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_BUYDRAW_TYPE 51925

class RpcC2G_BuyDraw : public CRpc
{
private:

	RpcC2G_BuyDraw():CRpc(RPCC2G_BUYDRAW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_BuyDraw *CreateRpc()
	{
		return new RpcC2G_BuyDraw;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const BuyDrawReq &roArg, const BuyDrawRes &roRes, const CUserData &roUserData);
	void OnTimeout(const BuyDrawReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const BuyDrawReq &roArg, BuyDrawRes &roRes);
	void OnDelayReplyRpc(const BuyDrawReq &roArg, BuyDrawRes &roRes, const CUserData &roUserData);

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
	BuyDrawReq m_oArg;
	BuyDrawRes m_oRes;
};

#endif
