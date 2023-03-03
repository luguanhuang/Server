#ifndef __RPCC2M_GUILDARENASIMPLEDEPLOYNEW_H__
#define __RPCC2M_GUILDARENASIMPLEDEPLOYNEW_H__

// generate by ProtoGen at date: 2016/9/22 16:23:48

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GUILDARENASIMPLEDEPLOYNEW_TYPE 42310

class RpcC2M_GuildArenaSimpleDeployNew : public CRpc
{
private:

	RpcC2M_GuildArenaSimpleDeployNew():CRpc(RPCC2M_GUILDARENASIMPLEDEPLOYNEW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GuildArenaSimpleDeployNew *CreateRpc()
	{
		return new RpcC2M_GuildArenaSimpleDeployNew;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GuildArenaSimpleDeployArg &roArg, const GuildArenaSimpleDeployRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GuildArenaSimpleDeployArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GuildArenaSimpleDeployArg &roArg, GuildArenaSimpleDeployRes &roRes);
	void OnDelayReplyRpc(const GuildArenaSimpleDeployArg &roArg, GuildArenaSimpleDeployRes &roRes, const CUserData &roUserData);

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
	GuildArenaSimpleDeployArg m_oArg;
	GuildArenaSimpleDeployRes m_oRes;
};

#endif
