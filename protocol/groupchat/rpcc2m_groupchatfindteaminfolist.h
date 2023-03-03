#ifndef __RPCC2M_GROUPCHATFINDTEAMINFOLIST_H__
#define __RPCC2M_GROUPCHATFINDTEAMINFOLIST_H__

// generate by ProtoGen at date: 2017/7/13 11:38:54

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GROUPCHATFINDTEAMINFOLIST_TYPE 46399

class RpcC2M_GroupChatFindTeamInfoList : public CRpc
{
private:

	RpcC2M_GroupChatFindTeamInfoList():CRpc(RPCC2M_GROUPCHATFINDTEAMINFOLIST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GroupChatFindTeamInfoList *CreateRpc()
	{
		return new RpcC2M_GroupChatFindTeamInfoList;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GroupChatFindTeamInfoListC2S &roArg, const GroupChatFindTeamInfoListS2C &roRes, const CUserData &roUserData);
	void OnTimeout(const GroupChatFindTeamInfoListC2S &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GroupChatFindTeamInfoListC2S &roArg, GroupChatFindTeamInfoListS2C &roRes);
	void OnDelayReplyRpc(const GroupChatFindTeamInfoListC2S &roArg, GroupChatFindTeamInfoListS2C &roRes, const CUserData &roUserData);

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
	GroupChatFindTeamInfoListC2S m_oArg;
	GroupChatFindTeamInfoListS2C m_oRes;
};

#endif
