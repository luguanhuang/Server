#ifndef __RPCC2G_GETDAILYTASKREWARD_H__
#define __RPCC2G_GETDAILYTASKREWARD_H__

// generate by ProtoGen at date: 2016/11/14 20:00:27

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETDAILYTASKREWARD_TYPE 59899

class RpcC2G_GetDailyTaskReward : public CRpc
{
private:

	RpcC2G_GetDailyTaskReward():CRpc(RPCC2G_GETDAILYTASKREWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetDailyTaskReward *CreateRpc()
	{
		return new RpcC2G_GetDailyTaskReward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetDailyTaskRewardArg &roArg, const GetDailyTaskRewardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetDailyTaskRewardArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetDailyTaskRewardArg &roArg, GetDailyTaskRewardRes &roRes);
	void OnDelayReplyRpc(const GetDailyTaskRewardArg &roArg, GetDailyTaskRewardRes &roRes, const CUserData &roUserData);

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
	GetDailyTaskRewardArg m_oArg;
	GetDailyTaskRewardRes m_oRes;
};

#endif
