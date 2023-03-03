#ifndef __RPCC2G_GETSPACTIVITYREWARD_H__
#define __RPCC2G_GETSPACTIVITYREWARD_H__

// generate by ProtoGen at date: 2016/10/28 17:04:32

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETSPACTIVITYREWARD_TYPE 7905

class RpcC2G_GetSpActivityReward : public CRpc
{
private:

	RpcC2G_GetSpActivityReward():CRpc(RPCC2G_GETSPACTIVITYREWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetSpActivityReward *CreateRpc()
	{
		return new RpcC2G_GetSpActivityReward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetSpActivityRewardArg &roArg, const GetSpActivityRewardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetSpActivityRewardArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetSpActivityRewardArg &roArg, GetSpActivityRewardRes &roRes);
	void OnDelayReplyRpc(const GetSpActivityRewardArg &roArg, GetSpActivityRewardRes &roRes, const CUserData &roUserData);

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
	GetSpActivityRewardArg m_oArg;
	GetSpActivityRewardRes m_oRes;
};

#endif
