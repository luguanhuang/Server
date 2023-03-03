#ifndef __RPCC2M_GROUPCHATGETGROUPINFO_H__
#define __RPCC2M_GROUPCHATGETGROUPINFO_H__

// generate by ProtoGen at date: 2017/7/13 11:46:57

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GROUPCHATGETGROUPINFO_TYPE 64081

class RpcC2M_GroupChatGetGroupInfo : public CRpc
{
private:

	RpcC2M_GroupChatGetGroupInfo():CRpc(RPCC2M_GROUPCHATGETGROUPINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GroupChatGetGroupInfo *CreateRpc()
	{
		return new RpcC2M_GroupChatGetGroupInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GroupChatGetGroupInfoC2S &roArg, const GroupChatGetGroupInfoS2C &roRes, const CUserData &roUserData);
	void OnTimeout(const GroupChatGetGroupInfoC2S &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GroupChatGetGroupInfoC2S &roArg, GroupChatGetGroupInfoS2C &roRes);
	void OnDelayReplyRpc(const GroupChatGetGroupInfoC2S &roArg, GroupChatGetGroupInfoS2C &roRes, const CUserData &roUserData);

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
	GroupChatGetGroupInfoC2S m_oArg;
	GroupChatGetGroupInfoS2C m_oRes;
};

#endif
