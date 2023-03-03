#ifndef __RPCC2M_GUILDUNBINDGROUP_H__
#define __RPCC2M_GUILDUNBINDGROUP_H__

// generate by ProtoGen at date: 2016/10/10 15:56:01

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GUILDUNBINDGROUP_TYPE 28516

class RpcC2M_GuildUnBindGroup : public CRpc
{
private:

	RpcC2M_GuildUnBindGroup():CRpc(RPCC2M_GUILDUNBINDGROUP_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GuildUnBindGroup *CreateRpc()
	{
		return new RpcC2M_GuildUnBindGroup;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GuildUnBindGroupReq &roArg, const GuildUnBindGroupRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GuildUnBindGroupReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GuildUnBindGroupReq &roArg, GuildUnBindGroupRes &roRes);
	void OnDelayReplyRpc(const GuildUnBindGroupReq &roArg, GuildUnBindGroupRes &roRes, const CUserData &roUserData);

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
	GuildUnBindGroupReq m_oArg;
	GuildUnBindGroupRes m_oRes;
};

#endif
