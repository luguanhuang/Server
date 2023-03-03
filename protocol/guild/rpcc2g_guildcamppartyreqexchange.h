#ifndef __RPCC2G_GUILDCAMPPARTYREQEXCHANGE_H__
#define __RPCC2G_GUILDCAMPPARTYREQEXCHANGE_H__

// generate by ProtoGen at date: 2017/4/26 19:08:18

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GUILDCAMPPARTYREQEXCHANGE_TYPE 51021

class RpcC2G_GuildCampPartyReqExchange : public CRpc
{
private:

	RpcC2G_GuildCampPartyReqExchange():CRpc(RPCC2G_GUILDCAMPPARTYREQEXCHANGE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GuildCampPartyReqExchange *CreateRpc()
	{
		return new RpcC2G_GuildCampPartyReqExchange;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GuildCampPartyReqExchangeReq &roArg, const GuildCampPartyReqExchangeRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GuildCampPartyReqExchangeReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GuildCampPartyReqExchangeReq &roArg, GuildCampPartyReqExchangeRes &roRes);
	void OnDelayReplyRpc(const GuildCampPartyReqExchangeReq &roArg, GuildCampPartyReqExchangeRes &roRes, const CUserData &roUserData);

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
	GuildCampPartyReqExchangeReq m_oArg;
	GuildCampPartyReqExchangeRes m_oRes;
};

#endif
