#ifndef __RPCC2G_BUYJADESLOT_H__
#define __RPCC2G_BUYJADESLOT_H__

// generate by ProtoGen at date: 2015/6/30 10:44:12

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_BUYJADESLOT_TYPE 37813

class RpcC2G_BuyJadeSlot : public CRpc
{
private:

	RpcC2G_BuyJadeSlot():CRpc(RPCC2G_BUYJADESLOT_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_BuyJadeSlot *CreateRpc()
	{
		return new RpcC2G_BuyJadeSlot;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const BuyJadeSlotArg &roArg, const BuyJadeSlotRes &roRes, const CUserData &roUserData);
	void OnTimeout(const BuyJadeSlotArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const BuyJadeSlotArg &roArg, BuyJadeSlotRes &roRes);
	void OnDelayReplyRpc(const BuyJadeSlotArg &roArg, BuyJadeSlotRes &roRes, const CUserData &roUserData);

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
	BuyJadeSlotArg m_oArg;
	BuyJadeSlotRes m_oRes;
};

#endif
