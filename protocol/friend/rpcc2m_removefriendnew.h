#ifndef __RPCC2M_REMOVEFRIENDNEW_H__
#define __RPCC2M_REMOVEFRIENDNEW_H__

// generate by ProtoGen at date: 2016/7/28 23:01:40

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_REMOVEFRIENDNEW_TYPE 2841

class RpcC2M_RemoveFriendNew : public CRpc
{
private:

	RpcC2M_RemoveFriendNew():CRpc(RPCC2M_REMOVEFRIENDNEW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_RemoveFriendNew *CreateRpc()
	{
		return new RpcC2M_RemoveFriendNew;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RemoveFriendArg &roArg, const RemoveFriendRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RemoveFriendArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RemoveFriendArg &roArg, RemoveFriendRes &roRes);
	void OnDelayReplyRpc(const RemoveFriendArg &roArg, RemoveFriendRes &roRes, const CUserData &roUserData);

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
	RemoveFriendArg m_oArg;
	RemoveFriendRes m_oRes;
};

#endif
