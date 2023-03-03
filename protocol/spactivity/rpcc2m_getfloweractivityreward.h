#ifndef __RPCC2M_GETFLOWERACTIVITYREWARD_H__
#define __RPCC2M_GETFLOWERACTIVITYREWARD_H__

// generate by ProtoGen at date: 2017/6/7 15:14:13

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETFLOWERACTIVITYREWARD_TYPE 36979

class RpcC2M_GetFlowerActivityReward : public CRpc
{
private:

	RpcC2M_GetFlowerActivityReward():CRpc(RPCC2M_GETFLOWERACTIVITYREWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetFlowerActivityReward *CreateRpc()
	{
		return new RpcC2M_GetFlowerActivityReward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetFlowerActivityRewardArg &roArg, const GetFlowerActivityRewardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetFlowerActivityRewardArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetFlowerActivityRewardArg &roArg, GetFlowerActivityRewardRes &roRes);
	void OnDelayReplyRpc(const GetFlowerActivityRewardArg &roArg, GetFlowerActivityRewardRes &roRes, const CUserData &roUserData);

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
	GetFlowerActivityRewardArg m_oArg;
	GetFlowerActivityRewardRes m_oRes;
};

#endif
