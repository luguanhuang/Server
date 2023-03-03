#ifndef __RPCC2G_GUILDGOBLININFO_H__
#define __RPCC2G_GUILDGOBLININFO_H__

// generate by ProtoGen at date: 2015/12/15 10:13:50

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GUILDGOBLININFO_TYPE 59865

class RpcC2G_GuildGoblinInfo : public CRpc
{
private:

	RpcC2G_GuildGoblinInfo():CRpc(RPCC2G_GUILDGOBLININFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GuildGoblinInfo *CreateRpc()
	{
		return new RpcC2G_GuildGoblinInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GuildGoblinInfoArg &roArg, const GuildGoblinInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GuildGoblinInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GuildGoblinInfoArg &roArg, GuildGoblinInfoRes &roRes);
	void OnDelayReplyRpc(const GuildGoblinInfoArg &roArg, GuildGoblinInfoRes &roRes, const CUserData &roUserData);

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
	GuildGoblinInfoArg m_oArg;
	GuildGoblinInfoRes m_oRes;
};

#endif
