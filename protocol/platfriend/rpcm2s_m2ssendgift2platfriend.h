#ifndef __RPCM2S_M2SSENDGIFT2PLATFRIEND_H__
#define __RPCM2S_M2SSENDGIFT2PLATFRIEND_H__

// generate by ProtoGen at date: 2016/10/26 15:56:11

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2S_M2SSENDGIFT2PLATFRIEND_TYPE 48281

class RpcM2S_M2SSendGift2PlatFriend : public CRpc
{
private:

	RpcM2S_M2SSendGift2PlatFriend():CRpc(RPCM2S_M2SSENDGIFT2PLATFRIEND_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2S_M2SSendGift2PlatFriend *CreateRpc()
	{
		return new RpcM2S_M2SSendGift2PlatFriend;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const M2SSendGift2PlatFriendArg &roArg, const M2SSendGift2PlatFriendRes &roRes, const CUserData &roUserData);
	void OnTimeout(const M2SSendGift2PlatFriendArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const M2SSendGift2PlatFriendArg &roArg, M2SSendGift2PlatFriendRes &roRes);
	void OnDelayReplyRpc(const M2SSendGift2PlatFriendArg &roArg, M2SSendGift2PlatFriendRes &roRes, const CUserData &roUserData);

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
	M2SSendGift2PlatFriendArg m_oArg;
	M2SSendGift2PlatFriendRes m_oRes;
};

#endif
