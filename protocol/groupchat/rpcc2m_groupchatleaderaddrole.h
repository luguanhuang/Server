#ifndef __RPCC2M_GROUPCHATLEADERADDROLE_H__
#define __RPCC2M_GROUPCHATLEADERADDROLE_H__

// generate by ProtoGen at date: 2017/7/25 13:15:43

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GROUPCHATLEADERADDROLE_TYPE 44703

class RpcC2M_GroupChatLeaderAddRole : public CRpc
{
private:

	RpcC2M_GroupChatLeaderAddRole():CRpc(RPCC2M_GROUPCHATLEADERADDROLE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GroupChatLeaderAddRole *CreateRpc()
	{
		return new RpcC2M_GroupChatLeaderAddRole;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GroupChatLeaderAddRoleC2S &roArg, const GroupChatLeaderAddRoleS2C &roRes, const CUserData &roUserData);
	void OnTimeout(const GroupChatLeaderAddRoleC2S &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GroupChatLeaderAddRoleC2S &roArg, GroupChatLeaderAddRoleS2C &roRes);
	void OnDelayReplyRpc(const GroupChatLeaderAddRoleC2S &roArg, GroupChatLeaderAddRoleS2C &roRes, const CUserData &roUserData);

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
	GroupChatLeaderAddRoleC2S m_oArg;
	GroupChatLeaderAddRoleS2C m_oRes;
};

#endif
