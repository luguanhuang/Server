#ifndef __RPCC2G_GUILDCAMPEXCHANGEOPERATE_H__
#define __RPCC2G_GUILDCAMPEXCHANGEOPERATE_H__

// generate by ProtoGen at date: 2017/5/2 21:51:43

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GUILDCAMPEXCHANGEOPERATE_TYPE 31811

class RpcC2G_GuildCampExchangeOperate : public CRpc
{
private:

	RpcC2G_GuildCampExchangeOperate():CRpc(RPCC2G_GUILDCAMPEXCHANGEOPERATE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GuildCampExchangeOperate *CreateRpc()
	{
		return new RpcC2G_GuildCampExchangeOperate;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GuildCampExchangeOperateArg &roArg, const GuildCampExchangeOperateRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GuildCampExchangeOperateArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GuildCampExchangeOperateArg &roArg, GuildCampExchangeOperateRes &roRes);
	void OnDelayReplyRpc(const GuildCampExchangeOperateArg &roArg, GuildCampExchangeOperateRes &roRes, const CUserData &roUserData);

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
	GuildCampExchangeOperateArg m_oArg;
	GuildCampExchangeOperateRes m_oRes;
};

#endif
