#ifndef __RPCC2M_GUILDAUCTREQALL_H__
#define __RPCC2M_GUILDAUCTREQALL_H__

// generate by ProtoGen at date: 2016/11/1 16:07:35

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GUILDAUCTREQALL_TYPE 41964

class RpcC2M_GuildAuctReqAll : public CRpc
{
private:

	RpcC2M_GuildAuctReqAll():CRpc(RPCC2M_GUILDAUCTREQALL_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GuildAuctReqAll *CreateRpc()
	{
		return new RpcC2M_GuildAuctReqAll;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GuildAuctReqArg &roArg, const GuildAuctReqRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GuildAuctReqArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GuildAuctReqArg &roArg, GuildAuctReqRes &roRes);
	void OnDelayReplyRpc(const GuildAuctReqArg &roArg, GuildAuctReqRes &roRes, const CUserData &roUserData);

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
	GuildAuctReqArg m_oArg;
	GuildAuctReqRes m_oRes;
};

#endif
