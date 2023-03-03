#ifndef __RPCC2M_GUILDCAMPINFO_H__
#define __RPCC2M_GUILDCAMPINFO_H__

// generate by ProtoGen at date: 2016/11/5 14:51:44

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GUILDCAMPINFO_TYPE 4221

class RpcC2M_GuildCampInfo : public CRpc
{
private:

	RpcC2M_GuildCampInfo():CRpc(RPCC2M_GUILDCAMPINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GuildCampInfo *CreateRpc()
	{
		return new RpcC2M_GuildCampInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GuildCampInfoArg &roArg, const GuildCampInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GuildCampInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GuildCampInfoArg &roArg, GuildCampInfoRes &roRes);
	void OnDelayReplyRpc(const GuildCampInfoArg &roArg, GuildCampInfoRes &roRes, const CUserData &roUserData);

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
	GuildCampInfoArg m_oArg;
	GuildCampInfoRes m_oRes;
};

#endif
