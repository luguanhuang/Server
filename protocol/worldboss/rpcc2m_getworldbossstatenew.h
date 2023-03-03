#ifndef __RPCC2M_GETWORLDBOSSSTATENEW_H__
#define __RPCC2M_GETWORLDBOSSSTATENEW_H__

// generate by ProtoGen at date: 2016/8/8 19:24:07

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETWORLDBOSSSTATENEW_TYPE 17093

class RpcC2M_GetWorldBossStateNew : public CRpc
{
private:

	RpcC2M_GetWorldBossStateNew():CRpc(RPCC2M_GETWORLDBOSSSTATENEW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetWorldBossStateNew *CreateRpc()
	{
		return new RpcC2M_GetWorldBossStateNew;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetWorldBossStateArg &roArg, const GetWorldBossStateRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetWorldBossStateArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetWorldBossStateArg &roArg, GetWorldBossStateRes &roRes);
	void OnDelayReplyRpc(const GetWorldBossStateArg &roArg, GetWorldBossStateRes &roRes, const CUserData &roUserData);

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
	GetWorldBossStateArg m_oArg;
	GetWorldBossStateRes m_oRes;
};

#endif
