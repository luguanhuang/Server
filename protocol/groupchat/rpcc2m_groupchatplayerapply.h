#ifndef __RPCC2M_GROUPCHATPLAYERAPPLY_H__
#define __RPCC2M_GROUPCHATPLAYERAPPLY_H__

// generate by ProtoGen at date: 2017/7/13 11:45:10

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GROUPCHATPLAYERAPPLY_TYPE 24788

class RpcC2M_GroupChatPlayerApply : public CRpc
{
private:

	RpcC2M_GroupChatPlayerApply():CRpc(RPCC2M_GROUPCHATPLAYERAPPLY_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GroupChatPlayerApply *CreateRpc()
	{
		return new RpcC2M_GroupChatPlayerApply;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GroupChatPlayerApplyC2S &roArg, const GroupChatPlayerApplyS2C &roRes, const CUserData &roUserData);
	void OnTimeout(const GroupChatPlayerApplyC2S &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GroupChatPlayerApplyC2S &roArg, GroupChatPlayerApplyS2C &roRes);
	void OnDelayReplyRpc(const GroupChatPlayerApplyC2S &roArg, GroupChatPlayerApplyS2C &roRes, const CUserData &roUserData);

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
	GroupChatPlayerApplyC2S m_oArg;
	GroupChatPlayerApplyS2C m_oRes;
};

#endif
