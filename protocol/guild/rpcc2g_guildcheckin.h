#ifndef __RPCC2G_GUILDCHECKIN_H__
#define __RPCC2G_GUILDCHECKIN_H__

// generate by ProtoGen at date: 2015/10/8 14:12:03

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GUILDCHECKIN_TYPE 4174

class RpcC2G_GuildCheckin : public CRpc
{
private:

	RpcC2G_GuildCheckin():CRpc(RPCC2G_GUILDCHECKIN_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GuildCheckin *CreateRpc()
	{
		return new RpcC2G_GuildCheckin;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GuildCheckinArg &roArg, const GuildCheckinRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GuildCheckinArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GuildCheckinArg &roArg, GuildCheckinRes &roRes);
	void OnDelayReplyRpc(const GuildCheckinArg &roArg, GuildCheckinRes &roRes, const CUserData &roUserData);

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
	GuildCheckinArg m_oArg;
	GuildCheckinRes m_oRes;
};

#endif
