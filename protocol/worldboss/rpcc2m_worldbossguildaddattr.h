#ifndef __RPCC2M_WORLDBOSSGUILDADDATTR_H__
#define __RPCC2M_WORLDBOSSGUILDADDATTR_H__

// generate by ProtoGen at date: 2017/4/13 14:26:12

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_WORLDBOSSGUILDADDATTR_TYPE 9805

class RpcC2M_WorldBossGuildAddAttr : public CRpc
{
private:

	RpcC2M_WorldBossGuildAddAttr():CRpc(RPCC2M_WORLDBOSSGUILDADDATTR_TYPE)
	{
        m_dwTimeout = 15000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_WorldBossGuildAddAttr *CreateRpc()
	{
		return new RpcC2M_WorldBossGuildAddAttr;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const WorldBossGuildAddAttrArg &roArg, const WorldBossGuildAddAttrRes &roRes, const CUserData &roUserData);
	void OnTimeout(const WorldBossGuildAddAttrArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const WorldBossGuildAddAttrArg &roArg, WorldBossGuildAddAttrRes &roRes);
	void OnDelayReplyRpc(const WorldBossGuildAddAttrArg &roArg, WorldBossGuildAddAttrRes &roRes, const CUserData &roUserData);

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
	WorldBossGuildAddAttrArg m_oArg;
	WorldBossGuildAddAttrRes m_oRes;
};

#endif
