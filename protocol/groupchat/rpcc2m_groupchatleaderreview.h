#ifndef __RPCC2M_GROUPCHATLEADERREVIEW_H__
#define __RPCC2M_GROUPCHATLEADERREVIEW_H__

// generate by ProtoGen at date: 2017/7/13 11:44:25

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GROUPCHATLEADERREVIEW_TYPE 21611

class RpcC2M_GroupChatLeaderReview : public CRpc
{
private:

	RpcC2M_GroupChatLeaderReview():CRpc(RPCC2M_GROUPCHATLEADERREVIEW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GroupChatLeaderReview *CreateRpc()
	{
		return new RpcC2M_GroupChatLeaderReview;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GroupChatLeaderReviewC2S &roArg, const GroupChatLeaderReviewS2C &roRes, const CUserData &roUserData);
	void OnTimeout(const GroupChatLeaderReviewC2S &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GroupChatLeaderReviewC2S &roArg, GroupChatLeaderReviewS2C &roRes);
	void OnDelayReplyRpc(const GroupChatLeaderReviewC2S &roArg, GroupChatLeaderReviewS2C &roRes, const CUserData &roUserData);

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
	GroupChatLeaderReviewC2S m_oArg;
	GroupChatLeaderReviewS2C m_oRes;
};

#endif
