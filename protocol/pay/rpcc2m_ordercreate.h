#ifndef __RPCC2M_ORDERCREATE_H__
#define __RPCC2M_ORDERCREATE_H__

// generate by ProtoGen at date: 2016/10/24 11:42:17

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_ORDERCREATE_TYPE 10957

class RpcC2M_OrderCreate : public CRpc
{
private:

	RpcC2M_OrderCreate():CRpc(RPCC2M_ORDERCREATE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_OrderCreate *CreateRpc()
	{
		return new RpcC2M_OrderCreate;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const OrderCreateArg &roArg, const OrderCreateRes &roRes, const CUserData &roUserData);
	void OnTimeout(const OrderCreateArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const OrderCreateArg &roArg, OrderCreateRes &roRes);
	void OnDelayReplyRpc(const OrderCreateArg &roArg, OrderCreateRes &roRes, const CUserData &roUserData);

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
	OrderCreateArg m_oArg;
	OrderCreateRes m_oRes;
};

#endif
