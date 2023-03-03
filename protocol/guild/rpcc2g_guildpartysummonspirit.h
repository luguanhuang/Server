#ifndef __RPCC2G_GUILDPARTYSUMMONSPIRIT_H__
#define __RPCC2G_GUILDPARTYSUMMONSPIRIT_H__

// generate by ProtoGen at date: 2017/10/10 10:23:06

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GUILDPARTYSUMMONSPIRIT_TYPE 42269

class RpcC2G_GuildPartySummonSpirit : public CRpc
{
private:

	RpcC2G_GuildPartySummonSpirit():CRpc(RPCC2G_GUILDPARTYSUMMONSPIRIT_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GuildPartySummonSpirit *CreateRpc()
	{
		return new RpcC2G_GuildPartySummonSpirit;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GuildPartySummonSpiritArg &roArg, const GuildPartySummonSpiritRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GuildPartySummonSpiritArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GuildPartySummonSpiritArg &roArg, GuildPartySummonSpiritRes &roRes);
	void OnDelayReplyRpc(const GuildPartySummonSpiritArg &roArg, GuildPartySummonSpiritRes &roRes, const CUserData &roUserData);

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
	GuildPartySummonSpiritArg m_oArg;
	GuildPartySummonSpiritRes m_oRes;
};

#endif
