#ifndef __RPCC2M_GROUPCHATLEADERREVIEWLIST_H__
#define __RPCC2M_GROUPCHATLEADERREVIEWLIST_H__

// generate by ProtoGen at date: 2017/7/13 11:40:24

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GROUPCHATLEADERREVIEWLIST_TYPE 33836

class RpcC2M_GroupChatLeaderReviewList : public CRpc
{
private:

	RpcC2M_GroupChatLeaderReviewList():CRpc(RPCC2M_GROUPCHATLEADERREVIEWLIST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GroupChatLeaderReviewList *CreateRpc()
	{
		return new RpcC2M_GroupChatLeaderReviewList;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GroupChatLeaderReviewListC2S &roArg, const GroupChatLeaderReviewListS2C &roRes, const CUserData &roUserData);
	void OnTimeout(const GroupChatLeaderReviewListC2S &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GroupChatLeaderReviewListC2S &roArg, GroupChatLeaderReviewListS2C &roRes);
	void OnDelayReplyRpc(const GroupChatLeaderReviewListC2S &roArg, GroupChatLeaderReviewListS2C &roRes, const CUserData &roUserData);

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
	GroupChatLeaderReviewListC2S m_oArg;
	GroupChatLeaderReviewListS2C m_oRes;
};

#endif
