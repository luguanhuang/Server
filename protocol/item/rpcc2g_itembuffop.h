#ifndef __RPCC2G_ITEMBUFFOP_H__
#define __RPCC2G_ITEMBUFFOP_H__

// generate by ProtoGen at date: 2016/11/9 11:30:27

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_ITEMBUFFOP_TYPE 50404

class RpcC2G_ItemBuffOp : public CRpc
{
private:

	RpcC2G_ItemBuffOp():CRpc(RPCC2G_ITEMBUFFOP_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ItemBuffOp *CreateRpc()
	{
		return new RpcC2G_ItemBuffOp;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ItemBuffOpArg &roArg, const ItemBuffOpRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ItemBuffOpArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ItemBuffOpArg &roArg, ItemBuffOpRes &roRes);
	void OnDelayReplyRpc(const ItemBuffOpArg &roArg, ItemBuffOpRes &roRes, const CUserData &roUserData);

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
	ItemBuffOpArg m_oArg;
	ItemBuffOpRes m_oRes;
};

#endif
