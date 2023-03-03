#ifndef __RPCG2M_REQGUILDLADDERREWARD_H__
#define __RPCG2M_REQGUILDLADDERREWARD_H__

// generate by ProtoGen at date: 2016/9/30 10:08:35

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_REQGUILDLADDERREWARD_TYPE 21840

class RpcG2M_ReqGuildLadderReward : public CRpc
{
private:

	RpcG2M_ReqGuildLadderReward():CRpc(RPCG2M_REQGUILDLADDERREWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_ReqGuildLadderReward *CreateRpc()
	{
		return new RpcG2M_ReqGuildLadderReward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReqGuildLadderRewardArg &roArg, const ReqGuildLadderRewardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ReqGuildLadderRewardArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReqGuildLadderRewardArg &roArg, ReqGuildLadderRewardRes &roRes);
	void OnDelayReplyRpc(const ReqGuildLadderRewardArg &roArg, ReqGuildLadderRewardRes &roRes, const CUserData &roUserData);

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
	ReqGuildLadderRewardArg m_oArg;
	ReqGuildLadderRewardRes m_oRes;
};

#endif
