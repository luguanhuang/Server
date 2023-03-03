#ifndef __RPCC2M_ASKGUILDMEMBERS_H__
#define __RPCC2M_ASKGUILDMEMBERS_H__

// generate by ProtoGen at date: 2016/9/6 10:43:57

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_ASKGUILDMEMBERS_TYPE 57958

class RpcC2M_AskGuildMembers : public CRpc
{
private:

	RpcC2M_AskGuildMembers():CRpc(RPCC2M_ASKGUILDMEMBERS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_AskGuildMembers *CreateRpc()
	{
		return new RpcC2M_AskGuildMembers;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GuildMemberArg &roArg, const GuildMemberRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GuildMemberArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GuildMemberArg &roArg, GuildMemberRes &roRes);
	void OnDelayReplyRpc(const GuildMemberArg &roArg, GuildMemberRes &roRes, const CUserData &roUserData);

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
	GuildMemberArg m_oArg;
	GuildMemberRes m_oRes;
};

#endif
