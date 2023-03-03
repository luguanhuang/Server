#ifndef __RPCC2M_GUILDJOINBINDGROUP_H__
#define __RPCC2M_GUILDJOINBINDGROUP_H__

// generate by ProtoGen at date: 2016/10/10 15:55:03

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GUILDJOINBINDGROUP_TYPE 12928

class RpcC2M_GuildJoinBindGroup : public CRpc
{
private:

	RpcC2M_GuildJoinBindGroup():CRpc(RPCC2M_GUILDJOINBINDGROUP_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GuildJoinBindGroup *CreateRpc()
	{
		return new RpcC2M_GuildJoinBindGroup;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GuildJoinBindGroupReq &roArg, const GuildJoinBindGroupRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GuildJoinBindGroupReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GuildJoinBindGroupReq &roArg, GuildJoinBindGroupRes &roRes);
	void OnDelayReplyRpc(const GuildJoinBindGroupReq &roArg, GuildJoinBindGroupRes &roRes, const CUserData &roUserData);

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
	GuildJoinBindGroupReq m_oArg;
	GuildJoinBindGroupRes m_oRes;
};

#endif
