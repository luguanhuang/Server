#ifndef __RPCC2G_PAYFRIENDITEM_H__
#define __RPCC2G_PAYFRIENDITEM_H__

// generate by ProtoGen at date: 2017/3/14 10:25:06

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_PAYFRIENDITEM_TYPE 29289

class RpcC2G_PayFriendItem : public CRpc
{
private:

	RpcC2G_PayFriendItem():CRpc(RPCC2G_PAYFRIENDITEM_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_PayFriendItem *CreateRpc()
	{
		return new RpcC2G_PayFriendItem;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const PayFriendItemArg &roArg, const PayFriendItemRes &roRes, const CUserData &roUserData);
	void OnTimeout(const PayFriendItemArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const PayFriendItemArg &roArg, PayFriendItemRes &roRes);
	void OnDelayReplyRpc(const PayFriendItemArg &roArg, PayFriendItemRes &roRes, const CUserData &roUserData);

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
	PayFriendItemArg m_oArg;
	PayFriendItemRes m_oRes;
};

#endif
