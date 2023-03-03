#ifndef __RPCC2M_GETMOBABATTLEWEEKREWARD_H__
#define __RPCC2M_GETMOBABATTLEWEEKREWARD_H__

// generate by ProtoGen at date: 2017/6/26 14:36:04

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETMOBABATTLEWEEKREWARD_TYPE 55678

class RpcC2M_GetMobaBattleWeekReward : public CRpc
{
private:

	RpcC2M_GetMobaBattleWeekReward():CRpc(RPCC2M_GETMOBABATTLEWEEKREWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetMobaBattleWeekReward *CreateRpc()
	{
		return new RpcC2M_GetMobaBattleWeekReward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetMobaBattleWeekRewardArg &roArg, const GetMobaBattleWeekRewardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetMobaBattleWeekRewardArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetMobaBattleWeekRewardArg &roArg, GetMobaBattleWeekRewardRes &roRes);
	void OnDelayReplyRpc(const GetMobaBattleWeekRewardArg &roArg, GetMobaBattleWeekRewardRes &roRes, const CUserData &roUserData);

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
	GetMobaBattleWeekRewardArg m_oArg;
	GetMobaBattleWeekRewardRes m_oRes;
};

#endif
