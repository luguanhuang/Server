#ifndef __RPCC2M_GUILDBINDGROUP_H__
#define __RPCC2M_GUILDBINDGROUP_H__

// generate by ProtoGen at date: 2016/10/10 15:53:51

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GUILDBINDGROUP_TYPE 16003

class RpcC2M_GuildBindGroup : public CRpc
{
private:

	RpcC2M_GuildBindGroup():CRpc(RPCC2M_GUILDBINDGROUP_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GuildBindGroup *CreateRpc()
	{
		return new RpcC2M_GuildBindGroup;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GuildBindGroupReq &roArg, const GuildBindGroupRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GuildBindGroupReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GuildBindGroupReq &roArg, GuildBindGroupRes &roRes);
	void OnDelayReplyRpc(const GuildBindGroupReq &roArg, GuildBindGroupRes &roRes, const CUserData &roUserData);

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
	GuildBindGroupReq m_oArg;
	GuildBindGroupRes m_oRes;
};

#endif
