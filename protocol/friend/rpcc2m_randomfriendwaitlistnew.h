﻿#ifndef __RPCC2M_RANDOMFRIENDWAITLISTNEW_H__
#define __RPCC2M_RANDOMFRIENDWAITLISTNEW_H__

// generate by ProtoGen at date: 2016/7/29 18:34:44

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_RANDOMFRIENDWAITLISTNEW_TYPE 65353

class RpcC2M_RandomFriendWaitListNew : public CRpc
{
private:

	RpcC2M_RandomFriendWaitListNew():CRpc(RPCC2M_RANDOMFRIENDWAITLISTNEW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_RandomFriendWaitListNew *CreateRpc()
	{
		return new RpcC2M_RandomFriendWaitListNew;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RandomFriendWaitListArg &roArg, const RandomFriendWaitListRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RandomFriendWaitListArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RandomFriendWaitListArg &roArg, RandomFriendWaitListRes &roRes);
	void OnDelayReplyRpc(const RandomFriendWaitListArg &roArg, RandomFriendWaitListRes &roRes, const CUserData &roUserData);

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
	RandomFriendWaitListArg m_oArg;
	RandomFriendWaitListRes m_oRes;
};

#endif
