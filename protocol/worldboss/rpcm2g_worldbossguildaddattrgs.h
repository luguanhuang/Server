#ifndef __RPCM2G_WORLDBOSSGUILDADDATTRGS_H__
#define __RPCM2G_WORLDBOSSGUILDADDATTRGS_H__

// generate by ProtoGen at date: 2017/4/13 11:39:11

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_WORLDBOSSGUILDADDATTRGS_TYPE 40178

class RpcM2G_WorldBossGuildAddAttrGs : public CRpc
{
private:

	RpcM2G_WorldBossGuildAddAttrGs():CRpc(RPCM2G_WORLDBOSSGUILDADDATTRGS_TYPE)
	{
        m_dwTimeout = 10000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_WorldBossGuildAddAttrGs *CreateRpc()
	{
		return new RpcM2G_WorldBossGuildAddAttrGs;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const WorldBossGuildAddAttrGsArg &roArg, const WorldBossGuildAddAttrGsRes &roRes, const CUserData &roUserData);
	void OnTimeout(const WorldBossGuildAddAttrGsArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const WorldBossGuildAddAttrGsArg &roArg, WorldBossGuildAddAttrGsRes &roRes);
	void OnDelayReplyRpc(const WorldBossGuildAddAttrGsArg &roArg, WorldBossGuildAddAttrGsRes &roRes, const CUserData &roUserData);

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
	WorldBossGuildAddAttrGsArg m_oArg;
	WorldBossGuildAddAttrGsRes m_oRes;
};

#endif
