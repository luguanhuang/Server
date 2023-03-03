#ifndef __RPCS2M_S2MSENDGIFT2PLATFRIEND_H__
#define __RPCS2M_S2MSENDGIFT2PLATFRIEND_H__

// generate by ProtoGen at date: 2016/10/26 15:59:08

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCS2M_S2MSENDGIFT2PLATFRIEND_TYPE 23021

class RpcS2M_S2MSendGift2PlatFriend : public CRpc
{
private:

	RpcS2M_S2MSendGift2PlatFriend():CRpc(RPCS2M_S2MSENDGIFT2PLATFRIEND_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcS2M_S2MSendGift2PlatFriend *CreateRpc()
	{
		return new RpcS2M_S2MSendGift2PlatFriend;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const S2MSendGift2PlatFriendArg &roArg, const S2MSendGift2PlatFriendRes &roRes, const CUserData &roUserData);
	void OnTimeout(const S2MSendGift2PlatFriendArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const S2MSendGift2PlatFriendArg &roArg, S2MSendGift2PlatFriendRes &roRes);
	void OnDelayReplyRpc(const S2MSendGift2PlatFriendArg &roArg, S2MSendGift2PlatFriendRes &roRes, const CUserData &roUserData);

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
	S2MSendGift2PlatFriendArg m_oArg;
	S2MSendGift2PlatFriendRes m_oRes;
};

#endif
