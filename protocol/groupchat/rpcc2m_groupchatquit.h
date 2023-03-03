#ifndef __RPCC2M_GROUPCHATQUIT_H__
#define __RPCC2M_GROUPCHATQUIT_H__

// generate by ProtoGen at date: 2017/7/13 11:50:37

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GROUPCHATQUIT_TYPE 58833

class RpcC2M_GroupChatQuit : public CRpc
{
private:

	RpcC2M_GroupChatQuit():CRpc(RPCC2M_GROUPCHATQUIT_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GroupChatQuit *CreateRpc()
	{
		return new RpcC2M_GroupChatQuit;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GroupChatQuitC2S &roArg, const GroupChatQuitS2C &roRes, const CUserData &roUserData);
	void OnTimeout(const GroupChatQuitC2S &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GroupChatQuitC2S &roArg, GroupChatQuitS2C &roRes);
	void OnDelayReplyRpc(const GroupChatQuitC2S &roArg, GroupChatQuitS2C &roRes, const CUserData &roUserData);

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
	GroupChatQuitC2S m_oArg;
	GroupChatQuitS2C m_oRes;
};

#endif
