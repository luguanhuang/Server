#ifndef __RPCC2M_GUILDAPPROVALNEW_H__
#define __RPCC2M_GUILDAPPROVALNEW_H__

// generate by ProtoGen at date: 2016/9/15 21:03:11

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GUILDAPPROVALNEW_TYPE 28348

class RpcC2M_GuildApprovalNew : public CRpc
{
private:

	RpcC2M_GuildApprovalNew():CRpc(RPCC2M_GUILDAPPROVALNEW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GuildApprovalNew *CreateRpc()
	{
		return new RpcC2M_GuildApprovalNew;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GuildApprovalArg &roArg, const GuildApprovalRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GuildApprovalArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GuildApprovalArg &roArg, GuildApprovalRes &roRes);
	void OnDelayReplyRpc(const GuildApprovalArg &roArg, GuildApprovalRes &roRes, const CUserData &roUserData);

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
	GuildApprovalArg m_oArg;
	GuildApprovalRes m_oRes;
};

#endif
