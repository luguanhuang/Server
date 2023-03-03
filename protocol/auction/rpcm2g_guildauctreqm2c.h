#ifndef __RPCM2G_GUILDAUCTREQM2C_H__
#define __RPCM2G_GUILDAUCTREQM2C_H__

// generate by ProtoGen at date: 2016/11/1 23:24:13

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_GUILDAUCTREQM2C_TYPE 10626

class RpcM2G_GuildAuctReqM2C : public CRpc
{
private:

	RpcM2G_GuildAuctReqM2C():CRpc(RPCM2G_GUILDAUCTREQM2C_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_GuildAuctReqM2C *CreateRpc()
	{
		return new RpcM2G_GuildAuctReqM2C;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GuildAuctReqM2GArg &roArg, const GuildAuctReqM2GRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GuildAuctReqM2GArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GuildAuctReqM2GArg &roArg, GuildAuctReqM2GRes &roRes);
	void OnDelayReplyRpc(const GuildAuctReqM2GArg &roArg, GuildAuctReqM2GRes &roRes, const CUserData &roUserData);

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
	GuildAuctReqM2GArg m_oArg;
	GuildAuctReqM2GRes m_oRes;
};

#endif
