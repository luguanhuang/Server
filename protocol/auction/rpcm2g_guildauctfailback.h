#ifndef __RPCM2G_GUILDAUCTFAILBACK_H__
#define __RPCM2G_GUILDAUCTFAILBACK_H__

// generate by ProtoGen at date: 2016/11/3 20:15:28

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_GUILDAUCTFAILBACK_TYPE 34324

class RpcM2G_GuildAuctFailBack : public CRpc
{
private:

	RpcM2G_GuildAuctFailBack():CRpc(RPCM2G_GUILDAUCTFAILBACK_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_GuildAuctFailBack *CreateRpc()
	{
		return new RpcM2G_GuildAuctFailBack;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GuildAuctFailArg &roArg, const GuildAuctFailRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GuildAuctFailArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GuildAuctFailArg &roArg, GuildAuctFailRes &roRes);
	void OnDelayReplyRpc(const GuildAuctFailArg &roArg, GuildAuctFailRes &roRes, const CUserData &roUserData);

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
	GuildAuctFailArg m_oArg;
	GuildAuctFailRes m_oRes;
};

#endif
