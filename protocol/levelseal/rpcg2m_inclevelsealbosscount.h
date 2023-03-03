#ifndef __RPCG2M_INCLEVELSEALBOSSCOUNT_H__
#define __RPCG2M_INCLEVELSEALBOSSCOUNT_H__

// generate by ProtoGen at date: 2016/7/29 18:37:37

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_INCLEVELSEALBOSSCOUNT_TYPE 36102

class RpcG2M_IncLevelSealBossCount : public CRpc
{
private:

	RpcG2M_IncLevelSealBossCount():CRpc(RPCG2M_INCLEVELSEALBOSSCOUNT_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_IncLevelSealBossCount *CreateRpc()
	{
		return new RpcG2M_IncLevelSealBossCount;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const IncBossCountArg &roArg, const IncBossCountRes &roRes, const CUserData &roUserData);
	void OnTimeout(const IncBossCountArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const IncBossCountArg &roArg, IncBossCountRes &roRes);
	void OnDelayReplyRpc(const IncBossCountArg &roArg, IncBossCountRes &roRes, const CUserData &roUserData);

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
	IncBossCountArg m_oArg;
	IncBossCountRes m_oRes;
};

#endif
