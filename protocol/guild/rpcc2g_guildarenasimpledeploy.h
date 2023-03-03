#ifndef __RPCC2G_GUILDARENASIMPLEDEPLOY_H__
#define __RPCC2G_GUILDARENASIMPLEDEPLOY_H__

// generate by ProtoGen at date: 2016/7/19 23:53:59

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GUILDARENASIMPLEDEPLOY_TYPE 24284

class RpcC2G_GuildArenaSimpleDeploy : public CRpc
{
private:

	RpcC2G_GuildArenaSimpleDeploy():CRpc(RPCC2G_GUILDARENASIMPLEDEPLOY_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GuildArenaSimpleDeploy *CreateRpc()
	{
		return new RpcC2G_GuildArenaSimpleDeploy;
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
