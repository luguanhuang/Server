#ifndef __RPCC2M_FRIENDGIFTOPNEW_H__
#define __RPCC2M_FRIENDGIFTOPNEW_H__

// generate by ProtoGen at date: 2016/7/28 23:50:17

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_FRIENDGIFTOPNEW_TYPE 35639

class RpcC2M_FriendGiftOpNew : public CRpc
{
private:

	RpcC2M_FriendGiftOpNew():CRpc(RPCC2M_FRIENDGIFTOPNEW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_FriendGiftOpNew *CreateRpc()
	{
		return new RpcC2M_FriendGiftOpNew;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const FriendGiftOpArg &roArg, const FriendGiftOpRes &roRes, const CUserData &roUserData);
	void OnTimeout(const FriendGiftOpArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const FriendGiftOpArg &roArg, FriendGiftOpRes &roRes);
	void OnDelayReplyRpc(const FriendGiftOpArg &roArg, FriendGiftOpRes &roRes, const CUserData &roUserData);

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
	FriendGiftOpArg m_oArg;
	FriendGiftOpRes m_oRes;
};

#endif
