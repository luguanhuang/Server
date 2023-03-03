#ifndef __RPCC2M_WORLDBOSSEND_H__
#define __RPCC2M_WORLDBOSSEND_H__

// generate by ProtoGen at date: 2016/9/3 21:08:42

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_WORLDBOSSEND_TYPE 53655

class RpcC2M_WorldBossEnd : public CRpc
{
private:

	RpcC2M_WorldBossEnd():CRpc(RPCC2M_WORLDBOSSEND_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_WorldBossEnd *CreateRpc()
	{
		return new RpcC2M_WorldBossEnd;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const WorldBossEndArg &roArg, const WorldBossEndRes &roRes, const CUserData &roUserData);
	void OnTimeout(const WorldBossEndArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const WorldBossEndArg &roArg, WorldBossEndRes &roRes);
	void OnDelayReplyRpc(const WorldBossEndArg &roArg, WorldBossEndRes &roRes, const CUserData &roUserData);

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
	WorldBossEndArg m_oArg;
	WorldBossEndRes m_oRes;
};

#endif
