#ifndef __RPCC2M_DOADDFRIENDNEW_H__
#define __RPCC2M_DOADDFRIENDNEW_H__

// generate by ProtoGen at date: 2016/7/28 23:00:49

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_DOADDFRIENDNEW_TYPE 23397

class RpcC2M_DoAddFriendNew : public CRpc
{
private:

	RpcC2M_DoAddFriendNew():CRpc(RPCC2M_DOADDFRIENDNEW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_DoAddFriendNew *CreateRpc()
	{
		return new RpcC2M_DoAddFriendNew;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const DoAddFriendArg &roArg, const DoAddFriendRes &roRes, const CUserData &roUserData);
	void OnTimeout(const DoAddFriendArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const DoAddFriendArg &roArg, DoAddFriendRes &roRes);
	void OnDelayReplyRpc(const DoAddFriendArg &roArg, DoAddFriendRes &roRes, const CUserData &roUserData);

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
	DoAddFriendArg m_oArg;
	DoAddFriendRes m_oRes;
};

#endif
