#ifndef __RPCC2M_GROUPCHATCREATE_H__
#define __RPCC2M_GROUPCHATCREATE_H__

// generate by ProtoGen at date: 2017/7/15 11:50:39

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GROUPCHATCREATE_TYPE 59293

class RpcC2M_GroupChatCreate : public CRpc
{
private:

	RpcC2M_GroupChatCreate():CRpc(RPCC2M_GROUPCHATCREATE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GroupChatCreate *CreateRpc()
	{
		return new RpcC2M_GroupChatCreate;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GroupChatCreateC2S &roArg, const GroupChatCreateS2C &roRes, const CUserData &roUserData);
	void OnTimeout(const GroupChatCreateC2S &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GroupChatCreateC2S &roArg, GroupChatCreateS2C &roRes);
	void OnDelayReplyRpc(const GroupChatCreateC2S &roArg, GroupChatCreateS2C &roRes, const CUserData &roUserData);

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
	GroupChatCreateC2S m_oArg;
	GroupChatCreateS2C m_oRes;
};

#endif
