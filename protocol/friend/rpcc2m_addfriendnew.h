#ifndef __RPCC2M_ADDFRIENDNEW_H__
#define __RPCC2M_ADDFRIENDNEW_H__

// generate by ProtoGen at date: 2016/8/25 16:21:08

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_ADDFRIENDNEW_TYPE 5634

class RpcC2M_AddFriendNew : public CRpc
{
private:

	RpcC2M_AddFriendNew():CRpc(RPCC2M_ADDFRIENDNEW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_AddFriendNew *CreateRpc()
	{
		return new RpcC2M_AddFriendNew;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const AddFriendArg &roArg, const AddFriendRes &roRes, const CUserData &roUserData);
	void OnTimeout(const AddFriendArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const AddFriendArg &roArg, AddFriendRes &roRes);
	void OnDelayReplyRpc(const AddFriendArg &roArg, AddFriendRes &roRes, const CUserData &roUserData);

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
	AddFriendArg m_oArg;
	AddFriendRes m_oRes;
};

#endif
