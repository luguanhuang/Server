#ifndef __RPCC2G_GETHEROBATTLEWEEKREWARD_H__
#define __RPCC2G_GETHEROBATTLEWEEKREWARD_H__

// generate by ProtoGen at date: 2016/12/28 22:48:38

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETHEROBATTLEWEEKREWARD_TYPE 63058

class RpcC2G_GetHeroBattleWeekReward : public CRpc
{
private:

	RpcC2G_GetHeroBattleWeekReward():CRpc(RPCC2G_GETHEROBATTLEWEEKREWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetHeroBattleWeekReward *CreateRpc()
	{
		return new RpcC2G_GetHeroBattleWeekReward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetHeroBattleWeekRewardArg &roArg, const GetHeroBattleWeekRewardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetHeroBattleWeekRewardArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetHeroBattleWeekRewardArg &roArg, GetHeroBattleWeekRewardRes &roRes);
	void OnDelayReplyRpc(const GetHeroBattleWeekRewardArg &roArg, GetHeroBattleWeekRewardRes &roRes, const CUserData &roUserData);

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
	GetHeroBattleWeekRewardArg m_oArg;
	GetHeroBattleWeekRewardRes m_oRes;
};

#endif
