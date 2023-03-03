#ifndef __RPCC2M_GROUPCHATLEADERISSUEINFO_H__
#define __RPCC2M_GROUPCHATLEADERISSUEINFO_H__

// generate by ProtoGen at date: 2017/7/13 11:42:54

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GROUPCHATLEADERISSUEINFO_TYPE 9594

class RpcC2M_GroupChatLeaderIssueInfo : public CRpc
{
private:

	RpcC2M_GroupChatLeaderIssueInfo():CRpc(RPCC2M_GROUPCHATLEADERISSUEINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GroupChatLeaderIssueInfo *CreateRpc()
	{
		return new RpcC2M_GroupChatLeaderIssueInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GroupChatLeaderIssueInfoC2S &roArg, const GroupChatLeaderIssueInfoS2C &roRes, const CUserData &roUserData);
	void OnTimeout(const GroupChatLeaderIssueInfoC2S &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GroupChatLeaderIssueInfoC2S &roArg, GroupChatLeaderIssueInfoS2C &roRes);
	void OnDelayReplyRpc(const GroupChatLeaderIssueInfoC2S &roArg, GroupChatLeaderIssueInfoS2C &roRes, const CUserData &roUserData);

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
	GroupChatLeaderIssueInfoC2S m_oArg;
	GroupChatLeaderIssueInfoS2C m_oRes;
};

#endif
