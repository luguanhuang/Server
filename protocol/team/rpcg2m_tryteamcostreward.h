#ifndef __RPCG2M_TRYTEAMCOSTREWARD_H__
#define __RPCG2M_TRYTEAMCOSTREWARD_H__

// generate by ProtoGen at date: 2016/11/27 18:37:53

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_TRYTEAMCOSTREWARD_TYPE 44102

class RpcG2M_TryTeamCostReward : public CRpc
{
private:

	RpcG2M_TryTeamCostReward():CRpc(RPCG2M_TRYTEAMCOSTREWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_TryTeamCostReward *CreateRpc()
	{
		return new RpcG2M_TryTeamCostReward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const TryTeamCostReward &roArg, const TryTeamCostRewardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const TryTeamCostReward &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const TryTeamCostReward &roArg, TryTeamCostRewardRes &roRes);
	void OnDelayReplyRpc(const TryTeamCostReward &roArg, TryTeamCostRewardRes &roRes, const CUserData &roUserData);

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
	TryTeamCostReward m_oArg;
	TryTeamCostRewardRes m_oRes;
};

#endif
