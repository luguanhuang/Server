#ifndef __RPCC2G_GETWEEKLYTASKREWARD_H__
#define __RPCC2G_GETWEEKLYTASKREWARD_H__

// generate by ProtoGen at date: 2017/9/5 10:03:12

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETWEEKLYTASKREWARD_TYPE 30588

class RpcC2G_GetWeeklyTaskReward : public CRpc
{
private:

	RpcC2G_GetWeeklyTaskReward():CRpc(RPCC2G_GETWEEKLYTASKREWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetWeeklyTaskReward *CreateRpc()
	{
		return new RpcC2G_GetWeeklyTaskReward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetWeeklyTaskRewardArg &roArg, const GetWeeklyTaskRewardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetWeeklyTaskRewardArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetWeeklyTaskRewardArg &roArg, GetWeeklyTaskRewardRes &roRes);
	void OnDelayReplyRpc(const GetWeeklyTaskRewardArg &roArg, GetWeeklyTaskRewardRes &roRes, const CUserData &roUserData);

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
	GetWeeklyTaskRewardArg m_oArg;
	GetWeeklyTaskRewardRes m_oRes;
};

#endif
