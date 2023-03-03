#ifndef __RPCC2M_CHAT_H__
#define __RPCC2M_CHAT_H__

// generate by ProtoGen at date: 2016/7/25 15:26:06

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_CHAT_TYPE 56705

class RpcC2M_chat : public CRpc
{
private:

	RpcC2M_chat():CRpc(RPCC2M_CHAT_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_chat *CreateRpc()
	{
		return new RpcC2M_chat;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ChatArg &roArg, const ChatRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ChatArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ChatArg &roArg, ChatRes &roRes);
	void OnDelayReplyRpc(const ChatArg &roArg, ChatRes &roRes, const CUserData &roUserData);

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
	ChatArg m_oArg;
	ChatRes m_oRes;
};

#endif
