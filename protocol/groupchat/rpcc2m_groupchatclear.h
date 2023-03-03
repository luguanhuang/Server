#ifndef __RPCC2M_GROUPCHATCLEAR_H__
#define __RPCC2M_GROUPCHATCLEAR_H__

// generate by ProtoGen at date: 2017/7/17 16:58:02

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GROUPCHATCLEAR_TYPE 61477

class RpcC2M_GroupChatClear : public CRpc
{
private:

	RpcC2M_GroupChatClear():CRpc(RPCC2M_GROUPCHATCLEAR_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GroupChatClear *CreateRpc()
	{
		return new RpcC2M_GroupChatClear;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GroupChatClearC2S &roArg, const GroupChatClearS2C &roRes, const CUserData &roUserData);
	void OnTimeout(const GroupChatClearC2S &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GroupChatClearC2S &roArg, GroupChatClearS2C &roRes);
	void OnDelayReplyRpc(const GroupChatClearC2S &roArg, GroupChatClearS2C &roRes, const CUserData &roUserData);

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
	GroupChatClearC2S m_oArg;
	GroupChatClearS2C m_oRes;
};

#endif
