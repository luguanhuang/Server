#ifndef __RPCM2G_CHATCANSEND_H__
#define __RPCM2G_CHATCANSEND_H__

// generate by ProtoGen at date: 2016/7/25 15:35:56

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_CHATCANSEND_TYPE 38579

class RpcM2G_ChatCanSend : public CRpc
{
private:

	RpcM2G_ChatCanSend():CRpc(RPCM2G_CHATCANSEND_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_ChatCanSend *CreateRpc()
	{
		return new RpcM2G_ChatCanSend;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ChatCanSendReq &roArg, const ChatCanSendRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ChatCanSendReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ChatCanSendReq &roArg, ChatCanSendRes &roRes);
	void OnDelayReplyRpc(const ChatCanSendReq &roArg, ChatCanSendRes &roRes, const CUserData &roUserData);

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
	ChatCanSendReq m_oArg;
	ChatCanSendRes m_oRes;
};

#endif
