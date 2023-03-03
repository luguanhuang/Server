#ifndef __RPCC2M_GROUPCHATCHANGENAME_H__
#define __RPCC2M_GROUPCHATCHANGENAME_H__

// generate by ProtoGen at date: 2017/7/27 19:55:28

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GROUPCHATCHANGENAME_TYPE 44170

class RpcC2M_GroupChatChangeName : public CRpc
{
private:

	RpcC2M_GroupChatChangeName():CRpc(RPCC2M_GROUPCHATCHANGENAME_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GroupChatChangeName *CreateRpc()
	{
		return new RpcC2M_GroupChatChangeName;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GroupChatChangeNameC2S &roArg, const GroupChatChangeNameS2C &roRes, const CUserData &roUserData);
	void OnTimeout(const GroupChatChangeNameC2S &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GroupChatChangeNameC2S &roArg, GroupChatChangeNameS2C &roRes);
	void OnDelayReplyRpc(const GroupChatChangeNameC2S &roArg, GroupChatChangeNameS2C &roRes, const CUserData &roUserData);

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
	GroupChatChangeNameC2S m_oArg;
	GroupChatChangeNameS2C m_oRes;
};

#endif
