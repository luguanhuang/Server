#ifndef __RPCC2M_GROUPCHATMANAGER_H__
#define __RPCC2M_GROUPCHATMANAGER_H__

// generate by ProtoGen at date: 2017/7/13 11:49:24

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GROUPCHATMANAGER_TYPE 35391

class RpcC2M_GroupChatManager : public CRpc
{
private:

	RpcC2M_GroupChatManager():CRpc(RPCC2M_GROUPCHATMANAGER_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GroupChatManager *CreateRpc()
	{
		return new RpcC2M_GroupChatManager;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GroupChatManagerC2S &roArg, const GroupChatManagerS2C &roRes, const CUserData &roUserData);
	void OnTimeout(const GroupChatManagerC2S &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GroupChatManagerC2S &roArg, GroupChatManagerS2C &roRes);
	void OnDelayReplyRpc(const GroupChatManagerC2S &roArg, GroupChatManagerS2C &roRes, const CUserData &roUserData);

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
	GroupChatManagerC2S m_oArg;
	GroupChatManagerS2C m_oRes;
};

#endif
