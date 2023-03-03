#ifndef __RPCC2G_GUILDCHECKINBONUSINFO_H__
#define __RPCC2G_GUILDCHECKINBONUSINFO_H__

// generate by ProtoGen at date: 2016/6/16 14:25:52

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GUILDCHECKINBONUSINFO_TYPE 47251

class RpcC2G_GuildCheckInBonusInfo : public CRpc
{
private:

	RpcC2G_GuildCheckInBonusInfo():CRpc(RPCC2G_GUILDCHECKINBONUSINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GuildCheckInBonusInfo *CreateRpc()
	{
		return new RpcC2G_GuildCheckInBonusInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GuildCheckInBonusInfoArg &roArg, const GuildCheckInBonusInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GuildCheckInBonusInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GuildCheckInBonusInfoArg &roArg, GuildCheckInBonusInfoRes &roRes);
	void OnDelayReplyRpc(const GuildCheckInBonusInfoArg &roArg, GuildCheckInBonusInfoRes &roRes, const CUserData &roUserData);

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
	GuildCheckInBonusInfoArg m_oArg;
	GuildCheckInBonusInfoRes m_oRes;
};

#endif
