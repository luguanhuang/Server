#ifndef __RPCC2M_SENDGIFT2PLATFRIEND_H__
#define __RPCC2M_SENDGIFT2PLATFRIEND_H__

// generate by ProtoGen at date: 2016/10/26 16:45:31

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_SENDGIFT2PLATFRIEND_TYPE 57764

class RpcC2M_SendGift2PlatFriend : public CRpc
{
private:

	RpcC2M_SendGift2PlatFriend():CRpc(RPCC2M_SENDGIFT2PLATFRIEND_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_SendGift2PlatFriend *CreateRpc()
	{
		return new RpcC2M_SendGift2PlatFriend;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SendGift2PlatFriendArg &roArg, const SendGift2PlatFriendRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SendGift2PlatFriendArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SendGift2PlatFriendArg &roArg, SendGift2PlatFriendRes &roRes);
	void OnDelayReplyRpc(const SendGift2PlatFriendArg &roArg, SendGift2PlatFriendRes &roRes, const CUserData &roUserData);

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
	SendGift2PlatFriendArg m_oArg;
	SendGift2PlatFriendRes m_oRes;
};

#endif
