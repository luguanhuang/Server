#ifndef __RPCC2M_GROUPCHATFINDROLEINFOLIST_H__
#define __RPCC2M_GROUPCHATFINDROLEINFOLIST_H__

// generate by ProtoGen at date: 2017/7/13 11:39:39

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GROUPCHATFINDROLEINFOLIST_TYPE 7283

class RpcC2M_GroupChatFindRoleInfoList : public CRpc
{
private:

	RpcC2M_GroupChatFindRoleInfoList():CRpc(RPCC2M_GROUPCHATFINDROLEINFOLIST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GroupChatFindRoleInfoList *CreateRpc()
	{
		return new RpcC2M_GroupChatFindRoleInfoList;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GroupChatFindRoleInfoListC2S &roArg, const GroupChatFindRoleInfoListS2C &roRes, const CUserData &roUserData);
	void OnTimeout(const GroupChatFindRoleInfoListC2S &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GroupChatFindRoleInfoListC2S &roArg, GroupChatFindRoleInfoListS2C &roRes);
	void OnDelayReplyRpc(const GroupChatFindRoleInfoListC2S &roArg, GroupChatFindRoleInfoListS2C &roRes, const CUserData &roUserData);

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
	GroupChatFindRoleInfoListC2S m_oArg;
	GroupChatFindRoleInfoListS2C m_oRes;
};

#endif
